#include <jni.h>
#include <string>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgproc/types_c.h>
#include "Utils.h"
#include <android/log.h>

using namespace std;

#define NORMAL_WIDTH  360
#define NORMAL_HEIGHT 240

#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "error", __VA_ARGS__))
#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, "debug", __VA_ARGS__))

extern "C" JNIEXPORT jstring JNICALL
Java_hanlonglin_com_ocrrecognization_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

Utils mutils;

jobject createBitmap(JNIEnv * env,Mat srcData,jobject config){
    int imgWidth=srcData.cols;
    int imgHeight=srcData.rows;
    int numPix=imgWidth*imgHeight;
    jclass bmpClass=env->FindClass("android/graphics/Bitmap");
    jmethodID createBitmapMid=env->GetStaticMethodID(bmpClass,"createBitmap",
            "(IILandroid/graphics/Bitmap$Config;)Landroid/graphics/Bitmap;");
    jobject jBmpObj=env->CallStaticObjectMethod(bmpClass,createBitmapMid,imgWidth,imgHeight,config);
    mutils.Java_org_opencv_android_Utils_nMatToBitmap2(env,0,(jlong)&srcData,jBmpObj,false);
    return jBmpObj;
}

//获取身份证号码图片
extern "C"
JNIEXPORT jobject JNICALL
Java_hanlonglin_com_ocrrecognization_MainActivity_getIdNumber(JNIEnv *env, jobject instance,
                                                              jobject bitmap, jobject config) {
    // TODO
    jclass type=env->GetObjectClass(instance);
    //Bitmap转化为Mat
    Mat src_img;
//    mutils.BitmapToMat2(env,bitmap,src_img,false);
    mutils.Java_org_opencv_android_Utils_nBitmapToMat2(env,type, bitmap,(jlong)&src_img,0);

    //Mat src_img = image;
    Mat resize_img;
    //大小归一
    cv::Size RE_SIZE(NORMAL_WIDTH, NORMAL_HEIGHT);
    resize(src_img, resize_img, RE_SIZE);
    LOGE("大小归一");

    //图像灰度化
    Mat gray_img;
    cvtColor(resize_img, gray_img, CV_BGR2GRAY);
    LOGE("灰度化");

    //灰度图二值化
    /*
     f(i,j)=   0 (gray<=100)
               255(gray>100)
     */
    Mat gray2_img;
    threshold(gray_img, gray2_img, 100, 255, CV_THRESH_BINARY);
    LOGE("灰度二值化");

    //膨胀
    Mat peng_img;
    Mat erodeElement = getStructuringElement(MORPH_RECT, cv::Size(10, 10));
    erode(gray2_img, peng_img, erodeElement);
    LOGE("膨胀");

    //轮廓检测
    Mat dst_img; //身份证号
    vector<vector<cv::Point>> conturs;
    vector<Rect> rects;
    int max = 0;
    findContours(peng_img, conturs, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
    for (int i = 0; i < conturs.size(); i++) {
        Rect rect = boundingRect(conturs.at(i));
        rectangle(peng_img, rect, Scalar(0, 0, 255));

        if (rect.width >= rect.height * 8) {
            rects.push_back(rect);
            rectangle(peng_img, rect, Scalar(0, 0, 255));
            dst_img = resize_img(rect);
        }
    }
    //只需要坐标最低的那个矩形
    if (rects.size() == 1) {
        Rect rect = rects.at(0);
        dst_img = resize_img(rect);
    }
    else {
        int lowPoint = 0;
        Rect finalRect;
        for (int i = 0; i < rects.size();i++) {
            Rect rect = rects.at(i);
            Point point = rect.tl();
            if (point.y > lowPoint) {
                lowPoint = point.y;
                finalRect = rect;
            }
        }
        rectangle(resize_img, finalRect, Scalar(255, 255, 255));
        dst_img = resize_img(finalRect);
    }
    LOGE("轮廓检测");

    if(dst_img.empty()){
        LOGE("目标图片为空");
        return bitmap;
    }

    //Mat转化为bitmap
    jobject resultBitmap=createBitmap(env,dst_img,config);
    //mutils.MatToBitmap2(env,dst_img,bitmap,false);
    //mutils.Java_org_opencv_android_Utils_nMatToBitmap2(env,type,(jlong)&dst_img,bitmap,0);
    return resultBitmap;
}
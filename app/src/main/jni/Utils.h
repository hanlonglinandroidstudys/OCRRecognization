//
// Created by hanlonglin on 2019/3/13.
//

#include <jni.h>
#include <opencv2/core.hpp>
#include <opencv2/objdetect.hpp>
#include <iostream>

using namespace cv;

#ifndef OCRRECOGNIZATION_UTILS_H
#define OCRRECOGNIZATION_UTILS_H

class Utils {
public:
    void BitmapToMat2(JNIEnv *env, jobject& bitmap, Mat& mat, jboolean needUnPremultiplyAlpha) ;

    void MatToBitmap2(JNIEnv *env, Mat& mat, jobject& bitmap, jboolean needPremultiplyAlpha);

    void Java_org_opencv_android_Utils_nBitmapToMat2
    (JNIEnv * env, jclass type, jobject bitmap, jlong m_addr, jboolean needUnPremultiplyAlpha);

    void Java_org_opencv_android_Utils_nMatToBitmap2
            (JNIEnv * env, jclass type, jlong m_addr, jobject bitmap, jboolean needPremultiplyAlpha);

private:
 //   void LOGD(std::string msg);

    void LOGE(std::string msg);
};


#endif //OCRRECOGNIZATION_UTILS_H

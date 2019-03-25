# 身份证号数字OCR识别

## java
*  StartActivity 权限获取
*  MainActivity  界面，选择图片，截取图片，识别图片；
        使用tess-two第三方库，对剪切出来的身份证号码识别；
        需要加入训练文件，必须从assert文件中复制到手机存储的一个tessdata子目录下，然后再能使用；具体见https://github.com/rmtheis/tess-two

## cpp
*  native 从下载的opencv4Android中复制的native文件夹
*  opencv_libs暂时没用了
*  Android.mk jni 编译配置
*  native-lib.cpp jni实现函数，使用opencv进行图片的处理，最终截取出身份证号码区域
*  Utils.cpp Utils.h 实现Bitmap 和 Mat的转化，从github上的opencv中util.cpp中复制,地址 [util.cpp](https://github.com/opencv/opencv/blob/master/modules/java/generator/src/cpp/utils.cpp)


### ndk配置：
        项目增加了cmake方式配置，详情可见CmakeList.txt;
        可以自由切换cmake或者ndk-build方式，可以在gradle.properties中isCmake变量实现；
        cmake方式使用CmakeList.txt编译配置，ndk-build方式使用Android.mk方式配置，两者都已配置好


### 注意的地方：
*  1.使用ndk-build方式会在build目录中生成 libnative-lib.so 和 libopencv_java4.so等，但是使用cmake方式只会生成libnative-lib.so,也就是自己的so库
    cmake方式不会复制libopencv-java4.so；所以，使用cmake方式时，需要指定我们依赖的libopencv_java4.so的路径，不然在运行时会报错 dlopen failed: could not load library "libopencv_java4.so"，
    指定运行时libopencv_java4.so的路径的方式，需要在build.gradle中添加：
                    //自定jni目录
                    <br>
                    <p>
                    <code>
                    sourceSets.main{
                        jniLibs.srcDirs=['libs','src/main/jni/native/libs']  //设置运行时依赖so库的路径，这里如果不写上，运行时会报错找不到libopencv_java4.so
                        jni.srcDirs=[]   //设置禁止gradle生成Android.mk
                    }
                    </code>
                    </p>
                    <br>
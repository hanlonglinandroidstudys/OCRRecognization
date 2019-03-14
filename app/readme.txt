# 身份证号数字OCR识别

# ----java
    ----StartActivity 权限获取
    ----MainActivity  界面，选择图片，截取图片，识别图片；
        使用tess-two第三方库，对剪切出来的身份证号码识别；
        需要加入训练文件，必须从assert文件中复制到手机存储的一个tessdata子目录下，然后再能使用；具体见https://github.com/rmtheis/tess-two

# ----cpp
    ----native 从下载的opencv4Android中复制的native文件夹
    ----opencv_libs暂时没用了
    ----Android.mk jni 编译配置
    ----native-lib.cpp jni实现函数，使用opencv进行图片的处理，最终截取出身份证号码区域
    ----Utils.cpp Utils.h 实现Bitmap 和 Mat的转化，从github上的opencv中util.cpp中复制
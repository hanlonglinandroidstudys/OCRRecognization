LOCAL_PATH := $(call my-dir)
#APP_STL :=gnustl_static

#native-lib
include $(CLEAR_VARS)
OPENCV_INSTALL_MODULES:=on
OPENCV_LIB_TYPE:=SHARED

ifdef OPENCV_ANDROID_SDK
  ifneq ("","$(wildcard $(OPENCV_ANDROID_SDK)/OpenCV.mk)")
    include ${OPENCV_ANDROID_SDK}/OpenCV.mk
  else
    include ${OPENCV_ANDROID_SDK}/sdk/native/jni/OpenCV.mk
  endif
else
  include $(LOCAL_PATH)/native/jni/OpenCV.mk
endif

LOCAL_MODULE    := native-lib
#try..catch支持
LOCAL_CPP_FEATURES += exceptions
LOCAL_C_INCLUDES+=  $(NDK_ROOT)/sources/cxx-stl/llvm-libc++/include
LOCAL_SRC_FILES := native-lib.cpp Utils.cpp
#重点需要引入系统库jnigraphics
LOCAL_LDLIBS := -lm -llog -ljnigraphics
include $(BUILD_SHARED_LIBRARY)
APP_STL := stlport_shared
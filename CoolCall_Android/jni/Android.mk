LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

include D:/OpenCV_android/sdk/native/jni/OpenCV.mk

LOCAL_MODULE    := process_frame 
LOCAL_SRC_FILES := process_frame.cpp
LOCAL_LDLIBS +=  -llog -ldl

include $(BUILD_SHARED_LIBRARY)

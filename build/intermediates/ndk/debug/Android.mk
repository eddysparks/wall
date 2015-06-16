LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := androidImageExample
LOCAL_SRC_FILES := \
	/home/ekko/Appz/openFrameworks/examples/android/androidImageExample/jni/Android.mk \
	/home/ekko/Appz/openFrameworks/examples/android/androidImageExample/jni/Application.mk \

LOCAL_C_INCLUDES += /home/ekko/Appz/openFrameworks/examples/android/androidImageExample/jni
LOCAL_C_INCLUDES += /home/ekko/Appz/openFrameworks/examples/android/androidImageExample/src/debug/jni

include $(BUILD_SHARED_LIBRARY)

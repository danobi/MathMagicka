LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)


LOCAL_MODULE 	:= localsolver
LOCAL_SRC_FILES := localsolver.c

LOCAL_LDLIBS 	:= -llog

include $(BUILD_SHARED_LIBRARY)

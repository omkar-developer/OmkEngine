
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := libopenal
LOCAL_SRC_FILES := ../Dependencies/lib/release/libopenal.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../Dependencies/include $(LOCAL_PATH)/..
LOCAL_MODULE    := GameEngine
LOCAL_SRC_FILES := ../GameCode.cpp
LOCAL_LDLIBS    := -L$(LOCAL_PATH)/../Dependencies/lib/release -lGameEngine -lGLESv1_CM -llog -lpng15_static -lz -lminizip -lBox2D -langelscript -langel_addon -lopenal -lvorbisfile -lvorbis_static -logg_static

include $(BUILD_SHARED_LIBRARY)

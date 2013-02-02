
LOCAL_PATH := $(call my-dir)

GAME_FILES := GameCode.cpp

include $(CLEAR_VARS)
LOCAL_MODULE := libopenal
LOCAL_SRC_FILES := ../Dependencies/lib/release/libopenal.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../Dependencies/include $(LOCAL_PATH)/..
LOCAL_MODULE    := GameEngine
LOCAL_SRC_FILES := ../AndPlatformThread.cpp ../AndPlatformWindow.cpp ../AndTimer.cpp ../native.cpp ../NativeSound.cpp ../OpenGLDevice.cpp ../OpenGLPlatformResource.cpp ../OpenGLRenderer.cpp ../PlatformFile.cpp ../SoundPlayer.cpp ../Box2DExt/b2Polygon.cpp ../Box2DExt/b2Triangle.cpp ../Animation.cpp ../Core.cpp ../Object.cpp ../Physics.cpp ../Scripting.cpp ../Sprite.cpp ../TTFont.cpp ../ZipFile.cpp ../BaseIO.cpp ../BaseResource.cpp ../Game.cpp ../Signals.cpp ../stdafx.cpp ../UIWString.cpp $(GAME_FILES)
LOCAL_LDLIBS    := -L$(LOCAL_PATH)/../Dependencies/lib/release -lGLESv1_CM -llog -lpng15_static -lz -lminizip -lBox2D -langelscript -langel_addon -lopenal -lvorbisfile -lvorbis_static -logg_static

include $(BUILD_SHARED_LIBRARY)

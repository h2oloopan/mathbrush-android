LOCAL_PATH:= $(call my-dir)


include $(LOCAL_PATH)/recognizer.mk

include $(LOCAL_PATH)/libiconv.mk
include $(LOCAL_PATH)/libxml2.mk
include $(LOCAL_PATH)/renderer.mk



include $(CLEAR_VARS)
LOCAL_MODULE := wrappers
LOCAL_STATIC_LIBRARIES:= libmathreco librenderer
LOCAL_C_INCLUDES := $(LOCAL_PATH)/mathreco/include $(LOCAL_PATH)/renderer/include
LOCAL_SRC_FILES := recognition-wrapper.cpp rendering-wrapper.cpp
LOCAL_LDLIBS += -lstdc++ -llog -lz
LOCAL_CPPFLAGS += -fexceptions -w
include $(BUILD_SHARED_LIBRARY)
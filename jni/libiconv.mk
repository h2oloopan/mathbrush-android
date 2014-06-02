#libiconv
ICONV := libiconv
LOCAL_PATH := $(call my-dir)
LIB_PATH_REL := $(ICONV)
LIB_PATH_ABS := $(LOCAL_PATH)/$(ICONV)

include $(CLEAR_VARS)
#TARGET_ARCH_ABI := armeabi-v7a
LOCAL_MODULE    := libiconv
LOCAL_CFLAGS    := \
    -Wno-multichar \
    -D_ANDROID \
    -DLIBDIR="\"c\"" \
    -DBUILDING_LIBICONV \
    -DIN_LIBRARY \
    -w
LOCAL_C_INCLUDES := \
    $(LIB_PATH_ABS) \
    $(LIB_PATH_ABS)/include \
    $(LIB_PATH_ABS)/lib \
    $(LIB_PATH_ABS)/libcharset/include
LOCAL_SRC_FILES := \
    $(LIB_PATH_REL)/lib/iconv.c \
    $(LIB_PATH_REL)/lib/relocatable.c \
    $(LIB_PATH_REL)/libcharset/lib/localcharset.c
include $(BUILD_STATIC_LIBRARY)
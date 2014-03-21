# Generate native libs for Android

LOCAL_PATH:= $(call my-dir)

SYMBOLS_SOURCE_PATH := $(LOCAL_PATH)/mathreco/libmathreco
MATRIX_SOURCE_PATH := $(LOCAL_PATH)/mathreco/matrixreco
EXPANDER_SOURCE_PATH := $(LOCAL_PATH)/mathreco/range-expander
CASERVER_SOURCE_PATH := $(LOCAL_PATH)/mathreco/caserver

# Generate mathreco lib
include $(CLEAR_VARS)
LOCAL_MODULE := mathreco

LOCAL_CPP_EXTENSION := .cc .cpp
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/mathreco/include

SYMBOLS_FILE_LIST := $(wildcard $(SYMBOLS_SOURCE_PATH)/*.cpp) $(wildcard $(SYMBOLS_SOURCE_PATH)/*.cc)
MATRIX_FILE_LIST := $(wildcard $(MATRIX_SOURCE_PATH)/*.cpp)
EXPANDER_FILE_LIST := $(wildcard $(EXPANDER_SOURCE_PATH)/*.cpp)
CASERVER_FILE_LIST := $(wildcard $(CASERVER_SOURCE_PATH)/*.cc)

FILE_LIST := $(SYMBOLS_FILE_LIST:$(LOCAL_PATH)/%=%) $(MATRIX_FILE_LIST:$(LOCAL_PATH)/%=%) $(EXPANDER_FILE_LIST:$(LOCAL_PATH)/%=%) $(CASERVER_FILE_LIST:$(LOCAL_PATH)/%=%) 

LOCAL_SRC_FILES := $(FILE_LIST)

include $(BUILD_STATIC_LIBRARY)


include $(CLEAR_VARS)

LOCAL_MODULE := wrappers
LOCAL_C_INCLUDES := $(LOCAL_PATH)/mathreco/include
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/mathreco/include
$(warning $(LOCAL_C_INCLUDES))
LOCAL_SRC_FILES := recognition-wrapper.cpp
LOCAL_STATIC_LIBRARY := libstlport mathreco

include $(BUILD_SHARED_LIBRARY)

# first lib, which will be built statically
#
#include $(CLEAR_VARS)

#LOCAL_MODULE    := libtwolib-first
#LOCAL_SRC_FILES := first.c

#include $(BUILD_STATIC_LIBRARY)

# second lib, which will depend on and include the first one
#
#include $(CLEAR_VARS)

#LOCAL_MODULE    := libtwolib-second
#LOCAL_SRC_FILES := second.c

#LOCAL_STATIC_LIBRARIES := libtwolib-first

#include $(BUILD_SHARED_LIBRARY)

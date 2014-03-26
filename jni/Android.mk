# Generate native libs for Android

LOCAL_PATH:= $(call my-dir)

# Generate mathreco static lib
include $(CLEAR_VARS)
SYMBOLS_SOURCE_PATH := $(LOCAL_PATH)/mathreco/libmathreco
MATRIX_SOURCE_PATH := $(LOCAL_PATH)/mathreco/matrixreco
EXPANDER_SOURCE_PATH := $(LOCAL_PATH)/mathreco/range-expander
CASERVER_SOURCE_PATH := $(LOCAL_PATH)/mathreco/caserver

LOCAL_MODULE := mathreco
LOCAL_CPP_EXTENSION := .cc .cpp
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/mathreco/include
LOCAL_C_INCLUDES := $(LOCAL_PATH)/mathreco/include $(LOCAL_PATH)
LOCAL_CPPFLAGS += -fexceptions -frtti

SYMBOLS_FILE_LIST_CPP := $(wildcard $(SYMBOLS_SOURCE_PATH)/*.cpp) 
SYMBOLS_FILE_LIST_CC := $(wildcard $(SYMBOLS_SOURCE_PATH)/*.cc)
MATRIX_FILE_LIST := $(wildcard $(MATRIX_SOURCE_PATH)/*.cpp)
EXPANDER_FILE_LIST := $(wildcard $(EXPANDER_SOURCE_PATH)/*.cpp)
CASERVER_FILE_LIST := mathreco/caserver/client.cc mathreco/caserver/io.cc mathreco/caserver/log.cc mathreco/caserver/expr.cc mathreco/caserver/CASOperation.cc

FILE_LIST := $(SYMBOLS_FILE_LIST_CC:$(LOCAL_PATH)/%=%) $(SYMBOLS_FILE_LIST_CPP:$(LOCAL_PATH)/%=%) $(MATRIX_FILE_LIST:$(LOCAL_PATH)/%=%) $(EXPANDER_FILE_LIST:$(LOCAL_PATH)/%=%) $(CASERVER_FILE_LIST) 
LOCAL_SRC_FILES := $(FILE_LIST) wrapper.cc
include $(BUILD_STATIC_LIBRARY)

# Generate wrappers shared lib
include $(CLEAR_VARS)
LOCAL_MODULE := wrappers
LOCAL_STATIC_LIBRARIES:= mathreco
LOCAL_C_INCLUDES := $(LOCAL_PATH)/mathreco/include
LOCAL_SRC_FILES := recognition-wrapper.cpp
LOCAL_LDLIBS := -lstdc++ -llog
include $(BUILD_SHARED_LIBRARY)
#LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/mathreco/include
#include $(BUILD_STATIC_LIBRARY)

#LOCAL_MODULE := wrappers
#LOCAL_LDLIBS += -llog -ldl
#LOCAL_C_INCLUDES := $(LOCAL_PATH)/mathreco/include
#LOCAL_STATIC_LIBRARIES := mathreco
#LOCAL_SRC_FILES := recognition-wrapper.cpp
#include $(BUILD_SHARED_LIBRARY)


LOCAL_PATH := $(call my-dir)
REC := mathreco
LIB_PATH_ABS := $(LOCAL_PATH)/$(REC)

# Generate mathreco static lib
include $(CLEAR_VARS)
SYMBOLS_SOURCE_PATH := $(LIB_PATH_ABS)/libmathreco
MATRIX_SOURCE_PATH := $(LIB_PATH_ABS)/matrixreco
EXPANDER_SOURCE_PATH := $(LIB_PATH_ABS)/range-expander
CASERVER_SOURCE_PATH := $(LIB_PATH_ABS)/caserver

LOCAL_MODULE := libmathreco
LOCAL_CPP_EXTENSION := .cc .cpp
LOCAL_EXPORT_C_INCLUDES := $(LIB_PATH_ABS)/include
LOCAL_C_INCLUDES := $(LIB_PATH_ABS)/include $(LOCAL_PATH)
LOCAL_CPPFLAGS += -fexceptions -frtti -w

SYMBOLS_FILE_LIST_CPP := $(wildcard $(SYMBOLS_SOURCE_PATH)/*.cpp) 
SYMBOLS_FILE_LIST_CC := $(wildcard $(SYMBOLS_SOURCE_PATH)/*.cc)
MATRIX_FILE_LIST := $(wildcard $(MATRIX_SOURCE_PATH)/*.cpp)
EXPANDER_FILE_LIST := $(wildcard $(EXPANDER_SOURCE_PATH)/*.cpp)
CASERVER_FILE_LIST := $(REC)/caserver/client.cc $(REC)/caserver/io.cc $(REC)/caserver/log.cc $(REC)/caserver/expr.cc $(REC)/caserver/CASOperation.cc

FILE_LIST := $(SYMBOLS_FILE_LIST_CC:$(LOCAL_PATH)/%=%) $(SYMBOLS_FILE_LIST_CPP:$(LOCAL_PATH)/%=%) $(MATRIX_FILE_LIST:$(LOCAL_PATH)/%=%) $(EXPANDER_FILE_LIST:$(LOCAL_PATH)/%=%) $(CASERVER_FILE_LIST) 
LOCAL_SRC_FILES := $(FILE_LIST) wrapper.cc
include $(BUILD_STATIC_LIBRARY)
LOCAL_PATH := $(call my-dir)
R_PATH := renderer/src

include $(CLEAR_VARS)

LOCAL_MODULE := librenderer
LOCAL_CPP_EXTENSION := .cpp .cc
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/renderer/include
LOCAL_C_INCLUDES := $(LOCAL_PATH)/renderer/include $(LOCAL_PATH) $(LOCAL_PATH)/mathreco/include $(LOCAL_PATH)/libxml2/include $(LOCAL_PATH)/libiconv/include
LOCAL_CPPFLAGS += -fexceptions -w

LOCAL_SRC_FILES := $(R_PATH)/CDevice.cpp $(R_PATH)/BuildTreeFromExpr.cpp $(R_PATH)/dictionaries.cpp $(R_PATH)/Functs.cpp $(R_PATH)/Lines.cpp $(R_PATH)/Symbol.cpp $(R_PATH)/SymbolBox.cpp $(R_PATH)/BuildTreeFromMathML.cpp wrapper.cc

LOCAL_STATIC_LIBRARIES := libiconv libxml2
include $(BUILD_STATIC_LIBRARY)

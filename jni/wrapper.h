#include <android/log.h>
#define LOG(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, "pan", __VA_ARGS__))
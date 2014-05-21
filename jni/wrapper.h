#ifndef WRAPPER_H_
#define WRAPPER_H_

#include <android/log.h>
#include <string>


#define LOG(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, "pan", __VA_ARGS__))

namespace android {
	std::wstring str2wstr(const std::string &str);
	std::string wstr2str(const std::wstring &wstr);
}


#endif
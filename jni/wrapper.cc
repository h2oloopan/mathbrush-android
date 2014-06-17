#include "wrapper.h"

namespace android {
	std::wstring str2wstr(const std::string &str) {
		std::wstring result;
		result.assign(str.begin(), str.end());
		return result;
	}
	std::string wstr2str(const std::wstring &wstr) {
		std::string result;
		result.assign(wstr.begin(), wstr.end());
		return result;
	}
	jstring wchar2jstring(JNIEnv* env, const wchar_t* w) {
		LOG("CONVERTING %s", w);
		size_t len = wcslen(w);
		jchar* str = (jchar*)malloc((len+1)*sizeof(jchar));
		int i;
		for (i = 0; i < len; i++) {
			str[i] = w[i];
			LOG("%x", str[i]);
		}
		str[len] = 0;
		jstring js = env->NewString(str, len);
		free(str);
		return js;
	}

}
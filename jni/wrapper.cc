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
	char* wchar2char(const wchar_t* w) {
		return NULL;
	}
	jstring wchar2jstring(JNIEnv* env, const wchar_t* w) {
		size_t len = wcslen(w);
		LOG("CONVERTING SIZE %d", len);
		jchar* str = (jchar*)malloc((len+1)*sizeof(jchar));
		//char* str = (char*)malloc((len+1)*sizeof(char));
		int i;
		for (i = 0; i < len; i++) {
			str[i] = w[i];
			LOG("%x", str[i]);
		}
		str[len] = 0;
		jstring js = env->NewString(str, len);
		//jstring js = env->NewStringUTF(str);
		free(str);
		return js;
	}

}

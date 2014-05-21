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
}
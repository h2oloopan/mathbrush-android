#include "wrapper.h"

namespace android {
	std::wstring str2wstr(const std::string &str) {
		std::wstring result;
		result.assign(str.begin(), str.end());
		return result;
	}
}
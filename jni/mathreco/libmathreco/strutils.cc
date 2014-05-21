#include "strutils.h"
#include <cstdlib>
#include <iostream>
#include "wrapper.h"

namespace scg {

std::wstring
str2wstr(const std::string &s) {
#ifdef __ANDROID__
	std::wstring wstr = android::str2wstr(s);
	return wstr;
#else
	
	size_t mbsz = mbstowcs(0, s.c_str(), 0);
	std::wstring ws;
	//ws.resize(mbsz);

	wchar_t *wcs = new wchar_t[mbsz+1];

	//It's crashing here!
	
	if (mbstowcs(wcs, s.c_str(), mbsz+1) != mbsz || wcs[mbsz] != 0) {
		LOG("Going to abort %s", wcs);
		abort();
	}


	ws = wcs;
	delete[] wcs;
	return ws;

#endif

	
	
}

std::string
wstr2str(const std::wstring &ws) {
#ifdef __ANDROID__
	std::string str = android::wstr2str(ws);
	return str;
#else
	size_t sz = wcstombs(0, ws.c_str(), 0);
	std::string s;
	char *cs = new char[sz+1];
	if (wcstombs(cs, ws.c_str(), sz+1) != sz || cs[sz] != 0) {
		abort();
	}
	s = cs;
	delete[] cs;
	return s;
#endif
}

}

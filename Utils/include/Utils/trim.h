#pragma once

#include "utils_stdafx.h"
#include "UtilsAPI.h"

namespace Utils
{
	inline UTILS_API void trim_front(std::string &s) {
		s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
			return !std::isspace(ch);
		}));
	}

	inline UTILS_API void trim_back(std::string &s) {
		s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
			return !std::isspace(ch);
		}).base(), s.end());
	}

	inline UTILS_API void trim(std::string &s) {
		trim_front(s);
		trim_back(s);
	}


	inline UTILS_API std::string trim_front_copy(std::string s) {
		trim_front(s);
		return s;
	}

	inline UTILS_API std::string trim_back_copy(std::string s) {
		trim_back(s);
		return s;
	}

	inline UTILS_API std::string trim_copy(std::string s) {
		trim(s);
		return s;
	}
}
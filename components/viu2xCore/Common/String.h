/* Copyright (C) Hao Qin. All rights reserved. */

#pragma once

#include <stdint.h>
#include <string>
#include <vector>

namespace v2x {

	typedef std::wstring String;
	typedef wchar_t Char;

	class StrUtils
	{
	public:

		//static String format(const String & format, ...);
		static String format(const Char * format, ...);

		//static String vformat(const String & format, va_list params);
		static String vformat(const Char * format, va_list params);

		static String toStr(const std::string & s);
		static String toStr(const char * & s);

		static String trim(const String & s);

		static void split(const String & s, const String & splitters, std::vector<String> & tokens);

	private:
		static void multibyteToUnicode(String & unicodeString, const char * multibyteString);

	};
}


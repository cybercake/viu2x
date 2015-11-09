/* Copyright (C) Hao Qin. All rights reserved. */

#include "Config.h"

#include "String.h"
#include "Exceptions.h"

#include <inttypes.h>
#include <exception>
#include <stdarg.h>
#include <Windows.h>

namespace v2x {

	//String StrUtils::format(const String & format, ...) {
	//	va_list params;
	//	va_start(params, format);
	//	String result = vformat(format, params);
	//	va_end(params);
	//	return result;
	//}

	String StrUtils::format(const Char * format, ...) {
		va_list params;
		va_start(params, format);
		String result = vformat(format, params);
		va_end(params);
		return result;
	}

	//String StrUtils::vformat(const String & format, va_list params) {
	//	String result = vformat(format.c_str(), params);
	//	return result;
	//}

	String StrUtils::vformat(const Char * format, va_list params) {

		const size_t maxSize = 0x7FFFFFFF;
		const size_t initialSize = 256;

		size_t bufferSize = initialSize;

		while (bufferSize < maxSize) {

			Char * buf = new Char[bufferSize];
			size_t actualSize = _vsnwprintf_s(buf, bufferSize, bufferSize - 1, format, params) + 1; // reserve place for '\0'

			// Check if it succeeded.
			if (actualSize > 0 && actualSize < bufferSize) {
				String result(buf);
				delete[] buf;
				return result;
			}
			else {
				delete[] buf;
				bufferSize *= 2;
			}
		}

		throw Exception(L"StrUtils::format(): Failed to format string! String length out of range!");
	}

	String StrUtils::toStr(const std::string & s) {

		String result;
		multibyteToUnicode(result, s.c_str());
		return result;
	}

	String StrUtils::toStr(const char * & s){
		String result;
		multibyteToUnicode(result, s);
		return result;
	}

	String StrUtils::trim(const String & s) {

		if (s.empty())
			return String();

		size_t left = 0;
		for (; left < s.size(); ++left)
			if (!isspace(s[left]))
				break;

		size_t right = s.size() - 1;
		for (;; --right) {
			if (!isspace(s[right]))
				break;
			if (right == 0)
				break;
		}

		if (right > left) {
			return s.substr(left, right - left + 1);
		}
		else return String();
	}

	void StrUtils::split(const String & s, const String & splitters, std::vector<String> & tokens) {

		int tokenStart = 0;
		int i = 0;

		for (; i < (int)s.length(); ++i) {

			// Check if it is splitter
			if (splitters.find(s[i]) != String::npos) {

				// Check if it is not right after another splitter
				if (i > tokenStart)
					tokens.push_back(s.substr(tokenStart, i - tokenStart));
				tokenStart = i + 1;
			}
		}

		// Process the last token
		if (i > tokenStart)
			tokens.push_back(s.substr(tokenStart, i - tokenStart));
	}

	//! Convert a multibyte string to unicode and assign to the current
	//! string object.
	//!
	//! @param [in]     multibyteString     The multibyte zero-terminated string to convert.
	//!
	//! @return         None
	void StrUtils::multibyteToUnicode(String & unicodeString, const char * multibyteString) {

#ifdef V2X_WINDOWS

		if (multibyteString == NULL)
			throw Exception(L"StrUtils::multibyteToUnicode(): The input string is NULL!");

		// Get the required buffer size
		int requiredLength = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, multibyteString, -1, NULL, 0);
		if (requiredLength <= 0)
			OsException::throwLatest(L"StrUtils::multibyteToUnicode()");

		// Allocate the buffer
		Char * dst = new Char[requiredLength + 1];
		if (dst == NULL)
			throw Exception(L"StrUtils::multibyteToUnicode(): Failed to allocate temporary buffer!");

		try {
			// Convert to unicode
			int writtenLength = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, multibyteString, -1, dst, requiredLength + 1);

			// Check if conversion was successful
			if (writtenLength == 0)
				OsException::throwLatest(L"StrUtils::multibyteToUnicode()");

			// Check if the string length before/after conversion are identical.
			if (writtenLength != requiredLength)
				throw Exception(L"StrUtils::multibyteToUnicode(): The length of the converted string is not expected!");
		}
		catch (...) {
			delete[] dst;
			throw;
		}

		unicodeString.assign(dst);
		delete[] dst;
#else
#error Not implemented!
#endif
		}
	}
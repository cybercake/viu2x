/* Copyright (C) Hao Qin. All rights reserved. */

#include "Exceptions.h"
#include "Config.h"

#include <Windows.h>
#include <stdarg.h>

namespace v2x {

	///////////////
	// Exception //
	///////////////

	//Exception::Exception(const String & message, ...) {

	//	va_list params;
	//	va_start(params, message);
	//	initialize(StrUtils::format(message, params));
	//	va_end(params);
	//}

	//Exception::Exception(const String & message, va_list params) {

	//	initialize(StrUtils::format(message, params));
	//}

	//Exception::Exception(const Exception & internalException, const String & message, ...) {

	//	va_list params;
	//	va_start(params, message);
	//	initialize(StrUtils::format(message, params), internalException);
	//	va_end(params);
	//}

	//Exception::Exception(const Exception & internalException, const String & message, va_list params) {

	//	initialize(StrUtils::format(message, params), internalException);
	//}

	Exception::Exception(const Char * message, ...) {

		va_list params;
		va_start(params, message);
		initialize(StrUtils::format(message, params));
		va_end(params);
	}

	Exception::Exception(const Char * message, va_list params) {

		initialize(StrUtils::format(message, params));
	}

	Exception::Exception(const Exception & internalException, const Char * message, ...) {

		va_list params;
		va_start(params, message);
		initialize(StrUtils::format(message, params), internalException);
		va_end(params);
	}

	Exception::Exception(const Exception & internalException, const Char * message, va_list params) {

		initialize(StrUtils::format(message, params), internalException);
	}

	Exception::~Exception() {
	}

	void Exception::initialize(const String & message) {

		// Copy all previous exception messages
		m_messages.push_front(message);
	}

	void Exception::initialize(const String & message, const Exception & internalException) {

		// Copy all previous exception messages
		m_messages.assign(internalException.getMessages().begin(), internalException.getMessages().end());

		m_messages.push_front(message);
	}

	const String & Exception::getMessage() const {
		return m_messages.front();
	}

	const std::deque<String> & Exception::getMessages() const {
		return m_messages;
	}

	/////////////////
	// OsException //
	/////////////////

	//OsException::OsException(const String & caller) :
	//	Exception(caller + L": " + getLastErrorMessage()) {
	//}

	//OsException::OsException(const Exception & internalException, const String & caller) :
	//	Exception(caller + L": " + getLastErrorMessage(), internalException) {
	//}

	OsException::OsException(const Char * caller) :
		Exception(StrUtils::format(L"%s: %s", caller, getLastErrorMessage().c_str()).c_str()) {
	}

	OsException::OsException(const Exception & internalException, const Char * caller) :
		Exception(internalException, L"%s: %s", caller, getLastErrorMessage().c_str()) {
	}

	OsException::~OsException() {
		// Nothing needs to be done.
	}

	String OsException::getLastErrorMessage() {

#ifdef V2X_WINDOWS
		DWORD errorCode = GetLastError();
		LPWSTR outputStr = NULL;

		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			errorCode,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPWSTR)&outputStr,
			0, NULL);

		if (outputStr == NULL)
			throw Exception(L"OsException::getLastErrorMessage(): Failed to retrieve system error message!");

		String message(outputStr);
		LocalFree(outputStr);

		return message;
#else
#error Not implemented!
#endif
	}

	//void OsException::throwLatest(const String & caller, const Exception & internalException) {
	//	throw OsException(internalException, caller);
	//}

	//void OsException::throwLatest(const String & caller) {
	//	throw OsException(caller);
	//}

	void OsException::throwLatest(const Char * caller, const Exception & internalException) {
		throw OsException(internalException, caller);
	}

	void OsException::throwLatest(const Char * caller) {
		throw OsException(caller);
	}
}
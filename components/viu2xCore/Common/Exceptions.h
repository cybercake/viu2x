/* Copyright (C) Hao Qin. All rights reserved. */

#pragma once

#include "String.h"

#include <deque>

namespace v2x {

	/**
	 * The common base of all viu2x exceptions.
	 *
	 * @test Types/Exceptions
	 *
	 * @author  Qin
	 */
	class Exception {

	public:
		//Exception(const String & message, ...);
		//Exception(const String & message, va_list params);
		explicit Exception(const Char * message, ...);
		Exception(const Char * message, va_list params);
		//Exception(const Exception & internalException, const String & message, ...);
		//Exception(const Exception & internalException, const String & message, va_list params);
		Exception(const Exception & internalException, const Char * message, ...);
		Exception(const Exception & internalException, const Char * message, va_list params);
		virtual ~Exception();

		const String & getMessage() const;
		const std::deque<String> & getMessages() const;

	protected:
		std::deque<String> m_messages;

		void initialize(const String & message);
		void initialize(const String & message, const Exception & internalException);
	};

	/**
	 * The exception class of all OS specific exceptions.
	 *
	 * @test Types/Exceptions
	 *
	 * @author  Qin
	 */
	class OsException : public Exception {

	public:
		//OsException(const String & caller);
		//OsException(const Exception & internalException, const String & caller);
		OsException(const Char * caller);
		OsException(const Exception & internalException, const Char * caller);
		virtual ~OsException();

		static void throwLatest(const String & caller);
		static void throwLatest(const String & caller, const Exception & internalException);
		static void throwLatest(const Char * caller);
		static void throwLatest(const Char * caller, const Exception & internalException);

	private:
		static String getLastErrorMessage();
	};

}
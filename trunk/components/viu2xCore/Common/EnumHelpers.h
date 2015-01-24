/* Copyright (C) Hao Qin. All rights reserved. */

#pragma once

#define DEFINE_ENUM_AND_CONVERTERS_1 (enumtype, value1) \
	typedef enum { \
		value1, \
	} enumtype; \
	inline static const Char * toString(enumtype value) { \
		switch (value) { \
			case value1 : return L#value1; \
			default: throw Exception(L"%s::toString(): Unexpected enum value!", #enumtype); \
		} \
	} \
	inline static enumtype fromString(const String & s) { \
		if (s == L#value1) return value1; \
		throw Exception(L"%s::fromString(): Unexpected enum string %s!", #enumtype, s.c_str()); \
	} \
	inline static bool fromString(const String & s, enumtype & value) { \
		if (s == L#value1) { value = value1; return true; } \
		return false; \
	}

#define DEFINE_ENUM_AND_CONVERTERS_2 (enumtype, value1, value2) \
	typedef enum { \
		value1, \
		value2, \
	} enumtype; \
	inline static const Char * toString(enumtype value) { \
		switch (value) { \
			case value1 : return L#value1; \
			case value2 : return L#value2; \
			default: throw Exception(L"%s::toString(): Unexpected enum value!", #enumtype); \
		} \
	} \
	inline static enumtype fromString(const String & s) { \
		if (s == L#value1) return value1; \
		if (s == L#value2) return value2; \
		throw Exception(L"%s::fromString(): Unexpected enum string %s!", #enumtype, s.c_str()); \
	} \
	inline static bool fromString(const String & s, enumtype & value) { \
		if (s == L#value1) { value = value1; return true; } \
		if (s == L#value2) { value = value2; return true; } \
		return false; \
	}
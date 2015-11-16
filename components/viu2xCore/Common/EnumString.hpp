/* Copyright (C) Hao Qin. All rights reserved. */

#pragma once

#include "String.h"
#include "Exceptions.h"

namespace v2x {

	template <typename t>
	class EnumString {
	public:
		static String toString(t value) {

			int count = (int)m_strings.size();
			int i = static_cast<int>(value);

			if (i >= count)
				throw Exception(L"EnumString::toString(): String for the input value (%d) is not found!", i);

			return m_strings[static_cast<int>(value)];
		}

		static t fromString(const String & s) {

			int count = (int)m_strings.size();
			for (int i = 0; i < count; ++i)
				if (m_strings[i] == s)
					return static_cast<T>(i);

			throw Exception(L"EnumString::fromString(): \"%s\" is not a valid enum value!", s.c_str());
		}

	private:
		static std::vector<const Char *> m_strings;
	};

}
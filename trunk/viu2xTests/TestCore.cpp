#include "stdafx.h"
#include "CppUnitTest.h"
#include <viu2xCore/Property.h>
#include <viu2xCore/String.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace v2x;

namespace viu2xTests
{
	class PropertyContainerTest : protected PropertyContainer {
	public:
		PropertyContainerTest() :
			REGISTER_PROPERTY_WITH_DEFAULT_VALIDATOR(PropertyContainerTest, Property1, 0),
			REGISTER_PROPERTY_WITH_DEFAULT_NOTIFIER(PropertyContainerTest, Property2, 0)
		{
		}

		virtual ~PropertyContainerTest() {}

		static const int32_t PROPERTY1_VALUE = 123;
		static const int32_t PROPERTY1_TRUE_VALUE = 321;
		static const int32_t PROPERTY2_VALUE = 123.312;

		Int32Property Property1;
		DoubleProperty Property2;

		bool Property1Changed = false;
		bool Property2Changed = false;
		bool Property1Ok = false;
		bool Property2Ok = false;

	protected:
		virtual void validateProperty1(const AbstractProperty * prop, int32_t * value) { *value = PROPERTY1_TRUE_VALUE; }
		virtual void notifyProperty1(const AbstractProperty * prop, const int32_t * value) { Property1Changed = true; Property1Ok = *value == PROPERTY1_TRUE_VALUE; }
		virtual void notifyProperty2(const AbstractProperty * prop, const double * value) { Property2Changed = true; Property2Ok = *value == PROPERTY2_VALUE; }
	};

	TEST_CLASS(TestCore)
	{
	public:

		TEST_METHOD(TestString)
		{
			Assert::AreEqual(L"This is a string", StrUtils::format(L"This is a %s", L"string").c_str(), L"Failed to format string value");
			Assert::AreEqual(L"Value = -1", StrUtils::format(L"Value = %d", 0xFFFFFFFF).c_str(), L"Failed to format int value");
			Assert::AreEqual(L"Value = 4294967295", StrUtils::format(L"Value = %u", 0xFFFFFFFF).c_str(), L"Failed to format uint value");
			Assert::AreEqual(L"Value = -1", StrUtils::format(L"Value = %lld", 0xFFFFFFFFFFFFFFFF).c_str(), L"Failed to format int64_t value");
			Assert::AreEqual(L"Value = 18446744073709551615", StrUtils::format(L"Value = %llu", 0xFFFFFFFFFFFFFFFF).c_str(), L"Failed to format uint64_t value");
			Assert::AreEqual(L"Value = 123.456000", StrUtils::format(L"Value = %f", 123.456).c_str(), L"Failed to format float value");
			Assert::AreEqual(L"Value = 123.456", StrUtils::format(L"Value = %.3f", 123.456).c_str(), L"Failed to format float value");
			Assert::AreEqual(L"Value = 123.46", StrUtils::format(L"Value = %.2f", 123.456).c_str(), L"Failed to format float value");

			Assert::AreEqual(L"char*", StrUtils::toStr("char*").c_str(), L"Failed to convert char *");
			Assert::AreEqual(L"std::string", StrUtils::toStr(std::string("std::string")).c_str(), L"Failed to convert std::string");

			Assert::AreEqual(L"Trim", StrUtils::trim(L"\t\n Trim\n\t\t ").c_str(), L"Failed to trim string");
		}

		TEST_METHOD(TestProperty)
		{
			PropertyContainerTest obj;

			Assert::AreEqual(true, obj.Property1.getName() == L"Property1");
			Assert::AreEqual(true, obj.Property2.getName() == L"Property2");

			Assert::AreEqual(false, obj.Property1Changed);
			Assert::AreEqual(false, obj.Property2Changed);
			Assert::AreEqual(false, obj.Property1Ok);
			Assert::AreEqual(false, obj.Property2Ok);

			obj.Property1 = PropertyContainerTest::PROPERTY1_VALUE;
			obj.Property2 = PropertyContainerTest::PROPERTY2_VALUE;

			Assert::AreEqual(true, obj.Property1 == PropertyContainerTest::PROPERTY1_TRUE_VALUE);
			Assert::AreEqual(true, obj.Property2 == PropertyContainerTest::PROPERTY2_VALUE);
			Assert::AreEqual(true, obj.Property1Changed);
			Assert::AreEqual(true, obj.Property2Changed);
			Assert::AreEqual(true, obj.Property1Ok);
			Assert::AreEqual(true, obj.Property2Ok);
		}

	};
}
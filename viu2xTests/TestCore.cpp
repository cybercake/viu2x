#include "stdafx.h"
#include <iostream>
#include "CppUnitTest.h"

#include <math.h>
#include <viu2xCore/common.h>
#include <viu2xCore/gui.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace v2x;

namespace viu2xTests
{
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
			class PropertyContainerTest : protected PropertyContainer {
			public:
				Int32Property Property1;
				DoubleProperty Property2;
				StringProperty Property3;

				PropertyContainerTest() :
					REGISTER_PROPERTY_WITH_DEFAULT_VALIDATOR(PropertyContainerTest, Property1, 0),
					REGISTER_PROPERTY_WITH_DEFAULT_NOTIFIER(PropertyContainerTest, Property2, 0),
					REGISTER_PROPERTY_WITH_NOTIFIER(PropertyContainerTest, Property3, L"", doOnProperty3Change)
				{
				}

				virtual ~PropertyContainerTest() {}

				const int32_t PROPERTY1_VALUE = 123;
				const int32_t PROPERTY1_TRUE_VALUE = 321;
				const double PROPERTY2_VALUE = 123.312;

				bool Property1Changed = false;
				bool Property2Changed = false;
				bool Property1Ok = false;
				bool Property2Ok = false;

			protected:
				virtual void validateProperty1(const PropertyDescriptor * prop, int32_t * value) { *value = PROPERTY1_TRUE_VALUE; }
				virtual void notifyProperty1(const PropertyDescriptor * prop, const int32_t * value) { Property1Changed = true; Property1Ok = *value == PROPERTY1_TRUE_VALUE; }
				virtual void notifyProperty2(const PropertyDescriptor * prop, const double * value) { Property2Changed = true; Property2Ok = *value == PROPERTY2_VALUE; }
				virtual void doOnProperty3Change(const PropertyDescriptor * prop, const String * value) { }
			};

			/// Test property change notification in descendant class.
			class PropertyContainerTest2 : public PropertyContainerTest {
			public:
				virtual ~PropertyContainerTest2() {}

				bool Property3Changed = false;
			protected:
				void doOnProperty3Change(const PropertyDescriptor * prop, const String * value) { Property3Changed = true; }
			};

			PropertyContainerTest2 obj;

			Assert::AreEqual(true, obj.Property1.getName() == L"Property1");
			Assert::AreEqual(true, obj.Property2.getName() == L"Property2");

			Assert::AreEqual(false, obj.Property1Changed);
			Assert::AreEqual(false, obj.Property2Changed);
			Assert::AreEqual(false, obj.Property1Ok);
			Assert::AreEqual(false, obj.Property2Ok);

			obj.Property1 = obj.PROPERTY1_VALUE;
			obj.Property2 = obj.PROPERTY2_VALUE;
			obj.Property3 = L"aaa";

			Assert::AreEqual(true, obj.Property1 == obj.PROPERTY1_TRUE_VALUE);
			Assert::AreEqual(true, obj.Property2 == obj.PROPERTY2_VALUE);
			Assert::AreEqual(true, obj.Property1Changed);
			Assert::AreEqual(true, obj.Property2Changed);
			Assert::AreEqual(true, obj.Property3Changed);
			Assert::AreEqual(true, obj.Property1Ok);
			Assert::AreEqual(true, obj.Property2Ok);
		}

		TEST_METHOD(TestEvents)
		{
			class EventData1 : public Object {
			public:
				DEFINE_POINTERS(EventData1);
				EventData1(int data) : m_data(data) {}
				virtual ~EventData1() {}

				int m_data;
			};

			class EventTrigger : public Object {
			public:
				DEFINE_POINTERS(EventTrigger);
				EventTrigger() {}
				virtual ~EventTrigger() {}

				EventSlot OnEvent1;
				EventSlot OnEvent2;

				void triggerEvents() {
					OnEvent1.notifyEvent(Event::Shared(new Event(shared_from_this(), EventData1::Shared(new EventData1(12345)))));
					OnEvent2.notifyEvent(Event::Shared(new Event(shared_from_this(), EventData1::Shared(new EventData1(54321)))));
				}
			};

			class EventHandler1 : public Object {
			public:
				DEFINE_POINTERS(EventHandler1);
				EventHandler1() : m_eventData1(0), m_eventData2(0){}
				virtual ~EventHandler1() {}

				int m_eventData1;
				int m_eventData2;

				void doOnEvent1(Event::Shared e) {
					if (e->Data != nullptr)
						m_eventData1 = e->getDataAs<EventData1>()->m_data;
				}
				void doOnEvent2(Event::Shared e) {
					if (e->Data != nullptr)
						m_eventData2 = std::dynamic_pointer_cast<const EventData1> (e->Data)->m_data;
				}
			};

			class EventHandler2 : public Object {
			public:
				DEFINE_POINTERS(EventHandler2);
				EventHandler2() : m_eventData1(0), m_eventData2(0){}
				virtual ~EventHandler2() {}

				int m_eventData1;
				int m_eventData2;

				virtual void doOnEvent1(Event::Shared e) {
					if (e->Data != nullptr)
						m_eventData1 = std::dynamic_pointer_cast<const EventData1> (e->Data)->m_data;
				}
				void doOnEvent2(Event::Shared e) {
					if (e->Data != nullptr)
						m_eventData2 = std::dynamic_pointer_cast<const EventData1> (e->Data)->m_data;
				}
			};

			class EventHandler3 : public EventHandler2 {
			public:
				DEFINE_POINTERS(EventHandler3);
				EventHandler3() {}
				virtual ~EventHandler3() {}

				virtual void doOnEvent1(Event::Shared e) {
					// do nothing!
				}
			};

			EventTrigger::Shared trigger(new EventTrigger());
			EventHandler1::Shared handler1(new EventHandler1());
			EventHandler2::Shared handler2(new EventHandler2());
			EventHandler2::Shared handler3(new EventHandler3());
			EventHandler2::Weak wh2 = handler2;

			// Adding event handler
			trigger->OnEvent1 += EventHandler(handler1, std::bind(&EventHandler1::doOnEvent1, handler1.get(), std::placeholders::_1));
			trigger->OnEvent2 += EventHandler(handler1, std::bind(&EventHandler1::doOnEvent2, handler1.get(), std::placeholders::_1));
			trigger->OnEvent1 += EVENTHANDLER(handler2, EventHandler2::doOnEvent1);
			trigger->OnEvent2 += EVENTHANDLER(handler2, EventHandler2::doOnEvent2);
			trigger->OnEvent1 += EVENTHANDLER(handler3, EventHandler2::doOnEvent1);
			trigger->OnEvent2 += EVENTHANDLER(handler3, EventHandler2::doOnEvent2);

			Assert::AreEqual(0, handler1->m_eventData1);
			Assert::AreEqual(0, handler1->m_eventData2);
			Assert::AreEqual(0, handler2->m_eventData1);
			Assert::AreEqual(0, handler2->m_eventData2);
			Assert::AreEqual(0, handler3->m_eventData1);
			Assert::AreEqual(0, handler3->m_eventData2);

			trigger->triggerEvents();

			Assert::AreEqual(12345, handler1->m_eventData1);
			Assert::AreEqual(54321, handler1->m_eventData2);
			Assert::AreEqual(12345, handler2->m_eventData1);
			Assert::AreEqual(54321, handler2->m_eventData2);
			Assert::AreEqual(0, handler3->m_eventData1);
			Assert::AreEqual(54321, handler3->m_eventData2);

			// Test removing event handler
			handler3->m_eventData2 = 0;
			trigger->OnEvent2 -= EVENTHANDLER(handler3, EventHandler2::doOnEvent2);
			trigger->triggerEvents();
			Assert::AreEqual(0, handler3->m_eventData2);

			// Event handler connections should NEVER prevent deconstruction of 
			// the receiver object instance.
			handler2.reset();
			Assert::AreEqual(true, nullptr == wh2.lock().get());
			Assert::AreEqual(true, wh2.expired());
			trigger->triggerEvents();
		}

		TEST_METHOD(TestObject) {

			class TestObj : public Object {
			public:
				TestObj() {}
				virtual ~TestObj() {}
			};

			std::shared_ptr<TestObj> obj0(new TestObj());
			Object::Shared obj1 = obj0;
			Object::SharedConst obj2 = obj1;
			Assert::AreEqual(true, obj2 == obj1);
			Assert::AreEqual(true, obj2 == obj1->shared_from_this());
			Assert::AreEqual(true, obj2 == obj0);
		}

		TEST_METHOD(TestVector2D) {

			///////////////////
			// Vector2D Test //
			///////////////////

			Vector2DR v1(123, 321);

			Assert::AreEqual(123.0, v1.x);
			Assert::AreEqual(321.0, v1.y);
			Assert::AreEqual(false, v1.isInf());
			Assert::AreEqual(false, v1.isNaN());
			Assert::AreEqual(false, v1.isZero());

			Vector2DR v2(v1);
			Assert::AreEqual(123.0, v2.x);
			Assert::AreEqual(321.0, v2.y);
			Assert::AreEqual(true, v1 == v2);

			Vector2DR v3;
			Assert::AreEqual(0.0, v3.x);
			Assert::AreEqual(0.0, v3.y);
			Assert::AreEqual(true, v3.isZero());

			v3 = v2 + v1;
			Assert::AreEqual(246.0, v3.x);
			Assert::AreEqual(642.0, v3.y);

			v3 = v1 * 2;
			Assert::AreEqual(246.0, v3.x);
			Assert::AreEqual(642.0, v3.y);

			v3 = v3 / 2;
			Assert::AreEqual(123.0, v3.x);
			Assert::AreEqual(321.0, v3.y);

			Vector2DR v4(NAN, 123);
			Assert::AreEqual(false, v4.isInf());
			Assert::AreEqual(true, v4.isNaN());
			Assert::AreEqual(false, v4.isZero());
			Assert::AreEqual(false, v4 == v4);
			Assert::AreEqual(false, v4 != v4);

			Vector2DR v5 = v4 + v3;
			Assert::AreEqual(false, v4.isInf());
			Assert::AreEqual(true, v4.isNaN());
			Assert::AreEqual(false, v4.isZero());
		}

		TEST_METHOD(TestVector3D) {

			///////////////////
			// Vector2D Test //
			///////////////////

			Vector3DR v1(123, 321, 111);

			Assert::AreEqual(123.0, v1.x);
			Assert::AreEqual(321.0, v1.y);
			Assert::AreEqual(111.0, v1.z);
			Assert::AreEqual(false, v1.isInf());
			Assert::AreEqual(false, v1.isNaN());
			Assert::AreEqual(false, v1.isZero());

			Vector3DR v2(v1);
			Assert::AreEqual(123.0, v2.x);
			Assert::AreEqual(321.0, v2.y);
			Assert::AreEqual(111.0, v2.z);
			Assert::AreEqual(true, v1 == v2);

			Vector3DR v3;
			Assert::AreEqual(0.0, v3.x);
			Assert::AreEqual(0.0, v3.y);
			Assert::AreEqual(0.0, v3.z);
			Assert::AreEqual(true, v3.isZero());

			v3 = v2 + v1;
			Assert::AreEqual(246.0, v3.x);
			Assert::AreEqual(642.0, v3.y);
			Assert::AreEqual(222.0, v3.z);

			v3 = v1 * 2;
			Assert::AreEqual(246.0, v3.x);
			Assert::AreEqual(642.0, v3.y);
			Assert::AreEqual(222.0, v3.z);

			v3 = v3 / 2;
			Assert::AreEqual(123.0, v3.x);
			Assert::AreEqual(321.0, v3.y);
			Assert::AreEqual(111.0, v3.z);

			Vector3DR v4(NAN, NAN, 123);
			Assert::AreEqual(false, v4.isInf());
			Assert::AreEqual(true, v4.isNaN());
			Assert::AreEqual(false, v4.isZero());
			Assert::AreEqual(false, v4 == v4);
			Assert::AreEqual(false, v4 != v4);

			v4 += v3;
			Assert::AreEqual(false, v4.isInf());
			Assert::AreEqual(true, v4.isNaN());
			Assert::AreEqual(false, v4.isZero());
		}

	};
}
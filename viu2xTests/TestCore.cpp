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

			std::vector<String> tokens;
			StrUtils::split(L"", L"", tokens);
			Assert::AreEqual(0u, tokens.size());
			tokens.clear();
			StrUtils::split(L"", L"\\", tokens);
			Assert::AreEqual(0u, tokens.size());			
			tokens.clear();
			StrUtils::split(L"aaa\\bbb\\/", L"\\/", tokens);
			Assert::AreEqual(2u, tokens.size());
			Assert::AreEqual(L"aaa", tokens[0].c_str());
			Assert::AreEqual(L"bbb", tokens[1].c_str());
			tokens.clear();
			StrUtils::split(L"\\aaa\\//bbb", L"\\/", tokens);
			Assert::AreEqual(2u, tokens.size());
			Assert::AreEqual(L"aaa", tokens[0].c_str());
			Assert::AreEqual(L"bbb", tokens[1].c_str());
			tokens.clear();
			StrUtils::split(L"\\aaa\\//bbb//\\", L"\\/", tokens);
			Assert::AreEqual(2u, tokens.size());
			Assert::AreEqual(L"aaa", tokens[0].c_str());
			Assert::AreEqual(L"bbb", tokens[1].c_str());
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

				bool triggerEvents() {

					bool result = false;
					result |= OnEvent1.notifyEvent(Event::Shared(new Event(shared_from_this(), EventData1::Shared(new EventData1(12345)))));
					result |= OnEvent2.notifyEvent(Event::Shared(new Event(shared_from_this(), EventData1::Shared(new EventData1(54321)))));

					return result;
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
					e->Handled = true;
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

			Assert::AreEqual(true, trigger->triggerEvents());

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
			// Vector2D_T Test //
			///////////////////

			Vector2D v1(123, 321);

			Assert::AreEqual(123.0, v1.x);
			Assert::AreEqual(321.0, v1.y);
			Assert::AreEqual(false, v1.isInf());
			Assert::AreEqual(false, v1.isNaN());
			Assert::AreEqual(false, v1.isZero());

			Vector2D v2(v1);
			Assert::AreEqual(123.0, v2.x);
			Assert::AreEqual(321.0, v2.y);
			Assert::AreEqual(true, v1 == v2);

			Vector2D v3;
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

			Vector2D v4(NAN, 123);
			Assert::AreEqual(false, v4.isInf());
			Assert::AreEqual(true, v4.isNaN());
			Assert::AreEqual(false, v4.isZero());
			Assert::AreEqual(false, v4 == v4);
			Assert::AreEqual(false, v4 != v4);

			Vector2D v5 = v4 + v3;
			Assert::AreEqual(false, v4.isInf());
			Assert::AreEqual(true, v4.isNaN());
			Assert::AreEqual(false, v4.isZero());
		}

		TEST_METHOD(TestVector3D) {

			///////////////////
			// Vector2D_T Test //
			///////////////////

			Vector3D v1(123, 321, 111);

			Assert::AreEqual(123.0, v1.x);
			Assert::AreEqual(321.0, v1.y);
			Assert::AreEqual(111.0, v1.z);
			Assert::AreEqual(false, v1.isInf());
			Assert::AreEqual(false, v1.isNaN());
			Assert::AreEqual(false, v1.isZero());

			Vector3D v2(v1);
			Assert::AreEqual(123.0, v2.x);
			Assert::AreEqual(321.0, v2.y);
			Assert::AreEqual(111.0, v2.z);
			Assert::AreEqual(true, v1 == v2);

			Vector3D v3;
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

			Vector3D v4(NAN, NAN, 123);
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
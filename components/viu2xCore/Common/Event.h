/* Copyright (C) Hao Qin. All rights reserved. */

#pragma once

#include "Object.h"

#include <functional>
#include <vector>
#include <memory>

namespace v2x {

	/// Event object is a carrier of user-defined event data, which is sent by 
	/// the event trigger to receivers i.e. event handlers.
	class Event : public Object
	{
	public:
		DEFINE_POINTERS(Event);
		Event();
		Event(Object::Weak sender, Object::Shared data);
		virtual ~Event();

		Object::Weak Sender;
		Object::Shared Data;

		bool Handled;

		template <typename T>
		std::shared_ptr<T> getDataAs() const {
			std::shared_ptr<T> result = std::dynamic_pointer_cast<T> (Data);
			if (Data && !result)
				throw Exception(L"Event::getDataAs(): The actual event data type is unexpected!");

			return result;
		}
	};

	/// Event callback fuunction is a function that must be implemented by 
	/// event handlers. It'll be called by the event trigger.
	typedef std::function<void(Event::Shared e)> EventCallback;

	/// The event handler maintains the event handler method and the reference 
	/// to the receiver object.
	/// Event handlers must be connected to a event slot so that it receives 
	/// events. An event slot can accept multiple event handlers.
	/// To create an event handler, you need the weak reference to the receiver
	/// and the handler function created by std::bind. You can also use the 
	/// EVENTHANDLER macro to make it simpler.
	class EventHandler {
		friend class EventSlot;
	public:
		EventHandler(Object::Weak handler, EventCallback callback);
		~EventHandler();

		bool operator == (const EventHandler & slot) const;
		bool operator != (const EventHandler & slot) const;

	protected:
		bool isExpired() const;
		bool notifyEvent(Event::Shared e) const;

	private:
		Object::Weak m_handler;
		EventCallback m_callback;
	};

	/// This macro creates an event handler which can be added to an event slot
	/// @param shared_instance	The std::shared_ptr to the receiver instance
	/// @param member_function	Full function name of the handler method e.g. 
	///							MyClass::doOnEventX
#define EVENTHANDLER(shared_instance, member_function) \
	EventHandler((shared_instance), \
		std::bind(&member_function, shared_instance.get(), std::placeholders::_1))

	/// This macro creates an event handler which can be added to an event slot
	///
	/// The specified member function must be a member function of the current 
	/// class.
	///
	/// @param member_function	Full function name of the handler method e.g. 
	///							MyClass::doOnEventX
#define EVENTHANDLER_FROM_THIS(member_function) \
	EventHandler(shared_from_this(), \
		std::bind(&member_function, this, std::placeholders::_1))

	/// Event slot maintains the links to external event handlers.
	/// Objects which allows external event handling should expose an instance 
	/// to the corresponding slot. External objects can add its own event 
	/// handler to the slot to get notification when the event is triggered.
	class EventSlot {
	public:
		EventSlot();
		virtual ~EventSlot();

		void operator += (const EventHandler & handler);
		void operator -= (const EventHandler & handler);

		void clear();
		bool notifyEvent(Event::Shared e) const;
		bool notifyEvent(Object::Weak sender, Object::Shared data) const;

	private:
		typedef std::vector<EventHandler> EventHandlerList;

		EventHandlerList m_slots;

		EventHandlerList::iterator findHandler(const EventHandler & handler);
	};
}
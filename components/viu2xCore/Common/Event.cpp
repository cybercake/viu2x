/* Copyright (C) Hao Qin. All rights reserved. */

#include "Event.h"

namespace v2x {

	///////////
	// Event //
	///////////

	Event::Event() {
	}

	Event::Event(Object::Weak sender, Object::Shared data) : Sender(sender), Data(data) {
	}

	/// We always need virtual deconstructor!
	Event::~Event() {
	}

	//////////////////
	// EventHandler //
	//////////////////

	EventHandler::EventHandler(Object::Weak handler, EventCallback callback) : m_handler(handler), m_callback(callback) {
	}

	bool EventHandler::operator == (const EventHandler & slot) const {
		Object::SharedConst h1 = m_handler.lock();
		Object::SharedConst h2 = slot.m_handler.lock();
		return h1 == h2 && m_callback.target<void(Event::SharedConst e)>() == slot.m_callback.target<void(Event::SharedConst e)>();
	}

	bool EventHandler::operator != (const EventHandler & slot) const {
		Object::SharedConst h1 = m_handler.lock();
		Object::SharedConst h2 = slot.m_handler.lock();
		return h1 != h2 || m_callback.target<void(Event::SharedConst e)>() != slot.m_callback.target<void(Event::SharedConst e)>();
	}

	bool EventHandler::isExpired() const {
		return m_handler.expired();
	}

	void EventHandler::notifyEvent(Event::Shared e) const {
		Object::SharedConst handlerLock = m_handler.lock();
		if (handlerLock != NULL)
			m_callback(e);
		handlerLock.reset();
	}

	/// We always need virtual deconstructor!
	EventHandler::~EventHandler() {
	}

	///////////////
	// EventSlot //
	///////////////

	/// The default constructor
	EventSlot::EventSlot() {
	}

	/// We always need virtual deconstructor!
	EventSlot::~EventSlot() {
		m_slots.clear();
	}

	void EventSlot::operator += (const EventHandler & handler) {

		EventSlot::EventHandlerList::iterator i = findHandler(handler);
		if (i == m_slots.end())
			m_slots.push_back(handler);
	}

	void EventSlot::operator -= (const EventHandler & handler) {

		EventSlot::EventHandlerList::iterator i = findHandler(handler);
		if (i != m_slots.end())
			m_slots.erase(i);
	}

	EventSlot::EventHandlerList::iterator EventSlot::findHandler(const EventHandler & handler) {
		EventSlot::EventHandlerList::iterator result;
		for (result = m_slots.begin(); result != m_slots.end(); ++result) {
			if ((*result) == handler)
				break;
		}

		return result;
	}

	void EventSlot::clear() {
		m_slots.clear();
	}

	void EventSlot::notifyEvent(Event::Shared e) const {
		EventSlot::EventHandlerList::const_iterator i;
		for (i = m_slots.begin(); i != m_slots.end(); ++i) {
			i->notifyEvent(e);
		}
	}

	void EventSlot::notifyEvent(Object::Weak sender, Object::Shared data) const {
		Event::Shared e(new Event(sender, data));
		EventSlot::EventHandlerList::const_iterator i;
		for (i = m_slots.begin(); i != m_slots.end(); ++i) {
			i->notifyEvent(e);
		}
	}
}
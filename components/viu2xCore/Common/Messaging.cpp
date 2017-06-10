/* Copyright (C) Hao Qin. All rights reserved. */

#include "Messaging.h"

namespace v2x {

	Message::Message() : m_id(UNSPECIFIED_MESSAGE_ID) {
	}

	Message::Message(Id id, const Object::Shared & data) : m_id(id), m_data(data) {
	}

	/// We always need a virtual destructor.
	Message::~Message() {
	}

	Message::Id Message::getId() const {
		return m_id;
	}

	Object::Shared Message::getData() const {
		return m_data;
	}
}
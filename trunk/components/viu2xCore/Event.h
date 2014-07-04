/* Copyright (C) Hao Qin. All rights reserved. */

#pragma once

#include "Object.h"

#include <functional>

#include <memory>

namespace v2x {

	class Event
	{
	public:
		Event();
		virtual ~Event();

		WObject Sender;
	};

	typedef std::weak_ptr<Event> WEvent;
	typedef std::shared_ptr<Event> SEvent;

}
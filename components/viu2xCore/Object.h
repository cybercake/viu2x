/* Copyright (C) Hao Qin. All rights reserved. */

#pragma once

#include <memory>

namespace v2x {

	class Object
	{
	public:
		virtual ~Object();

		template <typename ValueType>
		bool is() { return (dynamic_cast<ValueType*>(this)) != nullptr; }
	};

	typedef std::weak_ptr<Object> WObject;
	typedef std::shared_ptr<Object> SObject;

}
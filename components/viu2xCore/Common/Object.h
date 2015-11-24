/* Copyright (C) Hao Qin. All rights reserved. */

#pragma once

#include <memory>

namespace v2x {

#define DEFINE_POINTERS(class_name) \
	typedef std::weak_ptr<class_name> Weak; \
	typedef std::weak_ptr<const class_name> ConstWeak; \
	typedef std::shared_ptr<class_name> Shared; \
	typedef std::shared_ptr<const class_name> ConstShared

	/// Root of all viu2x specific classes.
	class Object : public std::enable_shared_from_this < Object >
	{
	public:
		DEFINE_POINTERS(Object);
		Object();
		virtual ~Object();

		/*template <typename T>
		std::shared_ptr<T> as() const {
			return std::dynamic_pointer_cast<T> (shared_from_this());
		}*/
	};

}
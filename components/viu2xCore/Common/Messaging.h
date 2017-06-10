/* Copyright (C) Hao Qin. All rights reserved. */

#pragma once

#include "Object.h"

namespace v2x {

	/// Message is a general object that travels between communicating subjects.
	///
	/// It has an ID indicating its purpose/cause and a custom defined data.
	class Message : public Object {
	public:
		DEFINE_POINTERS(Message);

		/// The type for message ID
		typedef uint32_t Id;

		/// The unspecified message ID
		static const Id UNSPECIFIED_MESSAGE_ID = 0;

		/// The default constructor
		Message();

		/// The constructor refers to an external data. It is also the only way 
		/// to set the message ID.
		Message(Id id, const Object::Shared & data);

		/// We always need a virtual destructor.
		virtual ~Message();

		/// This function returns the message ID i.e. the purpose or cause of 
		/// this message.
		Id getId() const;

		/// This function simply returns the original shared data pointer.
		Object::Shared getData() const;

		/// This is a convenient function to cast data to the expected type.
		template<class T>
		std::shared_ptr<T> getDataAs() const { 
			static_assert(std::is_base_of<Object, T>::value, "Type of message data must derive from v2x::Object");
			return std::dynamic_pointer_cast<T>(m_data); 
		}

	protected:
		const Id m_id;
		Object::Shared m_data;
	};

}
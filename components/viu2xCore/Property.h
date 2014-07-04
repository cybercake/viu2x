/* Copyright (C) Hao Qin. All rights reserved. */

#pragma once

#include "String.h"
#include "Exceptions.h"
#include <map>
#include <functional>

namespace v2x {

	class AbstractProperty;
	class PropertyContainer;

	typedef std::map<String, AbstractProperty *> PropertyMap;

	class AbstractProperty {

	public:

		AbstractProperty(PropertyContainer * owner, const String & name);
		virtual ~AbstractProperty();

		const String & getName() const;

		/// This function returns a flag indicating if any value has been 
		/// assigned to the property.
		virtual const bool isSpecified() const = 0;
		/// This function set the property to unspecified state.
		virtual void unset() = 0;

		/// This function returns a flag indicating if the current value of the 
		/// property is the same as the default one.
		virtual const bool isDefault() const = 0;
		/// This function set the property value to default.
		virtual void reset() = 0;

	protected:
		/// State description for the current property instance.
		typedef enum {
			/// The value is not specified. User can take another value 
			/// according to specific use case.
			Unspecified,

			/// The value is the default one. It is usually used by 
			/// serialization to determine it the value needs to be saved.
			Default,

			/// A non-default value is specified by user.
			Specified
		} ValueState;

	private:
		PropertyMap::const_iterator m_entry;
	};

	template <typename ValueType>
	class Property : public AbstractProperty {
	public:

		/// Owner of the property object may provide a before-change handler to 
		/// verify the new value. It can modify the value through the value 
		/// pointer. If it is not allowed to change the value (e.g. at a 
		/// certain time) the owner should throw an exception.
		typedef std::function<void(const AbstractProperty *, ValueType *)> Validator;

		/// Owner of the property object usually has a after-change handler to 
		/// perform related operations (e.g. update the GUI) when a property is 
		/// changed.
		typedef std::function<void(const AbstractProperty *, const ValueType *)> Notifier;

		/// The constructor
		Property( //
			PropertyContainer * owner,
			const String & name, //
			const ValueType & defaultValue, //
			Notifier notifier = nullptr, //
			Validator validator = nullptr) : //
			AbstractProperty(owner, name), m_valueState(Default), m_value(defaultValue), m_defaultValue(defaultValue),
			m_notifier(notifier), m_validator(validator) {
		}

		virtual ~Property() {}

		/// Implicit cast
		operator const ValueType & () const {
			if (m_valueState == Unspecified) {
				String name = getName();
				throw Exception(L"Property::Implicit(): Property %s is not specified!", name.c_str());
			}

			return m_valueState == Specified ? m_value : m_defaultValue;
		}

		/// Assignment
		const ValueType & operator = (const ValueType & value) {

			if (m_value != value) {

				ValueType v = value;
				if (m_validator != nullptr)
					m_validator(this, &v);

				m_valueState = v == m_defaultValue ? Default : Specified;

				if (m_valueState == Specified) {
					m_value = v;
					if (m_notifier != nullptr)
						m_notifier(this, &m_value);
				}
			}

			if (m_valueState == Default)
				return m_defaultValue;
			else return m_value;
		}

		/// This function returns a flag indicating if any value has been 
		/// assigned to the property.
		const bool isSpecified() const { return m_valueState != Unspecified; }

		/// This function set the property to unspecified state.
		void unset() {
			if (m_valueState != Unspecified) {

				if (m_validator != nullptr)
					m_validator(this, nullptr);

				m_valueState = Unspecified;

				if (m_notifier != nullptr)
					m_notifier(this, nullptr);
			}
		}

		/// This function returns a flag indicating if the current value of the 
		/// property is the same as the default one.
		const bool isDefault() const { return m_valueState == Default; }

		/// This function set the property value to default.
		void reset() {

			if (m_valueState != Default) {

				ValueType v = m_defaultValue;
				if (m_validator != nullptr)
					m_validator(this, &v);

				m_valueState = Default;

				if (m_notifier != nullptr)
					m_notifier(this, &m_defaultValue);
			}
		}

	private:

		/// State of the current property instance.
		ValueState m_valueState;

		/// Storage for user-specified values.
		ValueType m_value;

		/// Storage for the default value.
		const ValueType m_defaultValue;

		/// The after-change event handler. It is initialized by construction and can
		/// be null if the user didn't specify any handler.
		Notifier m_notifier;

		/// The before-change event handler. It is initialized by construction and 
		/// can be null if the user didn't specify any handler.
		Validator m_validator;
	};

	typedef Property<int32_t> Int32Property;
	typedef Property<uint32_t> UInt32Property;
	typedef Property<int64_t> Int64Property;
	typedef Property<uint64_t> UInt64Property;
	typedef Property<bool> BoolProperty;
	typedef Property<float> FloatProperty;
	typedef Property<double> DoubleProperty;
	typedef Property<String> StringProperty;

	/// General host of properties. All classes expected to have properties 
	/// should be derived from this class.
	class PropertyContainer {

		friend class AbstractProperty;

	public:
		PropertyContainer();
		virtual ~PropertyContainer();

		const PropertyMap & getProperties();

	private:
		PropertyMap m_properties;
	};

#define REGISTER_PROPERTY(className, propertyName, defaultValue) \
	propertyName(this, L###propertyName, defaultValue, \
	std::bind(&className::notify##propertyName, this, std::placeholders::_1, std::placeholders::_2))

#define REGISTER_PROPERTY_WITH_NOTIFIER(className, propertyName, defaultValue, notifier) \
	propertyName(this, L###propertyName, defaultValue, \
	std::bind(&className::notifier, this, std::placeholders::_1, std::placeholders::_2))

#define REGISTER_PROPERTY_WITH_VALIDATOR(className, propertyName, defaultValue, notifier, validator) \
	propertyName(this, L###propertyName, defaultValue, \
	std::bind(&className::notifier, this, std::placeholders::_1, std::placeholders::_2), \
	std::bind(&className::validator, this, std::placeholders::_1, std::placeholders::_2))

#define REGISTER_PROPERTY_WITH_DEFAULT_NOTIFIER(className, propertyName, defaultValue) \
	propertyName(this, L###propertyName, defaultValue, \
	std::bind(&className::notify##propertyName, this, std::placeholders::_1, std::placeholders::_2))

#define REGISTER_PROPERTY_WITH_DEFAULT_VALIDATOR(className, propertyName, defaultValue) \
	propertyName(this, L###propertyName, defaultValue, \
	std::bind(&className::notify##propertyName, this, std::placeholders::_1, std::placeholders::_2), \
	std::bind(&className::validate##propertyName, this, std::placeholders::_1, std::placeholders::_2))

}
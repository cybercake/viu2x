/* Copyright (C) Hao Qin. All rights reserved. */

#include "Property.h"
#include "Exceptions.h"

namespace v2x {

	////////////////////////
	/// AbstractProperty ///
	////////////////////////

	AbstractProperty::AbstractProperty(PropertyContainer * owner, const String & name) {

		if (owner == nullptr)
			throw Exception(L"AbstractProperty::AbstractProperty(): Owner is null!");

		PropertyMap::const_iterator i = owner->m_properties.find(name);
		if (i != owner->m_properties.end())
			throw Exception(L"AbstractProperty::AbstractProperty(): Property name %s duplicated!", name.c_str());

		owner->m_properties[name] = this;
		m_entry = owner->m_properties.find(name);
	}

	/// We always need virtual deconstructor!
	AbstractProperty::~AbstractProperty() {}

	const String & AbstractProperty::getName() const {
		return m_entry->first;
	}

	/////////////////////////
	/// PropertyContainer ///
	/////////////////////////

	PropertyContainer::PropertyContainer() {}

	/// We always need virtual deconstructor!
	PropertyContainer::~PropertyContainer() {}

	const PropertyMap & PropertyContainer::getProperties() {
		return m_properties;
	}
}
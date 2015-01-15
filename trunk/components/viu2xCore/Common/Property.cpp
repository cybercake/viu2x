/* Copyright (C) Hao Qin. All rights reserved. */

#include "Property.h"
#include "Exceptions.h"

namespace v2x {

	////////////////////////
	/// PropertyDescriptor ///
	////////////////////////

	PropertyDescriptor::PropertyDescriptor(PropertyContainer * owner, const String & name) {

		if (owner == nullptr)
			throw Exception(L"PropertyDescriptor::PropertyDescriptor(): Owner is null!");

		PropertyMap::const_iterator i = owner->m_properties.find(name);
		if (i != owner->m_properties.end())
			throw Exception(L"PropertyDescriptor::PropertyDescriptor(): Property name %s duplicated!", name.c_str());

		owner->m_properties[name] = this;
		m_entry = owner->m_properties.find(name);
	}

	/// We always need virtual deconstructor!
	PropertyDescriptor::~PropertyDescriptor() {}

	const String & PropertyDescriptor::getName() const {
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
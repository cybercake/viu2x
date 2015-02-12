/* Copyright (C) Hao Qin. All rights reserved. */

#pragma once

#include <functional>

namespace v2x {

	typedef std::function < void(const void * sender, const void * data) > Listener;

#define LISTENER(owner_ptr, method_ptr) std::bind(method_ptr, owner_ptr, std::placeholders::_1, std::placeholders::_2);

	template <typename T>
	class Notifier {
	public:
		Notifier(const Listener & listener) : //
			m_subListener(std::bind(&Notifier<T>::notifyChange, this, std::placeholders::_1, std::placeholders::_2)), //
			m_beginUpdateCount(0), m_isChanged(false), m_listener(listener) {}
		virtual ~Notifier() {}

		void beginUpdate() { ++m_beginUpdateCount; }
		void endUpdate() {
			if (m_beginUpdateCount > 0) {
				--m_beginUpdateCount;
				if (m_beginUpdateCount == 0 && m_isChanged)
					notifyChange(this, this);
			}
		}
		bool isChanged() const {
			return m_isChanged;
		}

	protected:

		Listener m_subListener;

		void notifyChange(const void * sender, const void * data) {

			if (sender == nullptr)
				throw Exception(L"Notifier::notifyChange(): The input sender pointer is NULL!");

			if (m_beginUpdateCount == 0) {
				if (m_listener != nullptr)
					m_listener(sender, data);
				m_isChanged = false;
			}
			else m_isChanged = true;
		}

	private:
		int m_beginUpdateCount;
		bool m_isChanged;

		Listener m_listener;
	};

	class Specification {
	public:
		Specification() : m_isSet(false) {};
		Specification(bool isSet) : m_isSet(isSet) {};
		virtual ~Specification();

		virtual bool isSet() const { return m_isSet; }
		virtual void unset() { m_isSet = false; }

	protected:
		bool m_isSet;
	};

	template <typename T>
	class SimpleSpec : public Specification, public Notifier < SimpleSpec <T> > {

	public:

		typedef T ValueType;

		SimpleSpec(const Listener & listener = nullptr) : //
			Specification(false),
			Notifier < SimpleSpec <T> >(listener) {
			memset(&m_value, 0, sizeof(m_value));
		}
		SimpleSpec(const T & value, const Listener & listener = nullptr) :
			Specification(true),
			Notifier < SimpleSpec <T> >(listener), m_value(value) {
		}
		virtual ~SimpleSpec() {}

		// Implicit conversion
		operator T() const { return m_value; }

		// A getter
		const T & get() const { return m_value; }

		// A setter
		void set(const T & value) {
			m_value = value;
			m_isSet = true;
			notifyChange(this, &m_value);
		}

		// Assignment
		SimpleSpec <T> & operator = (const T & value) {
			m_value = value;
			m_isSet = true;
			notifyChange(this, &m_value);
			return *this;
		}

		// Assignment
		SimpleSpec <T> & operator = (const SimpleSpec<T> & value) {
			m_value = value.get();
			m_isSet = value.isSet();
			notifyChange(this, &m_value);
			return *this;
		}

		void unset() {
			if (!m_isSet)
				return;
			Specification::unset();
			notifyChange(this, &m_isSet);
		}

		bool operator == (const SimpleSpec <T> & value) const {

			if (m_isSet != value.isSet())
				return false;

			if (!m_isSet)
				return true;

			return m_value == value.get();
		}
		bool operator != (const SimpleSpec <T> & value) const {

			if (isSet() != value.isSet())
				return true;

			return m_value != value.get();
		}

	private:
		T m_value;
	};

	typedef SimpleSpec<int32_t> IntSpec;
	typedef SimpleSpec<uint32_t> UIntSpec;
	typedef SimpleSpec<int64_t> Int64Spec;
	typedef SimpleSpec<uint64_t> UInt64Spec;

	template <>
	class SimpleSpec<double> : public Specification, public Notifier < SimpleSpec <double> > {

	public:
		SimpleSpec(const Listener & listener = nullptr) : //
			Specification(false),
			Notifier < SimpleSpec <double> >(listener), m_value(0) {
		}
		SimpleSpec(const double & value, const Listener & listener = nullptr) :
			Specification(true),
			Notifier < SimpleSpec <double> >(listener), m_value(value) {
		}
		virtual ~SimpleSpec();

		// Implicit conversion
		operator double() const { return m_value; }

		// A getter
		const double & get() const { return m_value; }

		// A setter
		void set(const double & value) {
			m_value = value;
			m_isSet = true;
			notifyChange(this, &m_value);
		}

		void unset() {
			if (!m_isSet)
				return;
			Specification::unset();
			notifyChange(this, &m_isSet);
		}

		// Assignment
		SimpleSpec <double> & operator = (const double & value) {
			m_value = value;
			m_isSet = true;
			notifyChange(this, &m_value);
		}

		// Assignment
		SimpleSpec <double> & operator = (const SimpleSpec<double> & value) {
			m_value = value.get();
			m_isSet = value.isSet();
			notifyChange(this, &m_value);
			return *this;
		}

		bool operator == (const SimpleSpec <double> & value) const {

			if (isSet() != value.isSet())
				return false;

			if (!isSet())
				return true;

			if (std::isnan(m_value))
				return std::isnan(value.get());

			return m_value == value.get();
		}
		bool operator != (const SimpleSpec <double> & value) const {

			if (isSet() != value.isSet())
				return true;

			if (std::isnan(m_value))
				return !std::isnan(value.get());

			return m_value != value.get();
		}

	private:
		double m_value;
	};

	typedef SimpleSpec<double> NumberSpec;

	template <>
	class SimpleSpec<String> : public Specification, public Notifier < SimpleSpec <String> > {

	public:
		SimpleSpec(const Listener & listener = nullptr) : //
			Specification(false),
			Notifier < SimpleSpec <String> >(listener), m_value(0) {}
		SimpleSpec(const String & value, const Listener & listener = nullptr) : //
			Specification(false),
			Notifier < SimpleSpec <String> >(listener), m_value(value) {}
		virtual ~SimpleSpec();

		// Implicit conversion
		operator String() const { return m_value; }

		// A getter
		const String & get() const { return m_value; }

		// A setter
		void set(const String & value) {
			m_value = value;
			m_isSet = true;
			notifyChange(this, &m_value);
		}

		void unset() {
			if (!m_isSet)
				return;
			Specification::unset();
			notifyChange(this, &m_isSet);
		}

		// Assignment
		SimpleSpec <String> & operator = (const String & value) {
			m_value = value;
			m_isSet = true;
			notifyChange(this, &m_value);
		}

		// Assignment
		SimpleSpec <String> & operator = (const SimpleSpec<String> & value) {
			m_value = value.get();
			m_isSet = value.isSet();
			notifyChange(this, &m_value);
		}

		bool operator == (const SimpleSpec <String> & value) const {

			if (isSet() != value.isSet())
				return false;

			if (!isSet())
				return true;

			return m_value == value.get();
		}
		bool operator != (const SimpleSpec <String> & value) const {

			if (isSet() != value.isSet())
				return true;

			return m_value != value.get();
		}

	private:
		String m_value;
	};

	typedef SimpleSpec<String> StringSpec;
}
/* Copyright (C) Hao Qin. All rights reserved. */

#pragma once

#include <functional>

namespace v2x {

	template <typename T>
	using Listener = std::function < void(const T * sender, const void * data) > ;

#define LISTENER(owner_ptr, method_ptr) std::bind(method_ptr, owner_ptr, std::placeholders::_1, std::placeholders::_2);

	template <typename T>
	class Notifier {
	public:
		Notifier() : m_beginUpdateCount(0), m_isChanged(false), m_notifier(nullptr) {}
		~Notifier() {}

		void beginUpdate() { ++m_beginUpdateCount; }
		void endUpdate() {
			if (m_beginUpdateCount > 0) {
				--m_beginUpdateCount;
				if (m_beginUpdateCount == 0 && m_isChanged)
					notifyChange(this);
			}
		}
		bool isChanged() const {
			return m_isChanged;
		}

	protected:
		void notifyChange(const T * sender, const void * data) {
			if (m_beginUpdateCount == 0) {
				if (m_notifier != nullptr)
					m_notifier(sender, data);
				m_isChanged = false;
			}
			else m_isChanged = true;
		}

	private:
		int m_beginUpdateCount;
		bool m_isChanged;

		Listener<T> m_notifier;
	};

}
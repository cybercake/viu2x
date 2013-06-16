#include "Event.h"

namespace viu2x {

    Event::Event(const String & name, bool isManualReset) :
        m_name(name), m_isManualReset(isManualReset) {

#ifdef VIU2X_WINDOWS
        m_event = CreateEvent(
                      NULL, m_isManualReset, false, m_name.empty() ? NULL : m_name.c_str());
        if (m_event == NULL)
            ExceptionOs::throwLatestOsError(
                String::format(L"Event::Event(\"%s\", %s)", name.c_str(), isManualReset ? L"true" : L"false"));
#endif
    }

    Event::~Event() {

#ifdef VIU2X_WINDOWS
        if (m_event != NULL)
            CloseHandle(m_event);
#endif
    }

    void Event::resetEvent() {

        if (m_event == NULL)
            throw Exception(L"Event::resetEvent(): The internal event object is not initialized!");

#ifdef VIU2X_WINDOWS
        if (!ResetEvent(m_event))
            ExceptionOs::throwLatestOsError(L"Event::resetEvent()");
#endif
    }

    void Event::setEvent() {

        if (m_event == NULL)
            throw Exception(L"Event::resetEvent(): The internal event object is not initialized!");

#ifdef VIU2X_WINDOWS
        if (!SetEvent(m_event))
            ExceptionOs::throwLatestOsError(L"Event::setEvent()");
#endif
    }

    bool Event::wait(const Timespan & timeout) {

        if (m_event == NULL)
            throw Exception(L"Event::resetEvent(): The internal event object is not initialized!");

#ifdef VIU2X_WINDOWS
        DWORD ret = WaitForSingleObject(m_event, timeout.isInfinite() ? INFINITE : timeout.getTimeInMilliSecond());
        if (ret == WAIT_FAILED)
            ExceptionOs::throwLatestOsError(String::format(L"Event::wait(%.6f)", timeout.getTimeInSecond()));

        return ret == WAIT_OBJECT_0;
#endif
    }
}

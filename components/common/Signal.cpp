#include "Signal.h"

namespace viu2x {

    Signal::Signal() :
        m_name(L""), m_isManualReset(false) {

#ifdef VIU2X_WINDOWS
        m_event = CreateEvent(
                      NULL, m_isManualReset, false, m_name.isEmpty() ? NULL : m_name.cStr());
        if (m_event == NULL)
            OsException::throwLatestOsError(
                String::format(L"Event::Event(\"%s\", %s)", m_name.cStr(), m_isManualReset ? L"true" : L"false"));
#endif
    }

    Signal::Signal(const String & name, bool isManualReset) :
        m_name(name), m_isManualReset(isManualReset) {

#ifdef VIU2X_WINDOWS
        m_event = CreateEvent(
                      NULL, m_isManualReset, false, m_name.isEmpty() ? NULL : m_name.cStr());
        if (m_event == NULL)
            OsException::throwLatestOsError(
                String::format(L"Event::Event(\"%s\", %s)", m_name.cStr(), m_isManualReset ? L"true" : L"false"));
#endif
    }

    Signal::~Signal() {

#ifdef VIU2X_WINDOWS
        if (m_event != NULL)
            CloseHandle(m_event);
#endif
    }

    void Signal::resetSignal() {

        if (m_event == NULL)
            throw Exception(L"Event::resetSignal(): The internal event object is not initialized!");

#ifdef VIU2X_WINDOWS
        if (!ResetEvent(m_event))
            OsException::throwLatestOsError(L"Event::resetSignal()");
#endif
    }

    void Signal::setSignal() {

        if (m_event == NULL)
            throw Exception(L"Event::resetSignal(): The internal event object is not initialized!");

#ifdef VIU2X_WINDOWS
        if (!SetEvent(m_event))
            OsException::throwLatestOsError(L"Event::setSignal()");
#endif
    }

    bool Signal::waitForSignal(const Timespan & timeout) {

        if (m_event == NULL)
            throw Exception(L"Event::resetSignal(): The internal event object is not initialized!");

#ifdef VIU2X_WINDOWS
        DWORD ret = WaitForSingleObject(m_event, timeout.isInfinite() ? INFINITE : timeout.getTimeInMilliSecond());
        if (ret == WAIT_FAILED)
            OsException::throwLatestOsError(String::format(L"Event::waitForSignal(%.6f)", timeout.getTimeInSecond()));

        return ret == WAIT_OBJECT_0;
#endif
    }
}

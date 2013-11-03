#include "CriticalSection.h"
#include "Timestamp.h"

namespace viu2x {

    CriticalSection::CriticalSection() {
#ifdef VIU2X_WINDOWS
        InitializeCriticalSection(&m_cs);
#endif
    }

    CriticalSection::~CriticalSection() {
#ifdef VIU2X_WINDOWS
        DeleteCriticalSection(&m_cs);
#endif
    }

    void CriticalSection::enter() {
#ifdef VIU2X_WINDOWS
        EnterCriticalSection(&m_cs);
#endif
    }

    bool CriticalSection::tryEnter() {
#ifdef VIU2X_WINDOWS
        return TryEnterCriticalSection(&m_cs);
#endif
    }

    bool CriticalSection::tryEnter(const Timespan & timeout) {

        // The time when to stop waiting.
        Timestamp endTime = Timestamp::now() + timeout;
        while (Timestamp::now() < endTime) {
#ifdef VIU2X_WINDOWS
            if (TryEnterCriticalSection(&m_cs))
                return true;
            else Sleep(1);
#endif
        }

        return false;
    }

    void CriticalSection::leave() {
#ifdef VIU2X_WINDOWS
        LeaveCriticalSection(&m_cs);
#endif
    }

    CriticalSectionOnStack::CriticalSectionOnStack(CriticalSection & lock) : m_lock(lock) {
        m_lock.enter();
    }

    CriticalSectionOnStack::~CriticalSectionOnStack() {
        m_lock.leave();
    }
}

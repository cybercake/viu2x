#include "Mutex.h"

namespace viu2x {

    Mutex::Mutex() : m_name(L""), m_mutex(NULL) {
        createMutex();
    }

    Mutex::Mutex(const String & name) : m_name(name), m_mutex(NULL) {
        createMutex();
    }

    Mutex::~Mutex() {
        freeMutex();
    }

    void Mutex::createMutex() {

#ifdef VIU2X_WINDOWS
        // Try to open an existing mutex first if the name has been specified.
        if (m_name.length() > 0)
            m_mutex = OpenMutex(SYNCHRONIZE, false, m_name.cStr());

        // Try to create a new mutex
        if (m_mutex == NULL)
            m_mutex = CreateMutex(NULL, false, m_name.length() > 0 ? m_name.cStr() : NULL);

        // Check the created mutex handle
        if (m_mutex == NULL)
            throw Exception(L"Mutex::createMutex(): Failed to initialize mutex!");
#endif
    }

    void Mutex::freeMutex() {

#ifdef VIU2X_WINDOWS
        // Guards
        if (m_mutex == NULL)
            throw Exception(L"Mutex::freeMutex(): The mutex is not initialized!");

        // Try to release the mutex
        if (!CloseHandle(m_mutex))
            throw Exception(L"Mutex::freeMutex(): Failed to free mutex!");

        m_mutex = NULL;
#endif
    }

    void Mutex::require() {

#ifdef VIU2X_WINDOWS
        // Guards
        if (m_mutex == NULL)
            throw Exception(L"Mutex::release(): The mutex is not initialized!");

        DWORD r = WaitForSingleObject(m_mutex, INFINITE);

        if (r == WAIT_FAILED)
            throw Exception(L"Mutex::require(): Wait failed!");

        if (r == WAIT_TIMEOUT)
            throw Exception(L"Mutex::require(): Wait timeout!");
#endif
    }

    bool Mutex::tryRequire() {

#ifdef VIU2X_WINDOWS
        // Guards
        if (m_mutex == NULL)
            throw Exception(L"Mutex::tryRequire(): The mutex is not initialized!");

        DWORD r = WaitForSingleObject(m_mutex, 0);

        if (r == WAIT_FAILED)
            throw Exception(L"Mutex::tryRequire(): Wait failed!");

        if (r == WAIT_TIMEOUT)
            return false;

        return true;
#endif
    }

    bool Mutex::tryRequire(const Timespan & timeout) {

#ifdef VIU2X_WINDOWS
        // Guards
        if (m_mutex == NULL)
            throw Exception(L"Mutex::tryRequire(): The mutex is not initialized!");

        DWORD r = WaitForSingleObject(m_mutex, MIN(1, (uint32_t)timeout.getTimeInMilliSecond()));

        if (r == WAIT_FAILED)
            throw Exception(L"Mutex::tryRequire(): Wait failed!");

        if (r == WAIT_TIMEOUT)
            return false;

        return true;
#endif
    }

    void Mutex::release() {

#ifdef VIU2X_WINDOWS
        // Guards
        if (m_mutex == NULL)
            throw Exception(L"Mutex::release(): The mutex is not initialized!");

        // Try to release the mutex
        if (!ReleaseMutex(m_mutex))
            throw Exception(L"Mutex::release(): Failed to release mutex!");
#endif
    }
}

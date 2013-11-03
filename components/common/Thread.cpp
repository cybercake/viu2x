#include "Thread.h"

namespace viu2x {

#ifdef VIU2X_WINDOWS
    DWORD WINAPI viu2xThreadProc(_In_ LPVOID lpParameter) {

        DWORD result = 0;

        if (lpParameter != nullptr)
            try {
                ((Thread*)lpParameter)->_threadProc();
            } catch (Exception & e) {
                result = 1;
            }

        return result;
    }
#endif

    ConstString Thread::stateToString(State state) {
        switch (state) {
        case Initial:
            return L"Initial";
        case Creating:
            return L"Creating";
        case Created:
            return L"Created";
        case Initializing:
            return L"Initializing";
        case Initialized:
            return L"Initialized";
        case Starting:
            return L"Starting";
        case Running:
            return L"Running";
        case Suspending:
            return L"Suspending";
        case Suspended:
            return L"Suspended";
        case Resuming:
            return L"Resuming";
        case Terminating:
            return L"Terminating";
        case Terminated:
            return L"Terminated";
        case Finalizing:
            return L"Finalizing";
        case Finalized:
            return L"Finalized";
        case Error:
            return L"Error";
        default:
            throw Exception(L"Thread::stateToString(): Invalid input state!");
        }
    }

    Thread::Thread() :
#ifdef VIU2X_WINDOWS
        m_state(Initial),
        m_threadId(0),
        m_threadHandle(NULL)
#endif // VIU2X_WINDOWS
    {
        initializeThread();
    }

    Thread::~Thread() {
        try {
            finalizeThread();
        } catch (...) {
            // Do nothing...
        }
    }

    Thread::State Thread::queryState() {
        return m_state;
    }

    bool Thread::checkState(State expectedState) {
        return m_state == expectedState;
    }

    void Thread::requireState(State expectedState1, State expectedState2, ConstString caller) {
        State state = m_state; // Take a snapshot of the current state.
        if (state != expectedState1 && state != expectedState2)
            throw Exception(L"%s: The current state (%s) is not expected (%s | %s)!",
                            caller, stateToString(state),
                            stateToString(expectedState1), stateToString(expectedState2));
    }

    void Thread::requireState(State expectedState, ConstString caller) {

        State state = m_state; // Take a snapshot of the current state.
        if (state != expectedState)
            throw Exception(L"%s: The current state (%s) is not expected (%s)!",
                            caller, stateToString(state), stateToString(expectedState));
    }

    void Thread::requireState(State expectedState) {
        requireState(expectedState, L"Thread::requireState()");
    }

    void Thread::start() {

        CriticalSectionOnStack lock(m_stateLock);

        try {
            requireState(Initialized, L"Thread::start()");
            m_state = Starting;

#ifdef VIU2X_WINDOWS
            if (ResumeThread(m_threadHandle) == 0xFFFFFFFF)
                throw OsException(L"Thread::start()");
#endif // VIU2X_WINDOWS

            m_state = Running;
        } catch (...) {
            m_state = Error;
            throw;
        }
    }

    void Thread::suspend() {

        CriticalSectionOnStack lock(m_stateLock);

        try {
            requireState(Running, L"Thread::suspend()");
            m_state = Starting;

#ifdef VIU2X_WINDOWS
            if (SuspendThread(m_threadHandle) == 0xFFFFFFFF)
                throw OsException(L"Thread::suspend()");
#endif // VIU2X_WINDOWS

            m_state = Suspended;
        } catch (...) {
            m_state = Error;
            throw;
        }
    }

    void Thread::resume() {

        CriticalSectionOnStack lock(m_stateLock);

        try {
            requireState(Suspended, L"Thread::resume()");
            m_state = Resuming;

#ifdef VIU2X_WINDOWS
            if (ResumeThread(m_threadHandle) == 0xFFFFFFFF)
                throw OsException(L"Thread::resume()");
#endif // VIU2X_WINDOWS

            m_state = Running;
        } catch (...) {
            m_state = Error;
            throw;
        }
    }

    void Thread::terminate() {

        CriticalSectionOnStack lock(m_stateLock);

        try {
            requireState(Running, Suspended, L"Thread::terminate()");
            m_state = Terminating;
        } catch (...) {
            m_state = Error;
            throw;
        }
    }

    void Thread::_threadProc() {
        try {
            threadProc();
        } catch (...) {
            CriticalSectionOnStack lock(m_stateLock);
            m_state = Error;
        }

        {
            CriticalSectionOnStack lock(m_stateLock);
            if (checkState(Running) || checkState(Terminating))
                m_state = Terminated;
        }
    }

    void Thread::threadProc() {
    }

    void Thread::initializeThread() {

        CriticalSectionOnStack lock(m_stateLock);

        try {
            requireState(Initial);
            m_state = Initializing;

#ifdef VIU2X_WINDOWS
            if (m_threadHandle == NULL) {
                m_threadHandle = CreateThread(NULL, 0, viu2xThreadProc, this, CREATE_SUSPENDED, &m_threadId);
                if (m_threadHandle == NULL)
                    throw OsException(L"Thread::initializeThread()");
            }
#endif // VIU2X_WINDOWS

            m_state = Initialized;
        } catch (...) {
            m_state = Error;
            throw;
        }
    }

    void Thread::finalizeThread() {

        CriticalSectionOnStack lock(m_stateLock);

        try {
            requireState(Terminated, Error, L"Thread::finalizeThread()");
            m_state = Finalizing;

#ifdef VIU2X_WINDOWS
            if (m_threadHandle != NULL) {
                if (!CloseHandle(m_threadHandle))
                    throw OsException(L"Thread::finalizeThread()");
                m_threadHandle = NULL;
                m_threadId = 0;
            }
#endif // VIU2X_WINDOWS

            m_state = Finalized;
        } catch (...) {
            m_state = Error;
            throw;
        }
    }
}

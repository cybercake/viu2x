#ifndef THREAD_H
#define THREAD_H

#include "Viu2x.h"
#include "CriticalSection.h"

namespace viu2x {

    /**
     * @auhor   Qin
     */
    class Thread : public Object {
        public:

            typedef enum {
                Initial,
                Creating,
                Created,
                Initializing,
                Initialized,
                Starting,
                Running,
                Suspending,
                Suspended,
                Resuming,
                Terminating,
                Terminated,
                Error,
                Finalizing,
                Finalized,
            } State;

            static ConstString stateToString(State state);

            Thread();
            virtual ~Thread();

            State queryState();
            bool checkState(State expectedState);
            void requireState(State expectedState);

            void _threadProc();

            virtual void start();
            virtual void suspend();
            virtual void resume();
            virtual void terminate();

        protected:

            void requireState(State expectedState, ConstString caller);
            void requireState(State expectedState1, State expectedState2, ConstString caller);

            virtual void initializeThread();
            virtual void threadProc();
            virtual void finalizeThread();

        private:

            State m_state;
            CriticalSection m_stateLock;

#ifdef VIU2X_WINDOWS
            DWORD m_threadId;
            HANDLE m_threadHandle;
#endif
    };
}
#endif // THREAD_H

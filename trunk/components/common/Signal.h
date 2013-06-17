#ifndef EVENT_H
#define EVENT_H

#include "Viu2x.h"
#include "Timespan.h"

namespace viu2x {

    /**
     * Signals are used to create a waiting mechanism for asynchronous data
     * processing. A Signal can be reset by starting processing and get set
     * when the processing is finished. Threads that wait for the signal will
     * be blocked until the signal is set by the owner.
     *
     * @auhor   Qin
     */
    class Signal : public Object {

        public:
            Signal();
            Signal(const String & name, bool isManualReset);
            virtual ~Signal();

            void resetSignal();
            void setSignal();
            bool waitForSignal(const Timespan & timeout);

        protected:

        private:

            /**
             * Name of the event. Set by construction. If there is already an
             * event object with the same name, the constructor will throw an
             * exception. This field can be empty.
             */
            const String m_name;

            /**
             * A flag indicating if the user has to reset the event manually
             * after getting it. Set by construction.
             */
            const bool m_isManualReset;

#ifdef VIU2X_WINDOWS
            HANDLE m_event;
#endif
    };

}

#endif // EVENT_H

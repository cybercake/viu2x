#ifndef CRITICALSECTION_H
#define CRITICALSECTION_H

#include "Viu2x.h"
#include "Timespan.h"

namespace viu2x {

    /**
     * @auhor   Qin
     */
    class CriticalSection : public Object {
        public:
            CriticalSection();
            virtual ~CriticalSection();

            void enter();
            bool tryEnter();
            bool tryEnter(const Timespan & timeout);
            void leave();

        private:

#ifdef VIU2X_WINDOWS
            CRITICAL_SECTION m_cs;
#endif

    };

    /**
     * @auhor   Qin
     */
    class CriticalSectionOnStack : public Object {
        public:
            CriticalSectionOnStack(CriticalSection & lock);
            ~CriticalSectionOnStack();
        private:
            CriticalSection & m_lock;
    };

}

#endif // CRITICALSECTION_H

#ifndef CRITICALSECTION_H
#define CRITICALSECTION_H

#include "Viu2x.h"
#include "Timespan.h"

namespace viu2x {

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

}

#endif // CRITICALSECTION_H

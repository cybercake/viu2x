#ifndef MUTEX_H
#define MUTEX_H

#include "Viu2x.h"
#include "Timespan.h"

namespace viu2x {

    class Mutex : public Object {

        public:
            Mutex();
            Mutex(const String & name);
            virtual ~Mutex();

            void require();
            bool tryRequire();
            bool tryRequire(const Timespan & timeout);

            void release();

        private:
            const String m_name;

#ifdef VIU2X_WINDOWS
            HANDLE m_mutex;
#endif

            void createMutex();
            void freeMutex();
    };
}
#endif // MUTEX_H

#ifndef THREAD_H
#define THREAD_H

#include "Viu2x.h"

namespace viu2x {

    class Thread : public Object {
        public:
            Thread();
            virtual ~Thread();

            virtual void start() = 0;
            virtual void suspend() = 0;
            virtual void resume() = 0;
            virtual void terminate() = 0;

        protected:
            virtual void run() = 0;

        private:
    };
}
#endif // THREAD_H

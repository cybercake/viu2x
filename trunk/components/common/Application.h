#ifndef APPLICATION_H
#define APPLICATION_H

#include "Viu2x.h"

namespace viu2x {

    class Application : public Object {
        public:
            Application();
            virtual ~Application();

            virtual void initialize() = 0;
            virtual void run() = 0;
            virtual void finalize() = 0;
    };
}
#endif // APPLICATION_H

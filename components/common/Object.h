#ifndef OBJECT_H
#define OBJECT_H

#include "stdint.h"

namespace viu2x {

    /**
     * The common root of all viu2x classes.
     *
     * @author  Qin
     */
    class Object {
        public:
            Object();
            virtual ~Object();

            virtual uint32_t getHash();
    };

}

#endif // OBJECT_H

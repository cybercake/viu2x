#include "Object.h"

namespace viu2x {

    Object::Object() {
        // Nothing needs to be done.
    }

    Object::~Object() {
        // Nothing needs to be done.
    }

    uint32_t Object::getHash() {
        return (uint32_t)this;
    }
}

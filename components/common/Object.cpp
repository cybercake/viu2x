#include "Object.h"

namespace viu2x {

    Object::Object() {
        // Nothing needs to be done.
    }

    Object::~Object() {
        // Nothing needs to be done.
    }

    uint64_t Object::getHash() {
        return (uint64_t)this;
    }
}

#ifndef POINTER_CPP
#define POINTER_CPP

#include "Pointer.hpp"

namespace viu2x {

    template<typename T>
    inst<T>::inst() : shared<T>(new T()) {}

}
#endif // POINTER_CPP

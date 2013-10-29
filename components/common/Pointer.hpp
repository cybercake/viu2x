#ifndef POINTER_H
#define POINTER_H

#include <stddef.h>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>

namespace viu2x {

    template <typename T> using scoped = boost::scoped_ptr<T>;

    template <typename T> using shared = boost::shared_ptr<T>;

    template <typename T>
    class inst : public shared<T> {
        public:
            inst();
    };

    template <typename T> using weak = boost::weak_ptr<T>;

}

#include "Pointer.cpp"

#endif // POINTER_H

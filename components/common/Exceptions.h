#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include "Types.h"

namespace viu2x {

    class Exception {

        public:
            Exception(const String & message);
            virtual ~Exception();

            const String & getMessage();

        protected:
            String m_message;
    };

    class ExceptionOs : public Exception {

        public:
            ExceptionOs(const String & message);
            virtual ~ExceptionOs();

            static void throwLatestOsError(const String & caller);
    };
}
#endif // EXCEPTIONS_H

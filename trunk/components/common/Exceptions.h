#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include "Types.h"
#include <deque>

namespace viu2x {

    /**
     * The common base of all viu2x exceptions.
     *
     * @test Types/Exceptions
     *
     * @author  Qin
     */
    class Exception {

        public:
            Exception(const String & message, const Exception * internalException = NULL);
            virtual ~Exception();

            const String & getMessage() const;
            const std::deque<String> & getMessages() const;

        protected:
            std::deque<String> m_messages;
    };

    /**
     * The common base of all viu2x exceptions.
     *
     * @test Types/Exceptions
     *
     * @author  Qin
     */
    class OsException : public Exception {

        public:
            OsException(const String & caller, const Exception * internalException = NULL);
            virtual ~OsException();

            static void throwLatestOsError(const String & caller, const Exception * internalException = NULL);

        private:
            static String getLastErrorMessage();
    };
}
#endif // EXCEPTIONS_H

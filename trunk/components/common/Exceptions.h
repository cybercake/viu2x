#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include "Types.h"
#include "Object.h"
#include <deque>

namespace viu2x {

    /**
     * The common base of all viu2x exceptions.
     *
     * @test Types/Exceptions
     *
     * @author  Qin
     */
    class Exception : public Object {

        public:
            Exception(const String & message, ...);
            Exception(const String & message, va_list params);
            Exception(const Exception * internalException, const String & message, ...);
            Exception(const Exception & internalException, const String & message, ...);
            Exception(const Exception * internalException, const String & message, va_list params);
            Exception(const Exception & internalException, const String & message, va_list params);
            virtual ~Exception();

            const String & getMessage() const;
            const std::deque<String> & getMessages() const;

        protected:
            std::deque<String> m_messages;

            void initialize(const String & message, const Exception * internalException = nullptr);
            void initialize(const String & message, const Exception & internalException);
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
            OsException(const String & caller, const Exception * internalException = nullptr);
            virtual ~OsException();

            static void throwLatestOsError(const String & caller, const Exception * internalException = nullptr);

        private:
            static String getLastErrorMessage();
    };
}
#endif // EXCEPTIONS_H

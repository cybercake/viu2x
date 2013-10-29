#include "Exceptions.h"
#include "OS.h"

namespace viu2x {

    ///////////////
    // Exception //
    ///////////////

    Exception::Exception(const String & message, ...) {

        va_list params;
        va_start(params, message);
        initialize(String::format(message, params), NULL);
        va_end(params);
    }

    Exception::Exception(const String & message, va_list params) {

        initialize(String::format(message, params), NULL);
    }

    Exception::Exception(const Exception * internalException, const String & message, ...) {

        va_list params;
        va_start(params, message);
        initialize(String::format(message, params), internalException);
        va_end(params);
    }

    Exception::Exception(const Exception & internalException, const String & message, ...) {

        va_list params;
        va_start(params, message);
        initialize(String::format(message, params), internalException);
        va_end(params);
    }

    Exception::Exception(const Exception * internalException, const String & message, va_list params) {

        initialize(String::format(message, params), internalException);
    }

    Exception::Exception(const Exception & internalException, const String & message, va_list params) {

        initialize(String::format(message, params), internalException);
    }

    Exception::~Exception() {
    }

    void Exception::initialize(const String & message, const Exception * internalException) {

        // Copy all previous exception messages
        if (internalException != NULL)
            m_messages.assign(internalException->getMessages().begin(), internalException->getMessages().end());

        m_messages.push_front(message);
    }

    void Exception::initialize(const String & message, const Exception & internalException) {

        // Copy all previous exception messages
        m_messages.assign(internalException.getMessages().begin(), internalException.getMessages().end());

        m_messages.push_front(message);
    }

    const String & Exception::getMessage() const {
        return m_messages.front();
    }

    const std::deque<String> & Exception::getMessages() const {
        return m_messages;
    }

    /////////////////
    // OsException //
    /////////////////

    OsException::OsException(const String & caller, const Exception * internalException) :
        Exception(caller + L": " + getLastErrorMessage(), internalException) {
    }

    OsException::~OsException() {
        // Nothing needs to be done.
    }

    String OsException::getLastErrorMessage() {

#ifdef VIU2X_WINDOWS
        DWORD errorCode = GetLastError();
        LPWSTR outputStr = NULL;

        FormatMessage(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            errorCode,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPWSTR)&outputStr,
            0, NULL);

        if (outputStr == NULL)
            throw Exception(L"OsException::getLastErrorMessage(): Failed to retrieve system error message!");

        String message(outputStr);
        LocalFree(outputStr);

        return message;
#endif
    }

    void OsException::throwLatestOsError(const String & caller, const Exception * internalException) {
        throw OsException(caller, internalException);
    }
}

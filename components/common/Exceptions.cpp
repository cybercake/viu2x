#include "Exceptions.h"
#include "OS.h"

namespace viu2x {

    ///////////////
    // Exception //
    ///////////////

    Exception::Exception(const String & message,
                         const Exception * internalException) {

        if (internalException != NULL)
            m_messages.assign(internalException->getMessages().begin(), internalException->getMessages().end());

        m_messages.push_front(message);
    }

    Exception::~Exception() {
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

#include "Exceptions.h"
#include "OS.h"

namespace viu2x {

    ///////////////
    // Exception //
    ///////////////

    Exception::Exception(const String & format, ...) {
        va_list params;
        va_start(params, format);
        m_message = String::vformat(format, params);
        va_end(params);
    }

    Exception::Exception(const std::string & format, ...) {
        va_list params;
        va_start(params, format);
        m_message = String::vformat(format, params);
        va_end(params);
    }

    Exception::Exception(const std::wstring & format, ...) {
        va_list params;
        va_start(params, format);
        m_message = String::vformat(format, params);
        va_end(params);
    }

    Exception::Exception(const char * format, ...) {
        va_list params;
        va_start(params, format);
        m_message = String::vformat(format, params);
        va_end(params);
    }

    Exception::Exception(const wchar_t * format, ...) {
        va_list params;
        va_start(params, format);
        m_message = String::vformat(format, params);
        va_end(params);
    }

    Exception::~Exception() {
    }

    const String & Exception::getMessage() {
        return m_message;
    }

    /////////////////
    // ExceptionOs //
    /////////////////

    ExceptionOs::ExceptionOs(const String & message) :
        Exception(message) {
    }

    ExceptionOs::~ExceptionOs() {}

    void ExceptionOs::throwLatestOsError(const String & caller) {

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
            throw Exception(L"ExceptionOs::throwLatestOsError(): Failed to retrieve system error message!");

        String message(outputStr);
        LocalFree(outputStr);

        if (caller.length() > 0)
            throw ExceptionOs(caller + L": " + message);
        else throw ExceptionOs(message);
#endif
    }
}

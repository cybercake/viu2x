#include "String.h"
#include "Exceptions.h"

namespace viu2x {

    //! The default constructor resulting in an empty string.
    String::String() : std::wstring() {
    }

    //! The constructor copying data from another String object.
    //!
    //! @param [in]     source      The String object from which the data will
    //!                             be copied.
    String::String(const String & source) : std::wstring(source) {
        // Nothing needs to be done here
    }

    //! The constructor copying data from a std::wstring object.
    //!
    //! @param [in]     source      The string object from which the data will
    //!                             be copied.
    String::String(const std::wstring & source) : std::wstring(source) {
        // Nothing needs to be done here
    }

    //! The constructor copying data from a std::string object.
    //!
    //! @param [in]     source      The string object from which the data will
    //!                             be copied.
    String::String(const std::string & source) : std::wstring() {
        copyFromMultibyte(source.c_str());
    }

    //! The constructor copying data from a unicode character array.
    //!
    //! @param [in]     source      The unicode character array from which the
    //!                             data will be copied.
    String::String(const wchar_t * source) : std::wstring(source) {
        // Nothing needs to be done here
    }

    //! The constructor copying data from a multibype character array.
    //!
    //! @param [in]     source      The multibyte character array from which
    //!                             the data will be copied.
    String::String(const char * source) : std::wstring() {
        copyFromMultibyte(source);
    }

    //! The destructor must be virtual!
    String::~String() {
        // Nothing needs to be done here
    }

    //! The assignment operator copying data from a std:string object.
    //!
    //! @param [in]     source      The multibyte string from which the data
    //!                             will be copied.
    //!
    //! @return         A reference to the current String object
    String & String::operator = (const std::string & op) {
        copyFromMultibyte(op.c_str());
        return *this;
    }

    //! The assignment operator copying data from a multibype character array.
    //!
    //! @param [in]     source      The multibyte character array from which
    //!                             the data will be copied.
    //!
    //! @return         A reference to the current String object
    String & String::operator = (const char * op) {
        copyFromMultibyte(op);
        return *this;
    }

    bool String::operator == (const std::string & op) {
        String temp(op);
        return (*this) == temp;
    }

    bool String::operator == (const char * op) {
        String temp(op);
        return (*this) == temp;
    }

    bool String::operator != (const std::string & op) {
        String temp(op);
        return (*this) != temp;
    }

    bool String::operator != (const char * op) {
        String temp(op);
        return (*this) != temp;
    }

    String & String::operator += (const char * op) {
        String temp(op);
        std::wstring::append(temp);
        return *this;
    }

    String & String::operator += (const wchar_t * op) {
        String temp(op);
        std::wstring::append(temp);
        return *this;
    }

    String & String::operator += (const std::string & op) {
        String temp(op);
        std::wstring::append(temp);
        return *this;
    }

    String & String::operator += (const std::wstring & op) {
        String temp(op);
        std::wstring::append(temp);
        return *this;
    }

    String String::operator + (const char * op) {
        String result(*this);
        result += op;
        return result;
    }

    String String::operator + (const wchar_t * op) {
        String result(*this);
        result += op;
        return result;
    }

    String String::operator + (const std::string & op) {
        String result(*this);
        result += op;
        return result;
    }

    String String::operator + (const std::wstring & op) {
        String result(*this);
        result += op;
        return result;
    }

    String String::format(const String & fmt, ...) {
        va_list param;
        va_start(param, fmt);
        String result = format(fmt.c_str(), param);
        va_end(param);
        return result;
    }

    String String::format(const std::string & fmt, ...) {
        String f(fmt);
        va_list param;
        va_start(param, fmt);
        String result = format(f.c_str(), param);
        va_end(param);
        return result;
    }

    String String::format(const std::wstring & fmt, ...) {
        va_list param;
        va_start(param, fmt);
        String result = format(fmt.c_str(), param);
        va_end(param);
        return result;
    }

    String String::format(const char * fmt, ...) {
        String f(fmt);
        va_list param;
        va_start(param, fmt);
        String result = format(f.c_str(), param);
        va_end(param);
        return result;
    }

    String String::format(const wchar_t * fmt, ...) {
        String f(fmt);
        va_list param;
        va_start(param, fmt);
        String result = format(fmt, param);
        va_end(param);
        return result;
    }

    String String::format(const wchar_t * fmt, va_list params) {

        const int initialSize = 8;
        wchar_t initialBuf[initialSize];

        int requiredSize = _vsnwprintf(initialBuf, initialSize, fmt, params) + 1;

        if (requiredSize > initialSize) {

            wchar_t * buf = new wchar_t[requiredSize];
            const int actualSize = _vsnwprintf(initialBuf, requiredSize, fmt, params);
            String result(buf);
            delete [] buf;

            result.resize(actualSize);
            return result;

        } else {
            String result(initialBuf);
            result.resize(requiredSize - 1);
            return result;
        }
    }

    //! Convert a multibyte string to unicode and assign to the current
    //! string object.
    //!
    //! @param [in]     multibyteString     The multibyte zero-terminated string to convert.
    //!
    //! @return         None
    void String::copyFromMultibyte(const char * multibyteString) {

#ifdef VIU2X_WINDOWS
        if (multibyteString == NULL)
            throw Exception(L"String::setAsMultibyte(): The input string is NULL!");

        // Get the required buffer size
        int requiredLength = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, multibyteString, -1, NULL, 0);
        if (requiredLength <= 0)
            ExceptionOs::throwLatestOsError(L"String::setAsMultibyte()");

        // Allocate the buffer
        wchar_t * dst = new wchar_t[requiredLength + 1];
        if (dst == NULL)
            throw Exception(L"String::setAsMultibyte(): Failed to allocate temporary buffer!");

        try {
            // Convert to unicode
            int writtenLength = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, multibyteString, -1, dst, requiredLength + 1);

            // Check if conversion was successful
            if (writtenLength == 0)
                ExceptionOs::throwLatestOsError(L"String::setAsMultibyte()");

            // Check if the string length before/after conversion are identical.
            if (writtenLength != requiredLength)
                throw Exception(L"String::setAsMultibyte(): The length of the converted string is not expected!");
        } catch (...) {
            delete [] dst;
            throw;
        }

        std::wstring::assign(dst);
        delete [] dst;
#endif
    }
}

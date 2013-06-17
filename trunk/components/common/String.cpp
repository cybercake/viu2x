#include "String.h"
#include "Exceptions.h"

namespace viu2x {

    const String String::EMPTY = String(L"");

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

    String String::format(const String & format, ...) {
        va_list params;
        va_start(params, format);
        String result = vformat(format, params);
        va_end(params);
        return result;
    }

    String String::format(const std::string & format, ...) {
        va_list params;
        va_start(params, format);
        String result = vformat(format, params);
        va_end(params);
        return result;
    }

    String String::format(const std::wstring & format, ...) {
        va_list params;
        va_start(params, format);
        String result = vformat(format, params);
        va_end(params);
        return result;
    }

    String String::format(const char * format, ...) {
        va_list params;
        va_start(params, format);
        String result = vformat(format, params);
        va_end(params);
        return result;
    }

    String String::format(const wchar_t * format, ...) {
        va_list params;
        va_start(params, format);
        String result = vformat(format, params);
        va_end(params);
        return result;
    }

    String String::vformat(const String & format, va_list params) {
        String result = vformat(format.c_str(), params);
        return result;
    }

    String String::vformat(const std::string & format, va_list params) {
        String f(format);
        String result = vformat(f.c_str(), params);
        return result;
    }

    String String::vformat(const std::wstring & format, va_list params) {
        String result = vformat(format.c_str(), params);
        return result;
    }

    String String::vformat(const char * format, va_list params) {
        String f(format);
        String result = vformat(f.c_str(), params);
        return result;
    }

    String String::vformat(const wchar_t * format, va_list params) {

        const size_t maxSize = 0x7FFFFFFF;
        const size_t initialSize = 256;

        size_t bufferSize = initialSize;

        while (bufferSize < maxSize) {

            wchar_t * buf = new wchar_t[bufferSize];
            size_t actualSize = _vsnwprintf(buf, bufferSize, format, params) + 1; // reserve place for '\0'

            // Check if it succeeded.
            if (actualSize > 0 && actualSize < bufferSize) {
                String result(buf);
                delete [] buf;
                return result;
            } else {
                delete [] buf;
                bufferSize *= 2;
            }
        }

        throw Exception(L"String::format(): Failed to format string! String length out of range!");
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
            OsException::throwLatestOsError(L"String::setAsMultibyte()");

        // Allocate the buffer
        wchar_t * dst = new wchar_t[requiredLength + 1];
        if (dst == NULL)
            throw Exception(L"String::setAsMultibyte(): Failed to allocate temporary buffer!");

        try {
            // Convert to unicode
            int writtenLength = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, multibyteString, -1, dst, requiredLength + 1);

            // Check if conversion was successful
            if (writtenLength == 0)
                OsException::throwLatestOsError(L"String::setAsMultibyte()");

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

#include "String.h"
#include "Exceptions.h"
#include <sstream>
#include <stdexcept>

namespace viu2x {

    const String String::EMPTY = String(L"");
    const String String::DEFAULT_INT32_FORMAT = L"%d";
    const String String::DEFAULT_UINT32_FORMAT = L"%u";
    const String String::DEFAULT_INT64_FORMAT = L"%lld";
    const String String::DEFAULT_UINT64_FORMAT = L"%llu";
    const String String::DEFAULT_FLOAT_FORMAT = L"%f";
    const String String::DEFAULT_DOUBLE_FORMAT = L"%f";

    //! The default constructor resulting in an empty string.
    String::String() : m_string() {
    }

    //! The constructor copying data from another String object.
    //!
    //! @param [in]     source      The String object from which the data will
    //!                             be copied.
    String::String(const String & source) : m_string(source.cStr()) {
        // Nothing needs to be done here
    }

    //! The constructor copying data from a std::wstring object.
    //!
    //! @param [in]     source      The string object from which the data will
    //!                             be copied.
    String::String(const std::wstring & source) : m_string(source) {
        // Nothing needs to be done here
    }

    //! The constructor copying data from a std::string object.
    //!
    //! @param [in]     source      The string object from which the data will
    //!                             be copied.
    String::String(const std::string & source) : m_string() {
        copyFromMultibyte(source.c_str());
    }

    //! The constructor copying data from a multibype character array.
    //!
    //! @param [in]     source      The multibyte character array from which
    //!                             the data will be copied.
    String::String(const char * source) : m_string() {
        copyFromMultibyte(source);
    }

    //! The constructor copying data from a unicode character array.
    //!
    //! @param [in]     source      The unicode character array from which the
    //!                             data will be copied.
    String::String(ConstString source) : m_string(source) {
        // Nothing needs to be done here
    }

    String::String(const char source) : m_string() {
        copyFromMultibyte(&source, 1);
    }

    String::String(const Char source) : m_string(&source, 1) {
    }

    String::String (const int & value, const String & format) :
        m_string(String::format(format, value).cStr()) {
        // @todo: Make it more efficient!
    }

    String::String (const unsigned int & value, const String & format) :
        m_string(String::format(format, value).cStr()) {
        // @todo: Make it more efficient!
    }

    String::String (const int64_t & value, const String & format)  :
        m_string(String::format(format, value).cStr()) {
        // @todo: Make it more efficient!
    }

    String::String (const uint64_t & value, const String & format)  :
        m_string(String::format(format, value).cStr()) {
        // @todo: Make it more efficient!
    }

    String::String (const float & value, const String & format)  :
        m_string(String::format(format, value).cStr()) {
        // @todo: Make it more efficient!
    }

    String::String (const double & value, const String & format) :
        m_string(String::format(format, value).cStr()) {
        // @todo: Make it more efficient!
    }

    //! The destructor must be virtual!
    String::~String() {
        // Nothing needs to be done here
    }

    //! The assignment operator copying data from a std:string object.
    //!
    //! @param [in]     op          The multibyte string from which the data
    //!                             will be copied.
    //!
    //! @return         A reference to the current String object
    const String & String::operator = (const String & op) {
        m_string = op.cStr();
        return *this;
    }

    //! The assignment operator copying data from a std:string object.
    //!
    //! @param [in]     op          The multibyte string from which the data
    //!                             will be copied.
    //!
    //! @return         A reference to the current String object
    const String & String::operator = (const std::string & op) {
        copyFromMultibyte(op.c_str());
        return *this;
    }

    //! The assignment operator copying data from a std:string object.
    //!
    //! @param [in]     op          The multibyte string from which the data
    //!                             will be copied.
    //!
    //! @return         A reference to the current String object
    const String & String::operator = (const std::wstring & op) {
        m_string = op;
        return *this;
    }

    //! The assignment operator copying data from a multibype character array.
    //!
    //! @param [in]     op          The multibyte character array from which
    //!                             the data will be copied.
    //!
    //! @return         A reference to the current String object
    const String & String::operator = (const char * op) {
        copyFromMultibyte(op);
        return *this;
    }

    //! The assignment operator copying data from a multibype character array.
    //!
    //! @param [in]     op          The multibyte character array from which
    //!                             the data will be copied.
    //!
    //! @return         A reference to the current String object
    const String & String::operator = (ConstString op) {
        m_string = op;
        return *this;
    }

    //! The assignment operator copying data from a multibype character array.
    //!
    //! @param [in]     op          The multibyte character array from which
    //!                             the data will be copied.
    //!
    //! @return         A reference to the current String object
    const String & String::operator = (const char op) {
        copyFromMultibyte(&op, 1);
        return *this;
    }

    //! The assignment operator copying data from a multibype character array.
    //!
    //! @param [in]     op          The multibyte character array from which
    //!                             the data will be copied.
    //!
    //! @return         A reference to the current String object
    const String & String::operator = (const Char op) {
        m_string = op;
        return *this;
    }

    bool String::operator == (const String & op) const {
        return m_string == op.cStr();
    }

    bool String::operator == (const std::string & op) const {
        String temp(op);
        return m_string == temp.cStr();
    }

    bool String::operator == (const std::wstring & op) const {
        return m_string == op;
    }

    bool String::operator == (const char * op) const {
        String temp(op);
        return m_string == temp.cStr();
    }

    bool String::operator == (ConstString op) const {
        return m_string == op;
    }

    bool String::operator == (const char op) const {
        String temp(op);
        return m_string == temp.cStr();
    }

    bool String::operator == (const Char op) const {
        return m_string == std::wstring(&op, 1);
    }

    bool String::operator != (const String & op) const {
        return m_string != op.cStr();
    }

    bool String::operator != (const std::string & op) const {
        String temp(op);
        return m_string != temp.cStr();
    }

    bool String::operator != (const std::wstring & op) const {
        return m_string != op;
    }

    bool String::operator != (const char * op) const {
        String temp(op);
        return m_string != temp.cStr();
    }

    bool String::operator != (ConstString op) const {
        return m_string != op;
    }

    bool String::operator != (const char op) const {
        String temp(op);
        return m_string != temp.cStr();
    }

    bool String::operator != (const Char op) const {
        return m_string != std::wstring(&op, 1);
    }

    String & String::operator += (const std::string & op) {
        String temp(op);
        m_string.append(temp.cStr());
        return *this;
    }

    String & String::operator += (const std::wstring & op) {
        m_string.append(op);
        return *this;
    }

    String & String::operator += (const String & op) {
        m_string.append(op.cStr());
        return *this;
    }

    String & String::operator += (const char * op) {
        String temp(op);
        m_string.append(temp.cStr());
        return *this;
    }

    String & String::operator += (ConstString op) {
        m_string.append(op);
        return *this;
    }

    String & String::operator += (const char op) {
        String temp(op);
        m_string.append(temp.cStr());
        return *this;
    }

    String & String::operator += (const Char op) {
        m_string.append(&op, 1);
        return *this;
    }

    String String::operator + (const std::string & op) const {
        String result(*this);
        result += op;
        return result;
    }

    String String::operator + (const std::wstring & op) const {
        String result(*this);
        result += op;
        return result;
    }

    String String::operator + (const String & op) const {
        String result(*this);
        result += op;
        return result;
    }

    String String::operator + (const char * op) const {
        String result(*this);
        result += op;
        return result;
    }

    String String::operator + (ConstString op) const {
        String result(*this);
        result += op;
        return result;
    }

    String String::operator + (const char op) const {
        String result(*this);
        result += op;
        return result;
    }

    String String::operator + (const Char op) const {
        String result(*this);
        result += op;
        return result;
    }

    size_t String::length() const {
        return m_string.length();
    }

    bool String::isEmpty() const {
        return m_string.empty();
    }

    ConstString String::cStr() const {
        return m_string.c_str();
    }

    const std::wstring & String::stdStr() const {
        return m_string;
    }

    String String::trim() const {

        if (m_string.empty())
            return String();

        size_t left = 0;
        for (; left < length(); ++left)
            if (!isspace(m_string.c_str()[left]))
                break;

        size_t right = length() - 1;
        for (; ; --right) {
            if (!isspace(m_string.c_str()[right]))
                break;
            if (right == 0)
                break;
        }

        if (right > left) {
            return m_string.substr(left, right - left + 1);
        } else return String();
    }

    String String::subStr(size_t pos, size_t len) const {
        return m_string.substr(pos, len);
    }

    size_t String::indexOf(const String & subStr, const size_t searchStart) const {
        return m_string.find(subStr.cStr(), searchStart);
    }

    size_t String::indexOf(const std::string & subStr, const size_t searchStart) const {
        return m_string.find(String(subStr).cStr(), searchStart);
    }

    size_t String::indexOf(const std::wstring & subStr, const size_t searchStart) const {
        return m_string.find(subStr, searchStart);
    }

    size_t String::indexOf(const char * subStr, const size_t searchStart) const {
        return m_string.find(String(subStr).cStr(), searchStart);
    }

    size_t String::indexOf(ConstString subStr, const size_t searchStart) const {
        return m_string.find(subStr, searchStart);
    }

    size_t String::indexOf(const char character, const size_t searchStart) const {
        return m_string.find(String(character).cStr(), searchStart);
    }

    size_t String::indexOf(const Char character, const size_t searchStart) const {
        return m_string.find(character, searchStart);
    }

    int32_t String::asInt32() const {
        try {
            return std::stol(m_string, nullptr, 0);
        } catch (std::invalid_argument & e) {
            throw Exception(L"String::asInt32(): %s is not a valid integer expression!", m_string.c_str());
        } catch (std::out_of_range & e) {
            throw Exception(L"String::asInt32(): Failed to convert %s! Out of range!", m_string.c_str());
        } catch(...) {
            throw Exception(L"String::asInt32(): Failed to convert %s to integer!", m_string.c_str());
        }
    }

    uint32_t String::asUInt32() const {
        try {
            return std::stoul(m_string, nullptr, 0);
        } catch (std::invalid_argument & e) {
            throw Exception(L"String::asUInt32(): %s is not a valid integer expression!", m_string.c_str());
        } catch (std::out_of_range & e) {
            throw Exception(L"String::asUInt32(): Failed to convert %s! Out of range!", m_string.c_str());
        } catch(...) {
            throw Exception(L"String::asUInt32(): Failed to convert %s to integer!", m_string.c_str());
        }
    }

    int64_t String::asInt64() const {
        try {
            return std::stoll(m_string, nullptr, 0);
        } catch (std::invalid_argument & e) {
            throw Exception(L"String::asInt64(): %s is not a valid integer expression!", m_string.c_str());
        } catch (std::out_of_range & e) {
            throw Exception(L"String::asInt64(): Failed to convert %s! Out of range!", m_string.c_str());
        } catch(...) {
            throw Exception(L"String::asInt64(): Failed to convert %s to integer!", m_string.c_str());
        }
    }

    uint64_t String::asUInt64() const {
        try {
            return std::stoull(m_string, nullptr, 0);
        } catch (std::invalid_argument & e) {
            throw Exception(L"String::asUInt64(): %s is not a valid integer expression!", m_string.c_str());
        } catch (std::out_of_range & e) {
            throw Exception(L"String::asUInt64(): Failed to convert %s! Out of range!", m_string.c_str());
        } catch(...) {
            throw Exception(L"String::asUInt64(): Failed to convert %s to integer!", m_string.c_str());
        }
    }

    float String::asFloat() const {
        try {
            return std::stof(m_string);
        } catch (std::invalid_argument & e) {
            throw Exception(L"String::asFloat(): %s is not a valid decimal expression!", m_string.c_str());
        } catch (std::out_of_range & e) {
            throw Exception(L"String::asFloat(): Failed to convert %s! Out of range!", m_string.c_str());
        } catch(...) {
            throw Exception(L"String::asFloat(): Failed to convert %s to integer!", m_string.c_str());
        }
    }

    double String::asDouble() const {
        try {
            return std::stod(m_string);
        } catch (std::invalid_argument & e) {
            throw Exception(L"String::asDouble(): %s is not a valid decimal expression!", m_string.c_str());
        } catch (std::out_of_range & e) {
            throw Exception(L"String::asDouble(): Failed to convert %s! Out of range!", m_string.c_str());
        } catch(...) {
            throw Exception(L"String::asDouble(): Failed to convert %s to integer!", m_string.c_str());
        }
    }

    bool String::tryParse(int32_t & value) const {
        try {
            value = asInt32();
            return true;
        } catch (...) {
            return false;
        }
    }

    bool String::tryParse(uint32_t & value) const {
        try {
            value = asUInt32();
            return true;
        } catch (...) {
            return false;
        }
    }

    bool String::tryParse(int64_t & value) const {
        try {
            value = asInt64();
            return true;
        } catch (...) {
            return false;
        }
    }

    bool String::tryParse(uint64_t & value) const {
        try {
            value = asUInt64();
            return true;
        } catch (...) {
            return false;
        }
    }

    bool String::tryParse(float & value) const {
        try {
            value = asFloat();
            return true;
        } catch (...) {
            return false;
        }
    }

    bool String::tryParse(double & value) const {
        try {
            value = asDouble();
            return true;
        } catch (...) {
            return false;
        }
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

    String String::format(ConstString format, ...) {
        va_list params;
        va_start(params, format);
        String result = vformat(format, params);
        va_end(params);
        return result;
    }

    String String::vformat(const String & format, va_list params) {
        String result = vformat(format.cStr(), params);
        return result;
    }

    String String::vformat(const std::string & format, va_list params) {
        String f(format);
        String result = vformat(f.cStr(), params);
        return result;
    }

    String String::vformat(const std::wstring & format, va_list params) {
        String result = vformat(format.c_str(), params);
        return result;
    }

    String String::vformat(const char * format, va_list params) {
        String f(format);
        String result = vformat(f.cStr(), params);
        return result;
    }

    String String::vformat(ConstString format, va_list params) {

        const size_t maxSize = 0x7FFFFFFF;
        const size_t initialSize = 256;

        size_t bufferSize = initialSize;

        while (bufferSize < maxSize) {

            Char * buf = new Char[bufferSize];
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
    void String::copyFromMultibyte(const char * multibyteString, size_t len) {

#ifdef VIU2X_WINDOWS
        if (multibyteString == NULL)
            throw Exception(L"String::setAsMultibyte(): The input string is NULL!");

        // Get the required buffer size
        int requiredLength = len;
        if (requiredLength == 0)
            requiredLength = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, multibyteString, -1, NULL, 0);
        if (requiredLength <= 0)
            OsException::throwLatestOsError(L"String::setAsMultibyte()");

        // Allocate the buffer
        Char * dst = new Char[requiredLength + 1];
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

        m_string.assign(dst);
        delete [] dst;
#endif
    }
}

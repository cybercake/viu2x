#ifndef STRING_H
#define STRING_H

#include "OS.h"
#include <string>

namespace viu2x {

    typedef wchar_t Char;

    typedef const Char * ConstString;

    //! A Unicode string class extending the STL wstring with built-in multi-
    //! byte character set support.
    //!
    //! @test   Types/String
    //!
    //! @author Qin
    class String {
        public:
            static const String EMPTY;
            static const String DEFAULT_INT32_FORMAT;
            static const String DEFAULT_UINT32_FORMAT;
            static const String DEFAULT_INT64_FORMAT;
            static const String DEFAULT_UINT64_FORMAT;
            static const String DEFAULT_FLOAT_FORMAT;
            static const String DEFAULT_DOUBLE_FORMAT;

            String();

            String(const String & source);
            String(const std::wstring & source);
            String(const std::string & source);
            String(const char * source);
            String(ConstString source);
            String(const char source);
            String(const Char source);

            explicit String (const int & value, const String & format = DEFAULT_INT32_FORMAT);
            explicit String (const unsigned int & value, const String & format = DEFAULT_UINT32_FORMAT);
            explicit String (const int64_t & value, const String & format = DEFAULT_INT64_FORMAT);
            explicit String (const uint64_t & value, const String & format = DEFAULT_UINT64_FORMAT);
            explicit String (const float & value, const String & format = DEFAULT_FLOAT_FORMAT);
            explicit String (const double & value, const String & format = DEFAULT_DOUBLE_FORMAT);
            virtual ~String();

            const String & operator = (const String & op);
            const String & operator = (const std::string & op);
            const String & operator = (const std::wstring & op);
            const String & operator = (const char * op);
            const String & operator = (ConstString op);
            const String & operator = (const char op);
            const String & operator = (const Char op);

            bool operator == (const String & op) const;
            bool operator == (const std::string & op) const;
            bool operator == (const std::wstring & op) const;
            bool operator == (const char * op) const;
            bool operator == (ConstString op) const;
            bool operator == (const char op) const;
            bool operator == (const Char op) const;

            bool operator != (const String & op) const;
            bool operator != (const std::string & op) const;
            bool operator != (const std::wstring & op) const;
            bool operator != (const char * op) const;
            bool operator != (ConstString op) const;
            bool operator != (const char op) const;
            bool operator != (const Char op) const;

            String & operator += (const String & op);
            String & operator += (const std::string & op);
            String & operator += (const std::wstring & op);
            String & operator += (const char * op);
            String & operator += (ConstString op);
            String & operator += (const char op);
            String & operator += (const Char op);

            String operator + (const String & op) const;
            String operator + (const std::string & op) const;
            String operator + (const std::wstring & op) const;
            String operator + (const char * op) const;
            String operator + (ConstString op) const;
            String operator + (const char op) const;
            String operator + (const Char op) const;

            size_t length() const;
            bool isEmpty() const;
            ConstString cStr() const;
            const std::wstring & stdStr() const;
            String trim() const;
            String subStr(size_t startIndex, size_t numberOfChars) const;

            size_t indexOf(const String & subStr, const size_t searchStart = 0) const;
            size_t indexOf(const std::string & subStr, const size_t searchStart = 0) const;
            size_t indexOf(const std::wstring & subStr, const size_t searchStart = 0) const;
            size_t indexOf(const char * subStr, const size_t searchStart = 0) const;
            size_t indexOf(ConstString subStr, const size_t searchStart = 0) const;
            size_t indexOf(const char character, const size_t searchStart = 0) const;
            size_t indexOf(const Char character, const size_t searchStart = 0) const;

            int32_t asInt32() const;
            uint32_t asUInt32() const;
            int64_t asInt64() const;
            uint64_t asUInt64() const;
            float asFloat() const;
            double asDouble() const;

            bool tryParse(int32_t & value) const;
            bool tryParse(uint32_t & value) const;
            bool tryParse(int64_t & value) const;
            bool tryParse(uint64_t & value) const;
            bool tryParse(float & value) const;
            bool tryParse(double & value) const;

            static String format(const String & format, ...);
            static String format(const std::string & format, ...);
            static String format(const std::wstring & format, ...);
            static String format(const char * format, ...);
            static String format(ConstString format, ...);

            static String vformat(const String & format, va_list params);
            static String vformat(const std::string & format, va_list params);
            static String vformat(const std::wstring & format, va_list params);
            static String vformat(const char * format, va_list params);
            static String vformat(ConstString format, va_list params);

        private:
            std::wstring m_string;
            void copyFromMultibyte(const char * multibyteString, size_t len = 0);
    };

}
#endif // STRING_H

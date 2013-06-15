#ifndef STRING_H
#define STRING_H

#include "OS.h"
#include <string>

namespace viu2x {

    //! A Unicode string class extending the STL wstring with built-in multi-
    //! byte character set support.
    //!
    //! @test   Types/String
    //!
    //! @author Qin
    class String : public std::wstring {
        public:
            String();
            String(const String & source);
            String(const std::wstring & source);
            String(const std::string & source);
            String(const wchar_t * source);
            String(const char * source);
            virtual ~String();

            String & operator = (const std::string & op);
            String & operator = (const char * op);

            bool operator == (const std::string & op);
            bool operator == (const char * op);
            bool operator != (const std::string & op);
            bool operator != (const char * op);

            String & operator += (const char * op);
            String & operator += (const wchar_t * op);
            String & operator += (const std::string & op);
            String & operator += (const std::wstring & op);

            String operator + (const char * op);
            String operator + (const wchar_t * op);
            String operator + (const std::string & op);
            String operator + (const std::wstring & op);

            static String format(const String & fmt, ...);
            static String format(const std::string & fmt, ...);
            static String format(const std::wstring & fmt, ...);
            static String format(const char * fmt, ...);
            static String format(const wchar_t * fmt, ...);

        private:
            void copyFromMultibyte(const char * multibyteString);
            static String format(const wchar_t * fmt, va_list params);
    };

}
#endif // STRING_H

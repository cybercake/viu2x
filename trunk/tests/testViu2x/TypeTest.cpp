#include <iostream>
#include <boost/test/unit_test.hpp>
#include <stdio.h>

#include "common/Viu2x.h"
#include "common/Timespan.h"
#include "common/Timestamp.h"

using namespace std;

BOOST_AUTO_TEST_SUITE (Types) // name of the test suite is stringtest

BOOST_AUTO_TEST_CASE (String) {

    viu2x::String s1, s2, s;
    BOOST_REQUIRE_NO_THROW(s1 = L"This is a string");
    BOOST_REQUIRE_NO_THROW(s2 = "This is a string");
    BOOST_REQUIRE_EQUAL(true, s1 == s2);
    BOOST_REQUIRE_EQUAL(true, s1 + s2 == "This is a stringThis is a string");
    BOOST_REQUIRE_EQUAL(true, s1 + s2 == L"This is a stringThis is a string");
    BOOST_REQUIRE_EQUAL(false, s1 + s2 != "This is a stringThis is a string");
    BOOST_REQUIRE_EQUAL(false, s1 + s2 != L"This is a stringThis is a string");
    BOOST_REQUIRE_EQUAL(true, s1 + s2 != "This is a string");
    BOOST_REQUIRE_EQUAL(true, s1 + s2 != L"This is a string");
    BOOST_REQUIRE_NO_THROW(s = s1);
    BOOST_REQUIRE_NO_THROW(s += s2);
    BOOST_REQUIRE_EQUAL(true, s1 + s2 == s);

#define COMPARE_BASE L"Format(1, 2.10, aaa, This is a string)\n"
    BOOST_REQUIRE_NO_THROW(s = viu2x::String::format(L"Format(%d, %.2f, %s, %s)\n", 1, 2.1, L"aaa", s2.c_str()));
    BOOST_REQUIRE_EQUAL(true, s == COMPARE_BASE);

    BOOST_REQUIRE_NO_THROW(s = viu2x::String::format("Format(%d, %.2f, %s, %s)\n", 1, 2.1, L"aaa", s2.c_str()));
    BOOST_REQUIRE_EQUAL(true, s == COMPARE_BASE);

    BOOST_REQUIRE_NO_THROW(s = viu2x::String::format(viu2x::String(L"Format(%d, %.2f, %s, %s)\n"), 1, 2.1, L"aaa", s2.c_str()));
    BOOST_REQUIRE_EQUAL(true, s == COMPARE_BASE);

    BOOST_REQUIRE_NO_THROW(s = viu2x::String::format(std::wstring(L"Format(%d, %.2f, %s, %s)\n"), 1, 2.1, L"aaa", s2.c_str()));
    BOOST_REQUIRE_EQUAL(true, s == COMPARE_BASE);

    BOOST_REQUIRE_NO_THROW(s = viu2x::String::format(std::string("Format(%d, %.2f, %s, %s)\n"), 1, 2.1, L"aaa", s2.c_str()));
    BOOST_REQUIRE_EQUAL(true, s == COMPARE_BASE);
}

BOOST_AUTO_TEST_CASE (Timespan) {

    viu2x::Timespan ts1, ts2;

    BOOST_REQUIRE_NO_THROW(ts1 = viu2x::Timespan(12345));
    BOOST_REQUIRE_NO_THROW(ts2 = viu2x::Timespan(54321));

    BOOST_REQUIRE_EQUAL(12345 * 1e-9, ts1.getTimeInSecond());
    BOOST_REQUIRE_EQUAL(54321 * 1e-9, ts2.getTimeInSecond());
    BOOST_REQUIRE_EQUAL(12345 * 1e-6, ts1.getTimeInMilliSecond());
    BOOST_REQUIRE_EQUAL(54321 * 1e-6, ts2.getTimeInMilliSecond());
    BOOST_REQUIRE_EQUAL(12345 * 1e-3, ts1.getTimeInMicroSecond());
    BOOST_REQUIRE_EQUAL(54321 * 1e-3, ts2.getTimeInMicroSecond());
    BOOST_REQUIRE_EQUAL(12345, ts1.getTimeInNanoSecond());
    BOOST_REQUIRE_EQUAL(54321, ts2.getTimeInNanoSecond());

    BOOST_REQUIRE_EQUAL(66666, (ts1 + ts2).getTimeInNanoSecond());
    BOOST_REQUIRE_EQUAL(66666, (ts2 + ts1).getTimeInNanoSecond());
    BOOST_REQUIRE_EQUAL(12345 - 54321, (ts1 - ts2).getTimeInNanoSecond());
    BOOST_REQUIRE_EQUAL(54321 - 12345, (ts2 - ts1).getTimeInNanoSecond());

    BOOST_REQUIRE_EQUAL(12345 * 10, (ts1 * 10).getTimeInNanoSecond());
    BOOST_REQUIRE_EQUAL(12345 * 10, (ts1 * 10.0).getTimeInNanoSecond());

    BOOST_REQUIRE_EQUAL(1234, (ts1 / 10).getTimeInNanoSecond());
    BOOST_REQUIRE_EQUAL(1234, (ts1 / 10.0).getTimeInNanoSecond());

    BOOST_REQUIRE_EQUAL(false, ts1 == ts2);
    BOOST_REQUIRE_EQUAL(true, ts1 != ts2);
    BOOST_REQUIRE_EQUAL(true, ts1 < ts2);
    BOOST_REQUIRE_EQUAL(true, ts1 <= ts2);
    BOOST_REQUIRE_EQUAL(false, ts1 > ts2);
    BOOST_REQUIRE_EQUAL(false, ts1 >= ts2);

    ts1 = ts2;
    BOOST_REQUIRE_EQUAL(true, ts1 == ts2);
    BOOST_REQUIRE_EQUAL(false, ts1 != ts2);
    BOOST_REQUIRE_EQUAL(false, ts1 < ts2);
    BOOST_REQUIRE_EQUAL(true, ts1 <= ts2);
    BOOST_REQUIRE_EQUAL(false, ts1 > ts2);
    BOOST_REQUIRE_EQUAL(true, ts1 >= ts2);
}

BOOST_AUTO_TEST_CASE (Timestamp) {

    viu2x::Timestamp t1, t2;
    viu2x::Timespan ts1, ts2;

    BOOST_REQUIRE_NO_THROW(t1 = viu2x::Timestamp(12345));
    BOOST_REQUIRE_NO_THROW(t2 = viu2x::Timestamp(54321));
    BOOST_REQUIRE_NO_THROW(ts1 = viu2x::Timespan(54321));
    BOOST_REQUIRE_NO_THROW(ts2 = viu2x::Timespan(12345));

    BOOST_REQUIRE_EQUAL(12345 * 1e-9, t1.getTimeInSecond());
    BOOST_REQUIRE_EQUAL(54321 * 1e-9, t2.getTimeInSecond());
    BOOST_REQUIRE_EQUAL(12345 * 1e-6, t1.getTimeInMilliSecond());
    BOOST_REQUIRE_EQUAL(54321 * 1e-6, t2.getTimeInMilliSecond());
    BOOST_REQUIRE_EQUAL(12345 * 1e-3, t1.getTimeInMicroSecond());
    BOOST_REQUIRE_EQUAL(54321 * 1e-3, t2.getTimeInMicroSecond());
    BOOST_REQUIRE_EQUAL(12345u, t1.getTimeInNanoSecond());
    BOOST_REQUIRE_EQUAL(54321u, t2.getTimeInNanoSecond());

    BOOST_REQUIRE_EQUAL(66666, (t1 + ts1).getTimeInNanoSecond());
    BOOST_REQUIRE_EQUAL(66666, (t2 + ts2).getTimeInNanoSecond());
    BOOST_REQUIRE_EQUAL(12345 - 54321, (t1 - ts1).getTimeInNanoSecond());
    BOOST_REQUIRE_EQUAL(54321 - 12345, (t2 - ts2).getTimeInNanoSecond());

    BOOST_REQUIRE_EQUAL(false, t1 == t2);
    BOOST_REQUIRE_EQUAL(true, t1 != t2);
    BOOST_REQUIRE_EQUAL(true, t1 < t2);
    BOOST_REQUIRE_EQUAL(true, t1 <= t2);
    BOOST_REQUIRE_EQUAL(false, t1 > t2);
    BOOST_REQUIRE_EQUAL(false, t1 >= t2);

    t1 = t2;
    BOOST_REQUIRE_EQUAL(true, t1 == t2);
    BOOST_REQUIRE_EQUAL(false, t1 != t2);
    BOOST_REQUIRE_EQUAL(false, t1 < t2);
    BOOST_REQUIRE_EQUAL(true, t1 <= t2);
    BOOST_REQUIRE_EQUAL(false, t1 > t2);
    BOOST_REQUIRE_EQUAL(true, t1 >= t2);
}

BOOST_AUTO_TEST_CASE (Exceptions) {

    viu2x::String s;
    std::deque<viu2x::String> messages;
    try {
        try {
            throw viu2x::Exception(L"Message1", NULL);
        } catch (viu2x::Exception & e) {
            throw viu2x::Exception(L"Message2", &e);
        }
    } catch (viu2x::Exception & e) {
        BOOST_REQUIRE_NO_THROW(messages.assign(e.getMessages().begin(), e.getMessages().end()));
        s = e.getMessage();
    }

    BOOST_REQUIRE_EQUAL(2u, messages.size());
    BOOST_REQUIRE_EQUAL(true, s == L"Message2");
    BOOST_REQUIRE_EQUAL(true, messages[1] == L"Message1");
    BOOST_REQUIRE_EQUAL(true, messages[0] == L"Message2");

    // @todo Test OsException
}

BOOST_AUTO_TEST_SUITE_END( )

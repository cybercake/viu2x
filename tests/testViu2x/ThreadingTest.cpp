#include <iostream>
#include <boost/test/unit_test.hpp>
#include <stdio.h>
//#include <boost/shared_ptr.hpp>

#include "common/Viu2x.h"
#include "common/CriticalSection.h"
#include "common/Mutex.h"
#include "common/Signal.h"

using namespace std;

BOOST_AUTO_TEST_SUITE (Threading) // name of the test suite is stringtest

BOOST_AUTO_TEST_CASE (CriticalSection) {

    boost::shared_ptr<viu2x::CriticalSection> cs;

    BOOST_REQUIRE_NO_THROW(cs.reset(new viu2x::CriticalSection()));

    BOOST_REQUIRE_NO_THROW(cs->enter());

    BOOST_REQUIRE_NO_THROW(cs->enter());

    BOOST_REQUIRE_NO_THROW(cs->leave());

    BOOST_REQUIRE_NO_THROW(cs->leave());

    BOOST_REQUIRE_NO_THROW(cs.reset());
}

BOOST_AUTO_TEST_CASE (Mutex) {

    boost::shared_ptr<viu2x::Mutex> m;

    BOOST_REQUIRE_NO_THROW(m.reset(new viu2x::Mutex()));

    BOOST_REQUIRE_NO_THROW(m->require());

    BOOST_REQUIRE_NO_THROW(m->require());

    BOOST_REQUIRE_NO_THROW(m->release());

    BOOST_REQUIRE_NO_THROW(m->release());

    BOOST_REQUIRE_NO_THROW(m.reset());
}

BOOST_AUTO_TEST_CASE (Signal) {

    boost::shared_ptr<viu2x::Signal> s;

    BOOST_REQUIRE_NO_THROW(s.reset(new viu2x::Signal(L"", false)));

    BOOST_REQUIRE_EQUAL(false, s->waitForSignal(viu2x::Timespan(100000000))); // Wait 100ms

    BOOST_REQUIRE_NO_THROW(s->setSignal());

    BOOST_REQUIRE_EQUAL(true, s->waitForSignal(viu2x::Timespan::Zero));

    BOOST_REQUIRE_NO_THROW(s.reset());
}

BOOST_AUTO_TEST_SUITE_END( )

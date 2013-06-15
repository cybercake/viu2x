#include <iostream>
#include <boost/test/included/unit_test.hpp>
#include <stdio.h>

#include "common/Viu2x.h"

using namespace std;

boost::test_suite* init_unit_test_suite (int, char* []) {

    boost::framework::master_test_suite ().p_name.value = "Viu2xTests";

    return NULL;
}

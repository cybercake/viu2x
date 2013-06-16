#ifndef TYPES_H
#define TYPES_H

#include "CompilationSettings.h"
#include "String.h"
#include <string>
#include <inttypes.h>

namespace viu2x {

#define MAX_INT32 2147483647
#define MIN_INT32 ¨C2147483648
#define MAX_UNSIGNED_INT32 4294967295

#define MAX_INT64 9223372036854775807
#define MIN_INT64 ¨C9223372036854775808
#define MAX_UNSIGNED_INT64 18446744073709551615

    typedef double Real;

#define MIN(a, b) (a) < (b) ? (a) : (b)
#define MAX(a, b) (a) > (b) ? (a) : (b)

}

#endif // TYPES_H

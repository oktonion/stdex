#define _STDEX_TESTS_CHECK_HEADER_NAME "some not stdex header"
#include "./testsuit/macro_defines_check.h"

#undef _STDEX_TESTS_CHECK_HEADER_NAME
#define _STDEX_TESTS_CHECK_HEADER_NAME "../stdex/include/core.h"
#include _STDEX_TESTS_CHECK_HEADER_NAME
#include "./testsuit/macro_defines_check.h"

#undef _STDEX_TESTS_CHECK_HEADER_NAME
#define _STDEX_TESTS_CHECK_HEADER_NAME "../stdex/include/algorithm.hpp"
#include _STDEX_TESTS_CHECK_HEADER_NAME
#include "./testsuit/macro_defines_check.h"

#undef _STDEX_TESTS_CHECK_HEADER_NAME
#define _STDEX_TESTS_CHECK_HEADER_NAME "../stdex/include/chrono.hpp"
#include _STDEX_TESTS_CHECK_HEADER_NAME
#include "./testsuit/macro_defines_check.h"

#undef _STDEX_TESTS_CHECK_HEADER_NAME
#define _STDEX_TESTS_CHECK_HEADER_NAME "../stdex/include/condition_variable.hpp"
#include _STDEX_TESTS_CHECK_HEADER_NAME
#include "./testsuit/macro_defines_check.h"

#undef _STDEX_TESTS_CHECK_HEADER_NAME
#define _STDEX_TESTS_CHECK_HEADER_NAME "../stdex/include/cstdint.hpp"
#include _STDEX_TESTS_CHECK_HEADER_NAME
#include "./testsuit/macro_defines_check.h"

#undef _STDEX_TESTS_CHECK_HEADER_NAME
#define _STDEX_TESTS_CHECK_HEADER_NAME "../stdex/include/iterator.hpp"
#include _STDEX_TESTS_CHECK_HEADER_NAME
#include "./testsuit/macro_defines_check.h"

#undef _STDEX_TESTS_CHECK_HEADER_NAME
#define _STDEX_TESTS_CHECK_HEADER_NAME "../stdex/include/mutex.hpp"
#include _STDEX_TESTS_CHECK_HEADER_NAME
#include "./testsuit/macro_defines_check.h"

#undef _STDEX_TESTS_CHECK_HEADER_NAME
#define _STDEX_TESTS_CHECK_HEADER_NAME "../stdex/include/ratio.hpp"
#include _STDEX_TESTS_CHECK_HEADER_NAME
#include "./testsuit/macro_defines_check.h"

#undef _STDEX_TESTS_CHECK_HEADER_NAME
#define _STDEX_TESTS_CHECK_HEADER_NAME "../stdex/include/sstream.hpp"
#include _STDEX_TESTS_CHECK_HEADER_NAME
#include "./testsuit/macro_defines_check.h"

#undef _STDEX_TESTS_CHECK_HEADER_NAME
#define _STDEX_TESTS_CHECK_HEADER_NAME "../stdex/include/string.hpp"
#include _STDEX_TESTS_CHECK_HEADER_NAME
#include "./testsuit/macro_defines_check.h"

#undef _STDEX_TESTS_CHECK_HEADER_NAME
#define _STDEX_TESTS_CHECK_HEADER_NAME "../stdex/include/system_error.hpp"
#include _STDEX_TESTS_CHECK_HEADER_NAME
#include "./testsuit/macro_defines_check.h"

#undef _STDEX_TESTS_CHECK_HEADER_NAME
#define _STDEX_TESTS_CHECK_HEADER_NAME "../stdex/include/thread.hpp"
#include _STDEX_TESTS_CHECK_HEADER_NAME
#include "./testsuit/macro_defines_check.h"

#undef _STDEX_TESTS_CHECK_HEADER_NAME
#define _STDEX_TESTS_CHECK_HEADER_NAME "../stdex/include/type_traits.hpp"
#include _STDEX_TESTS_CHECK_HEADER_NAME
#include "./testsuit/macro_defines_check.h"

#undef _STDEX_TESTS_CHECK_HEADER_NAME
#define _STDEX_TESTS_CHECK_HEADER_NAME "../stdex/include/typeindex.hpp"
#include _STDEX_TESTS_CHECK_HEADER_NAME
#include "./testsuit/macro_defines_check.h"

int main(void)
{
    return 0;
}
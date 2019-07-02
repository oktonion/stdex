#define _STDEX_TESTS_STRINGIZE_NX(A) #A
#define _STDEX_TESTS_STRINGIZE(A) _STDEX_TESTS_STRINGIZE_NX(A)

#define _STDEX_TESTS_MAKE_INCLUDE_HEADER_NAME1(name)\
    ../stdex/include/name.hpp
#define _STDEX_TESTS_MAKE_INCLUDE_HEADER_NAME(name)\
    _STDEX_TESTS_STRINGIZE(_STDEX_TESTS_MAKE_INCLUDE_HEADER_NAME1(name))

#define _STDEX_TESTS_INCLUDE_HPP_NAME _STDEX_TESTS_MAKE_INCLUDE_HEADER_NAME(_STDEX_TESTS_CHECK_HEADER_NAME)

#define _STDEX_TESTS_CHECK_HEADER_NAME some_not_stdex_header
#include "./testsuit/macro_defines_check.h"


#undef _STDEX_TESTS_CHECK_HEADER_NAME
#define _STDEX_TESTS_CHECK_HEADER_NAME core
#include "../stdex/include/core.h"
#include "./testsuit/macro_defines_check.h"

#undef _STDEX_TESTS_CHECK_HEADER_NAME
#define _STDEX_TESTS_CHECK_HEADER_NAME algorithm
#include _STDEX_TESTS_INCLUDE_HPP_NAME
#include "./testsuit/macro_defines_check.h"

#undef _STDEX_TESTS_CHECK_HEADER_NAME
#define _STDEX_TESTS_CHECK_HEADER_NAME chrono
#include _STDEX_TESTS_INCLUDE_HPP_NAME
#include "./testsuit/macro_defines_check.h"

#undef _STDEX_TESTS_CHECK_HEADER_NAME
#define _STDEX_TESTS_CHECK_HEADER_NAME condition_variable
#include _STDEX_TESTS_INCLUDE_HPP_NAME
#include "./testsuit/macro_defines_check.h"

#undef _STDEX_TESTS_CHECK_HEADER_NAME
#define _STDEX_TESTS_CHECK_HEADER_NAME cstdint
#include _STDEX_TESTS_INCLUDE_HPP_NAME
#include "./testsuit/macro_defines_check.h"

#undef _STDEX_TESTS_CHECK_HEADER_NAME
#define _STDEX_TESTS_CHECK_HEADER_NAME iterator
#include _STDEX_TESTS_INCLUDE_HPP_NAME
#include "./testsuit/macro_defines_check.h"

#undef _STDEX_TESTS_CHECK_HEADER_NAME
#define _STDEX_TESTS_CHECK_HEADER_NAME mutex
#include _STDEX_TESTS_INCLUDE_HPP_NAME
#include "./testsuit/macro_defines_check.h"

#undef _STDEX_TESTS_CHECK_HEADER_NAME
#define _STDEX_TESTS_CHECK_HEADER_NAME ratio
#include _STDEX_TESTS_INCLUDE_HPP_NAME
#include "./testsuit/macro_defines_check.h"

#undef _STDEX_TESTS_CHECK_HEADER_NAME
#define _STDEX_TESTS_CHECK_HEADER_NAME sstream
#include _STDEX_TESTS_INCLUDE_HPP_NAME
#include "./testsuit/macro_defines_check.h"

#undef _STDEX_TESTS_CHECK_HEADER_NAME
#define _STDEX_TESTS_CHECK_HEADER_NAME string
#include _STDEX_TESTS_INCLUDE_HPP_NAME
#include "./testsuit/macro_defines_check.h"

#undef _STDEX_TESTS_CHECK_HEADER_NAME
#define _STDEX_TESTS_CHECK_HEADER_NAME system_error
#include _STDEX_TESTS_INCLUDE_HPP_NAME
#include "./testsuit/macro_defines_check.h"

#undef _STDEX_TESTS_CHECK_HEADER_NAME
#define _STDEX_TESTS_CHECK_HEADER_NAME thread
#include _STDEX_TESTS_INCLUDE_HPP_NAME
#include "./testsuit/macro_defines_check.h"

#undef _STDEX_TESTS_CHECK_HEADER_NAME
#define _STDEX_TESTS_CHECK_HEADER_NAME type_traits
#include _STDEX_TESTS_INCLUDE_HPP_NAME
#include "./testsuit/macro_defines_check.h"

#undef _STDEX_TESTS_CHECK_HEADER_NAME
#define _STDEX_TESTS_CHECK_HEADER_NAME typeindex
#include _STDEX_TESTS_INCLUDE_HPP_NAME
#include "./testsuit/macro_defines_check.h"



int main(void)
{
    return 0;
}

// This will generate compiler error if any internal defines of stdex library are found

#ifndef _STDEX_TESTS_CHECK_HEADER_NAME
    #error "Macro _STDEX_TESTS_CHECK_HEADER_NAME was not defined before 'macro_defines_check.h' was used"
#endif

#ifdef STRINGIZE_HELPER
    #error "Macro STRINGIZE_HELPER was not undef in "\
    _STDEX_TESTS_CHECK_HEADER_NAME
#endif

#ifdef STRINGIZE
    #error "Macro STRINGIZE was not undef in "\
    _STDEX_TESTS_CHECK_HEADER_NAME
#endif

#ifdef WARNING
    #error "Macro WARNING was not undef in "\
    _STDEX_TESTS_CHECK_HEADER_NAME
#endif

#ifdef _STDEX_HAS_FEATURE_BUILTIN
    #error "Macro _STDEX_HAS_FEATURE_BUILTIN was not undef in "\
    _STDEX_TESTS_CHECK_HEADER_NAME
#endif

#ifdef _STDEX_HAS_EXTENSION_BUILTIN
    #error "Macro _STDEX_HAS_EXTENSION_BUILTIN was not undef in "\
    _STDEX_TESTS_CHECK_HEADER_NAME
#endif

#ifdef _STDEX_DELETED_FUNCTION
    #error "Macro _STDEX_DELETED_FUNCTION was not undef in "\
    _STDEX_TESTS_CHECK_HEADER_NAME
#endif

#ifdef _STDEX_DELETED_FUNCTION
    #error "Macro _STDEX_NOEXCEPT_FUNCTION was not undef in "\
    _STDEX_TESTS_CHECK_HEADER_NAME
#endif

#ifdef _STDEX_TWO_COMPLEMENT_REPRESENTATION_IS_USED
    #error "Macro _STDEX_TWO_COMPLEMENT_REPRESENTATION_IS_USED was not undef in "\
    _STDEX_TESTS_CHECK_HEADER_NAME
#endif

#ifdef _STDEX_MIN_RANGE_INT16_LOWER_BOUND
    #error "Macro _STDEX_MIN_RANGE_INT16_LOWER_BOUND was not undef in "\
    _STDEX_TESTS_CHECK_HEADER_NAME
#endif

#ifdef _STDEX_MIN_RANGE_INT16_UPPER_BOUND
    #error "Macro _STDEX_MIN_RANGE_INT16_LOWER_BOUND was not undef in "\
    _STDEX_TESTS_CHECK_HEADER_NAME
#endif

#ifdef _STDEX_MIN_RANGE_INT32_LOWER_BOUND
    #error "Macro _STDEX_MIN_RANGE_INT16_LOWER_BOUND was not undef in "\
    _STDEX_TESTS_CHECK_HEADER_NAME
#endif

#ifdef _STDEX_MIN_RANGE_INT32_UPPER_BOUND
    #error "Macro _STDEX_MIN_RANGE_INT32_UPPER_BOUND was not undef in "\
    _STDEX_TESTS_CHECK_HEADER_NAME
#endif

#ifdef _STDEX_MIN_RANGE_INT64_LOWER_BOUND
    #error "Macro _STDEX_MIN_RANGE_INT64_LOWER_BOUND was not undef in "\
    _STDEX_TESTS_CHECK_HEADER_NAME
#endif

#ifdef _STDEX_MIN_RANGE_INT64_UPPER_BOUND
    #error "Macro _STDEX_MIN_RANGE_INT64_UPPER_BOUND was not undef in "\
    _STDEX_TESTS_CHECK_HEADER_NAME
#endif

#ifdef _STDEX_SHRT_IS_IN_INT16_MAX_RANGE
    #error "Macro _STDEX_SHRT_IS_IN_INT16_MAX_RANGE was not undef in "\
    _STDEX_TESTS_CHECK_HEADER_NAME
#endif

#ifdef _STDEX_SHRT_IS_IN_INT32_MAX_RANGE
    #error "Macro _STDEX_SHRT_IS_IN_INT32_MAX_RANGE was not undef in "\
    _STDEX_TESTS_CHECK_HEADER_NAME
#endif

#ifdef _STDEX_INT_IS_IN_INT32_MAX_RANGE
    #error "Macro _STDEX_INT_IS_IN_INT32_MAX_RANGE was not undef in "\
    _STDEX_TESTS_CHECK_HEADER_NAME
#endif

#ifdef _STDEX_LONG_IS_IN_INT32_MAX_RANGE
    #error "Macro _STDEX_LONG_IS_IN_INT32_MAX_RANGE was not undef in "\
    _STDEX_TESTS_CHECK_HEADER_NAME
#endif

#ifdef _STDEX_SHRT_IS_IN_INT64_MAX_RANGE
    #error "Macro _STDEX_SHRT_IS_IN_INT64_MAX_RANGE was not undef in "\
    _STDEX_TESTS_CHECK_HEADER_NAME
#endif

#ifdef _STDEX_INT_IS_IN_INT64_MAX_RANGE
    #error "Macro _STDEX_INT_IS_IN_INT64_MAX_RANGE was not undef in "\
    _STDEX_TESTS_CHECK_HEADER_NAME
#endif

#ifdef _STDEX_LONG_IS_IN_INT64_MAX_RANGE
    #error "Macro _STDEX_LONG_IS_IN_INT64_MAX_RANGE was not undef in "\
    _STDEX_TESTS_CHECK_HEADER_NAME
#endif

#ifdef _STDEX_LLONG_IS_IN_INT64_MAX_RANGE
    #error "Macro _STDEX_LLONG_IS_IN_INT64_MAX_RANGE was not undef in "\
    _STDEX_TESTS_CHECK_HEADER_NAME
#endif

#ifdef _STDEX_PLATFORM_CAN_HAVE_STD_8_BIT_MULTIPLE_INT
    #error "Macro _STDEX_PLATFORM_CAN_HAVE_STD_8_BIT_MULTIPLE_INT was not undef in "\
    _STDEX_TESTS_CHECK_HEADER_NAME
#endif

#ifdef _STDEX_PLATFORM_CAN_HAVE_STD_8_BIT_INT
    #error "Macro _STDEX_PLATFORM_CAN_HAVE_STD_8_BIT_INT was not undef in "\
    _STDEX_TESTS_CHECK_HEADER_NAME
#endif

#ifdef _STDEX_PLATFORM_CAN_HAVE_STD_16_BIT_INT
    #error "Macro _STDEX_PLATFORM_CAN_HAVE_STD_16_BIT_INT was not undef in "\
    _STDEX_TESTS_CHECK_HEADER_NAME
#endif

#ifdef _STDEX_PLATFORM_CAN_HAVE_STD_32_BIT_INT
    #error "Macro _STDEX_PLATFORM_CAN_HAVE_STD_32_BIT_INT was not undef in "\
    _STDEX_TESTS_CHECK_HEADER_NAME
#endif

#ifdef _STDEX_PLATFORM_CAN_HAVE_STD_64_BIT_INT
    #error "Macro _STDEX_PLATFORM_CAN_HAVE_STD_64_BIT_INT was not undef in "\
    _STDEX_TESTS_CHECK_HEADER_NAME
#endif

#ifdef _STDEX_PLATFORM_CAN_HAVE_NON_STD_8_BIT_INT
    #error "Macro _STDEX_PLATFORM_CAN_HAVE_NON_STD_8_BIT_INT was not undef in "\
    _STDEX_TESTS_CHECK_HEADER_NAME
#endif

#ifdef _STDEX_PLATFORM_CAN_HAVE_NON_STD_16_BIT_INT
    #error "Macro _STDEX_PLATFORM_CAN_HAVE_NON_STD_16_BIT_INT was not undef in "\
    _STDEX_TESTS_CHECK_HEADER_NAME
#endif

#ifdef _STDEX_PLATFORM_CAN_HAVE_NON_STD_32_BIT_INT
    #error "Macro _STDEX_PLATFORM_CAN_HAVE_NON_STD_32_BIT_INT was not undef in "\
    _STDEX_TESTS_CHECK_HEADER_NAME
#endif

#ifdef _STDEX_PLATFORM_CAN_HAVE_NON_STD_64_BIT_INT
    #error "Macro _STDEX_PLATFORM_CAN_HAVE_NON_STD_64_BIT_INT was not undef in "\
    _STDEX_TESTS_CHECK_HEADER_NAME
#endif

#ifdef _STDEX_10_POW_01
    #error "Macro _STDEX_10_POW_01 was not undef in "\
    _STDEX_TESTS_CHECK_HEADER_NAME
#endif

#ifdef _STDEX_10_POW_02
    #error "Macro _STDEX_10_POW_02 was not undef in "\
    _STDEX_TESTS_CHECK_HEADER_NAME
#endif

#ifdef _STDEX_10_POW_03
    #error "Macro _STDEX_10_POW_03 was not undef in "\
    _STDEX_TESTS_CHECK_HEADER_NAME
#endif

#ifdef _STDEX_10_POW_06
    #error "Macro _STDEX_10_POW_06 was not undef in "\
    _STDEX_TESTS_CHECK_HEADER_NAME
#endif

#ifdef _STDEX_10_POW_09
    #error "Macro _STDEX_10_POW_09 was not undef in "\
    _STDEX_TESTS_CHECK_HEADER_NAME
#endif

#ifdef _STDEX_10_POW_12
    #error "Macro _STDEX_10_POW_12 was not undef in "\
    _STDEX_TESTS_CHECK_HEADER_NAME
#endif

#ifdef _STDEX_10_POW_15
    #error "Macro _STDEX_10_POW_15 was not undef in "\
    _STDEX_TESTS_CHECK_HEADER_NAME
#endif

#ifdef _STDEX_10_POW_18
    #error "Macro _STDEX_10_POW_18 was not undef in "\
    _STDEX_TESTS_CHECK_HEADER_NAME
#endif

#ifdef _STDEX_10_POW_21
    #error "Macro _STDEX_10_POW_21 was not undef in "\
    _STDEX_TESTS_CHECK_HEADER_NAME
#endif

#ifdef _STDEX_10_POW_24
    #error "Macro _STDEX_10_POW_24 was not undef in "\
    _STDEX_TESTS_CHECK_HEADER_NAME
#endif

#ifdef _STDEX_THREAD_WIN
    #error "Macro _STDEX_THREAD_WIN was not undef in "\
    _STDEX_TESTS_CHECK_HEADER_NAME
#endif

#ifdef _STDEX_TYPES
    #error "Macro _STDEX_TYPES was not undef in "\
    _STDEX_TESTS_CHECK_HEADER_NAME
#endif

#ifdef _STDEX_ARGS
    #error "Macro _STDEX_ARGS was not undef in "\
    _STDEX_TESTS_CHECK_HEADER_NAME
#endif

#ifdef _STDEX_IS_MEM_FUN_PTR
    #error "Macro _STDEX_IS_MEM_FUN_PTR was not undef in "\
    _STDEX_TESTS_CHECK_HEADER_NAME
#endif

#ifdef _STDEX_IS_MEM_FUN_PTR_CLR
    #error "Macro _STDEX_IS_MEM_FUN_PTR_CLR was not undef in "\
    _STDEX_TESTS_CHECK_HEADER_NAME
#endif

#ifdef _STDEX_IS_MEM_FUN_CDECL_PTR
    #error "Macro _STDEX_IS_MEM_FUN_CDECL_PTR was not undef in "\
    _STDEX_TESTS_CHECK_HEADER_NAME
#endif

#ifdef _STDEX_IS_MEM_FUN_STDCALL_PTR
    #error "Macro _STDEX_IS_MEM_FUN_STDCALL_PTR was not undef in "\
    _STDEX_TESTS_CHECK_HEADER_NAME
#endif

#ifdef _STDEX_IS_MEM_FUN_FASTCALL_PTR
    #error "Macro _STDEX_IS_MEM_FUN_FASTCALL_PTR was not undef in "\
    _STDEX_TESTS_CHECK_HEADER_NAME
#endif

#ifdef _STDEX_UNDEF_LEAN_AND_MEAN
    #error "Macro _STDEX_UNDEF_LEAN_AND_MEAN was not undef in "\
    _STDEX_TESTS_CHECK_HEADER_NAME
#endif
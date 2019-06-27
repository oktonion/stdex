#ifndef _STDEX_CSTDINT_H
#define _STDEX_CSTDINT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <cstddef>
#if !defined(CHAR_BIT)
    #include <climits> // CHAR_BIT, {TYPE}_MIN, {TYPE}_MAX
#endif

#if ( !defined(CHAR_BIT) || CHAR_BIT < 1 )
    #error "Number of bits in char (macro CHAR_BIT) is not defined!"
#endif

// custom defines - for this file only!
// minimum guaranteed range of N-bit signed integers is from {-(2^(N-1) - 1)} to {+2^(N-1) - 1}
// maximum possible range of N-bit signed integers is from {-2^(N-1)} to {+2^(N-1)} 
// (from {-(2^(N-1))} to {+2^(N-1) - 1} or (prior to C++20) from {-(2^(N-1) - 1)} to {+2^(N-1)})

// defining minimum rages for 16 and 32 bit types
#define _STDEX_MIN_RANGE_INT16_LOWER_BOUND -32767 // {-(2^15 - 1)}
#define _STDEX_MIN_RANGE_INT16_UPPER_BOUND 32767 // {+2^15 - 1}
#define _STDEX_MIN_RANGE_INT32_LOWER_BOUND -2147483647 // {-(2^31 - 1)}
#define _STDEX_MIN_RANGE_INT32_UPPER_BOUND 2147483647 // {+2^31 - 1}
#if defined(LLONG_MIN) && defined(LLONG_MAX)
    #define _STDEX_MIN_RANGE_INT64_LOWER_BOUND -9223372036854775807ll // {-(2^63 - 1)}
    #define _STDEX_MIN_RANGE_INT64_UPPER_BOUND 9223372036854775807ll // {+2^63 - 1}
#endif

// checking if standard types like short, int and long have less than N-bits
#define _STDEX_SHRT_IS_IN_INT16_MAX_RANGE ( ((SHRT_MIN - _STDEX_MIN_RANGE_INT16_LOWER_BOUND) >= -1) && ((SHRT_MAX - _STDEX_MIN_RANGE_INT16_UPPER_BOUND) <= 1) )

#define _STDEX_SHRT_IS_IN_INT32_MAX_RANGE ( ((SHRT_MIN - _STDEX_MIN_RANGE_INT32_LOWER_BOUND) >= -1) && ((SHRT_MAX - _STDEX_MIN_RANGE_INT32_UPPER_BOUND) <= 1) )
#define _STDEX_INT_IS_IN_INT32_MAX_RANGE ( ((INT_MIN - _STDEX_MIN_RANGE_INT32_LOWER_BOUND) >= -1) && ((INT_MAX - _STDEX_MIN_RANGE_INT32_UPPER_BOUND) <= 1) )
#define _STDEX_LONG_IS_IN_INT32_MAX_RANGE ( ((LONG_MIN - _STDEX_MIN_RANGE_INT32_LOWER_BOUND) >= -1) && ((LONG_MAX - _STDEX_MIN_RANGE_INT32_UPPER_BOUND) <= 1) )

#if defined(LLONG_MIN) && defined(LLONG_MAX)
    #define test (11 - _STDEX_MIN_RANGE_INT64_UPPER_BOUND)
    #define _STDEX_SHRT_IS_IN_INT64_MAX_RANGE ( ((SHRT_MIN - _STDEX_MIN_RANGE_INT64_LOWER_BOUND) >= -1ll) && ((SHRT_MAX - _STDEX_MIN_RANGE_INT64_UPPER_BOUND) <= 1ll) )
    #define _STDEX_INT_IS_IN_INT64_MAX_RANGE  ( ((INT_MIN - _STDEX_MIN_RANGE_INT64_LOWER_BOUND) >= -1ll) && ((INT_MAX - _STDEX_MIN_RANGE_INT64_UPPER_BOUND) <= 1ll) )
    #define _STDEX_LONG_IS_IN_INT64_MAX_RANGE  ( ((LONG_MIN - _STDEX_MIN_RANGE_INT64_LOWER_BOUND) >= -1ll) && ((LONG_MAX - _STDEX_MIN_RANGE_INT64_UPPER_BOUND) <= 1ll) )
    #define _STDEX_LLONG_IS_IN_INT64_MAX_RANGE  ( ((LLONG_MIN - _STDEX_MIN_RANGE_INT64_LOWER_BOUND) >= -1ll) && ((LLONG_MAX - _STDEX_MIN_RANGE_INT64_UPPER_BOUND) <= 1ll) )
#else
    #define _STDEX_SHRT_IS_IN_INT64_MAX_RANGE 0
    #define _STDEX_INT_IS_IN_INT64_MAX_RANGE 0
    #define _STDEX_LONG_IS_IN_INT64_MAX_RANGE 0
    #define _STDEX_LLONG_IS_IN_INT64_MAX_RANGE 0
#endif

#define _STDEX_PLATFORM_CAN_HAVE_STD_8_BIT_MULTIPLE_INT ((CHAR_BIT <= 8)/*must be 8 at least but to be sure*/ && ( ((CHAR_BIT % 2) == 0) || CHAR_BIT == 1))

// checks if platform can have N-bits standard type
#define _STDEX_PLATFORM_CAN_HAVE_STD_8_BIT_INT \
    ( _STDEX_PLATFORM_CAN_HAVE_STD_8_BIT_MULTIPLE_INT && (CHAR_BIT == 8) )

#define _STDEX_PLATFORM_CAN_HAVE_STD_16_BIT_INT \
     ( _STDEX_PLATFORM_CAN_HAVE_STD_8_BIT_MULTIPLE_INT && ( _STDEX_INT_IS_IN_INT16_MAX_RANGE || _STDEX_SHRT_IS_IN_INT16_MAX_RANGE || (CHAR_BIT == 16) ) )

#define _STDEX_PLATFORM_CAN_HAVE_STD_32_BIT_INT \
     ( _STDEX_PLATFORM_CAN_HAVE_STD_8_BIT_MULTIPLE_INT && ( _STDEX_LONG_IS_IN_INT32_MAX_RANGE || _STDEX_INT_IS_IN_INT32_MAX_RANGE || _STDEX_SHRT_IS_IN_INT32_MAX_RANGE || (CHAR_BIT == 32) ) )
     
#define _STDEX_PLATFORM_CAN_HAVE_STD_64_BIT_INT \
     ( _STDEX_PLATFORM_CAN_HAVE_STD_8_BIT_MULTIPLE_INT && ( _STDEX_LLONG_IS_IN_INT64_MAX_RANGE || _STDEX_LONG_IS_IN_INT64_MAX_RANGE || _STDEX_INT_IS_IN_INT64_MAX_RANGE || _STDEX_SHRT_IS_IN_INT64_MAX_RANGE || (CHAR_BIT == 64) ) )

namespace stdex
{
    namespace detail
    {
        namespace
        {
            typedef void _cstdint_invalid_type;
            enum {_cstdint_invalid_size = 9999};
            template<int> struct _sized_integer_map {static const char size[_cstdint_invalid_size]; typedef _cstdint_invalid_type signed_type;  typedef _cstdint_invalid_type unsigned_type;};
            enum {_sized_integer_rank = __LINE__};
            template<> struct _sized_integer_map<(__LINE__ - _sized_integer_rank)> {static const char size[int(sizeof(char) * CHAR_BIT)]; typedef signed char signed_type;  typedef unsigned char unsigned_type; };
            template<> struct _sized_integer_map<(__LINE__ - _sized_integer_rank)> {static const char size[int(sizeof(short int) * CHAR_BIT)]; typedef short int signed_type;  typedef unsigned short int unsigned_type; };
            template<> struct _sized_integer_map<(__LINE__ - _sized_integer_rank)> {static const char size[int(sizeof(int) * CHAR_BIT)]; typedef int signed_type;  typedef unsigned int unsigned_type; };
            template<> struct _sized_integer_map<(__LINE__ - _sized_integer_rank)> {static const char size[int(sizeof(long int) * CHAR_BIT)]; typedef long int signed_type;  typedef unsigned long int unsigned_type; };

        #if (_INTEGRAL_MAX_BITS == 64) // hack for MSVC and Borland C++ compilers
            #define _STDEX_PLATFORM_CAN_HAVE_NON_STD_64_BIT_INT 1
            template<> struct _sized_integer_map<(__LINE__ - _sized_integer_rank)> {static const char size[int(sizeof(__int64) * CHAR_BIT)]; typedef __int64 signed_type;  typedef unsigned __int64 unsigned_type; };
        #endif
        #if defined(LLONG_MIN) && defined(LLONG_MAX) && defined(ULLONG_MAX)
            template<> struct _sized_integer_map<(__LINE__ - _sized_integer_rank)> {static const char size[int(sizeof(long long int) * CHAR_BIT)]; typedef long long int signed_type;  typedef unsigned long long int unsigned_type; };
        #endif
            template<> struct _sized_integer_map<(__LINE__ - _sized_integer_rank)> {static const char size[int(sizeof(std::ptrdiff_t) * CHAR_BIT)]; typedef std::ptrdiff_t signed_type;  typedef _cstdint_invalid_type unsigned_type; };
            enum {_sized_integer_max_rank = __LINE__ - _sized_integer_rank};

            template<int _BitsCount, int _RankIt = 0, int _Found = sizeof(_sized_integer_map<_RankIt>::size)>
            struct _exact_sized_integer_step:
                _exact_sized_integer_step<_BitsCount, _RankIt + 1, sizeof(_sized_integer_map<_RankIt + 1>::size)>{};

            template<int _BitsCount>
            struct _exact_sized_integer_step<_BitsCount, _sized_integer_max_rank, _cstdint_invalid_size>:
                _sized_integer_map<_sized_integer_max_rank>{};

            template<int _BitsCount, int _RankIt>
            struct _exact_sized_integer_step<_BitsCount, _RankIt, _BitsCount>:
                _sized_integer_map<_RankIt>{};

            template<int _BitsCount, int _RankIt, int _FoundRank>
            struct _least_sized_integer_step_expr
            {
                static const bool result = (
                        sizeof(_sized_integer_map<_RankIt + 1>::size) < sizeof(_sized_integer_map<_FoundRank>::size) &&
                        sizeof(_sized_integer_map<_RankIt + 1>::size) >= _BitsCount
                    );
                static const int value = (result ? (_RankIt + 1) : _FoundRank);
            };

            template<int _BitsCount, int _RankIt = 0, int _FoundRank = 0>
            struct _least_sized_integer_step:
                _least_sized_integer_step<_BitsCount, _RankIt + 1,
                    _least_sized_integer_step_expr<_BitsCount, _RankIt, _FoundRank>::value>{};

            template<int _BitsCount, int _FoundRank>
            struct _least_sized_integer_step<_BitsCount, _sized_integer_max_rank, _FoundRank>:
                _sized_integer_map<_FoundRank>{};

            template<int _RankIt, int _FoundRank>
            struct _max_sized_integer_step_expr
            {
                static const bool result = 
                    (sizeof(_sized_integer_map<_FoundRank>::size) == _cstdint_invalid_size) ||
                    (
                        sizeof(_sized_integer_map<_RankIt + 1>::size) > sizeof(_sized_integer_map<_FoundRank>::size) &&
                        sizeof(_sized_integer_map<_RankIt + 1>::size) < sizeof(_sized_integer_map<_sized_integer_max_rank>::size)
                    );
                static const int value = (result ? (_RankIt + 1) : _FoundRank);
            };

            template<int _RankIt = 0, int _FoundRank = 0>
            struct _max_sized_integer_step:
                _max_sized_integer_step<_RankIt + 1, 
                    _max_sized_integer_step_expr<_RankIt, _FoundRank>::value>{};

            template<int _FoundRank>
            struct _max_sized_integer_step<_sized_integer_max_rank, _FoundRank>:
                _sized_integer_map<_FoundRank>{};
        }

        template<int _Size>
        struct _exact_sized_integer:
            _exact_sized_integer_step<_Size>
        {};

        template<int _AtLeast>
        struct _least_sized_integer:
            _least_sized_integer_step<_AtLeast>
        {};

        struct _max_sized_integer:
            _max_sized_integer_step<>
        {};
    }

    typedef detail::_least_sized_integer<8>::signed_type    int_least8_t;
    typedef detail::_least_sized_integer<8>::unsigned_type  uint_least8_t;
    typedef detail::_least_sized_integer<16>::signed_type   int_least16_t;
    typedef detail::_least_sized_integer<16>::unsigned_type uint_least16_t;
    typedef detail::_least_sized_integer<32>::signed_type   int_least32_t;
    typedef detail::_least_sized_integer<32>::unsigned_type uint_least32_t;
    typedef detail::_least_sized_integer<64>::signed_type   int_least64_t;
    typedef detail::_least_sized_integer<64>::unsigned_type uint_least64_t;

    typedef int_least8_t     int_fast8_t;
    typedef int_least16_t    int_fast16_t;
    typedef int_least32_t    int_fast32_t;
    typedef int_least64_t    int_fast64_t;
    typedef uint_least8_t    uint_fast8_t;
    typedef uint_least16_t   uint_fast16_t;
    typedef uint_least32_t   uint_fast32_t;
    typedef uint_least64_t   uint_fast64_t;
 
    typedef detail::_max_sized_integer::signed_type intmax_t;
    typedef detail::_max_sized_integer::unsigned_type uintmax_t;

    // optional
#if _STDEX_PLATFORM_CAN_HAVE_STD_8_BIT_INT || _STDEX_PLATFORM_CAN_HAVE_NON_STD_8_BIT_INT
    typedef detail::_exact_sized_integer<8>::signed_type int8_t;
    typedef detail::_exact_sized_integer<8>::unsigned_type uint8_t;
#endif
#if _STDEX_PLATFORM_CAN_HAVE_STD_16_BIT_INT || _STDEX_PLATFORM_CAN_HAVE_NON_STD_16_BIT_INT
    typedef detail::_exact_sized_integer<16>::signed_type int16_t;
    typedef detail::_exact_sized_integer<16>::unsigned_type uint16_t;
#endif
#if _STDEX_PLATFORM_CAN_HAVE_STD_32_BIT_INT || _STDEX_PLATFORM_CAN_HAVE_NON_STD_32_BIT_INT
    typedef detail::_exact_sized_integer<32>::signed_type int32_t;
    typedef detail::_exact_sized_integer<32>::unsigned_type uint32_t;
#endif
#if _STDEX_PLATFORM_CAN_HAVE_STD_64_BIT_INT || _STDEX_PLATFORM_CAN_HAVE_NON_STD_64_BIT_INT
    typedef detail::_exact_sized_integer<64>::signed_type int64_t;
    typedef detail::_exact_sized_integer<64>::unsigned_type uint64_t;
#endif
 
    typedef detail::_least_sized_integer<sizeof(void*) * CHAR_BIT>::signed_type intptr_t;
    typedef detail::_least_sized_integer<sizeof(void*) * CHAR_BIT>::unsigned_type uintptr_t;
};

// Macro constants
#define INT_FAST8_MIN
#define INT_FAST16_MIN
#define INT_FAST32_MIN
#define INT_FAST64_MIN
#define INT_LEAST8_MIN
#define INT_LEAST16_MIN
#define INT_LEAST32_MIN
#define INT_LEAST64_MIN
#define INTPTR_MIN
#define INTMAX_MIN
 


#define INT_FAST8_MAX
#define INT_FAST16_MAX
#define INT_FAST32_MAX
#define INT_FAST64_MAX
#define INT_LEAST8_MAX
#define INT_LEAST16_MAX
#define INT_LEAST32_MAX
#define INT_LEAST64_MAX
#define INTPTR_MAX
#define INTMAX_MAX



#define UINT_FAST8_MAX
#define UINT_FAST16_MAX
#define UINT_FAST32_MAX
#define UINT_FAST64_MAX
#define UINT_LEAST8_MAX
#define UINT_LEAST16_MAX
#define UINT_LEAST32_MAX
#define UINT_LEAST64_MAX
#define UINTPTR_MAX
#define UINTMAX_MAX

// optional
#if _STDEX_PLATFORM_CAN_HAVE_STD_8_BIT_INT || _STDEX_PLATFORM_CAN_HAVE_NON_STD_8_BIT_INT
    // Function macros for minimum-width integer constants
    #define INT8_C(value) static_cast<stdex::int8_t>(value)
    #define UINT8_C(value) static_cast<stdex::uint8_t>(value)
    // Macro constants
    #define INT8_MIN 
    #define INT8_MAX 
    #define UINT8_MAX 
#endif
#if _STDEX_PLATFORM_CAN_HAVE_STD_16_BIT_INT || _STDEX_PLATFORM_CAN_HAVE_NON_STD_16_BIT_INT
    // Function macros for minimum-width integer constants
    #define INT16_C(value) static_cast<stdex::int16_t>(value)
    #define UINT16_C(value) static_cast<stdex::uint16_t>(value)
    // Macro constants
    #define INT16_MIN
    #define INT16_MAX
    #define UINT16_MAX
#endif
#if _STDEX_PLATFORM_CAN_HAVE_STD_32_BIT_INT || _STDEX_PLATFORM_CAN_HAVE_NON_STD_32_BIT_INT
    // Function macros for minimum-width integer constants
    #define INT32_C(value) static_cast<stdex::int32_t>(value)
    #define UINT32_C(value) static_cast<stdex::uint32_t>(value)
    // Macro constants
    #define INT32_MIN
    #define INT32_MAX
    #define UINT32_MAX
#endif
#if _STDEX_PLATFORM_CAN_HAVE_STD_64_BIT_INT || _STDEX_PLATFORM_CAN_HAVE_NON_STD_64_BIT_INT
    // Function macros for minimum-width integer constants
    #define INT64_C(value) static_cast<stdex::int64_t>(value)
    #define UINT64_C(value) static_cast<stdex::uint64_t>(value)
    // Macro constants
    #define INT64_MIN
    #define INT64_MAX
    #define UINT64_MAX
#endif

// clear from defines
#undef _STDEX_MIN_RANGE_INT16_LOWER_BOUND
#undef _STDEX_MIN_RANGE_INT16_UPPER_BOUND
#undef _STDEX_MIN_RANGE_INT32_LOWER_BOUND
#undef _STDEX_MIN_RANGE_INT32_UPPER_BOUND
#undef _STDEX_MIN_RANGE_INT64_LOWER_BOUND
#undef _STDEX_MIN_RANGE_INT64_UPPER_BOUND

#undef _STDEX_SHRT_IS_IN_INT16_MAX_RANGE

#undef _STDEX_SHRT_IS_IN_INT32_MAX_RANGE
#undef _STDEX_INT_IS_IN_INT32_MAX_RANGE
#undef _STDEX_LONG_IS_IN_INT32_MAX_RANGE


#undef _STDEX_SHRT_IS_IN_INT64_MAX_RANGE
#undef _STDEX_INT_IS_IN_INT64_MAX_RANGE
#undef _STDEX_LONG_IS_IN_INT64_MAX_RANGE
#undef _STDEX_LLONG_IS_IN_INT64_MAX_RANGE

#undef _STDEX_PLATFORM_CAN_HAVE_STD_8_BIT_MULTIPLE_INT

#undef _STDEX_PLATFORM_CAN_HAVE_STD_8_BIT_INT
#undef _STDEX_PLATFORM_CAN_HAVE_STD_16_BIT_INT
#undef _STDEX_PLATFORM_CAN_HAVE_STD_32_BIT_INT
#undef _STDEX_PLATFORM_CAN_HAVE_STD_64_BIT_INT

#endif // _STDEX_CSTDINT_H
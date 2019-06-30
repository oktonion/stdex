#ifndef _STDEX_CSTDINT_H
#define _STDEX_CSTDINT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// stdex includes
/*none*/

// POSIX includes
/*none*/

// std includes
#include <cstddef> // std::ptrdiff_t, std::size_t, NULL
#include <cwchar> // std::wchar_t, std::wint_t, WCHAR_MAX, WCHAR_MIN
#include <cwctype> // std::wctype_t

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

// checking if two's-complement representation is used
#define _STDEX_TWO_COMPLEMENT_REPRESENTATION_IS_USED ( (SCHAR_MAX + SCHAR_MIN) == -1 )

// defining minimum rages for 16 and 32 bit types
#define _STDEX_MIN_RANGE_INT16_LOWER_BOUND (-32767) // {-(2^15 - 1)}
#define _STDEX_MIN_RANGE_INT16_UPPER_BOUND 32767 // {+2^15 - 1}
#define _STDEX_MIN_RANGE_INT32_LOWER_BOUND (-2147483647) // {-(2^31 - 1)}
#define _STDEX_MIN_RANGE_INT32_UPPER_BOUND 2147483647 // {+2^31 - 1}
#if defined(LLONG_MIN) && defined(LLONG_MAX)
    #define _STDEX_MIN_RANGE_INT64_LOWER_BOUND (-9223372036854775807ll) // {-(2^63 - 1)}
    #define _STDEX_MIN_RANGE_INT64_UPPER_BOUND 9223372036854775807ll // {+2^63 - 1}
#endif

// checking if standard types like short, int and long have less than N-bits
#define _STDEX_SHRT_IS_IN_INT16_MAX_RANGE ( ((SHRT_MIN - _STDEX_MIN_RANGE_INT16_LOWER_BOUND) >= -1) && ((SHRT_MAX - _STDEX_MIN_RANGE_INT16_UPPER_BOUND) <= 1) )

#define _STDEX_SHRT_IS_IN_INT32_MAX_RANGE ( ((SHRT_MIN - _STDEX_MIN_RANGE_INT32_LOWER_BOUND) >= -1) && ((SHRT_MAX - _STDEX_MIN_RANGE_INT32_UPPER_BOUND) <= 1) )
#define _STDEX_INT_IS_IN_INT32_MAX_RANGE ( ((INT_MIN - _STDEX_MIN_RANGE_INT32_LOWER_BOUND) >= -1) && ((INT_MAX - _STDEX_MIN_RANGE_INT32_UPPER_BOUND) <= 1) )
#define _STDEX_LONG_IS_IN_INT32_MAX_RANGE ( ((LONG_MIN - _STDEX_MIN_RANGE_INT32_LOWER_BOUND) >= -1) && ((LONG_MAX - _STDEX_MIN_RANGE_INT32_UPPER_BOUND) <= 1) )

#if defined(LLONG_MIN) && defined(LLONG_MAX)
    #define _STDEX_SHRT_IS_IN_INT64_MAX_RANGE ( ((SHRT_MIN - _STDEX_MIN_RANGE_INT64_LOWER_BOUND) >= -1ll) && ((SHRT_MAX - _STDEX_MIN_RANGE_INT64_UPPER_BOUND) <= 1ll) )
    #define _STDEX_INT_IS_IN_INT64_MAX_RANGE  ( ((INT_MIN - _STDEX_MIN_RANGE_INT64_LOWER_BOUND) >= -1ll) && ((INT_MAX - _STDEX_MIN_RANGE_INT64_UPPER_BOUND) <= 1ll) )
    #define _STDEX_LONG_IS_IN_INT64_MAX_RANGE  ( ((LONG_MIN - _STDEX_MIN_RANGE_INT64_LOWER_BOUND) >= -1ll) && ((LONG_MAX - _STDEX_MIN_RANGE_INT64_UPPER_BOUND) <= 1ll) )
    #define _STDEX_LLONG_IS_IN_INT64_MAX_RANGE  ( ((LLONG_MIN - _STDEX_MIN_RANGE_INT64_LOWER_BOUND) >= -1ll) && ((LLONG_MAX - _STDEX_MIN_RANGE_INT64_UPPER_BOUND) <= 1ll) )
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
        namespace cstdint_detail
        {
            struct _constructible_from_any
            {
                template<class _Tp>
                _constructible_from_any(_Tp);
            };

            typedef char _yes_type;
            struct _no_type
            {
                char _padding[8];
            };

            class _is_integral_constant_helper
            {
                class _type;
            public:
                static cstdint_detail::_yes_type check(_type*);
                static cstdint_detail::_no_type check(_constructible_from_any);
            };

            template<class _Tp>
            struct _is_integral_constant
            {
                static const bool value = 
                    sizeof(_is_integral_constant_helper::check(_Tp(NULL))) == sizeof(cstdint_detail::_yes_type) &&
                    sizeof(_is_integral_constant_helper::check(_Tp(1))) == sizeof(cstdint_detail::_no_type);
            };

            template<class _Tp>
            struct _is_integral_constant<_Tp&>
            {
                static const bool value = false;
            };

            template<class _Tp>
            struct _is_integral_constant<_Tp*>
            {
                static const bool value = false;
            };

            template<class _Tp, class _Cp>
            struct _is_integral_constant<_Tp _Cp::*>
            {
                static const bool value = false;
            };

            template<>
            struct _is_integral_constant<void>
            {
                static const bool value = false;
            };

            template<class _Tp>
            struct _is_signed
            {
                static const bool value = _Tp(-1) < _Tp(0);
            };

            template<class _Tp>
            struct _is_unsigned
            {
                static const bool value = _Tp(0) < _Tp(-1);
            };

            typedef void _cstdint_invalid_type;

            template<class _Tp, bool>
            struct _signed_definer
            {
                typedef _cstdint_invalid_type signed_type;  
            };

            template<class _Tp>
            struct _signed_definer<_Tp, true>
            {
                typedef _Tp signed_type;  
            };

            template<class _Tp, bool>
            struct _unsigned_definer
            {
                typedef _cstdint_invalid_type unsigned_type;  
            };

            template<class _Tp>
            struct _unsigned_definer<_Tp, true>
            {
                typedef _Tp unsigned_type;  
            };

            template<class _Tp>
            struct _signed_unsigned:
                _signed_definer<_Tp, cstdint_detail::_is_signed<_Tp>::value>,
                _unsigned_definer<_Tp, cstdint_detail::_is_unsigned<_Tp>::value>
            { };

            
            enum {_cstdint_invalid_size = 9999};
            template<int> struct _sized_integer_map_impl {static const char size[_cstdint_invalid_size]; typedef _cstdint_invalid_type signed_type;  typedef _cstdint_invalid_type unsigned_type;};
            enum {_sized_integer_rank = __LINE__};
            template<> struct _sized_integer_map_impl<(__LINE__ - _sized_integer_rank)> {static const char size[int(sizeof(char) * CHAR_BIT)]; typedef signed char signed_type;  typedef unsigned char unsigned_type; };
            template<> struct _sized_integer_map_impl<(__LINE__ - _sized_integer_rank)> {static const char size[int(sizeof(short int) * CHAR_BIT)]; typedef short int signed_type;  typedef unsigned short int unsigned_type; };
            template<> struct _sized_integer_map_impl<(__LINE__ - _sized_integer_rank)> {static const char size[int(sizeof(int) * CHAR_BIT)]; typedef int signed_type;  typedef unsigned int unsigned_type; };
            template<> struct _sized_integer_map_impl<(__LINE__ - _sized_integer_rank)> {static const char size[int(sizeof(long int) * CHAR_BIT)]; typedef long int signed_type;  typedef unsigned long int unsigned_type; };

        #if (_INTEGRAL_MAX_BITS == 64) // hack for MSVC and Borland C++ compilers
            #define _STDEX_PLATFORM_CAN_HAVE_NON_STD_64_BIT_INT 1
            template<> struct _sized_integer_map_impl<(__LINE__ - _sized_integer_rank)> {static const char size[int(sizeof(__int64) * CHAR_BIT)]; typedef __int64 signed_type;  typedef unsigned __int64 unsigned_type; };
        #endif
        #if defined(LLONG_MIN) && defined(LLONG_MAX) && defined(ULLONG_MAX)
            template<> struct _sized_integer_map_impl<(__LINE__ - _sized_integer_rank)> {static const char size[int(sizeof(long long int) * CHAR_BIT)]; typedef long long int signed_type;  typedef unsigned long long int unsigned_type; };
        #endif
        #if defined(WCHAR_MAX) && defined(WCHAR_MIN)
            template<> struct _sized_integer_map_impl<(__LINE__ - _sized_integer_rank)>: _signed_unsigned<wchar_t> { static const char size[int(sizeof(wchar_t) * CHAR_BIT)]; };
        #endif
            template<> struct _sized_integer_map_impl<(__LINE__ - _sized_integer_rank)>: _signed_unsigned<std::ptrdiff_t> { static const char size[int(sizeof(std::ptrdiff_t) * CHAR_BIT)]; };
            template<> struct _sized_integer_map_impl<(__LINE__ - _sized_integer_rank)>: _signed_unsigned<std::wint_t> { static const char size[int(sizeof(std::wint_t) * CHAR_BIT)]; };
            enum {_sized_integer_max_rank = __LINE__ - _sized_integer_rank};

            template<int _Rank, bool _IsIntConst> 
            struct _sized_integer_map_helper:
                _sized_integer_map_impl<_sized_integer_max_rank>
            { };

            template<int _Rank> 
            struct _sized_integer_map_helper<_Rank, true>:
                _sized_integer_map_impl<_Rank>
            { };

            template<int _Rank> 
            struct _sized_integer_map:
                _sized_integer_map_helper<_Rank, cstdint_detail::_is_integral_constant<typename _sized_integer_map_impl<_Rank>::signed_type>::value>
            { };


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
        } // namespace cstdint_detail

        template<int _Size>
        struct _exact_sized_integer:
            cstdint_detail::_exact_sized_integer_step<_Size>
        {};

        template<class _Signed, int _Size>
        struct _sized_integer_max_impl
        {
            static const _Signed max_value = ((_Signed(1) << (_Size - 2)) + ((_Signed(1) << (_Size - 2)) - _Signed(1)));
        };

        template<class _Signed, int _Size>
        struct _sized_integer_min_impl
        {
        #if _STDEX_TWO_COMPLEMENT_REPRESENTATION_IS_USED
            static const _Signed min_value = (-(_sized_integer_max_impl<_Signed, _Size>::max_value) - 1);
        #else
            static const _Signed min_value = (-(_sized_integer_max_impl<_Signed, _Size>::max_value));
        #endif
        };

        template<class _Unsigned, int _Size>
        struct _sized_integer_umax_impl
        {
            static const _Unsigned umax_value = (((_Unsigned(1) << (_Size - 1)) - 1) * _Unsigned(2) + _Unsigned(1));
        };

        template<class _Signed, class _Unsigned, int _Size>
        struct _max_sized_integer_min_max_impl:
            _sized_integer_min_impl<_Signed, _Size>,
            _sized_integer_max_impl<_Signed, _Size>,
            _sized_integer_umax_impl<_Unsigned, _Size>
        { };

        struct _max_sized_integer_impl
        {
            typedef cstdint_detail::_max_sized_integer_step<> base_type;
            typedef base_type::signed_type signed_type;
            typedef base_type::unsigned_type unsigned_type;
            typedef _max_sized_integer_min_max_impl<signed_type, unsigned_type, sizeof(base_type::size)> min_max_impl;

            struct type:
                base_type,
                min_max_impl
            { };
        };

        struct _max_sized_integer:
            _max_sized_integer_impl::type
        { };

        template<class _Signed, class _Unsigned, int _Size>
        struct _least_sized_integer_min_max_impl:
            _sized_integer_min_impl<_Signed, _Size>,
            _sized_integer_max_impl<_Signed, _Size>,
            _sized_integer_umax_impl<_Unsigned, _Size>
        { };

        template<class _Unsigned, int _Size>
        struct _least_sized_integer_min_max_impl<cstdint_detail::_cstdint_invalid_type, _Unsigned, _Size>:
            _sized_integer_umax_impl<_Unsigned, _Size>
        {
            typedef cstdint_detail::_cstdint_invalid_type min_value;
            typedef cstdint_detail::_cstdint_invalid_type max_value;
        };

        template<class _Signed, int _Size>
        struct _least_sized_integer_min_max_impl<_Signed, cstdint_detail::_cstdint_invalid_type, _Size>:
            _sized_integer_min_impl<_Signed, _Size>,
            _sized_integer_max_impl<_Signed, _Size>
        {
            typedef cstdint_detail::_cstdint_invalid_type umax_value;
        };

        template<int _Size>
        struct _least_sized_integer_min_max_impl<cstdint_detail::_cstdint_invalid_type, cstdint_detail::_cstdint_invalid_type, _Size>
        {
            typedef cstdint_detail::_cstdint_invalid_type min_value;
            typedef cstdint_detail::_cstdint_invalid_type max_value;
            typedef cstdint_detail::_cstdint_invalid_type umax_value;
        };

        template<int _AtLeast>
        struct _least_sized_integer_impl
        {
            typedef cstdint_detail::_least_sized_integer_step<_AtLeast> base_type;
            typedef typename base_type::signed_type signed_type;
            typedef typename base_type::unsigned_type unsigned_type;

            struct type:
                base_type,
                _least_sized_integer_min_max_impl<signed_type, unsigned_type, sizeof(base_type::size)>
            {};
        };

        template<int _AtLeast>
        struct _least_sized_integer:
            _least_sized_integer_impl<_AtLeast>::type
        {};
    } // namespace detail
} // namespace stdex


namespace stdex
{
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
} // namespace stdex

// undef any std macros

//#undef INTMAX_C
//#undef UINTMAX_C
//#undef INT8_C
//#undef UINT8_C
//#undef INT16_C
//#undef UINT16_C
//#undef INT32_C
//#undef UINT32_C
//#undef INT64_C
//#undef UINT64_C
//
//#undef INT_LEAST8_MIN  
//#undef INT_LEAST16_MIN 
//#undef INT_LEAST32_MIN 
//#undef INT_LEAST64_MIN 
//#undef INT_FAST8_MIN
//#undef INT_FAST16_MIN
//#undef INT_FAST32_MIN
//#undef INT_FAST64_MIN
//#undef INTPTR_MIN 
//#undef INTMAX_MIN 
// 
//
//#undef INT_LEAST8_MAX  
//#undef INT_LEAST16_MAX 
//#undef INT_LEAST32_MAX 
//#undef INT_LEAST64_MAX 
//#undef INT_FAST8_MAX
//#undef INT_FAST16_MAX
//#undef INT_FAST32_MAX
//#undef INT_FAST64_MAX
//#undef INTPTR_MAX 
//#undef INTMAX_MAX 
//
//
//#undef UINT_LEAST8_MAX  
//#undef UINT_LEAST16_MAX 
//#undef UINT_LEAST32_MAX 
//#undef UINT_LEAST64_MAX 
//#undef UINT_FAST8_MAX
//#undef UINT_FAST16_MAX
//#undef UINT_FAST32_MAX
//#undef UINT_FAST64_MAX
//#undef UINTPTR_MAX 
//#undef UINTMAX_MAX 

// Function macros for minimum-width integer constants
#define STDEX_INTMAX_C(value) static_cast<stdex::intmax_t>(value)
#define STDEX_UINTMAX_C(value) static_cast<stdex::uintmax_t>(value)

#define STDEX_INT8_C(value) static_cast<stdex::int_least8_t>(value)
#define STDEX_UINT8_C(value) static_cast<stdex::uint_least8_t>(value)
#define STDEX_INT16_C(value) static_cast<stdex::int16_t>(value)
#define STDEX_UINT16_C(value) static_cast<stdex::uint16_t>(value)   
#define STDEX_INT32_C(value) static_cast<stdex::int32_t>(value)
#define STDEX_UINT32_C(value) static_cast<stdex::uint32_t>(value)
#define STDEX_INT64_C(value) static_cast<stdex::int64_t>(value)
#define STDEX_UINT64_C(value) static_cast<stdex::uint64_t>(value)

// Macro constants
#define STDEX_INT_LEAST8_MIN  (stdex::detail::_least_sized_integer<8>::min_value)
#define STDEX_INT_LEAST16_MIN (stdex::detail::_least_sized_integer<16>::min_value)
#define STDEX_INT_LEAST32_MIN (stdex::detail::_least_sized_integer<32>::min_value)
#define STDEX_INT_LEAST64_MIN (stdex::detail::_least_sized_integer<64>::min_value)
#define STDEX_INT_FAST8_MIN INT_LEAST8_MIN
#define STDEX_INT_FAST16_MIN INT_LEAST16_MIN
#define STDEX_INT_FAST32_MIN INT_LEAST32_MIN
#define STDEX_INT_FAST64_MIN INT_LEAST64_MIN
#define STDEX_INTPTR_MIN (stdex::detail::_least_sized_integer<sizeof(void*) * CHAR_BIT>::min_value)
#define STDEX_INTMAX_MIN (stdex::detail::_max_sized_integer::min_value)
 

#define STDEX_INT_LEAST8_MAX  (stdex::detail::_least_sized_integer<8>::max_value)
#define STDEX_INT_LEAST16_MAX (stdex::detail::_least_sized_integer<16>::max_value)
#define STDEX_INT_LEAST32_MAX (stdex::detail::_least_sized_integer<32>::max_value)
#define STDEX_INT_LEAST64_MAX (stdex::detail::_least_sized_integer<64>::max_value)
#define STDEX_INT_FAST8_MAX INT_LEAST8_MAX
#define STDEX_INT_FAST16_MAX INT_LEAST16_MAX
#define STDEX_INT_FAST32_MAX INT_LEAST32_MAX
#define STDEX_INT_FAST64_MAX INT_LEAST64_MAX
#define STDEX_INTPTR_MAX (stdex::detail::_least_sized_integer<sizeof(void*) * CHAR_BIT>::max_value)
#define STDEX_INTMAX_MAX (stdex::detail::_max_sized_integer::max_value)


#define STDEX_UINT_LEAST8_MAX  (stdex::detail::_least_sized_integer<8>::umax_value)
#define STDEX_UINT_LEAST16_MAX (stdex::detail::_least_sized_integer<16>::umax_value)
#define STDEX_UINT_LEAST32_MAX (stdex::detail::_least_sized_integer<32>::umax_value)
#define STDEX_UINT_LEAST64_MAX (stdex::detail::_least_sized_integer<64>::umax_value)
#define STDEX_UINT_FAST8_MAX UINT_LEAST8_MAX
#define STDEX_UINT_FAST16_MAX UINT_LEAST16_MAX
#define STDEX_UINT_FAST32_MAX UINT_LEAST32_MAX
#define STDEX_UINT_FAST64_MAX UINT_LEAST64_MAX
#define STDEX_UINTPTR_MAX (stdex::detail::_least_sized_integer<sizeof(void*) * CHAR_BIT>::umax_value)
#define STDEX_UINTMAX_MAX (stdex::detail::_max_sized_integer::umax_value)

// optional
#if _STDEX_PLATFORM_CAN_HAVE_STD_8_BIT_INT || _STDEX_PLATFORM_CAN_HAVE_NON_STD_8_BIT_INT
    //#undef INT8_MIN
    //#undef INT8_MAX
    //#undef UINT8_MAX

    #if _STDEX_TWO_COMPLEMENT_REPRESENTATION_IS_USED
        #define STDEX_INT8_MIN static_cast<stdex::int8_t>(-128)
    #else
        #define STDEX_INT8_MIN static_cast<stdex::int8_t>(-127)
    #endif
    #define STDEX_INT8_MAX static_cast<stdex::int8_t>(127)
    #define STDEX_UINT8_MAX static_cast<stdex::uint8_t>(255)
#endif
#if _STDEX_PLATFORM_CAN_HAVE_STD_16_BIT_INT || _STDEX_PLATFORM_CAN_HAVE_NON_STD_16_BIT_INT
    //#undef INT16_MIN
    //#undef INT16_MAX
    //#undef UINT16_MAX
    
    #if _STDEX_TWO_COMPLEMENT_REPRESENTATION_IS_USED
        #define STDEX_INT16_MIN static_cast<stdex::int16_t>(-32768)
    #else
        #define STDEX_INT16_MIN static_cast<stdex::int16_t>(-32767)
    #endif
    #define STDEX_INT16_MAX static_cast<stdex::int16_t>(32767)
    #define STDEX_UINT16_MAX static_cast<stdex::uint16_t>(65535)
#endif
#if _STDEX_PLATFORM_CAN_HAVE_STD_32_BIT_INT || _STDEX_PLATFORM_CAN_HAVE_NON_STD_32_BIT_INT
    //#undef INT32_MIN
    //#undef INT32_MAX
    //#undef UINT32_MAX
    
    #if _STDEX_TWO_COMPLEMENT_REPRESENTATION_IS_USED
        #define STDEX_INT32_MIN static_cast<stdex::int32_t>(-2147483648)
    #else
        #define STDEX_INT32_MIN static_cast<stdex::int32_t>(-2147483647)
    #endif
    #define STDEX_INT32_MAX static_cast<stdex::int32_t>(2147483647)
    #define STDEX_UINT32_MAX static_cast<stdex::uint32_t>(4294967295)
#endif
#if _STDEX_PLATFORM_CAN_HAVE_STD_64_BIT_INT || _STDEX_PLATFORM_CAN_HAVE_NON_STD_64_BIT_INT
    //#undef INT64_MIN
    //#undef INT64_MAX
    //#undef UINT64_MAX
    
    #if _STDEX_TWO_COMPLEMENT_REPRESENTATION_IS_USED
        #define STDEX_INT64_MIN static_cast<stdex::int64_t>(-9223372036854775808)
    #else
        #define STDEX_INT64_MIN static_cast<stdex::int64_t>(-9223372036854775807)
    #endif
    #define STDEX_INT64_MAX static_cast<stdex::int64_t>(9223372036854775807)
    #define STDEX_UINT64_MAX static_cast<stdex::uint64_t>(18446744073709551615)
#endif

// clean from defines
#undef _STDEX_TWO_COMPLEMENT_REPRESENTATION_IS_USED
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
#undef _STDEX_PLATFORM_CAN_HAVE_NON_STD_8_BIT_INT
#undef _STDEX_PLATFORM_CAN_HAVE_STD_16_BIT_INT
#undef _STDEX_PLATFORM_CAN_HAVE_NON_STD_16_BIT_INT
#undef _STDEX_PLATFORM_CAN_HAVE_STD_32_BIT_INT
#undef _STDEX_PLATFORM_CAN_HAVE_NON_STD_32_BIT_INT
#undef _STDEX_PLATFORM_CAN_HAVE_STD_64_BIT_INT
#undef _STDEX_PLATFORM_CAN_HAVE_NON_STD_64_BIT_INT

#endif // _STDEX_CSTDINT_H

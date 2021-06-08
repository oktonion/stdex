#ifndef _STDEX_RATIO_H
#define _STDEX_RATIO_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// stdex includes
#include "./cstdint.hpp" // stdex::intmax_t, STDEX_INTMAX_MAX, STDEX_INTMAX_MIN

// POSIX includes
/*none*/

// std includes
#include <climits> // CHAR_BIT

namespace stdex
{
    namespace detail
    {
        template<stdex::intmax_t _Val>
        struct _sign_of
        {   // computes sign of _Val
            static const stdex::intmax_t value = _Val < 0 ? -1 : 1;
        };

        template<stdex::intmax_t _Val>
        struct _abs
        {   // computes absolute value of _Val
            static const stdex::intmax_t value = _Val * _sign_of<_Val>::value;
        };

        // [Greatest common divisor template]

        template<stdex::intmax_t _Ax, stdex::intmax_t _Bx>
        struct _gcdX
        {   // computes greatest common divisor of _Ax and _Bx
            static const stdex::intmax_t value = _gcdX<_Bx, _Ax % _Bx>::value;
        };

        template<stdex::intmax_t _Ax>
        struct _gcdX<_Ax, 0>
        {   // computes greatest common divisor of _Ax and 0
            static const stdex::intmax_t value = _Ax;
        };

        template<stdex::intmax_t _Ax, stdex::intmax_t _Bx>
        struct _gcd
        {   // computes greatest common divisor of abs(_Ax) and abs(_Bx)
            static const stdex::intmax_t value =
                _gcdX<_abs<_Ax>::value, _abs<_Bx>::value>::value;
        };

        template<>
        struct _gcd<0, 0>
        {   // avoids division by 0 in ratio_less
            static const stdex::intmax_t value = 1;    // contrary to mathematical convention
        };
    }

    namespace intern
    {
        // since we have no static_assert in pre-C++11 we just compile-time assert this way:
        namespace ratio_asserts
        {
            template<bool> 
            struct overflow_in_multiplication_assert; // if you are there means overflow in safe template multiplication occurred

            template<bool>
            struct internal_library_error_assert; // if you are there means internal library error occurred

            template<bool>
            struct denominator_cant_be_zero_assert; // if you are there means you put the denominator to zero

            template<bool>
            struct out_of_range; // if you are there means that value is out of range

            template<bool>
            struct division_by_zero; // if you are there means that divider is zero

            template<bool>
            struct overflow_in_addition_assert; // if you are there means overflow in safe template addition occurred

            template<>
            struct overflow_in_multiplication_assert<true>
            {
                typedef bool overflow_in_multiplication_assert_failed;
            };

            template<>
            struct internal_library_error_assert<true>
            {
                typedef bool internal_library_error_assert_failed;
            };

            template<>
            struct denominator_cant_be_zero_assert<true>
            {
                typedef bool denominator_cant_be_zero_assert_failed;
            };

            template<>
            struct out_of_range<true>
            {
                typedef bool out_of_range_failed;
            };

            template<>
            struct division_by_zero<true>
            {
                typedef bool division_by_zero_failed;
            };

            template<>
            struct overflow_in_addition_assert<true>
            {
                typedef bool overflow_in_addition_assert_failed;
            };
        } // namespace ratio_asserts
    } // namespace intern

    namespace detail
    {
        // [Safe add template]

        template<stdex::intmax_t _Pn, stdex::intmax_t _Qn, bool>
        struct _add_overflow_check_impl
        {
            static const stdex::intmax_t value = _Pn <= (STDEX_INTMAX_MAX - _Qn);
        };

        template<stdex::intmax_t _Pn, stdex::intmax_t _Qn>
        struct _add_overflow_check_impl<_Pn, _Qn, false>
        {
            static const stdex::intmax_t value = _Pn >= (STDEX_INTMAX_MIN - _Qn);
        };

        template<stdex::intmax_t _Pn, stdex::intmax_t _Qn>
        struct _add_overflow_check
            : _add_overflow_check_impl<_Pn, _Qn, (_Qn >= 0)>
        { };

        template<stdex::intmax_t _Pn, stdex::intmax_t _Qn>
        struct _safe_add
        {
            static const stdex::intmax_t value = _Pn + _Qn;

        private:
            

            typedef typename intern::ratio_asserts::overflow_in_addition_assert<bool(_add_overflow_check<_Pn, _Qn>::value != 0)>::
                overflow_in_addition_assert_failed
            check1; // if you are there means overflow in safe template addition occurred
        };

        struct _half_char_bit
        {
            static const stdex::uintmax_t value = (CHAR_BIT / 2);

        private:
            
            typedef intern::ratio_asserts::internal_library_error_assert< ( (CHAR_BIT % 2) == 0 ) >::
                internal_library_error_assert_failed
            check1; // if you are there means internal library error occurred (number of bits in char is not even on your platform)
        };

        // [Safe multiply template]

        // Let c = 2^(half # of bits in an intmax_t)
        // then we find a1, a0, b1, b0 s.t. N = a1*c + a0, M = b1*c + b0
        // The multiplication of N and M becomes,
        // N * M = (a1 * b1)c^2 + (a0 * b1 + b0 * a1)c + a0 * b0
        // Multiplication is safe if each term and the sum of the terms
        // is representable by intmax_t.
        template<stdex::intmax_t _Pn, stdex::intmax_t _Qn>
        struct _safe_multiply_helper
        {
            static const stdex::uintmax_t _c = stdex::uintmax_t(1) << (sizeof(stdex::intmax_t) * _half_char_bit::value);

            static const stdex::uintmax_t _a0 = _abs<_Pn>::value % _c;
            static const stdex::uintmax_t _a1 = _abs<_Pn>::value / _c;
            static const stdex::uintmax_t _b0 = _abs<_Qn>::value % _c;
            static const stdex::uintmax_t _b1 = _abs<_Qn>::value / _c;

            static const stdex::uintmax_t _intmax_max = STDEX_UINTMAX_C(STDEX_INTMAX_MAX);
        };

        template<stdex::intmax_t _Pn, stdex::intmax_t _Qn>
        struct _safe_multiply
        {
        private:
            typedef _safe_multiply_helper<_Pn, _Qn> _helper_type;

            typedef typename intern::ratio_asserts::overflow_in_multiplication_assert< bool(_helper_type::_a1 == 0 || _helper_type::_b1 == 0) >::
                overflow_in_multiplication_assert_failed
            check1; // if you are there means overflow in safe template multiplication occurred
            typedef typename intern::ratio_asserts::overflow_in_multiplication_assert< bool(_helper_type::_a0 * _helper_type::_b1 + _helper_type::_b0 * _helper_type::_a1 < (_helper_type::_c / stdex::uintmax_t(2))) >::
                overflow_in_multiplication_assert_failed
            check2; // if you are there means overflow in safe template multiplication occurred
            typedef typename intern::ratio_asserts::overflow_in_multiplication_assert< bool( _helper_type::_b0 * _helper_type::_a0 <= _helper_type::_intmax_max ) >::
                overflow_in_multiplication_assert_failed
            check3; // if you are there means overflow in safe template multiplication occurred
            typedef typename intern::ratio_asserts::overflow_in_multiplication_assert< bool((_helper_type::_a0 * _helper_type::_b1 + _helper_type::_b0 * _helper_type::_a1) * _helper_type::_c <= _helper_type::_intmax_max - _helper_type::_b0 * _helper_type::_a0) >::
                overflow_in_multiplication_assert_failed
            check4; // if you are there means overflow in safe template multiplication occurred

        public:
            static const stdex::intmax_t value = _Pn * _Qn;
        };

        // [Less template]

        // Some double-precision utilities, where numbers are represented as
        // _hi*2^(8*sizeof(uintmax_t)) + _lo.
        template<stdex::uintmax_t _hi1, stdex::uintmax_t _lo1, stdex::uintmax_t _hi2, stdex::uintmax_t _lo2>
        struct _big_less
        {
            static const bool value = (_hi1 < _hi2 || (_hi1 == _hi2 && _lo1 < _lo2));
        };

        template<stdex::uintmax_t _lo1, stdex::uintmax_t _lo2>
        struct _big_add_lo
        {
            static const stdex::uintmax_t value = (_lo1 + _lo2);
        };

        template<stdex::uintmax_t _hi1, stdex::uintmax_t _lo1, stdex::uintmax_t _hi2, stdex::uintmax_t _lo2>
        struct _big_add
        {
            typedef _big_add_lo<_lo1, _lo2> _lo_type;// overflow is ok
            static const stdex::uintmax_t _lo = _lo_type::value;
            static const stdex::uintmax_t _hi = (_hi1 + _hi2 +
                stdex::uintmax_t(_lo_type::value < _lo1) ); // carry
        };

        // [Subtract template]

        template<stdex::uintmax_t _hi1, stdex::uintmax_t _lo1, stdex::uintmax_t _hi2, stdex::uintmax_t _lo2>
        struct _big_sub_helper
        {
            static const stdex::uintmax_t _lo = _lo1 - _lo2;
            static const stdex::uintmax_t _hi = (_hi1 - _hi2 - (_lo1 < _lo2));
        };

        // Subtract a number from a bigger one.
        template<stdex::uintmax_t _hi1, stdex::uintmax_t _lo1, stdex::uintmax_t _hi2, stdex::uintmax_t _lo2>
        struct _big_sub
        {
            typedef _big_sub_helper<_hi1, _lo1, _hi2, _lo2> _helper_type;
            static const stdex::uintmax_t _lo = _helper_type::_lo;
            static const stdex::uintmax_t _hi = _helper_type::_hi;

        private:
            
            typedef typename intern::ratio_asserts::internal_library_error_assert< bool(!(_big_less<_hi1, _lo1, _hi2, _lo2>::value != 0)) >::
                internal_library_error_assert_failed
            check1; // if you are there means internal library error occurred
        };

        // [Safe multiply for bigger numbers template]
        template<stdex::uintmax_t _x, stdex::uintmax_t _y>
        struct _big_multiply_helper1
        {
            static const stdex::uintmax_t _c = stdex::uintmax_t(1) << (sizeof(stdex::intmax_t) * _half_char_bit::value);
            static const stdex::uintmax_t _x0 = _x % _c;
            static const stdex::uintmax_t _x1 = _x / _c;
            static const stdex::uintmax_t _y0 = _y % _c;
            static const stdex::uintmax_t _y1 = _y / _c;
        };

        template<stdex::uintmax_t _x, stdex::uintmax_t _y>
        struct _big_multiply_helper2
        {
            typedef _big_multiply_helper1<_x, _y> _helper_type;

            static const stdex::uintmax_t _x0y0 = _helper_type::_x0 * _helper_type::_y0;
            static const stdex::uintmax_t _x0y1 = _helper_type::_x0 * _helper_type::_y1;
            static const stdex::uintmax_t _x1y0 = _helper_type::_x1 * _helper_type::_y0;
            static const stdex::uintmax_t _x1y1 = _helper_type::_x1 * _helper_type::_y1;
        };

        template<stdex::uintmax_t _x, stdex::uintmax_t _y>
        struct _big_multiply_helper3
        {
            typedef _big_multiply_helper2<_x, _y> _helper_type;

            static const stdex::uintmax_t _mix = _helper_type::_x0y1 + _helper_type::_x1y0; // possible carry...
        };

        template<stdex::uintmax_t _x, stdex::uintmax_t _y>
        struct _big_multiply_helper
        {
            typedef _big_multiply_helper1<_x, _y> _helper_type1;
            typedef _big_multiply_helper2<_x, _y> _helper_type2;
            typedef _big_multiply_helper3<_x, _y> _helper_type3;

            static const stdex::uintmax_t _mix_lo = static_cast<const stdex::uintmax_t>(_helper_type3::_mix) * _helper_type1::_c;  // overflow is ok
            static const stdex::uintmax_t _mix_hi
                = _helper_type3::_mix / _helper_type1::_c + ((_helper_type3::_mix < _helper_type2::_x0y1) ? _helper_type1::_c : 0); // ... added here
        };

        // Same principle as _safe_multiply.
        template<stdex::uintmax_t _x, stdex::uintmax_t _y>
        struct _big_multiply
        {
        private:
            typedef _big_multiply_helper<_x, _y> _helper_type;
            typedef _big_multiply_helper2<_x, _y> _helper_type2;
            typedef _big_add<_helper_type::_mix_hi, _helper_type::_mix_lo, _helper_type2::_x1y1, _helper_type2::_x0y0> _Res;
        public:
            static const stdex::uintmax_t _hi = _Res::_hi;
            static const stdex::uintmax_t _lo = _Res::_lo;
        };
    }
    
    namespace detail
    {
        template<stdex::intmax_t _Num, stdex::intmax_t _Den = 1>
        struct _ratio
        {
            static const stdex::intmax_t num =
                _Num * _sign_of<_Den>::value / _gcd<_Num, _Den>::value;

            static const stdex::intmax_t den =
                _abs<_Den>::value / _gcd<_Num, _Den>::value;
        };

    } // namespace detail

    template<stdex::intmax_t _Num, stdex::intmax_t _Den = 1>
    struct ratio
    {
        // Note: sign(N) * abs(N) == N
        static const stdex::intmax_t num =
            detail::_ratio<_Num, _Den>::num;

        static const stdex::intmax_t den =
            detail::_ratio<_Num, _Den>::den;

        typedef 
        ratio<
            detail::_ratio<_Num, _Den>::num, 
            detail::_ratio<_Num, _Den>::den
        > type;

    private:

        typedef typename intern::ratio_asserts::denominator_cant_be_zero_assert< bool(_Den != 0) >::
            denominator_cant_be_zero_assert_failed
        check1; // if you are there means you put the denominator to zero
        typedef typename intern::ratio_asserts::out_of_range<bool( (_Num >= -STDEX_INTMAX_MAX) && (_Den >= -STDEX_INTMAX_MAX) )>::
            out_of_range_failed
        check2; // if you are there means that value is out of range
    };

    namespace detail
    {
        template<class _R1, class _R2>
        struct _ratio_multiply_gcd
        {
            static const stdex::intmax_t _gcd1 =
                _gcd<_R1::num, _R2::den>::value;
            static const stdex::intmax_t _gcd2 =
                _gcd<_R2::num, _R1::den>::value;
        };

        template<class _R1, class _R2>
        struct _ratio_multiply
        {
        private:
            typedef _ratio_multiply_gcd<_R1, _R2> _gcd_type;

        public:
            typedef ratio<
                _safe_multiply<stdex::intmax_t(_R1::num / _gcd_type::_gcd1),
                stdex::intmax_t(_R2::num / _gcd_type::_gcd2)>::value,
                _safe_multiply<stdex::intmax_t(_R1::den / _gcd_type::_gcd2),
                stdex::intmax_t(_R2::den / _gcd_type::_gcd1)>::value> type;

            static const stdex::intmax_t num = type::num;
            static const stdex::intmax_t den = type::den;
        };

        template<class _R1, class _R2>
        struct _ratio_multiply_den
        {
        private:
            typedef _ratio_multiply_gcd<_R1, _R2> _gcd_type;

        public:

            static const stdex::intmax_t value = 
                _safe_multiply<
                    stdex::intmax_t(_R1::den / _gcd_type::_gcd2),
                    stdex::intmax_t(_R2::den / _gcd_type::_gcd1)
                >::value;
        };

        template<class _R1, class _R2>
        struct _ratio_multiply_num
        {
        private:
            typedef _ratio_multiply_gcd<_R1, _R2> _gcd_type;

        public:

            static const stdex::intmax_t value = 
                _safe_multiply<
                    stdex::intmax_t(_R1::num / _gcd_type::_gcd1),
                    stdex::intmax_t(_R2::num / _gcd_type::_gcd2)
                >::value;
        };
    }

    template<class _R1, class _R2>
    struct ratio_multiply :
        detail::_ratio_multiply<_R1, _R2>::type
    {};

    namespace detail
    {

        template<class _R1, class _R2>
        struct _ratio_divide
        {
            typedef typename ratio_multiply<
                _R1,
                ratio<_R2::den, _R2::num> >::type type;

            static const stdex::intmax_t num = type::num;
            static const stdex::intmax_t den = type::den;

        private:
            

            typedef typename intern::ratio_asserts::out_of_range< bool(_R2::num != 0) >::
                out_of_range_failed
            check1;// if you are there means that divider is zero
        };

        template<class _R1, class _R2>
        struct _ratio_divide_den
        {
            typedef _ratio_multiply_den<
                _R1,
                ratio<_R2::den, _R2::num> > type;

            static const stdex::intmax_t value = type::value;
        private:
            

            typedef typename intern::ratio_asserts::out_of_range< bool(_R2::num != 0) >::
                out_of_range_failed
            check1;// if you are there means that divider is zero
        };

        template<class _R1, class _R2>
        struct _ratio_divide_num
        {
            typedef _ratio_multiply_num<
                _R1,
                ratio<_R2::den, _R2::num> > type;

            static const stdex::intmax_t value = type::value;
        private:
            

            typedef typename intern::ratio_asserts::out_of_range< bool(_R2::num != 0) >::
                out_of_range_failed
            check1;// if you are there means that divider is zero
        };
    }

    template<class _R1, class _R2>
    struct ratio_divide :
        detail::_ratio_divide<_R1, _R2>::type
    {};

    // ratio_equal
    template<class _R1, class _R2>
    struct ratio_equal
    { 
        static const bool value = _R1::num == _R2::num && _R1::den == _R2::den;
    };

    // ratio_not_equal
    template<class _R1, class _R2>
    struct ratio_not_equal
    { 
        static const bool value = !ratio_equal<_R1, _R2>::value;
    };

    namespace detail
    {
        // Both numbers are positive.
        template<class _R1, class _R2,
            class _Left = _big_multiply<_R1::num, _R2::den>,
            class _Right = _big_multiply<_R2::num, _R1::den> >
        struct _ratio_less_impl_1
        {
            static const bool value = _big_less<_Left::_hi, _Left::_lo, _Right::_hi, _Right::_lo>::value;
        };

        template<class _R1, class _R2,
            bool = (_R1::num == 0 || _R2::num == 0
                || (_sign_of<_R1::num>::value
                    != _sign_of<_R2::num>::value)),
            bool = (_sign_of<_R1::num>::value == -1
                && _sign_of<_R2::num>::value == -1)>
        struct _ratio_less_impl :
            _ratio_less_impl_1<_R1, _R2>
        { };

        template<class _R1, class _R2>
        struct _ratio_less_impl<_R1, _R2, true, false>
        {
            static const bool value = _R1::num < _R2::num;
        };

        template<class _R1, class _R2>
        struct _ratio_less_impl<_R1, _R2, false, true> :
            _ratio_less_impl_1< ratio<-_R2::num, _R2::den>, ratio<-_R1::num, _R1::den> >
        { };
    }

    // ratio_less
    template<class _R1, class _R2>
    struct ratio_less: 
        detail::_ratio_less_impl<_R1, _R2>
    { };

    // ratio_less_equal
    template<class _R1, class _R2>
    struct ratio_less_equal
    { 
        static const bool value = ratio_less<_R2, _R1>::value == bool(false);
    };

    // ratio_greater
    template<class _R1, class _R2>
    struct ratio_greater
    { 
        static const bool value = ratio_less<_R2, _R1>::value;
    };

    // ratio_greater_equal
    template<class _R1, class _R2>
    struct ratio_greater_equal
    { 
        static const bool value = !ratio_less<_R1, _R2>::value;
    };

    namespace detail
    {
        // ratio_add
        template<class _R1, class _R2>
        struct _ratio_add
        {    // add two ratios
        private:
            static const stdex::intmax_t _n1 = _R1::num;
            static const stdex::intmax_t _d1 = _R1::den;
            static const stdex::intmax_t _n2 = _R2::num;
            static const stdex::intmax_t _d2 = _R2::den;

            static const stdex::intmax_t _gx = _gcd<_d1, _d2>::value;

            static const stdex::intmax_t _n = _safe_add<
                _safe_multiply<_R1::num, stdex::intmax_t(_R2::den / _ratio_add::_gx)>::value,
                _safe_multiply<_R2::num, stdex::intmax_t(_R1::den / _ratio_add::_gx)>::value>::value;

            // The new numerator may have common factors with the denominator,
            // but they have to also be factors of __gcd.
            static const stdex::intmax_t _gx2 = _gcd<_ratio_add::_n, _ratio_add::_gx>::value;

        public:
            // typename ratio<>::type is necessary here
            typedef 
            typename 
            ratio<
                stdex::intmax_t(_ratio_add::_n / _ratio_add::_gx2),
                _safe_multiply<
                    stdex::intmax_t(_R1::den / _ratio_add::_gx2), stdex::intmax_t(_R2::den / _ratio_add::_gx)
                >::value
            >::type type;
        };
    }

    template<class _R1, class _R2>
    struct ratio_add :
        detail::_ratio_add<_R1, _R2>::type
    {};

    namespace detail
    {
        template<class _R1, class _R2>
        struct _ratio_subtract
        {
            typedef 
            typename 
            _ratio_add<
                _R1,
                ratio<
                    stdex::intmax_t(-_R2::num), 
                    _R2::den
                > 
            >::type type;

            static const stdex::intmax_t num = type::num;
            static const stdex::intmax_t den = type::den;
        };
    }

    template<class _R1, class _R2>
    struct ratio_subtract :
        detail::_ratio_subtract<_R1, _R2>::type
    {};

    namespace detail
    {
        template<int _MaxBitsN>
        struct _ratio_predefined_impl:
            _ratio_predefined_impl<_MaxBitsN - 1>
        { };

        template<bool>
        struct _10_pow_01
        {
            static const stdex::uintmax_t value = STDEX_UINTMAX_C(10);
        };

        template<>
        struct _10_pow_01<true>
        {
            static const stdex::uintmax_t value = STDEX_UINTMAX_C(1);
        };

        enum {
            _10_pow_01_bit_n = 4,
            _10_pow_02_bit_n = 7,
            _10_pow_03_bit_n = 10,
            _10_pow_06_bit_n = 20,
            _10_pow_09_bit_n = 30,
            _10_pow_12_bit_n = 40,
            _10_pow_15_bit_n = 50,
            _10_pow_18_bit_n = 60,
            _10_pow_21_bit_n = 70,
            _10_pow_24_bit_n = 80
        };

        #define _STDEX_10_POW(cond) _10_pow_01<(cond)>::value
        #define _STDEX_INTMAX_BIT_COUNT (sizeof(stdex::intmax_t) * CHAR_BIT)
        #define _STDEX_10_POW_IMPL(pow_n) _STDEX_10_POW(_10_pow_##pow_n##_bit_n > _STDEX_INTMAX_BIT_COUNT)
        #define _STDEX_10_POW_01_IMPL(pow_n) (_STDEX_10_POW_IMPL(pow_n))
        #define _STDEX_10_POW_02_IMPL(pow_n) (_STDEX_10_POW_01_IMPL(pow_n) * _STDEX_10_POW_01_IMPL(pow_n))
        #define _STDEX_10_POW_03_IMPL(pow_n) (_STDEX_10_POW_01_IMPL(pow_n) * _STDEX_10_POW_02_IMPL(pow_n))
        #define _STDEX_10_POW_06_IMPL(pow_n) (_STDEX_10_POW_03_IMPL(pow_n) * _STDEX_10_POW_03_IMPL(pow_n))
        #define _STDEX_10_POW_09_IMPL(pow_n) (_STDEX_10_POW_03_IMPL(pow_n) * _STDEX_10_POW_06_IMPL(pow_n))
        #define _STDEX_10_POW_12_IMPL(pow_n) (_STDEX_10_POW_06_IMPL(pow_n) * _STDEX_10_POW_06_IMPL(pow_n))
        #define _STDEX_10_POW_15_IMPL(pow_n) (_STDEX_10_POW_06_IMPL(pow_n) * _STDEX_10_POW_09_IMPL(pow_n))
        #define _STDEX_10_POW_18_IMPL(pow_n) (_STDEX_10_POW_09_IMPL(pow_n) * _STDEX_10_POW_09_IMPL(pow_n))
        #define _STDEX_10_POW_21_IMPL(pow_n) (_STDEX_10_POW_09_IMPL(pow_n) * _STDEX_10_POW_12_IMPL(pow_n))
        #define _STDEX_10_POW_24_IMPL(pow_n) (_STDEX_10_POW_12_IMPL(pow_n) * _STDEX_10_POW_12_IMPL(pow_n))

        #define _STDEX_10_POW_01 _STDEX_10_POW_01_IMPL(01)
        #define _STDEX_10_POW_02 _STDEX_10_POW_02_IMPL(02)
        #define _STDEX_10_POW_03 _STDEX_10_POW_03_IMPL(03)
        #define _STDEX_10_POW_06 _STDEX_10_POW_06_IMPL(06)
        #define _STDEX_10_POW_09 _STDEX_10_POW_09_IMPL(09)
        #define _STDEX_10_POW_12 _STDEX_10_POW_12_IMPL(12)
        #define _STDEX_10_POW_15 _STDEX_10_POW_15_IMPL(15)
        #define _STDEX_10_POW_18 _STDEX_10_POW_18_IMPL(18)
        #define _STDEX_10_POW_21 _STDEX_10_POW_21_IMPL(21)
        #define _STDEX_10_POW_24 _STDEX_10_POW_24_IMPL(24)

        

        template<>
        struct _ratio_predefined_impl<0>;

        template<>
        struct _ratio_predefined_impl<_10_pow_01_bit_n>;

        template<>
        struct _ratio_predefined_impl<_10_pow_02_bit_n>
        {
            struct type_cannot_be_implemented;

            typedef type_cannot_be_implemented yocto;
            typedef type_cannot_be_implemented zepto;
            typedef type_cannot_be_implemented atto;
            typedef type_cannot_be_implemented femto;
            typedef type_cannot_be_implemented pico;
            typedef type_cannot_be_implemented nano;
            typedef type_cannot_be_implemented micro;
            typedef type_cannot_be_implemented milli;
            typedef ratio<STDEX_INTMAX_C(1), static_cast<stdex::intmax_t>(_STDEX_10_POW_02)> centi;
            typedef ratio<STDEX_INTMAX_C(1), static_cast<stdex::intmax_t>(_STDEX_10_POW_01)> deci;
            typedef ratio<static_cast<stdex::intmax_t>(_STDEX_10_POW_01), STDEX_INTMAX_C(1)> deca;
            typedef ratio<static_cast<stdex::intmax_t>(_STDEX_10_POW_02), STDEX_INTMAX_C(1)> hecto;
            typedef type_cannot_be_implemented kilo;
            typedef type_cannot_be_implemented mega;
            typedef type_cannot_be_implemented giga;
            typedef type_cannot_be_implemented tera;
            typedef type_cannot_be_implemented peta;
            typedef type_cannot_be_implemented exa;
            typedef type_cannot_be_implemented zetta;
            typedef type_cannot_be_implemented yotta;
        };

        template<>
        struct _ratio_predefined_impl<_10_pow_03_bit_n>:
            _ratio_predefined_impl<_10_pow_02_bit_n>
        {
            typedef ratio<STDEX_INTMAX_C(1), static_cast<stdex::intmax_t>(_STDEX_10_POW_03)> milli;
            typedef ratio<static_cast<stdex::intmax_t>(_STDEX_10_POW_03), STDEX_INTMAX_C(1)> kilo;
        };

        template<>
        struct _ratio_predefined_impl<_10_pow_06_bit_n>:
            _ratio_predefined_impl<_10_pow_03_bit_n>
        {
            typedef ratio<STDEX_INTMAX_C(1), static_cast<stdex::intmax_t>(_STDEX_10_POW_06)> micro;
            typedef ratio<static_cast<stdex::intmax_t>(_STDEX_10_POW_06), STDEX_INTMAX_C(1)> mega;
        };

        template<>
        struct _ratio_predefined_impl<_10_pow_09_bit_n>:
            _ratio_predefined_impl<_10_pow_06_bit_n>
        {
            typedef ratio<STDEX_INTMAX_C(1), static_cast<stdex::intmax_t>(_STDEX_10_POW_09)> nano;
            typedef ratio<static_cast<stdex::intmax_t>(_STDEX_10_POW_09), STDEX_INTMAX_C(1)> giga;
        };

        template<>
        struct _ratio_predefined_impl<_10_pow_12_bit_n>:
            _ratio_predefined_impl<_10_pow_09_bit_n>
        {
            typedef ratio<STDEX_INTMAX_C(1), static_cast<stdex::intmax_t>(_STDEX_10_POW_12)> pico;
            typedef ratio<static_cast<stdex::intmax_t>(_STDEX_10_POW_12), STDEX_INTMAX_C(1)> tera;
        };

        template<>
        struct _ratio_predefined_impl<_10_pow_15_bit_n>:
            _ratio_predefined_impl<_10_pow_12_bit_n>
        {
            typedef ratio<STDEX_INTMAX_C(1), static_cast<stdex::intmax_t>(_STDEX_10_POW_15)> femto;
            typedef ratio<static_cast<stdex::intmax_t>(_STDEX_10_POW_15), STDEX_INTMAX_C(1)> peta;
        };

        template<>
        struct _ratio_predefined_impl<_10_pow_18_bit_n>:
            _ratio_predefined_impl<_10_pow_15_bit_n>
        {
            typedef ratio<STDEX_INTMAX_C(1), static_cast<stdex::intmax_t>(_STDEX_10_POW_18)> atto;
            typedef ratio<static_cast<stdex::intmax_t>(_STDEX_10_POW_18), STDEX_INTMAX_C(1)> exa;
        };

        template<>
        struct _ratio_predefined_impl<_10_pow_21_bit_n>:
            _ratio_predefined_impl<_10_pow_18_bit_n>
        {
            typedef ratio<STDEX_INTMAX_C(1), static_cast<stdex::intmax_t>(_STDEX_10_POW_21)> zepto;
            typedef ratio<static_cast<stdex::intmax_t>(_STDEX_10_POW_21), STDEX_INTMAX_C(1)> zetta;
        };

        template<>
        struct _ratio_predefined_impl<_10_pow_24_bit_n>:
            _ratio_predefined_impl<_10_pow_21_bit_n>
        {
            typedef ratio<STDEX_INTMAX_C(1), static_cast<stdex::intmax_t>(_STDEX_10_POW_24)> yocto;
            typedef ratio<static_cast<stdex::intmax_t>(_STDEX_10_POW_24), STDEX_INTMAX_C(1)> yotta;
        };

        #undef _STDEX_10_POW
        #undef _STDEX_INTMAX_BIT_COUNT
        #undef _STDEX_10_POW_IMPL

        #undef _STDEX_10_POW_01_IMPL
        #undef _STDEX_10_POW_02_IMPL
        #undef _STDEX_10_POW_03_IMPL
        #undef _STDEX_10_POW_06_IMPL
        #undef _STDEX_10_POW_09_IMPL
        #undef _STDEX_10_POW_12_IMPL
        #undef _STDEX_10_POW_15_IMPL
        #undef _STDEX_10_POW_18_IMPL
        #undef _STDEX_10_POW_21_IMPL
        #undef _STDEX_10_POW_24_IMPL

        #undef _STDEX_10_POW_01
        #undef _STDEX_10_POW_02
        #undef _STDEX_10_POW_03
        #undef _STDEX_10_POW_06
        #undef _STDEX_10_POW_09
        #undef _STDEX_10_POW_12
        #undef _STDEX_10_POW_15
        #undef _STDEX_10_POW_18
        #undef _STDEX_10_POW_21
        #undef _STDEX_10_POW_24

        struct _ratio_predefined:
            _ratio_predefined_impl<(sizeof(stdex::intmax_t) * CHAR_BIT)>
        { };
    } // namespace detail

    typedef detail::_ratio_predefined::yocto     yocto;
    typedef detail::_ratio_predefined::zepto     zepto;
    typedef detail::_ratio_predefined::atto      atto;
    typedef detail::_ratio_predefined::femto     femto;
    typedef detail::_ratio_predefined::pico      pico;
    typedef detail::_ratio_predefined::nano      nano;
    typedef detail::_ratio_predefined::micro     micro;
    typedef detail::_ratio_predefined::milli     milli;
    typedef detail::_ratio_predefined::centi     centi;
    typedef detail::_ratio_predefined::deci      deci;
    typedef detail::_ratio_predefined::deca      deca;
    typedef detail::_ratio_predefined::hecto     hecto;
    typedef detail::_ratio_predefined::kilo      kilo;
    typedef detail::_ratio_predefined::mega      mega;
    typedef detail::_ratio_predefined::giga      giga;
    typedef detail::_ratio_predefined::tera      tera;
    typedef detail::_ratio_predefined::peta      peta;
    typedef detail::_ratio_predefined::exa       exa;
    typedef detail::_ratio_predefined::zetta     zetta;
    typedef detail::_ratio_predefined::yotta     yotta;

} // namespace stdex

#endif // _STDEX_RATIO_H

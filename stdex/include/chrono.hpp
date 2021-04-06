#ifndef _STDEX_CHRONO_H
#define _STDEX_CHRONO_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// stdex includes
#include "./cstdint.hpp" // stdex::intmax_t, STDEX_INTMAX_MAX, STDEX_INTMAX_MIN
#include "./ratio.hpp" // all ratio
#include "./type_traits.hpp" // stdex::common_type, stdex::is_floating_point
#include "./ctime.hpp"

// POSIX includes
/*none*/

// std includes
//#include <climits> //???
#include <limits> // std::numeric_limit
#include <stdexcept>


#ifdef _STDEX_NATIVE_CPP11_SUPPORT

#define _STDEX_DELETED_FUNCTION =delete
#define _STDEX_NOEXCEPT_FUNCTION noexcept

#else

#define _STDEX_DELETED_FUNCTION 
#define _STDEX_NOEXCEPT_FUNCTION throw()

#endif

namespace stdex
{
    //! Minimal implementation of the @c chrono namespace.
    //! The @c chrono namespace provides types for specifying time intervals.
    namespace chrono
    {
        namespace detail
        {
            namespace chrono_detail
            {
                template<unsigned _Rank> struct _priority_tag : _priority_tag < _Rank - 1 > {};
                template<> struct _priority_tag<0> {};
            }
            template<class _Tp>
            struct _is_ratio
            {
                static const bool value = false;
            };

            template<stdex::intmax_t _Num, stdex::intmax_t _Den>
            struct _is_ratio< ratio<_Num, _Den> >
            {
                static const bool value = true;
            };

            template<class _Rep, class _Period, bool>
            struct _use_big_int_impl
            {
                static const bool value = false;
            };

            template<class _Rep, class _Period>
            struct _use_big_int_impl<_Rep, _Period, true>
            {
                static const bool value =
                    is_integral<_Rep>::value &&
                    (sizeof(_Rep) * CHAR_BIT) < 64;
            };

            template<class _Rep, class _Period>
            struct _use_big_int :
                _use_big_int_impl<_Rep, _Period, _is_ratio<_Period>::value>
            { };
        }

        template <class _Rep, class _Period = ratio<1> >
        class duration;

        template<class _Clock, class _Dur = typename _Clock::duration>
        class time_point;

        template <class _Rep>
        struct treat_as_floating_point : 
            stdex::is_floating_point<_Rep> {};
    }

    template<class _Rep1, class _Period1, class _Rep2, class _Period2>
    struct common_type<chrono::duration<_Rep1, _Period1>,
        chrono::duration<_Rep2, _Period2> >
    {
    private:
        typedef detail::_gcd<_Period1::num, _Period2::num>     _gcd_num;
        typedef detail::_gcd<_Period1::den, _Period2::den>     _gcd_den;
        typedef typename common_type<_Rep1, _Rep2>::type        _cr;
        typedef ratio<_gcd_num::value,
            (_Period1::den / _gcd_den::value) * _Period2::den> _r;

    public:
        typedef chrono::duration<_cr, _r>             type;
    };

    // specialization of common_type (for time_point)
    template<class _Clock, class _Dur1, class _Dur2>
    struct common_type<chrono::time_point<_Clock, _Dur1>,
        chrono::time_point<_Clock, _Dur2> >
    {
    private:
        typedef typename common_type<_Dur1, _Dur2>::type         _ct;

    public:
        typedef chrono::time_point<_Clock, _ct>             type;
    };

    namespace chrono
    {
        namespace detail
        {
            // this structure is used internally to represent platform specific big integers
            // and safe number precision checks (throws out_of_range)
            struct _big_int
            {
                char least64_value[8];

                _big_int(stdex::intmax_t _value = 0);

                _big_int(const _big_int&);
                _big_int& operator=(const _big_int&);

                _big_int operator+() const;
                _big_int operator-() const;

                _big_int& operator++();
                _big_int operator++(int);
                _big_int& operator--();
                _big_int operator--(int);

                _big_int& operator+=(const _big_int&);
                _big_int& operator-=(const _big_int&);
                _big_int& operator*=(const _big_int&);
                _big_int& operator/=(const _big_int&);
                _big_int& operator%=(const _big_int&);

                stdex::intmax_t to_integer() const;
                long double     to_floating_point() const;
            };

            _big_int operator+(_big_int, const _big_int&);
            _big_int operator-(_big_int, const _big_int&);
            _big_int operator*(_big_int, const _big_int&);
            _big_int operator/(_big_int, const _big_int&);
            _big_int operator%(_big_int, const _big_int&);

            bool operator< (const _big_int&, const _big_int&);
            bool operator> (const _big_int&, const _big_int&);
            bool operator==(const _big_int&, const _big_int&);
            bool operator!=(const _big_int&, const _big_int&);
            bool operator>=(const _big_int&, const _big_int&);
            bool operator<=(const _big_int&, const _big_int&);
            
            template<class _Tp>
            _big_int operator+(const _Tp& _lhs,
                typename
                conditional<is_integral<_Tp>::value, const _big_int&, class _disabled>::type _rhs)
            {
                return _big_int(_lhs) + _rhs;
            }

            template<class _Tp>
            _big_int operator-(const _Tp& _lhs,
                typename
                conditional<is_integral<_Tp>::value, const _big_int&, class _disabled>::type _rhs)
            {
                return _big_int(_lhs) - _rhs;
            }

            template<class _Tp>
            _big_int operator*(const _Tp& _lhs,
                typename
                conditional<is_integral<_Tp>::value, const _big_int&, class _disabled>::type _rhs)
            {
                return _big_int(_lhs) * _rhs;
            }

            template<class _Tp>
            _big_int operator/(const _Tp& _lhs,
                typename
                conditional<is_integral<_Tp>::value, const _big_int&, class _disabled>::type _rhs)
            {
                return _big_int(_lhs) / _rhs;
            }

            template<class _Tp>
            _big_int operator%(const _Tp& _lhs,
                typename
                conditional<is_integral<_Tp>::value, const _big_int&, class _disabled>::type _rhs)
            {
                return _big_int(_lhs) % _rhs;
            }

        template<class _To, class _From>
            _To _chrono_convert(_From _from,
                typename
                conditional<
                    is_same<typename remove_reference<typename remove_cv<_From>::type>::type, _big_int>::value == bool(false) &&
                    is_same<typename remove_reference<typename remove_cv<_To>::type>::type, _big_int>::value == bool(false),
                    const chrono_detail::_priority_tag<0>&,
                    class _disabled0
                >::type)
            {
                return static_cast<_To>(_from);
            }

            template<class _To, class _From>
            _To _chrono_convert(_From _from,
                typename
                conditional<
                    is_same<typename remove_reference<typename remove_cv<_From>::type>::type, _big_int>::value == bool(false) &&
                    is_same<typename remove_reference<typename remove_cv<_To>::type>::type, _big_int>::value == bool(true),
                    const chrono_detail::_priority_tag<1>&,
                    class _disabled1
                >::type)
            {
                return intmax_t(_from);
            }

            template<class _To, class _From>
            _To _chrono_convert(const _From& _from,
                typename
                conditional<
                    is_same<typename remove_reference<typename remove_cv<_From>::type>::type, _big_int>::value == bool(true) &&
                    is_same<typename remove_reference<typename remove_cv<_To>::type>::type, _big_int>::value == bool(false) &&
                    is_floating_point<_To>::value == bool(true),
                    const chrono_detail::_priority_tag<2>&,
                    class _disabled2
                >::type)
            {
                return _To(_from.to_floating_point());
            }

             template<class _To, class _From>
             _To _chrono_convert(const _From& _from,
                 typename
                 conditional <
                 is_same<typename remove_reference<typename remove_cv<_From>::type>::type, _big_int>::value == bool(true) &&
                 is_same<typename remove_reference<typename remove_cv<_To>::type>::type, _big_int>::value == bool(false) &&
                    is_floating_point<_To>::value == bool(false),
                    const chrono_detail::_priority_tag<3>&,
                    class _disabled3
                >::type)
            {
                return _To(_from.to_integer());
            }

            template<class _To, class _From>
            _To _chrono_convert(const _From& _from,
                typename
                conditional<
                    is_same<typename remove_reference<typename remove_cv<_From>::type>::type, _big_int>::value == bool(true) &&
                    is_same<typename remove_reference<typename remove_cv<_To>::type>::type, _big_int>::value == bool(true),
                    const chrono_detail::_priority_tag<4>&,
                    class _disabled4
                >::type)
            {
                return _from;
            }

            template <class _Rep, class _Period,
                bool _Fallback>
            class duration_base;

            struct  duration_secret
            {
                template<class _Rep, class _Period,
                    bool _Fallback>
                static 
                typename
                duration_base<_Rep, _Period, _Fallback>::
                internal_value_type duration_count(
                    const duration_base<_Rep, _Period, _Fallback> &_dur)
                {
                    return _dur._r;
                }
            };

            template <class _Rep, class _Period>
            class duration_base<_Rep, _Period, false>
            {
            protected:
            
                typedef _Rep internal_value_type;
                internal_value_type _r;

                template <class _Rep2>
                duration_base(const _Rep2& _r_in) :
                    _r(_chrono_convert<_Rep>(_r_in, chrono_detail::_priority_tag<4>())) {}

                friend
                struct stdex::chrono::detail::duration_secret;
            };

            template <class _Rep, class _Period>
            class duration_base<_Rep, _Period, true>
            {
            protected:
            
                typedef _big_int internal_value_type;
                internal_value_type _r;

                duration_base(const internal_value_type& _r_in) :
                    _r(_r_in) {}

                template <class _Rep2>
                duration_base(const _Rep2& _r_in) :
                    _r(_chrono_convert<_Rep>(_r_in, chrono_detail::_priority_tag<4>())) {}
                
                friend
                struct stdex::chrono::detail::duration_secret;
            };

            template <class _Rep, class _Period>
            _Rep
            duration_count(
                const duration_base<_Rep, _Period, false>& _dur)
            {
                return duration_secret::duration_count(_dur);
            }

            template <class _Rep, class _Period>
            _big_int
            duration_count(
                const duration_base<_Rep, _Period, true>& _dur)
            {
                return duration_secret::duration_count(_dur);
            }

            template<class _ToRep, class _FromRep, bool>
            struct _duration_common_type_impl
            {
                typedef _big_int type;
            };

            template<class _ToRep, class _FromRep>
            struct _duration_common_type_impl<_ToRep, _FromRep, false>
            {
                typedef 
                typename 
                common_type<_ToRep, _FromRep, stdex::intmax_t>::type type;
            };

            template<class _ToRep, class _FromRep, class _Period>
            struct _duration_common_type:
                _duration_common_type_impl <
                    _ToRep, 
                    _FromRep,
                    _use_big_int<
                        typename
                        common_type<_ToRep, _FromRep, stdex::intmax_t>::type, 
                        _Period
                    >::value
                >
            { };

            /*template<class _Tp>
            struct _can_be_big_int
            {
                typedef 
                typename remove_reference<
                    typename remove_cv<_Tp>::type
                >::type type;

                static const bool value =
                    is_same<type, _big_int>::value == bool(true) ||
                    is_integral<type>::value == bool(true) ||
                    is_floating_point<type>::value == bool(true);
            };*/

            

            // Primary template for duration_cast impl.
            template<class _ToDur, class _CF, class _CR,
                bool _NumIsOne = false, bool _DenIsOne = false>
            struct _duration_cast_impl
            {
                template<class _Rep, class _Period>
                static _ToDur _cast(const duration<_Rep, _Period>& _d)
                {
                    typedef typename _ToDur::rep _to_dur_rep;
                    typedef 
                    typename 
                    detail::_duration_common_type<_to_dur_rep, _to_dur_rep, _Period>::type
                        _to_rep;
                    return _ToDur(
                        _chrono_convert<_to_rep>(
                            _chrono_convert<_CR>(detail::duration_count(_d), chrono_detail::_priority_tag<4>())
                            * _chrono_convert<_CR>(_CF::num, chrono_detail::_priority_tag<4>())
                            / _chrono_convert<_CR>(_CF::den, chrono_detail::_priority_tag<4>())
                        , chrono_detail::_priority_tag<4>())
                    );
                }
            };

            template<class _ToDur, class _CF, class _CR>
            struct _duration_cast_impl<_ToDur, _CF, _CR, true, true>
            {
                template<class _Rep, class _Period>
                static _ToDur _cast(const duration<_Rep, _Period>& _d)
                {
                    typedef typename _ToDur::rep _to_dur_rep;
                    typedef 
                    typename 
                    detail::_duration_common_type<_to_dur_rep, _to_dur_rep, _Period>::type
                        _to_rep;
                    return _ToDur(
                        _chrono_convert<_to_rep>(
                            detail::duration_count(_d), 
                            chrono_detail::_priority_tag<4>())
                    );
                }
            };

            template<class _ToDur, class _CF, class _CR>
            struct _duration_cast_impl<_ToDur, _CF, _CR, true, false>
            {
                template<class _Rep, class _Period>
                static _ToDur _cast(const duration<_Rep, _Period>& _d)
                {
                    typedef typename _ToDur::rep _to_dur_rep;
                    typedef 
                    typename 
                    detail::_duration_common_type<_to_dur_rep, _to_dur_rep, _Period>::type
                        _to_rep;
                    return _ToDur(
                        _chrono_convert<_to_rep>(
                            _chrono_convert<_CR>(detail::duration_count(_d), chrono_detail::_priority_tag<4>()) / 
                            _chrono_convert<_CR>(_CF::den, chrono_detail::_priority_tag<4>())
                        , chrono_detail::_priority_tag<4>())
                    );
                }
            };

            template<class _ToDur, class _CF, class _CR>
            struct _duration_cast_impl<_ToDur, _CF, _CR, false, true>
            {
                template<class _Rep, class _Period>
                static _ToDur _cast(const duration<_Rep, _Period>& _d)
                {
                    typedef typename _ToDur::rep _to_dur_rep;
                    typedef 
                    typename 
                    detail::_duration_common_type<_to_dur_rep, _to_dur_rep, _Period>::type
                        _to_rep;
                    return _ToDur(
                        _chrono_convert<_to_rep>(
                            _chrono_convert<_CR>(detail::duration_count(_d), chrono_detail::_priority_tag<4>()) * 
                            _chrono_convert<_CR>(_CF::num, chrono_detail::_priority_tag<4>()), 
                        chrono_detail::_priority_tag<4>())
                    );
                }
            };

            template<class _Tp>
            struct _is_duration
            { 
                static const bool value = false;
            };

            template<class _Rep, class _Period>
            struct _is_duration<duration<_Rep, _Period> >
            { 
                static const bool value = true;
            };

            template <bool, class _Tp>
            struct _enable_if_is_duration_impl
            {};

            template <class _Tp>
            struct _enable_if_is_duration_impl<true, _Tp>
            {
                typedef _Tp type;
            };

            template <class _Tp>
            struct _enable_if_is_duration:
                _enable_if_is_duration_impl<_is_duration<_Tp>::value == bool(true), _Tp>
            {};

            template <class _Tp>
            struct _disable_if_is_duration :
                _enable_if_is_duration_impl<_is_duration<_Tp>::value == bool(false), _Tp>
            {};

        } // namespace detail

        // duration_cast
        template<class _ToDur, class _Rep, class _Period>
        inline
        typename detail::_enable_if_is_duration<_ToDur>::type 
        duration_cast(const duration<_Rep, _Period> &_d)
        {
            typedef typename _ToDur::period    _to_period;
            typedef typename _ToDur::rep _to_rep;
            typedef ratio_divide<_Period, _to_period> _cf;
            typedef typename detail::_duration_common_type<_to_rep, _Rep, _Period>::type
                _cr;
            typedef  detail::_duration_cast_impl<_ToDur, _cf, _cr,
                _cf::num == 1, _cf::den == 1> _dc;
                
            return _dc::_cast(_d);
        }
        

        // duration_values
        template<class _Rep>
        struct duration_values        
        {
            static _Rep zero()
            {
                return _Rep(0);
            }

        #ifdef max
            static _Rep(max)()
        #else
            static _Rep max()
        #endif
            {
                return (std::numeric_limits<_Rep>::max)();
            }

        #ifdef min
            static _Rep(min)()
        #else
            static _Rep min()
        #endif
            {
                return (std::numeric_limits<_Rep>::min)();
            }

            // since we have no constexpr use this in template params
            struct template_constants
            {
                static const _Rep zero = 0;
                static const _Rep max = std::numeric_limits<_Rep>::max;
                static const _Rep min = std::numeric_limits<_Rep>::min;
            };
        };

        template<>
        struct duration_values<stdex::intmax_t>
        {
            static stdex::intmax_t zero()
            {
                return stdex::intmax_t(0);
            }

        #ifdef max
            static stdex::intmax_t(max)()
        #else
            static stdex::intmax_t max()
        #endif
            {
                return STDEX_INTMAX_MAX;
            }

        #ifdef min
            static stdex::intmax_t(min)()
        #else
            static stdex::intmax_t min()
        #endif
            {
                return STDEX_INTMAX_MIN;
            }

            // since we have no constexpr use this in template params
            struct template_constants
            {
                static const stdex::intmax_t zero = 0;
                static const stdex::intmax_t max = STDEX_INTMAX_MAX;
                static const stdex::intmax_t min = STDEX_INTMAX_MIN;
            };
        };

        template<>
        struct duration_values<stdex::uintmax_t>
        {
            static stdex::uintmax_t zero()
            {
                return stdex::uintmax_t(0);
            }

        #ifdef max
            static stdex::uintmax_t(max)()
        #else
            static stdex::uintmax_t max()
        #endif
            {
                return STDEX_UINTMAX_MAX;
            }

        #ifdef min
            static stdex::uintmax_t(min)()
        #else
            static stdex::uintmax_t min()
        #endif
            {
                return 0;
            }

            // since we have no constexpr use this in template params
            struct template_constants
            {
                static const stdex::uintmax_t zero = 0;
                static const stdex::uintmax_t max = STDEX_UINTMAX_MAX;
                static const stdex::uintmax_t min = 0;
            };
        };

        namespace intern
        {
            // since we have no static_assert in pre-C++11 we just compile-time assert this way:
            struct chrono_asserts
            {
                template<bool>
                struct rep_cannot_be_a_duration_assert; // if you are there means 1st template param _Rep in duration class is duration type

                template<bool>
                struct period_must_be_a_specialization_of_ratio_assert; // if you are there means 2nd template param _Period in duration class is not a specialization of ratio class

                template<bool>
                struct period_must_be_positive_assert; // if you are there means 2nd template param _Period in duration class is ratio of negative

                template<bool>
                struct a_clocks_minimum_duration_cannot_be_less_than_its_epoch_assert; // if you are there means that what it says

                template<bool>
                struct a_duration_with_an_integer_tick_count_cannot_be_constructed_from_a_floating_point_value_assert; // if you are there means that what it says

                template<bool>
                struct duration_does_not_use_floating_point_ticks_or_other_duration_period_is_not_exactly_divisible_by_current_period; // if you are there means that what it says

                template<bool>
                struct a_duration_with_an_integer_tick_count_cannot_be_constructed_from_a_duration_with_floating_point_tick_assert; // if you are there means that what it says
            };

            template<>
            struct chrono_asserts::rep_cannot_be_a_duration_assert<true>
            {
                typedef bool rep_cannot_be_a_duration_assert_failed;
            };

            template<>
            struct chrono_asserts::period_must_be_a_specialization_of_ratio_assert<true>
            {
                typedef bool period_must_be_a_specialization_of_ratio_assert_failed;
            };

            template<>
            struct chrono_asserts::period_must_be_positive_assert<true>
            {
                typedef bool period_must_be_positive_assert_failed;
            };

            template<>
            struct chrono_asserts::a_clocks_minimum_duration_cannot_be_less_than_its_epoch_assert<true>
            {
                typedef bool a_clocks_minimum_duration_cannot_be_less_than_its_epoch_assert_failed;
            };

            template<>
            struct chrono_asserts::a_duration_with_an_integer_tick_count_cannot_be_constructed_from_a_floating_point_value_assert<true>
            {
                typedef bool a_duration_with_an_integer_tick_count_cannot_be_constructed_from_a_floating_point_value_assert_failed;
            };

            template<>
            struct chrono_asserts::duration_does_not_use_floating_point_ticks_or_other_duration_period_is_not_exactly_divisible_by_current_period<true>
            {
                typedef bool duration_does_not_use_floating_point_ticks_or_other_duration_period_is_not_exactly_divisible_by_current_period_assert_failed;
            };

            template<>
            struct chrono_asserts::a_duration_with_an_integer_tick_count_cannot_be_constructed_from_a_duration_with_floating_point_tick_assert<true>
            {
                typedef bool a_duration_with_an_integer_tick_count_cannot_be_constructed_from_a_duration_with_floating_point_tick_assert_failed;
            };
        }

        //! Duration template class. This class provides enough functionality to
        //! implement @c this_thread::sleep_for().
        template <class _Rep, class _Period>
        class duration:
            public detail::duration_base<_Rep, _Period,
                detail::_use_big_int<_Rep, _Period>::value>
        {
        private:

            typedef detail::duration_base<_Rep, _Period, 
                detail::_use_big_int<_Rep, _Period>::value> base_type;

            typedef intern::chrono_asserts check;
            struct _disabled1;
            struct _disabled2;
            
            void _modulus(const _Rep &_r_in) { base_type::_r %= _r_in; }
            void _modulus(const duration &other) { base_type::_r %= detail::duration_count(other); }
            void _modulus(const _disabled1 &) { }
            void _modulus(const _disabled2 &) { }

            typedef typename check::rep_cannot_be_a_duration_assert< (detail::_is_duration<_Rep>::value == bool(false)) >::
                rep_cannot_be_a_duration_assert_failed
            check1; // if you are there means 1st template param _Rep is duration type
            typedef typename check::period_must_be_a_specialization_of_ratio_assert< (detail::_is_ratio<typename _Period::type>::value) >::
                period_must_be_a_specialization_of_ratio_assert_failed
            check2; // if you are there means 2nd template param _Period is not a specialization of ratio class
            typedef typename check::period_must_be_positive_assert< (_Period::num > 0) >::
                period_must_be_positive_assert_failed
            check3; // if you are there means 2nd template param _Period in duration class is ratio of negative

        public:
            typedef _Rep rep;
            typedef _Period period;

            //! Construct a duration by default.
            explicit duration():
                base_type(0)
            {};

            //! Construct a duration object with the given duration.
            template <class _Rep2>
            duration(const _Rep2 &_r_in) : 
                base_type(_r_in)
            {
                typedef typename check::a_duration_with_an_integer_tick_count_cannot_be_constructed_from_a_floating_point_value_assert<(treat_as_floating_point<_Rep>::value == bool(true)) || (treat_as_floating_point<_Rep2>::value == bool(false))>::
                    a_duration_with_an_integer_tick_count_cannot_be_constructed_from_a_floating_point_value_assert_failed
                check4; // if you are there means rep type is integer but floating-point type is passed as argument
            }

            
            duration(const duration& other):
                base_type(static_cast<const base_type&>(other))
            {
            }

            template<class _Rep2, class _Period2>
            duration(const duration<_Rep2, _Period2> &other):
                base_type(detail::duration_count( duration_cast<duration>(other) ))
            {    // construct from a duration
                typedef ratio_divide<_Period2, _Period> _Checked_type;

                typedef typename check::duration_does_not_use_floating_point_ticks_or_other_duration_period_is_not_exactly_divisible_by_current_period<
                    (treat_as_floating_point<_Rep>::value == bool(true)) || 
                    ( (_Checked_type::den == 1 && treat_as_floating_point<_Rep2>::value == bool(false)) )
                    >::duration_does_not_use_floating_point_ticks_or_other_duration_period_is_not_exactly_divisible_by_current_period_assert_failed
                check5;


                typedef typename check::a_duration_with_an_integer_tick_count_cannot_be_constructed_from_a_duration_with_floating_point_tick_assert<(treat_as_floating_point<_Rep>::value == bool(true)) || (treat_as_floating_point<_Rep2>::value == bool(false))>::
                    a_duration_with_an_integer_tick_count_cannot_be_constructed_from_a_duration_with_floating_point_tick_assert_failed
                check6; // if you are there means rep type is integer but floating-point duration type is passed as argument
            }

            //! Return the value of the duration object.
            rep count() const
            {
                return 
                    detail::_chrono_convert<rep>(
                        base_type::_r, detail::chrono_detail::_priority_tag<4>());
            }

            duration operator+() const
            {    // get value
                return (*this);
            }

            duration operator-() const
            {    // get negated value
                return (-base_type::_r);
            }

            duration& operator++()
            {    // increment rep
                ++base_type::_r;
                return (*this);
            }

            duration operator++(int)
            {    // postincrement rep
                return (duration<_Rep, _Period>(base_type::_r++));
            }

            duration& operator--()
            {    // decrement rep
                --base_type::_r;
                return (*this);
            }

            duration operator--(int)
            {    // postdecrement rep
                return (duration<_Rep, _Period>(base_type::_r--));
            }

            duration& operator+=(const duration &other)
            {    // add other to rep
                base_type::_r += other.base_type::_r;
                return (*this);
            }

            duration& operator-=(const duration &other)
            {    // subtract other from rep
                base_type::_r -= other.base_type::_r;
                return (*this);
            }

            duration& operator*=(const _Rep &_r_in)
            {    // multiply rep by r
                base_type::_r *= _r_in;
                return (*this);
            }

            duration& operator/=(const _Rep &_r_in)
            {    // divide rep by r
                base_type::_r /= _r_in;
                return (*this);
            }

            duration& operator%=(
            typename
            conditional<
                treat_as_floating_point<_Rep>::value == bool(false),
                const _Rep &,
                _disabled1&
            >::type _r_in
            )
            {    // modulus rep by r
                _modulus(_r_in);
                return (*this);
            }

            duration& operator%=(
            typename
            conditional<
                treat_as_floating_point<_Rep>::value == bool(false),
                const duration &,
                _disabled2&
            >::type other
            )
            {    // modulus rep by other
                _modulus(other);
                return (*this);
            }

            static const duration zero()
            {    // get zero value
                return duration_values<_Rep>::zero();
            }

        #ifdef max
            static const duration(max)()
        #else
            static const duration max()
        #endif
            {
                return (duration_values<_Rep>::max)();
            }

        #ifdef min
            static const duration(min)()
        #else
            static const duration min()
        #endif
            {
                return (duration_values<_Rep>::min)();
            }
        };

        template<class _Rep1, class _Period1,
            class _Rep2, class _Period2>
        inline 
        typename 
        common_type< 
            duration<_Rep1, _Period1>, 
            duration<_Rep2, _Period2> 
        >::type
        operator+(const duration<_Rep1, _Period1> &lhs,
                const duration<_Rep2, _Period2> &rhs)
        {
            typedef duration<_Rep1, _Period1>            _dur1;
            typedef duration<_Rep2, _Period2>            _dur2;
            typedef typename common_type<_dur1, _dur2>::type    _cd;

            return _cd(
                detail::duration_count(_cd(lhs)) + 
                detail::duration_count(_cd(rhs))
            );
        }

        template<class _Rep1, class _Period1,
            class _Rep2, class _Period2>
        inline 
        typename
        common_type< 
            duration<_Rep1, _Period1>, 
            duration<_Rep2, _Period2> 
        >::type
        operator-(const duration<_Rep1, _Period1> &lhs,
                const duration<_Rep2, _Period2> & rhs)
        {
            typedef duration<_Rep1, _Period1>            _dur1;
            typedef duration<_Rep2, _Period2>            _dur2;
            typedef typename common_type<_dur1, _dur2>::type    _cd;

            return _cd(
                detail::duration_count(_cd(lhs)) -
                detail::duration_count(_cd(rhs))
            );
        }

        namespace detail
        {
            template<bool, class _CRep>
            struct _rep_t_enable_if
            { };

            template<class _CRep>
            struct _rep_t_enable_if<true, _CRep>
            {
                typedef _CRep type;
            };

            template<class _Rep1, class _Rep2,
                class _CRep = typename common_type<_Rep1, _Rep2>::type>
            struct _common_rep_t:
                _rep_t_enable_if<sizeof(_Rep2) <= sizeof(_CRep), _CRep>
            {};
        } // namespace detail

        template<class _Rep1, class _Period, class _Rep2>
        duration<typename detail::_common_rep_t<_Rep1, _Rep2>::type, _Period>
        operator*(const duration<_Rep1, _Period> &_d, const _Rep2 &_s)
        {
            typedef duration<typename common_type<_Rep1, _Rep2>::type, _Period> _cd;

            return _cd(
                detail::duration_count(_cd(_d)) * _s
            );
        }

        template<class _Rep1, class _Rep2, class _Period>
        duration<typename detail::_common_rep_t<_Rep2, _Rep1>::type, _Period>
        operator*(const _Rep1 &_s, const duration<_Rep2, _Period> &_d)
        {
            return _d * _s;
        }

        template<class _Rep1, class _Period, class _Rep2>
        duration<typename detail::_common_rep_t<_Rep1, typename detail::_disable_if_is_duration<_Rep2>::type >::type, _Period>
        operator/(const duration<_Rep1, _Period> &_d, const _Rep2 &_s)
        {
            typedef duration<typename common_type<_Rep1, _Rep2>::type, _Period> _cd;

            return _cd(
                detail::duration_count(_cd(_d)) / _s
            );
        }

        template<class _Rep1, class _Period1,
            class _Rep2, class _Period2>
        typename common_type<_Rep1, _Rep2>::type
        operator/(const duration<_Rep1, _Period1> &lhs, const duration<_Rep2, _Period2> &rhs)
        {
            typedef duration<_Rep1, _Period1>            _dur1;
            typedef duration<_Rep2, _Period2>            _dur2;
            typedef typename common_type<_dur1, _dur2>::type    _cd;

            return 
                _cd(
                    detail::duration_count(_cd(lhs)) / 
                    detail::duration_count(_cd(rhs))
                ).count();
        }

        // DR 934.
        template<class _Rep1, class _Period, class _Rep2>    
        duration<typename detail::_common_rep_t<_Rep1, typename detail::_disable_if_is_duration<_Rep2>::type >::type, _Period>
        operator%(const duration<_Rep1, _Period> &_d, const _Rep2 &_s)
        {
            typedef duration<typename common_type<_Rep1, _Rep2>::type, _Period> _cd;
            return _cd(
                detail::duration_count(_cd(_d)) % _s
            );
        }

        template<class _Rep1, class _Period1, class _Rep2, class _Period2>
        typename common_type< duration<_Rep1, _Period1>, duration<_Rep2, _Period2> >::type
        operator%(const duration<_Rep1, _Period1> &lhs, const duration<_Rep2, _Period2> &rhs)
        {
            typedef duration<_Rep1, _Period1>            _dur1;
            typedef duration<_Rep2, _Period2>            _dur2;
            typedef typename common_type<_dur1, _dur2>::type    _cd;
            return _cd(
                detail::duration_count(_cd(lhs)) % 
                detail::duration_count(_cd(rhs))
            );
        }

        // comparisons
        template<class _Rep1, class _Period1, class _Rep2, class _Period2>
        bool operator==(const duration<_Rep1, _Period1> &lhs, const duration<_Rep2, _Period2> &rhs)
        {
            typedef duration<_Rep1, _Period1>            _dur1;
            typedef duration<_Rep2, _Period2>            _dur2;
            typedef typename common_type<_dur1, _dur2>::type    _ct;

            return 
                detail::duration_count(_ct(lhs)) == 
                detail::duration_count(_ct(rhs));
        }

        template<class _Rep1, class _Period1, class _Rep2, class _Period2>
        bool operator<(const duration<_Rep1, _Period1> &lhs, const duration<_Rep2, _Period2> &rhs)
        {
            typedef duration<_Rep1, _Period1>            _dur1;
            typedef duration<_Rep2, _Period2>            _dur2;
            typedef typename common_type<_dur1, _dur2>::type    _ct;

            return
                detail::duration_count(_ct(lhs)) <
                detail::duration_count(_ct(rhs));
        }

        template<class _Rep1, class _Period1, class _Rep2, class _Period2>
        bool operator!=(const duration<_Rep1, _Period1> &lhs, const duration<_Rep2, _Period2> &rhs)
        {
            return !(lhs == rhs);
        }

        template<class _Rep1, class _Period1, class _Rep2, class _Period2>
        bool operator<=(const duration<_Rep1, _Period1> &lhs, const duration<_Rep2, _Period2> &rhs)
        {
            return !(rhs < lhs);
        }

        template<class _Rep1, class _Period1, class _Rep2, class _Period2>
        bool operator>(const duration<_Rep1, _Period1> &lhs, const duration<_Rep2, _Period2> &rhs)
        {
            return rhs < lhs;
        }

        template<class _Rep1, class _Period1, class _Rep2, class _Period2>
        bool operator>=(const duration<_Rep1, _Period1> &lhs, const duration<_Rep2, _Period2> &rhs)
        {
            return !(lhs < rhs);
        }

        // Standard duration types.
        typedef duration<stdex::intmax_t, ratio<1, 1000000000> > nanoseconds; //!< Duration with the unit nanoseconds.
        typedef duration<stdex::intmax_t, ratio<1, 1000000> > microseconds;   //!< Duration with the unit microseconds.
        typedef duration<stdex::intmax_t, ratio<1, 1000> > milliseconds;      //!< Duration with the unit milliseconds.
        typedef duration<stdex::intmax_t> seconds;                            //!< Duration with the unit seconds.
        typedef duration<stdex::intmax_t, ratio<60> > minutes;                //!< Duration with the unit minutes.
        typedef duration<stdex::intmax_t, ratio<3600> > hours;                //!< Duration with the unit hours.

        template<class _Clock, class _Duration>
        class time_point
        {    // represents a point in time
        public:
            typedef _Clock clock;
            typedef _Duration duration;
            typedef typename _Duration::rep rep;
            typedef typename _Duration::period period;

            time_point()
                : _d(_Duration::zero())
            {}

            // construct from a duration
            explicit time_point(const duration &_d_in)
                : _d(_d_in)
            {}

            time_point(const time_point &other)
                : _d(other._d)
            {}

            // construct from another duration
            template<class _Duration2>
            time_point(const time_point<_Clock, _Duration2> &_tp)
                : _d(_tp.time_since_epoch())
            {}

            duration time_since_epoch() const
            {    // get duration from epoch
                return (_d);
            }

            time_point& operator+=(const duration &_d_in)
            {    // increment by duration
                _d += _d_in;
                return (*this);
            }

            time_point& operator-=(const duration &_d_in)
            {    // decrement by duration
                _d -= _d_in;
                return (*this);
            }

        #ifdef min
            static const time_point(min)()
        #else
            static const time_point min()
        #endif
            {    // get minimum time point
                return (time_point((duration::min)()));
            }

        #ifdef max
            static const time_point(max)()
        #else
            static const time_point max()
        #endif
            {    // get maximum time point
                return (time_point((duration::max)()));
            }

        private:
            duration _d;    // duration since the epoch
        };

        namespace detail
        {
            template <bool, class _ToDur, class _Clock>
            struct _time_point_enable_if_is_duration_impl
            {
                typedef time_point<_Clock, _ToDur> type;
            };

            template <class _ToDur, class _Clock>
            struct _time_point_enable_if_is_duration_impl<false, _ToDur, _Clock>
            { };

            template <class _ToDur, class _Clock>
            struct _time_point_enable_if_is_duration:
                _time_point_enable_if_is_duration_impl< _is_duration<_ToDur>::value, _ToDur, _Clock>
            { };

        } // namespace detail

        // time_point_cast
        template<class _ToDur, class _Clock, class _Dur>
        inline 
        typename 
        detail::_time_point_enable_if_is_duration<
            _ToDur, 
            _Clock
        >::type
        time_point_cast(const time_point<_Clock, _Dur> &_t)
        {
            typedef time_point<_Clock, _ToDur>             _time_point;

            return _time_point(duration_cast<_ToDur>(_t.time_since_epoch()));
        }

        template<class _Clock, class _Dur1,
            class _Rep2, class _Period2>
        time_point<_Clock,
        typename common_type<_Dur1, duration<_Rep2, _Period2> >::type>
        operator+(const time_point<_Clock, _Dur1> &lhs, const duration<_Rep2, _Period2> &rhs)
        {
            typedef duration<_Rep2, _Period2>            _dur2;
            typedef typename common_type<_Dur1, _dur2>::type    _ct;
            typedef time_point<_Clock, _ct>             _time_point;

            return _time_point(lhs.time_since_epoch() + rhs);
        }

        template<class _Rep1, class _Period1,
            class _Clock, class _Dur2>
        time_point<_Clock,
        typename common_type<duration<_Rep1, _Period1>, _Dur2>::type>
        operator+(const duration<_Rep1, _Period1> &lhs, const time_point<_Clock, _Dur2> &rhs)
        {
            typedef duration<_Rep1, _Period1>            _dur1;
            typedef typename common_type<_dur1, _Dur2>::type    _ct;
            typedef time_point<_Clock, _ct>             _time_point;

            return _time_point(rhs.time_since_epoch() + lhs);
        }

        template<class _Clock, class _Dur1,
            class _Rep2, class _Period2>
        time_point<_Clock,
        typename common_type<_Dur1, duration<_Rep2, _Period2> >::type>
        operator-(const time_point<_Clock, _Dur1> &lhs, const duration<_Rep2, _Period2> &rhs)
        {
            typedef duration<_Rep2, _Period2>            _dur2;
            typedef typename common_type<_Dur1, _dur2>::type    _ct;
            typedef time_point<_Clock, _ct>             _time_point;

            return _time_point(lhs.time_since_epoch() - rhs);
        }

        template<class _Clock, class _Dur1, class _Dur2>
        typename common_type<_Dur1, _Dur2>::type
        operator-(const time_point<_Clock, _Dur1> &lhs, const time_point<_Clock, _Dur2> &rhs)
        {
            return lhs.time_since_epoch() - rhs.time_since_epoch();
        }

        template<class _Clock, class _Dur1, class _Dur2>
        bool operator==(const time_point<_Clock, _Dur1> &lhs, const time_point<_Clock, _Dur2> &rhs)
        {
            return lhs.time_since_epoch() == rhs.time_since_epoch();
        }

        template<class _Clock, class _Dur1, class _Dur2>
        bool operator!=(const time_point<_Clock, _Dur1> &lhs, const time_point<_Clock, _Dur2> &rhs)
        {
            return !(lhs == rhs);
        }

        template<class _Clock, class _Dur1, class _Dur2>
        bool operator<(const time_point<_Clock, _Dur1> &lhs, const time_point<_Clock, _Dur2> &rhs)
        {
            return  lhs.time_since_epoch() < rhs.time_since_epoch();
        }

        template<class _Clock, class _Dur1, class _Dur2>
        bool operator<=(const time_point<_Clock, _Dur1> &lhs, const time_point<_Clock, _Dur2> &rhs)
        {
            return !(rhs < lhs);
        }

        template<class _Clock, class _Dur1, class _Dur2>
        bool operator>(const time_point<_Clock, _Dur1> &lhs, const time_point<_Clock, _Dur2> &rhs)
        {
            return rhs < lhs;
        }

        template<class _Clock, class _Dur1, class _Dur2>
        bool operator>=(const time_point<_Clock, _Dur1> &lhs, const time_point<_Clock, _Dur2> &rhs)
        {
            return !(lhs < rhs);
        }

        

        /**
     *  @brief System clock.
     *
     *  Time returned represents wall time from the system-wide clock.
     */
        struct system_clock
        {
            typedef 
            stdex::conditional<
                (sizeof(chrono::nanoseconds::rep) * CHAR_BIT >= 64) ||
                (detail::_use_big_int<chrono::nanoseconds::rep, chrono::nanoseconds::period>::value == bool(true)), 
                chrono::nanoseconds, 
                chrono::microseconds
            >::type duration;
            typedef system_clock::duration::rep rep;
            typedef system_clock::duration::period period;
            typedef chrono::time_point<system_clock, duration>     time_point;

            static const bool is_steady;

            static time_point
                now() _STDEX_NOEXCEPT_FUNCTION;

            // Map to POSIX API
            static stdex::timespec
                to_timespec(const time_point&) _STDEX_NOEXCEPT_FUNCTION;

            // Map to C API
            static stdex::time_t
                to_time_t(const time_point &_t) _STDEX_NOEXCEPT_FUNCTION
            {
                return 
                stdex::time_t(
                    static_cast<chrono::seconds>(_t.time_since_epoch()).count()
                );
            }

            static time_point
                from_time_t(stdex::time_t _t) _STDEX_NOEXCEPT_FUNCTION;

        private:
            typedef intern::chrono_asserts check;

            typedef duration_values<rep>::template_constants duration_constants;
    
            typedef check::a_clocks_minimum_duration_cannot_be_less_than_its_epoch_assert< (duration_constants::min < duration_constants::zero) >::
                a_clocks_minimum_duration_cannot_be_less_than_its_epoch_assert_failed    
            check1; // if you are there means that what it says
        };

        /**
     *  @brief Monotonic clock
     *
     *  Time returned has the property of only increasing at a uniform rate.
     */
        struct steady_clock
        {
            typedef 
            stdex::conditional<
                (sizeof(chrono::nanoseconds::rep) * CHAR_BIT >= 64) ||
                (detail::_use_big_int<chrono::nanoseconds::rep, chrono::nanoseconds::period>::value == bool(true)), 
                chrono::nanoseconds, 
                chrono::microseconds
            >::type duration;
            typedef steady_clock::duration::rep rep;
            typedef steady_clock::duration::period period;
            typedef chrono::time_point<steady_clock, duration>     time_point;

            static const bool is_steady;

            static time_point
                now() _STDEX_NOEXCEPT_FUNCTION;
        };


        /**
     *  @brief Highest-resolution clock
     *
     *  This is the clock "with the shortest tick period." Alias to
     *  std::system_clock until higher-than-nanosecond definitions
     *  become feasible.
     */
        typedef
        stdex::conditional<
            (steady_clock::period::den > system_clock::period::den),
            steady_clock,
            system_clock
        >::type high_resolution_clock;
    } // namespace chrono

    // literals
    namespace literals
    {
        namespace chrono_literals
        {
            // non-standard operator >> for literals macros to work:
            template <class _Rep, class _Period, class _Rep2>
            chrono::duration<_Rep, _Period> operator,(const _Rep2 &input, const chrono::duration<_Rep, _Period> &dur)
            {
                return chrono::duration<_Rep, _Period>(input);
            }

            template <class _Rep, class _Period>
            chrono::duration<_Rep, _Period> operator,(const float &input, const chrono::duration<_Rep, _Period> &dur);

            template <class _Rep, class _Period>
            chrono::duration<_Rep, _Period> operator,(const double &input, const chrono::duration<_Rep, _Period> &dur);
        } // namespace chrono_literals
    } // namespace literals

    namespace chrono
    {
        using namespace literals::chrono_literals;
    } // namespace chrono

    // use this literals as (10,sec) + (5,min) + (6,h)

    /*static const stdex::chrono::hours h;
    static const stdex::chrono::minutes min;
    static const stdex::chrono::seconds s;
    static const stdex::chrono::milliseconds ms;
    static const stdex::chrono::microseconds us;
    static const stdex::chrono::nanoseconds ns;*/

} // namespace stdex

#undef _STDEX_DELETED_FUNCTION
#undef _STDEX_NOEXCEPT_FUNCTION

#endif // _STDEX_CHRONO_H

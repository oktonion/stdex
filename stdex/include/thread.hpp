#ifndef _STDEX_THREAD_H
#define _STDEX_THREAD_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// stdex includes
#include "./chrono.hpp"
#include "./condition_variable.hpp"
#include "./type_traits.hpp"

// POSIX includes
#include <pthread.h>

// std includes
#include <ostream>
#include <memory>
#include <algorithm> // std::swap
#include <utility> // std::swap
#include <limits>

#ifdef _STDEX_NATIVE_CPP11_SUPPORT

#define _STDEX_DELETED_FUNCTION =delete
#define _STDEX_NOEXCEPT_FUNCTION noexcept 

#else

#define _STDEX_DELETED_FUNCTION 
#define _STDEX_NOEXCEPT_FUNCTION throw()

#endif

namespace stdex
{
    namespace intern
    {
        // since we have no static_assert in pre-C++11 we just compile-time assert this way:
        struct thread_asserts
        {
            template<bool> 
            struct operator_comma_is_user_defined_for_this_type_assert;
        };

        template<>
        struct thread_asserts::operator_comma_is_user_defined_for_this_type_assert<true>
        {
            typedef bool operator_comma_is_user_defined_for_this_type_assert_failed;
        };
    }


    namespace detail
    {
        namespace thread_detail
        {
            template<class _Tp>
            _Tp declval();

            template<unsigned _Rank> struct _priority_tag : _priority_tag < _Rank - 1 > {};
            template<> struct _priority_tag<0> {};
        }

        enum eTypeNullptr { _type_nullptr };
        enum eTypeNotNullptr { _type_not_nullptr };

        template<class _FuncT> struct _function_traits;

        template<class _R>
        struct _function_traits<_R(*)(void)>
        {
            typedef _R result_type;
        };

        template<class _R, class _T1>
        struct _function_traits<_R(*)(_T1)>
        {
            typedef _R result_type;
            typedef _T1 arg1_type;
            typedef _T1 argument_type;
        };

        template<class _R, class _T1, class _T2>
        struct _function_traits<_R(*)(_T1, _T2)>
        {
            typedef _R result_type;
            typedef _T1 arg1_type;
            typedef _T2 arg2_type;
            
            
        };

        template<class _R, class _T1, class _T2, class _T3>
        struct _function_traits<_R(*)(_T1, _T2, _T3)>
        {
            typedef _R result_type;
            typedef _T1 arg1_type;
            typedef _T2 arg2_type;
            typedef _T3 arg3_type;
        };

        template<class _R, class _T1, class _T2, class _T3, class _T4>
        struct _function_traits<_R(*)(_T1, _T2, _T3, _T4)>
        {
            typedef _R result_type;
            typedef _T1 arg1_type;
            typedef _T2 arg2_type;
            typedef _T3 arg3_type;
            typedef _T4 arg4_type;
        };

        template<class _R, class _T1, class _T2, class _T3, class _T4,
            class _T5>
        struct _function_traits<_R(*)(_T1, _T2, _T3, _T4, _T5)>
        {
            typedef _R result_type;
            typedef _T1 arg1_type;
            typedef _T2 arg2_type;
            typedef _T3 arg3_type;
            typedef _T4 arg4_type;
            typedef _T5 arg5_type;
        };

        template<class _R, class _T1, class _T2, class _T3, class _T4,
            class _T5, class _T6>
        struct _function_traits<_R(*)(_T1, _T2, _T3, _T4, _T5, _T6)>
        {
            typedef _R result_type;
            typedef _T1 arg1_type;
            typedef _T2 arg2_type;
            typedef _T3 arg3_type;
            typedef _T4 arg4_type;
            typedef _T5 arg5_type;
            typedef _T6 arg6_type;
        };

        template<class _R, class _T1, class _T2, class _T3, class _T4,
            class _T5, class _T6, class _T7>
        struct _function_traits<_R(*)(_T1, _T2, _T3, _T4, _T5, _T6, _T7)>
        {
            typedef _R result_type;
            typedef _T1 arg1_type;
            typedef _T2 arg2_type;
            typedef _T3 arg3_type;
            typedef _T4 arg4_type;
            typedef _T5 arg5_type;
            typedef _T6 arg6_type;
            typedef _T7 arg7_type;
        };

        template<class _R, class _T1, class _T2, class _T3, class _T4,
            class _T5, class _T6, class _T7, class _T8>
        struct _function_traits<_R(*)(_T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8)>
        {
            typedef _R result_type;
            typedef _T1 arg1_type;
            typedef _T2 arg2_type;
            typedef _T3 arg3_type;
            typedef _T4 arg4_type;
            typedef _T5 arg5_type;
            typedef _T6 arg6_type;
            typedef _T7 arg7_type;
            typedef _T8 arg8_type;
        };


        template<bool>
        struct _type_is_nullptr_helper
        { 
            typedef eTypeNullptr type;
            static const type value = _type_nullptr;
        };

        template<>
        struct _type_is_nullptr_helper<false>
        { 
            typedef eTypeNotNullptr type;
            static const type value = _type_not_nullptr;
        };

        template<class _Tp, class _ArgT>
        struct _type_is_nullptr_base_chooser_helper
        {
            static const bool _is_nullptr =
                _is_nullptr_t<_Tp>::value == bool(true) && 
                (is_pointer<_ArgT>::value == bool(true) || is_member_function_pointer<_ArgT>::value == bool(true));
        };

        template<class _Tp, class _ArgT>
        struct _type_is_nullptr_base_chooser
        {
            typedef _type_is_nullptr_base_chooser_helper<_Tp, _ArgT> _helper;
            typedef _type_is_nullptr_helper<bool(_helper::_is_nullptr == bool(true))> type;
        };
        template<class _Tp, class _ArgT = void*>
        struct _type_is_nullptr:
            _type_is_nullptr_base_chooser<_Tp, _ArgT>::type
        { };

        template<class _Arg1, class _Arg2>
        struct _args_are_same_helper:
            stdex::is_same<_Arg1, _Arg2>
        { };

        template<class _Arg1>
        struct _args_are_same_helper<_Arg1, detail::void_type>:
            stdex::true_type
        { };

        template<class _Arg2>
        struct _args_are_same_helper<detail::void_type, _Arg2>:
            stdex::true_type
        { };

        template<>
        struct _args_are_same_helper<detail::void_type, detail::void_type>:
            stdex::true_type
        { };

        template<
            class _FArg1 = detail::void_type, class _Arg1 = detail::void_type,
            class _FArg2 = detail::void_type, class _Arg2 = detail::void_type,
            class _FArg3 = detail::void_type, class _Arg3 = detail::void_type,
            class _FArg4 = detail::void_type, class _Arg4 = detail::void_type
        >
        struct _args_are_same
        {
            static const bool value =
                _args_are_same_helper<_Arg1, _FArg1>::value &&
                _args_are_same_helper<_Arg2, _FArg2>::value &&
                _args_are_same_helper<_Arg3, _FArg3>::value &&
                _args_are_same_helper<_Arg4, _FArg4>::value;
        };

        namespace thread_detail
        {
            class _dummy;
            _yes_type _is_able_to_pass_nullptr_with_comma_op_tester1(void*);
            _no_type _is_able_to_pass_nullptr_with_comma_op_tester1(...);
            _yes_type _is_able_to_pass_nullptr_with_comma_op_tester2(void(_dummy::*)(float, int));
            _no_type _is_able_to_pass_nullptr_with_comma_op_tester2(...);

            void _dummy_void_f();

            struct _is_able_to_pass_nullptr_with_comma_op
            {
                static const bool value = 
                    /*sizeof(
                        _is_able_to_pass_nullptr_with_comma_op_tester1((_dummy_void_f(), nullptr))
                    ) == sizeof(_yes_type) &&
                    sizeof(
                        _is_able_to_pass_nullptr_with_comma_op_tester2((_dummy_void_f(), nullptr))
                    ) == sizeof(_yes_type)*/false;
            };
        }
    } // namespace detail

    namespace intern
    {
        template<class>
        struct _has_feature;

        template<class>
        struct _has_bug;

        template<>
        struct _has_feature<class _stdex_could_pass_nullptr_with_comma_operator>:
            detail::thread_detail::_is_able_to_pass_nullptr_with_comma_op
        { };

        template<>
        struct _has_bug<class _stdex_could_not_pass_nullptr_with_comma_operator>
        { 
            static const bool value = 
                !_has_feature<_stdex_could_pass_nullptr_with_comma_operator>::value;
        };
    } // namespace intern

    namespace detail
    {
        namespace thread_detail
        {
            template<class _Tp>
            class _ref_wrapper
            {
            public:
                _ref_wrapper(_Tp& ref_) :
                    _ptr(&ref_)
                { }

                _ref_wrapper(const _ref_wrapper& other) :
                    _ptr(other._ptr)
                { }

                _ref_wrapper& operator=(const _ref_wrapper& other)
                {
                    _ptr = other._ptr;
                    return (*this);
                }

                operator _Tp& () const
                {
                    return *_ptr;
                }

                _Tp& get() const
                {
                    return *_ptr;
                }

            private:
                _Tp* _ptr;
            };

            template<class _Tp>
            inline
            _Tp& operator,(_Tp &value, 
                typename
                conditional<is_null_pointer<_Tp>::value, class _disable1&, nullptr_t>::type)
            {
                return value;
            }

            template<class _Tp>
            inline
            const _Tp& operator,(const _Tp& value,
                typename
                conditional<is_null_pointer<_Tp>::value, class _disable2&, nullptr_t>::type)
            {
                return value;
            }
            
            template<class _Tp>
            inline
            _Tp& operator,(const _ref_wrapper<_Tp> &value, nullptr_t)
            {
                return value.get();
            }

            inline
            stdex::nullptr_t& operator,(const _ref_wrapper<stdex::nullptr_t> &value, nullptr_t)
            {
                return value.get();
            }

            template<class _Tp>
            inline
            typename
            conditional<
                is_scalar<_Tp>::value, 
                typename
                conditional<
                    _and_<is_null_pointer<_Tp>, _or_<is_class<_Tp>, is_enum<_Tp>/**/>/**/>::value,
                    void,
                    _ref_wrapper<_Tp>
                >::type,
                _Tp&
            >::type _arg(_Tp &value)
            {
                return _ref_wrapper<_Tp>(value);
            }

            template<class _Tp>
            inline
            typename
            conditional<
                is_scalar<_Tp>::value, 
                typename
                conditional<
                    _and_<is_null_pointer<_Tp>, _or_<is_class<_Tp>, is_enum<_Tp>/**/>/**/>::value,
                    void,
                    _ref_wrapper<const _Tp>
                >::type,
                const _Tp&
            >::type _arg(const _Tp& value)
            {
                return _ref_wrapper<const _Tp>(value);
            }

            //inline void _arg(nullptr_t) {}
            
            template<class _Tp>
            _yes_type _is_same_return_type(_Tp, _Tp);
            template<class _Tp1, class _Tp2>
            _no_type _is_same_return_type(_Tp1, _Tp2);

            template<class _Tp>
            struct _has_no_user_defined_operator_comma_op
            {
                static const bool value = 
                    sizeof(_is_same_return_type( (declval<_Tp>(), declval<_Tp>()), declval<_Tp>() )) == sizeof(_yes_type);
            };
        }

        template<std::size_t, class _T0 = detail::void_type, class _T1 = detail::void_type, class _T2 = detail::void_type, class _T3 = detail::void_type, class _T4 = detail::void_type, class _T5 = detail::void_type, class _T6 = detail::void_type, class _T7 = detail::void_type, class _T8 = detail::void_type, class _T9 = detail::void_type, class _T10 = detail::void_type, class _T11 = detail::void_type, class _T12 = detail::void_type, class _T13 = detail::void_type, class _T14 = detail::void_type, class _T15 = detail::void_type, class _T16 = detail::void_type, class _T17 = detail::void_type, class _T18 = detail::void_type, class _T19 = detail::void_type, class _T20 = detail::void_type, class _T21 = detail::void_type, class _T22 = detail::void_type, class _T23 = detail::void_type, class _T24 = detail::void_type>
        struct _thread_args_helper;
        

        template<>
        struct _thread_args_helper<0>
        {
            struct _arguments
            {
                static const unsigned int count = 0;

                template<class _FuncT>
                struct _call_helper
                {
                    static void call(_FuncT &fp) { fp(); }
                };

                _arguments()
                { }

                template<class _FuncT>
                void push(_FuncT &fp)
                {
                    _call_helper<_FuncT>::call(fp);
                }

                template<class _ObjectT>
                struct _functor
                {
                    /*template<class _ReturnT>
                    struct _member_function
                    {
                        typedef _ReturnT(_ObjectT::*type)();
                        typedef _ReturnT(_ObjectT::*type_const)() const;
                    };*/

                    template<class _ReturnT>
                    static void push(const thread_detail::_priority_tag<5>&,
                        _arguments& args, const _ObjectT &fp)
                    {
                        _call_helper<const _ObjectT>::call(fp);
                    }

                    template<class _ReturnT>
                    static void push(const thread_detail::_priority_tag<4>&, 
                        _arguments& args, _ObjectT &fp)
                    {
                        _call_helper<_ObjectT>::call(fp);
                    }
                };
                
            };

            typedef _arguments arguments_type;
        };

        template<class _Arg1>
        struct _thread_args_helper<1, _Arg1>
        {
            struct _arguments
            {
                static const unsigned int count = 1;

                _Arg1 arg1;

                _arguments(_Arg1 arg1_) :
                    arg1(arg1_)
                { }

                template<class _FuncT>
                struct _call_helper
                {
                    static void call(_FuncT &fp, _arguments &ina, eTypeNotNullptr) { fp(ina.arg1); }
                    static void call(_FuncT &fp, _arguments &, eTypeNullptr) { fp(nullptr); }
                };
                
                template<class _ObjectT>
                struct _functor
                {
                    /*template<class _ReturnT>
                    struct _member_function
                    {
                        typedef _ReturnT(_ObjectT::*type)(_Arg1);
                        typedef _ReturnT(_ObjectT::*type_const)(_Arg1) const;
                    };*/

                    static 
                    void push(const thread_detail::_priority_tag<4>&, 
                        typename
                        conditional<
                            intern::_has_feature<intern::_stdex_could_pass_nullptr_with_comma_operator>::value,
                            _arguments&, 
                            int
                        >::type args, _ObjectT &fp)
                    {

                        typedef typename intern::thread_asserts::operator_comma_is_user_defined_for_this_type_assert<
                            bool(thread_detail::_has_no_user_defined_operator_comma_op<_Arg1>::value == bool(true))
                        >::operator_comma_is_user_defined_for_this_type_assert_failed
                        check1; // if you are there means there is user defined operator comma somewhere in global namespace
                        
                        using thread_detail::operator,;
                        using thread_detail::_arg;
                        
                        fp((_arg(args.arg1), nullptr));
                    }

                    static 
                    void push(const thread_detail::_priority_tag<4>&, 
                        typename
                        conditional<
                            intern::_has_bug<intern::_stdex_could_not_pass_nullptr_with_comma_operator>::value,
                            _arguments&, 
                            int
                        >::type args, _ObjectT &fp)
                    {
                        _call_helper<_ObjectT>::call(fp, args,
                            _type_is_nullptr<_Arg1>::value);
                    }
                };
            
                template<class _FuncT>
                void push(_FuncT &fp)
                {
                    _call_helper<_FuncT>::call(fp, *this,
                        _type_is_nullptr<_Arg1, typename _function_traits<_FuncT>::arg1_type>::value);
                }
            };

            typedef _arguments arguments_type;
        };

        template<class _Arg1, class _Arg2>
        struct _thread_args_helper<2, _Arg1, _Arg2>
        {
            struct _arguments
            {
                static const unsigned int count = 2;

                _Arg1 arg1;
                _Arg2 arg2;

                _arguments(_Arg1 arg1_, _Arg2 arg2_) :
                    arg1(arg1_), arg2(arg2_)
                { }

                template<class _FuncT>
                struct _call_helper
                {
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr) { fp(ina.arg1,ina.arg2); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr) { fp(ina.arg1,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,ina.arg2); }
                    static void call(_FuncT &fp, _arguments &,eTypeNullptr,eTypeNullptr) { fp(nullptr,nullptr); }
                };

                template<class _FuncT>
                void push(_FuncT &fp)
                {
                    _call_helper<_FuncT>::call(fp, *this,
                        _type_is_nullptr<_Arg1, typename _function_traits<_FuncT>::arg1_type>::value,
                        _type_is_nullptr<_Arg2, typename _function_traits<_FuncT>::arg2_type>::value);
                }

                template<class _ObjectT>
                struct _functor
                {
                    /*template<class _ReturnT>
                    struct _member_function
                    {
                        typedef _ReturnT(_ObjectT::*type)(_Arg1, _Arg2);
                        typedef _ReturnT(_ObjectT::*type_const)(_Arg1, _Arg2) const;
                    };*/

                    static 
                    void push(const thread_detail::_priority_tag<4>&, 
                        typename
                        conditional<
                            intern::_has_feature<intern::_stdex_could_pass_nullptr_with_comma_operator>::value,
                            _arguments&, 
                            int
                        >::type args, _ObjectT &fp)
                    {
                        typedef intern::thread_asserts check;

                        typedef typename check::operator_comma_is_user_defined_for_this_type_assert<
                            thread_detail::_has_no_user_defined_operator_comma_op<_Arg1>::value &&
                            thread_detail::_has_no_user_defined_operator_comma_op<_Arg2>::value
                        >::operator_comma_is_user_defined_for_this_type_assert_failed
                        check1; // if you are there means there is user defined operator comma somewhere in global namespace
                        
                        using thread_detail::operator,;
                        using thread_detail::_arg;
                        
                        fp((_arg(args.arg1), nullptr),
                           (_arg(args.arg2), nullptr));
                    }

                    static 
                    void push(const thread_detail::_priority_tag<4>&, 
                        typename
                        conditional<
                            intern::_has_bug<intern::_stdex_could_not_pass_nullptr_with_comma_operator>::value,
                            _arguments&, 
                            int
                        >::type args, _ObjectT &fp)
                    {
                        _call_helper<_ObjectT>::call(fp, args,
                            _type_is_nullptr<_Arg1>::value,
                            _type_is_nullptr<_Arg2>::value);
                    }
                };
            };

            typedef _arguments arguments_type;
        };

        template<class _Arg1, class _Arg2, class _Arg3>
        struct _thread_args_helper<3, _Arg1, _Arg2, _Arg3>
        {
            struct _arguments
            {
                static const unsigned int count = 3;

                _Arg1 arg1;
                _Arg2 arg2;
                _Arg3 arg3;

                _arguments(_Arg1 arg1_, _Arg2 arg2_, _Arg3 arg3_) :
                    arg1(arg1_), arg2(arg2_), arg3(arg3_)
                { }

                template<class _FuncT>
                struct _call_helper
                {
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ina.arg1,ina.arg2,ina.arg3); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ina.arg1,ina.arg2,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ina.arg1,nullptr,ina.arg3); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(ina.arg1,nullptr,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,ina.arg2,ina.arg3); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,ina.arg2,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,ina.arg3); }
                    static void call(_FuncT &fp, _arguments &,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,nullptr,nullptr); }
                };

                template<class _FuncT>
                void push(_FuncT &fp)
                {
                    _call_helper<_FuncT>::call(fp, *this,
                        _type_is_nullptr<_Arg1, typename _function_traits<_FuncT>::arg1_type>::value,
                        _type_is_nullptr<_Arg2, typename _function_traits<_FuncT>::arg2_type>::value,
                        _type_is_nullptr<_Arg3, typename _function_traits<_FuncT>::arg3_type>::value);
                }

                template<class _ObjectT>
                struct _functor
                {
                    /*template<class _ReturnT>
                    struct _member_function
                    {
                        typedef _ReturnT(_ObjectT::*type)(_Arg1, _Arg2, _Arg3);
                        typedef _ReturnT(_ObjectT::*type_const)(_Arg1, _Arg2, _Arg3) const;
                    };*/

                    static 
                    void push(const thread_detail::_priority_tag<4>&, 
                        typename
                        conditional<
                            intern::_has_feature<intern::_stdex_could_pass_nullptr_with_comma_operator>::value,
                            _arguments&, 
                            int
                        >::type args, _ObjectT &fp)
                    {
                        typedef intern::thread_asserts check;

                        typedef typename check::operator_comma_is_user_defined_for_this_type_assert<
                            thread_detail::_has_no_user_defined_operator_comma_op<_Arg1>::value &&
                            thread_detail::_has_no_user_defined_operator_comma_op<_Arg2>::value &&
                            thread_detail::_has_no_user_defined_operator_comma_op<_Arg3>::value
                        >::operator_comma_is_user_defined_for_this_type_assert_failed
                        check1; // if you are there means there is user defined operator comma somewhere in global namespace
                        
                        using thread_detail::operator,;
                        using thread_detail::_arg;
                        
                        fp((_arg(args.arg1), nullptr),
                           (_arg(args.arg2), nullptr),
                           (_arg(args.arg3), nullptr));
                    }

                    static 
                    void push(const thread_detail::_priority_tag<4>&, 
                        typename
                        conditional<
                            intern::_has_bug<intern::_stdex_could_not_pass_nullptr_with_comma_operator>::value,
                            _arguments&, 
                            int
                        >::type args, _ObjectT &fp)
                    {
                        _call_helper<_ObjectT>::call(fp, args,
                            _type_is_nullptr<_Arg1>::value,
                            _type_is_nullptr<_Arg2>::value,
                            _type_is_nullptr<_Arg3>::value);
                    }
                };
            };

            typedef _arguments arguments_type;
        };

        template<class _Arg1, class _Arg2, class _Arg3, class _Arg4>
        struct _thread_args_helper<4, _Arg1, _Arg2, _Arg3, _Arg4>
        {
            struct _arguments
            {
                static const unsigned int count = 4;

                _Arg1 arg1;
                _Arg2 arg2;
                _Arg3 arg3;
                _Arg4 arg4;

                _arguments(_Arg1 arg1_, _Arg2 arg2_, _Arg3 arg3_, _Arg4 arg4_) :
                    arg1(arg1_), arg2(arg2_), arg3(arg3_), arg4(arg4_)
                { }

                template<class _FuncT>
                struct _call_helper
                {
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ina.arg1,ina.arg2,ina.arg3,ina.arg4); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ina.arg1,ina.arg2,ina.arg3,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ina.arg1,ina.arg2,nullptr,ina.arg4); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(ina.arg1,ina.arg2,nullptr,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ina.arg1,nullptr,ina.arg3,ina.arg4); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ina.arg1,nullptr,ina.arg3,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ina.arg1,nullptr,nullptr,ina.arg4); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(ina.arg1,nullptr,nullptr,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,ina.arg2,ina.arg3,ina.arg4); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,ina.arg2,ina.arg3,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,ina.arg2,nullptr,ina.arg4); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,ina.arg2,nullptr,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,ina.arg3,ina.arg4); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,nullptr,ina.arg3,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,nullptr,ina.arg4); }
                    static void call(_FuncT &fp, _arguments &,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr, nullptr, nullptr, nullptr); }
                };

                template<class _FuncT>
                void push(_FuncT &fp)
                {
                    _call_helper<_FuncT>::call(fp, *this,
                        _type_is_nullptr<_Arg1, typename _function_traits<_FuncT>::arg1_type>::value,
                        _type_is_nullptr<_Arg2, typename _function_traits<_FuncT>::arg2_type>::value,
                        _type_is_nullptr<_Arg3, typename _function_traits<_FuncT>::arg3_type>::value,
                        _type_is_nullptr<_Arg4, typename _function_traits<_FuncT>::arg4_type>::value);
                }

                template<class _ObjectT>
                struct _functor
                {
                    /*template<class _ReturnT>
                    struct _member_function
                    {
                        typedef _ReturnT(_ObjectT::*type)(_Arg1, _Arg2, _Arg3, _Arg4);
                        typedef _ReturnT(_ObjectT::*type_const)(_Arg1, _Arg2, _Arg3, _Arg4) const;
                    };*/

                    static 
                    void push(const thread_detail::_priority_tag<4>&, 
                        typename
                        conditional<
                            intern::_has_feature<intern::_stdex_could_pass_nullptr_with_comma_operator>::value,
                            _arguments&, 
                            int
                        >::type args, _ObjectT &fp)
                    {
                        typedef intern::thread_asserts check;

                        typedef typename check::operator_comma_is_user_defined_for_this_type_assert<
                            thread_detail::_has_no_user_defined_operator_comma_op<_Arg1>::value &&
                            thread_detail::_has_no_user_defined_operator_comma_op<_Arg2>::value &&
                            thread_detail::_has_no_user_defined_operator_comma_op<_Arg3>::value &&
                            thread_detail::_has_no_user_defined_operator_comma_op<_Arg4>::value
                        >::operator_comma_is_user_defined_for_this_type_assert_failed
                        check1; // if you are there means there is user defined operator comma somewhere in global namespace
                        
                        using thread_detail::operator,;
                        using thread_detail::_arg;
                        
                        fp((_arg(args.arg1), nullptr),
                           (_arg(args.arg2), nullptr),
                           (_arg(args.arg3), nullptr),
                           (_arg(args.arg4), nullptr));
                    }

                    static 
                    void push(const thread_detail::_priority_tag<4>&, 
                        typename
                        conditional<
                            intern::_has_bug<intern::_stdex_could_not_pass_nullptr_with_comma_operator>::value,
                            _arguments&, 
                            int
                        >::type args, _ObjectT &fp)
                    {
                        _call_helper<_ObjectT>::call(fp, args,
                            _type_is_nullptr<_Arg1>::value,
                            _type_is_nullptr<_Arg2>::value,
                            _type_is_nullptr<_Arg3>::value,
                            _type_is_nullptr<_Arg4>::value);
                    }
                };
            };

            typedef _arguments arguments_type;
        };

        template<class _Arg1, class _Arg2, class _Arg3, class _Arg4, class _Arg5>
        struct _thread_args_helper<5, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5>
        {
            struct _arguments
            {
                static const unsigned int count = 5;

                _Arg1 arg1;
                _Arg2 arg2;
                _Arg3 arg3;
                _Arg4 arg4;
                _Arg5 arg5;

                _arguments(_Arg1 arg1_, _Arg2 arg2_, _Arg3 arg3_, _Arg4 arg4_, _Arg5 arg5_) :
                    arg1(arg1_), arg2(arg2_), arg3(arg3_), arg4(arg4_), arg5(arg5_)
                { }

                template<class _FuncT>
                struct _call_helper
                {
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ina.arg1,ina.arg2,ina.arg3,ina.arg4,ina.arg5); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ina.arg1,ina.arg2,ina.arg3,ina.arg4,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ina.arg1,ina.arg2,ina.arg3,nullptr,ina.arg5); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(ina.arg1,ina.arg2,ina.arg3,nullptr,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ina.arg1,ina.arg2,nullptr,ina.arg4,ina.arg5); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ina.arg1,ina.arg2,nullptr,ina.arg4,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ina.arg1,ina.arg2,nullptr,nullptr,ina.arg5); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(ina.arg1,ina.arg2,nullptr,nullptr,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ina.arg1,nullptr,ina.arg3,ina.arg4,ina.arg5); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ina.arg1,nullptr,ina.arg3,ina.arg4,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ina.arg1,nullptr,ina.arg3,nullptr,ina.arg5); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(ina.arg1,nullptr,ina.arg3,nullptr,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ina.arg1,nullptr,nullptr,ina.arg4,ina.arg5); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ina.arg1,nullptr,nullptr,ina.arg4,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ina.arg1,nullptr,nullptr,nullptr,ina.arg5); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(ina.arg1,nullptr,nullptr,nullptr,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,ina.arg2,ina.arg3,ina.arg4,ina.arg5); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,ina.arg2,ina.arg3,ina.arg4,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,ina.arg2,ina.arg3,nullptr,ina.arg5); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,ina.arg2,ina.arg3,nullptr,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,ina.arg2,nullptr,ina.arg4,ina.arg5); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,ina.arg2,nullptr,ina.arg4,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,ina.arg2,nullptr,nullptr,ina.arg5); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,ina.arg2,nullptr,nullptr,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,ina.arg3,ina.arg4,ina.arg5); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,nullptr,ina.arg3,ina.arg4,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,ina.arg3,nullptr,ina.arg5); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,nullptr,ina.arg3,nullptr,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,nullptr,ina.arg4,ina.arg5); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,nullptr,nullptr,ina.arg4,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,nullptr,nullptr,ina.arg5); }
                    static void call(_FuncT &fp, _arguments &,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,nullptr,nullptr,nullptr,nullptr); }
                };

                template<class _FuncT>
                void push(_FuncT &fp)
                {
                    _call_helper<_FuncT>::call(fp, *this,
                        _type_is_nullptr<_Arg1, typename _function_traits<_FuncT>::arg1_type>::value,
                        _type_is_nullptr<_Arg2, typename _function_traits<_FuncT>::arg2_type>::value,
                        _type_is_nullptr<_Arg3, typename _function_traits<_FuncT>::arg3_type>::value,
                        _type_is_nullptr<_Arg4, typename _function_traits<_FuncT>::arg4_type>::value,
                        _type_is_nullptr<_Arg5, typename _function_traits<_FuncT>::arg5_type>::value);
                }

                template<class _ObjectT>
                struct _functor
                {
                    /*template<class _ReturnT>
                    struct _member_function
                    {
                        typedef _ReturnT(_ObjectT::*type)(_Arg1, _Arg2, _Arg3, _Arg4, _Arg5);
                        typedef _ReturnT(_ObjectT::*type_const)(_Arg1, _Arg2, _Arg3, _Arg4, _Arg5) const;
                    };*/

                    static 
                    void push(const thread_detail::_priority_tag<4>&, 
                        typename
                        conditional<
                            intern::_has_feature<intern::_stdex_could_pass_nullptr_with_comma_operator>::value,
                            _arguments&, 
                            int
                        >::type args, _ObjectT &fp)
                    {
                        typedef intern::thread_asserts check;

                        typedef typename check::operator_comma_is_user_defined_for_this_type_assert<
                            thread_detail::_has_no_user_defined_operator_comma_op<_Arg1>::value &&
                            thread_detail::_has_no_user_defined_operator_comma_op<_Arg2>::value &&
                            thread_detail::_has_no_user_defined_operator_comma_op<_Arg3>::value &&
                            thread_detail::_has_no_user_defined_operator_comma_op<_Arg4>::value &&
                            thread_detail::_has_no_user_defined_operator_comma_op<_Arg5>::value
                        >::operator_comma_is_user_defined_for_this_type_assert_failed
                        check1; // if you are there means there is user defined operator comma somewhere in global namespace
                        
                        using thread_detail::operator,;
                        using thread_detail::_arg;

                        fp((_arg(args.arg1), nullptr),
                           (_arg(args.arg2), nullptr),
                           (_arg(args.arg3), nullptr),
                           (_arg(args.arg4), nullptr),
                           (_arg(args.arg5), nullptr));
                    }

                    static 
                    void push(const thread_detail::_priority_tag<4>&, 
                        typename
                        conditional<
                            intern::_has_bug<intern::_stdex_could_not_pass_nullptr_with_comma_operator>::value,
                            _arguments&, 
                            int
                        >::type args, _ObjectT &fp)
                    {
                        _call_helper<_ObjectT>::call(fp, args,
                            _type_is_nullptr<_Arg1>::value,
                            _type_is_nullptr<_Arg2>::value,
                            _type_is_nullptr<_Arg3>::value,
                            _type_is_nullptr<_Arg4>::value,
                            _type_is_nullptr<_Arg5>::value);
                    }
                };
            };

            typedef _arguments arguments_type;
        };

        template<class _Arg1, class _Arg2, class _Arg3, class _Arg4, class _Arg5, class _Arg6>
        struct _thread_args_helper<6, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6>
        {
            struct _arguments
            {
                static const unsigned int count = 6;

                _Arg1 arg1;
                _Arg2 arg2;
                _Arg3 arg3;
                _Arg4 arg4;
                _Arg5 arg5;
                _Arg6 arg6;

                _arguments(_Arg1 arg1_, _Arg2 arg2_, _Arg3 arg3_, _Arg4 arg4_, _Arg5 arg5_, _Arg6 arg6_) :
                    arg1(arg1_), arg2(arg2_), arg3(arg3_), arg4(arg4_), arg5(arg5_), arg6(arg6_)
                { }

                template<class _FuncT>
                struct _call_helper
                {
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ina.arg1,ina.arg2,ina.arg3,ina.arg4,ina.arg5,ina.arg6); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ina.arg1,ina.arg2,ina.arg3,ina.arg4,ina.arg5,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ina.arg1,ina.arg2,ina.arg3,ina.arg4,nullptr,ina.arg6); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(ina.arg1,ina.arg2,ina.arg3,ina.arg4,nullptr,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ina.arg1,ina.arg2,ina.arg3,nullptr,ina.arg5,ina.arg6); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ina.arg1,ina.arg2,ina.arg3,nullptr,ina.arg5,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ina.arg1,ina.arg2,ina.arg3,nullptr,nullptr,ina.arg6); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(ina.arg1,ina.arg2,ina.arg3,nullptr,nullptr,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ina.arg1,ina.arg2,nullptr,ina.arg4,ina.arg5,ina.arg6); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ina.arg1,ina.arg2,nullptr,ina.arg4,ina.arg5,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ina.arg1,ina.arg2,nullptr,ina.arg4,nullptr,ina.arg6); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(ina.arg1,ina.arg2,nullptr,ina.arg4,nullptr,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ina.arg1,ina.arg2,nullptr,nullptr,ina.arg5,ina.arg6); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ina.arg1,ina.arg2,nullptr,nullptr,ina.arg5,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ina.arg1,ina.arg2,nullptr,nullptr,nullptr,ina.arg6); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(ina.arg1,ina.arg2,nullptr,nullptr,nullptr,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ina.arg1,nullptr,ina.arg3,ina.arg4,ina.arg5,ina.arg6); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ina.arg1,nullptr,ina.arg3,ina.arg4,ina.arg5,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ina.arg1,nullptr,ina.arg3,ina.arg4,nullptr,ina.arg6); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(ina.arg1,nullptr,ina.arg3,ina.arg4,nullptr,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ina.arg1,nullptr,ina.arg3,nullptr,ina.arg5,ina.arg6); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ina.arg1,nullptr,ina.arg3,nullptr,ina.arg5,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ina.arg1,nullptr,ina.arg3,nullptr,nullptr,ina.arg6); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(ina.arg1,nullptr,ina.arg3,nullptr,nullptr,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ina.arg1,nullptr,nullptr,ina.arg4,ina.arg5,ina.arg6); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ina.arg1,nullptr,nullptr,ina.arg4,ina.arg5,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ina.arg1,nullptr,nullptr,ina.arg4,nullptr,ina.arg6); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(ina.arg1,nullptr,nullptr,ina.arg4,nullptr,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ina.arg1,nullptr,nullptr,nullptr,ina.arg5,ina.arg6); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ina.arg1,nullptr,nullptr,nullptr,ina.arg5,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ina.arg1,nullptr,nullptr,nullptr,nullptr,ina.arg6); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(ina.arg1,nullptr,nullptr,nullptr,nullptr,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,ina.arg2,ina.arg3,ina.arg4,ina.arg5,ina.arg6); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,ina.arg2,ina.arg3,ina.arg4,ina.arg5,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,ina.arg2,ina.arg3,ina.arg4,nullptr,ina.arg6); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,ina.arg2,ina.arg3,ina.arg4,nullptr,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,ina.arg2,ina.arg3,nullptr,ina.arg5,ina.arg6); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,ina.arg2,ina.arg3,nullptr,ina.arg5,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,ina.arg2,ina.arg3,nullptr,nullptr,ina.arg6); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,ina.arg2,ina.arg3,nullptr,nullptr,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,ina.arg2,nullptr,ina.arg4,ina.arg5,ina.arg6); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,ina.arg2,nullptr,ina.arg4,ina.arg5,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,ina.arg2,nullptr,ina.arg4,nullptr,ina.arg6); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,ina.arg2,nullptr,ina.arg4,nullptr,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,ina.arg2,nullptr,nullptr,ina.arg5,ina.arg6); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,ina.arg2,nullptr,nullptr,ina.arg5,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,ina.arg2,nullptr,nullptr,nullptr,ina.arg6); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,ina.arg2,nullptr,nullptr,nullptr,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,ina.arg3,ina.arg4,ina.arg5,ina.arg6); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,nullptr,ina.arg3,ina.arg4,ina.arg5,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,ina.arg3,ina.arg4,nullptr,ina.arg6); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,nullptr,ina.arg3,ina.arg4,nullptr,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,ina.arg3,nullptr,ina.arg5,ina.arg6); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,nullptr,ina.arg3,nullptr,ina.arg5,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,ina.arg3,nullptr,nullptr,ina.arg6); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,nullptr,ina.arg3,nullptr,nullptr,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,nullptr,ina.arg4,ina.arg5,ina.arg6); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,nullptr,nullptr,ina.arg4,ina.arg5,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,nullptr,ina.arg4,nullptr,ina.arg6); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,nullptr,nullptr,ina.arg4,nullptr,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,nullptr,nullptr,ina.arg5,ina.arg6); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,nullptr,nullptr,nullptr,ina.arg5,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,nullptr,nullptr,nullptr,ina.arg6); }
                    static void call(_FuncT &fp, _arguments &,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,nullptr,nullptr,nullptr,nullptr,nullptr); }
                };

                template<class _FuncT>
                void push(_FuncT &fp)
                {
                    _call_helper<_FuncT>::call(fp, *this,
                        _type_is_nullptr<_Arg1, typename _function_traits<_FuncT>::arg1_type>::value,
                        _type_is_nullptr<_Arg2, typename _function_traits<_FuncT>::arg2_type>::value,
                        _type_is_nullptr<_Arg3, typename _function_traits<_FuncT>::arg3_type>::value,
                        _type_is_nullptr<_Arg4, typename _function_traits<_FuncT>::arg4_type>::value,
                        _type_is_nullptr<_Arg5, typename _function_traits<_FuncT>::arg5_type>::value,
                        _type_is_nullptr<_Arg6, typename _function_traits<_FuncT>::arg6_type>::value);
                }

                template<class _ObjectT>
                struct _functor
                {
                    /*template<class _ReturnT>
                    struct _member_function
                    {
                        typedef _ReturnT(_ObjectT::*type)(_Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6);
                        typedef _ReturnT(_ObjectT::*type_const)(_Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6) const;
                    };*/

                    static 
                    void push(const thread_detail::_priority_tag<4>&, 
                        typename
                        conditional<
                            intern::_has_feature<intern::_stdex_could_pass_nullptr_with_comma_operator>::value,
                            _arguments&, 
                            int
                        >::type args, _ObjectT &fp)
                    {
                        typedef intern::thread_asserts check;

                        typedef typename check::operator_comma_is_user_defined_for_this_type_assert<
                            thread_detail::_has_no_user_defined_operator_comma_op<_Arg1>::value &&
                            thread_detail::_has_no_user_defined_operator_comma_op<_Arg2>::value &&
                            thread_detail::_has_no_user_defined_operator_comma_op<_Arg3>::value &&
                            thread_detail::_has_no_user_defined_operator_comma_op<_Arg4>::value &&
                            thread_detail::_has_no_user_defined_operator_comma_op<_Arg5>::value && 
                            thread_detail::_has_no_user_defined_operator_comma_op<_Arg6>::value
                        >::operator_comma_is_user_defined_for_this_type_assert_failed
                        check1; // if you are there means there is user defined operator comma somewhere in global namespace
                        
                        using thread_detail::operator,;
                        using thread_detail::_arg;
                        
                        fp((_arg(args.arg1), nullptr),
                           (_arg(args.arg2), nullptr),
                           (_arg(args.arg3), nullptr),
                           (_arg(args.arg4), nullptr),
                           (_arg(args.arg5), nullptr),
                           (_arg(args.arg6), nullptr));
                    }

                    static 
                    void push(const thread_detail::_priority_tag<4>&, 
                        typename
                        conditional<
                            intern::_has_bug<intern::_stdex_could_not_pass_nullptr_with_comma_operator>::value,
                            _arguments&, 
                            int
                        >::type args, _ObjectT &fp)
                    {
                        _call_helper<_ObjectT>::call(fp, args,
                            _type_is_nullptr<_Arg1>::value,
                            _type_is_nullptr<_Arg2>::value,
                            _type_is_nullptr<_Arg3>::value,
                            _type_is_nullptr<_Arg4>::value,
                            _type_is_nullptr<_Arg5>::value,
                            _type_is_nullptr<_Arg6>::value);
                    }
                };
            };

            typedef _arguments arguments_type;
        };

        template<class _Arg1, class _Arg2, class _Arg3, class _Arg4, class _Arg5, class _Arg6, class _Arg7>
        struct _thread_args_helper<7, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7>
        {
            struct _arguments
            {
                static const unsigned int count = 7;

                _Arg1 arg1;
                _Arg2 arg2;
                _Arg3 arg3;
                _Arg4 arg4;
                _Arg5 arg5;
                _Arg6 arg6;
                _Arg7 arg7;

                _arguments(_Arg1 arg1_, _Arg2 arg2_, _Arg3 arg3_, _Arg4 arg4_, _Arg5 arg5_, _Arg6 arg6_, _Arg7 arg7_) :
                    arg1(arg1_), arg2(arg2_), arg3(arg3_), arg4(arg4_), arg5(arg5_), arg6(arg6_), arg7(arg7_)
                { }

                template<class _FuncT>
                struct _call_helper
                {
                    static void call(_FuncT &fp,_arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ina.arg1,ina.arg2,ina.arg3,ina.arg4,ina.arg5,ina.arg6,ina.arg7); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ina.arg1,ina.arg2,ina.arg3,ina.arg4,ina.arg5,ina.arg6,nullptr); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ina.arg1,ina.arg2,ina.arg3,ina.arg4,ina.arg5,nullptr,ina.arg7); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(ina.arg1,ina.arg2,ina.arg3,ina.arg4,ina.arg5,nullptr,nullptr); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ina.arg1,ina.arg2,ina.arg3,ina.arg4,nullptr,ina.arg6,ina.arg7); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ina.arg1,ina.arg2,ina.arg3,ina.arg4,nullptr,ina.arg6,nullptr); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ina.arg1,ina.arg2,ina.arg3,ina.arg4,nullptr,nullptr,ina.arg7); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(ina.arg1,ina.arg2,ina.arg3,ina.arg4,nullptr,nullptr,nullptr); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ina.arg1,ina.arg2,ina.arg3,nullptr,ina.arg5,ina.arg6,ina.arg7); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ina.arg1,ina.arg2,ina.arg3,nullptr,ina.arg5,ina.arg6,nullptr); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ina.arg1,ina.arg2,ina.arg3,nullptr,ina.arg5,nullptr,ina.arg7); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(ina.arg1,ina.arg2,ina.arg3,nullptr,ina.arg5,nullptr,nullptr); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ina.arg1,ina.arg2,ina.arg3,nullptr,nullptr,ina.arg6,ina.arg7); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ina.arg1,ina.arg2,ina.arg3,nullptr,nullptr,ina.arg6,nullptr); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ina.arg1,ina.arg2,ina.arg3,nullptr,nullptr,nullptr,ina.arg7); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(ina.arg1,ina.arg2,ina.arg3,nullptr,nullptr,nullptr,nullptr); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ina.arg1,ina.arg2,nullptr,ina.arg4,ina.arg5,ina.arg6,ina.arg7); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ina.arg1,ina.arg2,nullptr,ina.arg4,ina.arg5,ina.arg6,nullptr); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ina.arg1,ina.arg2,nullptr,ina.arg4,ina.arg5,nullptr,ina.arg7); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(ina.arg1,ina.arg2,nullptr,ina.arg4,ina.arg5,nullptr,nullptr); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ina.arg1,ina.arg2,nullptr,ina.arg4,nullptr,ina.arg6,ina.arg7); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ina.arg1,ina.arg2,nullptr,ina.arg4,nullptr,ina.arg6,nullptr); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ina.arg1,ina.arg2,nullptr,ina.arg4,nullptr,nullptr,ina.arg7); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(ina.arg1,ina.arg2,nullptr,ina.arg4,nullptr,nullptr,nullptr); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ina.arg1,ina.arg2,nullptr,nullptr,ina.arg5,ina.arg6,ina.arg7); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ina.arg1,ina.arg2,nullptr,nullptr,ina.arg5,ina.arg6,nullptr); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ina.arg1,ina.arg2,nullptr,nullptr,ina.arg5,nullptr,ina.arg7); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(ina.arg1,ina.arg2,nullptr,nullptr,ina.arg5,nullptr,nullptr); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ina.arg1,ina.arg2,nullptr,nullptr,nullptr,ina.arg6,ina.arg7); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ina.arg1,ina.arg2,nullptr,nullptr,nullptr,ina.arg6,nullptr); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ina.arg1,ina.arg2,nullptr,nullptr,nullptr,nullptr,ina.arg7); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(ina.arg1,ina.arg2,nullptr,nullptr,nullptr,nullptr,nullptr); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ina.arg1,nullptr,ina.arg3,ina.arg4,ina.arg5,ina.arg6,ina.arg7); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ina.arg1,nullptr,ina.arg3,ina.arg4,ina.arg5,ina.arg6,nullptr); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ina.arg1,nullptr,ina.arg3,ina.arg4,ina.arg5,nullptr,ina.arg7); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(ina.arg1,nullptr,ina.arg3,ina.arg4,ina.arg5,nullptr,nullptr); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ina.arg1,nullptr,ina.arg3,ina.arg4,nullptr,ina.arg6,ina.arg7); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ina.arg1,nullptr,ina.arg3,ina.arg4,nullptr,ina.arg6,nullptr); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ina.arg1,nullptr,ina.arg3,ina.arg4,nullptr,nullptr,ina.arg7); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(ina.arg1,nullptr,ina.arg3,ina.arg4,nullptr,nullptr,nullptr); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ina.arg1,nullptr,ina.arg3,nullptr,ina.arg5,ina.arg6,ina.arg7); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ina.arg1,nullptr,ina.arg3,nullptr,ina.arg5,ina.arg6,nullptr); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ina.arg1,nullptr,ina.arg3,nullptr,ina.arg5,nullptr,ina.arg7); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(ina.arg1,nullptr,ina.arg3,nullptr,ina.arg5,nullptr,nullptr); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ina.arg1,nullptr,ina.arg3,nullptr,nullptr,ina.arg6,ina.arg7); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ina.arg1,nullptr,ina.arg3,nullptr,nullptr,ina.arg6,nullptr); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ina.arg1,nullptr,ina.arg3,nullptr,nullptr,nullptr,ina.arg7); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(ina.arg1,nullptr,ina.arg3,nullptr,nullptr,nullptr,nullptr); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ina.arg1,nullptr,nullptr,ina.arg4,ina.arg5,ina.arg6,ina.arg7); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ina.arg1,nullptr,nullptr,ina.arg4,ina.arg5,ina.arg6,nullptr); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ina.arg1,nullptr,nullptr,ina.arg4,ina.arg5,nullptr,ina.arg7); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(ina.arg1,nullptr,nullptr,ina.arg4,ina.arg5,nullptr,nullptr); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ina.arg1,nullptr,nullptr,ina.arg4,nullptr,ina.arg6,ina.arg7); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ina.arg1,nullptr,nullptr,ina.arg4,nullptr,ina.arg6,nullptr); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ina.arg1,nullptr,nullptr,ina.arg4,nullptr,nullptr,ina.arg7); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(ina.arg1,nullptr,nullptr,ina.arg4,nullptr,nullptr,nullptr); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ina.arg1,nullptr,nullptr,nullptr,ina.arg5,ina.arg6,ina.arg7); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ina.arg1,nullptr,nullptr,nullptr,ina.arg5,ina.arg6,nullptr); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ina.arg1,nullptr,nullptr,nullptr,ina.arg5,nullptr,ina.arg7); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(ina.arg1,nullptr,nullptr,nullptr,ina.arg5,nullptr,nullptr); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ina.arg1,nullptr,nullptr,nullptr,nullptr,ina.arg6,ina.arg7); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ina.arg1,nullptr,nullptr,nullptr,nullptr,ina.arg6,nullptr); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ina.arg1,nullptr,nullptr,nullptr,nullptr,nullptr,ina.arg7); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(ina.arg1,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,ina.arg2,ina.arg3,ina.arg4,ina.arg5,ina.arg6,ina.arg7); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,ina.arg2,ina.arg3,ina.arg4,ina.arg5,ina.arg6,nullptr); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,ina.arg2,ina.arg3,ina.arg4,ina.arg5,nullptr,ina.arg7); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,ina.arg2,ina.arg3,ina.arg4,ina.arg5,nullptr,nullptr); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,ina.arg2,ina.arg3,ina.arg4,nullptr,ina.arg6,ina.arg7); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,ina.arg2,ina.arg3,ina.arg4,nullptr,ina.arg6,nullptr); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,ina.arg2,ina.arg3,ina.arg4,nullptr,nullptr,ina.arg7); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,ina.arg2,ina.arg3,ina.arg4,nullptr,nullptr,nullptr); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,ina.arg2,ina.arg3,nullptr,ina.arg5,ina.arg6,ina.arg7); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,ina.arg2,ina.arg3,nullptr,ina.arg5,ina.arg6,nullptr); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,ina.arg2,ina.arg3,nullptr,ina.arg5,nullptr,ina.arg7); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,ina.arg2,ina.arg3,nullptr,ina.arg5,nullptr,nullptr); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,ina.arg2,ina.arg3,nullptr,nullptr,ina.arg6,ina.arg7); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,ina.arg2,ina.arg3,nullptr,nullptr,ina.arg6,nullptr); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,ina.arg2,ina.arg3,nullptr,nullptr,nullptr,ina.arg7); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,ina.arg2,ina.arg3,nullptr,nullptr,nullptr,nullptr); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,ina.arg2,nullptr,ina.arg4,ina.arg5,ina.arg6,ina.arg7); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,ina.arg2,nullptr,ina.arg4,ina.arg5,ina.arg6,nullptr); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,ina.arg2,nullptr,ina.arg4,ina.arg5,nullptr,ina.arg7); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,ina.arg2,nullptr,ina.arg4,ina.arg5,nullptr,nullptr); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,ina.arg2,nullptr,ina.arg4,nullptr,ina.arg6,ina.arg7); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,ina.arg2,nullptr,ina.arg4,nullptr,ina.arg6,nullptr); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,ina.arg2,nullptr,ina.arg4,nullptr,nullptr,ina.arg7); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,ina.arg2,nullptr,ina.arg4,nullptr,nullptr,nullptr); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,ina.arg2,nullptr,nullptr,ina.arg5,ina.arg6,ina.arg7); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,ina.arg2,nullptr,nullptr,ina.arg5,ina.arg6,nullptr); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,ina.arg2,nullptr,nullptr,ina.arg5,nullptr,ina.arg7); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,ina.arg2,nullptr,nullptr,ina.arg5,nullptr,nullptr); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,ina.arg2,nullptr,nullptr,nullptr,ina.arg6,ina.arg7); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,ina.arg2,nullptr,nullptr,nullptr,ina.arg6,nullptr); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,ina.arg2,nullptr,nullptr,nullptr,nullptr,ina.arg7); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,ina.arg2,nullptr,nullptr,nullptr,nullptr,nullptr); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,ina.arg3,ina.arg4,ina.arg5,ina.arg6,ina.arg7); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,nullptr,ina.arg3,ina.arg4,ina.arg5,ina.arg6,nullptr); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,ina.arg3,ina.arg4,ina.arg5,nullptr,ina.arg7); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,nullptr,ina.arg3,ina.arg4,ina.arg5,nullptr,nullptr); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,ina.arg3,ina.arg4,nullptr,ina.arg6,ina.arg7); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,nullptr,ina.arg3,ina.arg4,nullptr,ina.arg6,nullptr); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,ina.arg3,ina.arg4,nullptr,nullptr,ina.arg7); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,nullptr,ina.arg3,ina.arg4,nullptr,nullptr,nullptr); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,ina.arg3,nullptr,ina.arg5,ina.arg6,ina.arg7); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,nullptr,ina.arg3,nullptr,ina.arg5,ina.arg6,nullptr); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,ina.arg3,nullptr,ina.arg5,nullptr,ina.arg7); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,nullptr,ina.arg3,nullptr,ina.arg5,nullptr,nullptr); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,ina.arg3,nullptr,nullptr,ina.arg6,ina.arg7); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,nullptr,ina.arg3,nullptr,nullptr,ina.arg6,nullptr); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,ina.arg3,nullptr,nullptr,nullptr,ina.arg7); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,nullptr,ina.arg3,nullptr,nullptr,nullptr,nullptr); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,nullptr,ina.arg4,ina.arg5,ina.arg6,ina.arg7); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,nullptr,nullptr,ina.arg4,ina.arg5,ina.arg6,nullptr); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,nullptr,ina.arg4,ina.arg5,nullptr,ina.arg7); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,nullptr,nullptr,ina.arg4,ina.arg5,nullptr,nullptr); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,nullptr,ina.arg4,nullptr,ina.arg6,ina.arg7); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,nullptr,nullptr,ina.arg4,nullptr,ina.arg6,nullptr); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,nullptr,ina.arg4,nullptr,nullptr,ina.arg7); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,nullptr,nullptr,ina.arg4,nullptr,nullptr,nullptr); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,nullptr,nullptr,ina.arg5,ina.arg6,ina.arg7); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,nullptr,nullptr,nullptr,ina.arg5,ina.arg6,nullptr); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,nullptr,nullptr,ina.arg5,nullptr,ina.arg7); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,nullptr,nullptr,nullptr,ina.arg5,nullptr,nullptr); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,nullptr,nullptr,nullptr,ina.arg6,ina.arg7); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,nullptr,nullptr,nullptr,nullptr,ina.arg6,nullptr); }
                    static void call(_FuncT &fp,_arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,ina.arg7); }
                    static void call(_FuncT &fp,_arguments &,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr); }
                };

                template<class _FuncT>
                void push(_FuncT &fp)
                {
                    _call_helper<_FuncT>::call(fp, *this, 
                        _type_is_nullptr<_Arg1, typename _function_traits<_FuncT>::arg1_type>::value,
                        _type_is_nullptr<_Arg2, typename _function_traits<_FuncT>::arg2_type>::value,
                        _type_is_nullptr<_Arg3, typename _function_traits<_FuncT>::arg3_type>::value,
                        _type_is_nullptr<_Arg4, typename _function_traits<_FuncT>::arg4_type>::value,
                        _type_is_nullptr<_Arg5, typename _function_traits<_FuncT>::arg5_type>::value,
                        _type_is_nullptr<_Arg6, typename _function_traits<_FuncT>::arg6_type>::value,
                        _type_is_nullptr<_Arg7, typename _function_traits<_FuncT>::arg7_type>::value);
                }

                template<class _ObjectT>
                struct _functor
                {
                    /*template<class _ReturnT>
                    struct _member_function
                    {
                        typedef _ReturnT(_ObjectT::*type)(_Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7);
                        typedef _ReturnT(_ObjectT::*type_const)(_Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7) const;
                    };*/

                    static 
                    void push(const thread_detail::_priority_tag<4>&, 
                        typename
                        conditional<
                            intern::_has_feature<intern::_stdex_could_pass_nullptr_with_comma_operator>::value,
                            _arguments&, 
                            int
                        >::type args, _ObjectT &fp)
                    {
                        typedef intern::thread_asserts check;

                        typedef typename check::operator_comma_is_user_defined_for_this_type_assert<
                            thread_detail::_has_no_user_defined_operator_comma_op<_Arg1>::value &&
                            thread_detail::_has_no_user_defined_operator_comma_op<_Arg2>::value &&
                            thread_detail::_has_no_user_defined_operator_comma_op<_Arg3>::value &&
                            thread_detail::_has_no_user_defined_operator_comma_op<_Arg4>::value &&
                            thread_detail::_has_no_user_defined_operator_comma_op<_Arg5>::value && 
                            thread_detail::_has_no_user_defined_operator_comma_op<_Arg6>::value &&
                            thread_detail::_has_no_user_defined_operator_comma_op<_Arg7>::value
                        >::operator_comma_is_user_defined_for_this_type_assert_failed
                        check1; // if you are there means there is user defined operator comma somewhere in global namespace
                        
                        using thread_detail::operator,;
                        using thread_detail::_arg;
                        
                        fp((_arg(args.arg1), nullptr),
                           (_arg(args.arg2), nullptr),
                           (_arg(args.arg3), nullptr),
                           (_arg(args.arg4), nullptr),
                           (_arg(args.arg5), nullptr),
                           (_arg(args.arg6), nullptr),
                           (_arg(args.arg7), nullptr));
                    }

                    static 
                    void push(const thread_detail::_priority_tag<4>&, 
                        typename
                        conditional<
                            intern::_has_bug<intern::_stdex_could_not_pass_nullptr_with_comma_operator>::value,
                            _arguments&, 
                            int
                        >::type args, _ObjectT &fp)
                    {
                        _call_helper<_ObjectT>::call(fp, args,
                            _type_is_nullptr<_Arg1>::value,
                            _type_is_nullptr<_Arg2>::value,
                            _type_is_nullptr<_Arg3>::value,
                            _type_is_nullptr<_Arg4>::value,
                            _type_is_nullptr<_Arg5>::value,
                            _type_is_nullptr<_Arg6>::value,
                            _type_is_nullptr<_Arg7>::value);
                    }
                };
            };

            typedef _arguments arguments_type;
        };


        template<class _Arg1, class _Arg2, class _Arg3, class _Arg4, class _Arg5, class _Arg6, class _Arg7, class _Arg8>
        struct _thread_args_helper<8, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8>
        {
            struct _arguments
            {
                static const unsigned int count = 8;

                _Arg1 arg1;
                _Arg2 arg2;
                _Arg3 arg3;
                _Arg4 arg4;
                _Arg5 arg5;
                _Arg6 arg6;
                _Arg7 arg7;
                _Arg8 arg8;

                _arguments(_Arg1 arg1_, _Arg2 arg2_, _Arg3 arg3_, _Arg4 arg4_, _Arg5 arg5_, _Arg6 arg6_, _Arg7 arg7_, _Arg8 arg8_) :
                    arg1(arg1_), arg2(arg2_), arg3(arg3_), arg4(arg4_), arg5(arg5_), arg6(arg6_), arg7(arg7_), arg8(arg8_)
                { }

                template<class _FuncT>
                struct _call_helper
                {
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ina.arg1,ina.arg2,ina.arg3,ina.arg4,ina.arg5,ina.arg6,ina.arg7,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ina.arg1,ina.arg2,ina.arg3,ina.arg4,ina.arg5,ina.arg6,ina.arg7,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ina.arg1,ina.arg2,ina.arg3,ina.arg4,ina.arg5,ina.arg6,nullptr,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(ina.arg1,ina.arg2,ina.arg3,ina.arg4,ina.arg5,ina.arg6,nullptr,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ina.arg1,ina.arg2,ina.arg3,ina.arg4,ina.arg5,nullptr,ina.arg7,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ina.arg1,ina.arg2,ina.arg3,ina.arg4,ina.arg5,nullptr,ina.arg7,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ina.arg1,ina.arg2,ina.arg3,ina.arg4,ina.arg5,nullptr,nullptr,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(ina.arg1,ina.arg2,ina.arg3,ina.arg4,ina.arg5,nullptr,nullptr,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ina.arg1,ina.arg2,ina.arg3,ina.arg4,nullptr,ina.arg6,ina.arg7,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ina.arg1,ina.arg2,ina.arg3,ina.arg4,nullptr,ina.arg6,ina.arg7,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ina.arg1,ina.arg2,ina.arg3,ina.arg4,nullptr,ina.arg6,nullptr,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(ina.arg1,ina.arg2,ina.arg3,ina.arg4,nullptr,ina.arg6,nullptr,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ina.arg1,ina.arg2,ina.arg3,ina.arg4,nullptr,nullptr,ina.arg7,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ina.arg1,ina.arg2,ina.arg3,ina.arg4,nullptr,nullptr,ina.arg7,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ina.arg1,ina.arg2,ina.arg3,ina.arg4,nullptr,nullptr,nullptr,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(ina.arg1,ina.arg2,ina.arg3,ina.arg4,nullptr,nullptr,nullptr,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ina.arg1,ina.arg2,ina.arg3,nullptr,ina.arg5,ina.arg6,ina.arg7,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ina.arg1,ina.arg2,ina.arg3,nullptr,ina.arg5,ina.arg6,ina.arg7,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ina.arg1,ina.arg2,ina.arg3,nullptr,ina.arg5,ina.arg6,nullptr,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(ina.arg1,ina.arg2,ina.arg3,nullptr,ina.arg5,ina.arg6,nullptr,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ina.arg1,ina.arg2,ina.arg3,nullptr,ina.arg5,nullptr,ina.arg7,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ina.arg1,ina.arg2,ina.arg3,nullptr,ina.arg5,nullptr,ina.arg7,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ina.arg1,ina.arg2,ina.arg3,nullptr,ina.arg5,nullptr,nullptr,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(ina.arg1,ina.arg2,ina.arg3,nullptr,ina.arg5,nullptr,nullptr,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ina.arg1,ina.arg2,ina.arg3,nullptr,nullptr,ina.arg6,ina.arg7,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ina.arg1,ina.arg2,ina.arg3,nullptr,nullptr,ina.arg6,ina.arg7,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ina.arg1,ina.arg2,ina.arg3,nullptr,nullptr,ina.arg6,nullptr,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(ina.arg1,ina.arg2,ina.arg3,nullptr,nullptr,ina.arg6,nullptr,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ina.arg1,ina.arg2,ina.arg3,nullptr,nullptr,nullptr,ina.arg7,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ina.arg1,ina.arg2,ina.arg3,nullptr,nullptr,nullptr,ina.arg7,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ina.arg1,ina.arg2,ina.arg3,nullptr,nullptr,nullptr,nullptr,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(ina.arg1,ina.arg2,ina.arg3,nullptr,nullptr,nullptr,nullptr,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ina.arg1,ina.arg2,nullptr,ina.arg4,ina.arg5,ina.arg6,ina.arg7,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ina.arg1,ina.arg2,nullptr,ina.arg4,ina.arg5,ina.arg6,ina.arg7,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ina.arg1,ina.arg2,nullptr,ina.arg4,ina.arg5,ina.arg6,nullptr,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(ina.arg1,ina.arg2,nullptr,ina.arg4,ina.arg5,ina.arg6,nullptr,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ina.arg1,ina.arg2,nullptr,ina.arg4,ina.arg5,nullptr,ina.arg7,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ina.arg1,ina.arg2,nullptr,ina.arg4,ina.arg5,nullptr,ina.arg7,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ina.arg1,ina.arg2,nullptr,ina.arg4,ina.arg5,nullptr,nullptr,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(ina.arg1,ina.arg2,nullptr,ina.arg4,ina.arg5,nullptr,nullptr,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ina.arg1,ina.arg2,nullptr,ina.arg4,nullptr,ina.arg6,ina.arg7,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ina.arg1,ina.arg2,nullptr,ina.arg4,nullptr,ina.arg6,ina.arg7,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ina.arg1,ina.arg2,nullptr,ina.arg4,nullptr,ina.arg6,nullptr,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(ina.arg1,ina.arg2,nullptr,ina.arg4,nullptr,ina.arg6,nullptr,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ina.arg1,ina.arg2,nullptr,ina.arg4,nullptr,nullptr,ina.arg7,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ina.arg1,ina.arg2,nullptr,ina.arg4,nullptr,nullptr,ina.arg7,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ina.arg1,ina.arg2,nullptr,ina.arg4,nullptr,nullptr,nullptr,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(ina.arg1,ina.arg2,nullptr,ina.arg4,nullptr,nullptr,nullptr,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ina.arg1,ina.arg2,nullptr,nullptr,ina.arg5,ina.arg6,ina.arg7,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ina.arg1,ina.arg2,nullptr,nullptr,ina.arg5,ina.arg6,ina.arg7,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ina.arg1,ina.arg2,nullptr,nullptr,ina.arg5,ina.arg6,nullptr,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(ina.arg1,ina.arg2,nullptr,nullptr,ina.arg5,ina.arg6,nullptr,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ina.arg1,ina.arg2,nullptr,nullptr,ina.arg5,nullptr,ina.arg7,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ina.arg1,ina.arg2,nullptr,nullptr,ina.arg5,nullptr,ina.arg7,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ina.arg1,ina.arg2,nullptr,nullptr,ina.arg5,nullptr,nullptr,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(ina.arg1,ina.arg2,nullptr,nullptr,ina.arg5,nullptr,nullptr,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ina.arg1,ina.arg2,nullptr,nullptr,nullptr,ina.arg6,ina.arg7,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ina.arg1,ina.arg2,nullptr,nullptr,nullptr,ina.arg6,ina.arg7,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ina.arg1,ina.arg2,nullptr,nullptr,nullptr,ina.arg6,nullptr,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(ina.arg1,ina.arg2,nullptr,nullptr,nullptr,ina.arg6,nullptr,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ina.arg1,ina.arg2,nullptr,nullptr,nullptr,nullptr,ina.arg7,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ina.arg1,ina.arg2,nullptr,nullptr,nullptr,nullptr,ina.arg7,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ina.arg1,ina.arg2,nullptr,nullptr,nullptr,nullptr,nullptr,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(ina.arg1,ina.arg2,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ina.arg1,nullptr,ina.arg3,ina.arg4,ina.arg5,ina.arg6,ina.arg7,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ina.arg1,nullptr,ina.arg3,ina.arg4,ina.arg5,ina.arg6,ina.arg7,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ina.arg1,nullptr,ina.arg3,ina.arg4,ina.arg5,ina.arg6,nullptr,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(ina.arg1,nullptr,ina.arg3,ina.arg4,ina.arg5,ina.arg6,nullptr,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ina.arg1,nullptr,ina.arg3,ina.arg4,ina.arg5,nullptr,ina.arg7,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ina.arg1,nullptr,ina.arg3,ina.arg4,ina.arg5,nullptr,ina.arg7,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ina.arg1,nullptr,ina.arg3,ina.arg4,ina.arg5,nullptr,nullptr,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(ina.arg1,nullptr,ina.arg3,ina.arg4,ina.arg5,nullptr,nullptr,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ina.arg1,nullptr,ina.arg3,ina.arg4,nullptr,ina.arg6,ina.arg7,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ina.arg1,nullptr,ina.arg3,ina.arg4,nullptr,ina.arg6,ina.arg7,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ina.arg1,nullptr,ina.arg3,ina.arg4,nullptr,ina.arg6,nullptr,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(ina.arg1,nullptr,ina.arg3,ina.arg4,nullptr,ina.arg6,nullptr,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ina.arg1,nullptr,ina.arg3,ina.arg4,nullptr,nullptr,ina.arg7,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ina.arg1,nullptr,ina.arg3,ina.arg4,nullptr,nullptr,ina.arg7,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ina.arg1,nullptr,ina.arg3,ina.arg4,nullptr,nullptr,nullptr,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(ina.arg1,nullptr,ina.arg3,ina.arg4,nullptr,nullptr,nullptr,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ina.arg1,nullptr,ina.arg3,nullptr,ina.arg5,ina.arg6,ina.arg7,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ina.arg1,nullptr,ina.arg3,nullptr,ina.arg5,ina.arg6,ina.arg7,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ina.arg1,nullptr,ina.arg3,nullptr,ina.arg5,ina.arg6,nullptr,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(ina.arg1,nullptr,ina.arg3,nullptr,ina.arg5,ina.arg6,nullptr,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ina.arg1,nullptr,ina.arg3,nullptr,ina.arg5,nullptr,ina.arg7,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ina.arg1,nullptr,ina.arg3,nullptr,ina.arg5,nullptr,ina.arg7,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ina.arg1,nullptr,ina.arg3,nullptr,ina.arg5,nullptr,nullptr,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(ina.arg1,nullptr,ina.arg3,nullptr,ina.arg5,nullptr,nullptr,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ina.arg1,nullptr,ina.arg3,nullptr,nullptr,ina.arg6,ina.arg7,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ina.arg1,nullptr,ina.arg3,nullptr,nullptr,ina.arg6,ina.arg7,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ina.arg1,nullptr,ina.arg3,nullptr,nullptr,ina.arg6,nullptr,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(ina.arg1,nullptr,ina.arg3,nullptr,nullptr,ina.arg6,nullptr,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ina.arg1,nullptr,ina.arg3,nullptr,nullptr,nullptr,ina.arg7,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ina.arg1,nullptr,ina.arg3,nullptr,nullptr,nullptr,ina.arg7,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ina.arg1,nullptr,ina.arg3,nullptr,nullptr,nullptr,nullptr,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(ina.arg1,nullptr,ina.arg3,nullptr,nullptr,nullptr,nullptr,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ina.arg1,nullptr,nullptr,ina.arg4,ina.arg5,ina.arg6,ina.arg7,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ina.arg1,nullptr,nullptr,ina.arg4,ina.arg5,ina.arg6,ina.arg7,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ina.arg1,nullptr,nullptr,ina.arg4,ina.arg5,ina.arg6,nullptr,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(ina.arg1,nullptr,nullptr,ina.arg4,ina.arg5,ina.arg6,nullptr,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ina.arg1,nullptr,nullptr,ina.arg4,ina.arg5,nullptr,ina.arg7,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ina.arg1,nullptr,nullptr,ina.arg4,ina.arg5,nullptr,ina.arg7,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ina.arg1,nullptr,nullptr,ina.arg4,ina.arg5,nullptr,nullptr,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(ina.arg1,nullptr,nullptr,ina.arg4,ina.arg5,nullptr,nullptr,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ina.arg1,nullptr,nullptr,ina.arg4,nullptr,ina.arg6,ina.arg7,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ina.arg1,nullptr,nullptr,ina.arg4,nullptr,ina.arg6,ina.arg7,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ina.arg1,nullptr,nullptr,ina.arg4,nullptr,ina.arg6,nullptr,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(ina.arg1,nullptr,nullptr,ina.arg4,nullptr,ina.arg6,nullptr,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ina.arg1,nullptr,nullptr,ina.arg4,nullptr,nullptr,ina.arg7,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ina.arg1,nullptr,nullptr,ina.arg4,nullptr,nullptr,ina.arg7,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ina.arg1,nullptr,nullptr,ina.arg4,nullptr,nullptr,nullptr,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(ina.arg1,nullptr,nullptr,ina.arg4,nullptr,nullptr,nullptr,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ina.arg1,nullptr,nullptr,nullptr,ina.arg5,ina.arg6,ina.arg7,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ina.arg1,nullptr,nullptr,nullptr,ina.arg5,ina.arg6,ina.arg7,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ina.arg1,nullptr,nullptr,nullptr,ina.arg5,ina.arg6,nullptr,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(ina.arg1,nullptr,nullptr,nullptr,ina.arg5,ina.arg6,nullptr,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ina.arg1,nullptr,nullptr,nullptr,ina.arg5,nullptr,ina.arg7,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ina.arg1,nullptr,nullptr,nullptr,ina.arg5,nullptr,ina.arg7,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ina.arg1,nullptr,nullptr,nullptr,ina.arg5,nullptr,nullptr,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(ina.arg1,nullptr,nullptr,nullptr,ina.arg5,nullptr,nullptr,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ina.arg1,nullptr,nullptr,nullptr,nullptr,ina.arg6,ina.arg7,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ina.arg1,nullptr,nullptr,nullptr,nullptr,ina.arg6,ina.arg7,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ina.arg1,nullptr,nullptr,nullptr,nullptr,ina.arg6,nullptr,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(ina.arg1,nullptr,nullptr,nullptr,nullptr,ina.arg6,nullptr,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ina.arg1,nullptr,nullptr,nullptr,nullptr,nullptr,ina.arg7,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ina.arg1,nullptr,nullptr,nullptr,nullptr,nullptr,ina.arg7,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ina.arg1,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(ina.arg1,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,ina.arg2,ina.arg3,ina.arg4,ina.arg5,ina.arg6,ina.arg7,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,ina.arg2,ina.arg3,ina.arg4,ina.arg5,ina.arg6,ina.arg7,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,ina.arg2,ina.arg3,ina.arg4,ina.arg5,ina.arg6,nullptr,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,ina.arg2,ina.arg3,ina.arg4,ina.arg5,ina.arg6,nullptr,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,ina.arg2,ina.arg3,ina.arg4,ina.arg5,nullptr,ina.arg7,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,ina.arg2,ina.arg3,ina.arg4,ina.arg5,nullptr,ina.arg7,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,ina.arg2,ina.arg3,ina.arg4,ina.arg5,nullptr,nullptr,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,ina.arg2,ina.arg3,ina.arg4,ina.arg5,nullptr,nullptr,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,ina.arg2,ina.arg3,ina.arg4,nullptr,ina.arg6,ina.arg7,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,ina.arg2,ina.arg3,ina.arg4,nullptr,ina.arg6,ina.arg7,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,ina.arg2,ina.arg3,ina.arg4,nullptr,ina.arg6,nullptr,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,ina.arg2,ina.arg3,ina.arg4,nullptr,ina.arg6,nullptr,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,ina.arg2,ina.arg3,ina.arg4,nullptr,nullptr,ina.arg7,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,ina.arg2,ina.arg3,ina.arg4,nullptr,nullptr,ina.arg7,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,ina.arg2,ina.arg3,ina.arg4,nullptr,nullptr,nullptr,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,ina.arg2,ina.arg3,ina.arg4,nullptr,nullptr,nullptr,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,ina.arg2,ina.arg3,nullptr,ina.arg5,ina.arg6,ina.arg7,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,ina.arg2,ina.arg3,nullptr,ina.arg5,ina.arg6,ina.arg7,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,ina.arg2,ina.arg3,nullptr,ina.arg5,ina.arg6,nullptr,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,ina.arg2,ina.arg3,nullptr,ina.arg5,ina.arg6,nullptr,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,ina.arg2,ina.arg3,nullptr,ina.arg5,nullptr,ina.arg7,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,ina.arg2,ina.arg3,nullptr,ina.arg5,nullptr,ina.arg7,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,ina.arg2,ina.arg3,nullptr,ina.arg5,nullptr,nullptr,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,ina.arg2,ina.arg3,nullptr,ina.arg5,nullptr,nullptr,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,ina.arg2,ina.arg3,nullptr,nullptr,ina.arg6,ina.arg7,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,ina.arg2,ina.arg3,nullptr,nullptr,ina.arg6,ina.arg7,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,ina.arg2,ina.arg3,nullptr,nullptr,ina.arg6,nullptr,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,ina.arg2,ina.arg3,nullptr,nullptr,ina.arg6,nullptr,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,ina.arg2,ina.arg3,nullptr,nullptr,nullptr,ina.arg7,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,ina.arg2,ina.arg3,nullptr,nullptr,nullptr,ina.arg7,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,ina.arg2,ina.arg3,nullptr,nullptr,nullptr,nullptr,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,ina.arg2,ina.arg3,nullptr,nullptr,nullptr,nullptr,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,ina.arg2,nullptr,ina.arg4,ina.arg5,ina.arg6,ina.arg7,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,ina.arg2,nullptr,ina.arg4,ina.arg5,ina.arg6,ina.arg7,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,ina.arg2,nullptr,ina.arg4,ina.arg5,ina.arg6,nullptr,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,ina.arg2,nullptr,ina.arg4,ina.arg5,ina.arg6,nullptr,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,ina.arg2,nullptr,ina.arg4,ina.arg5,nullptr,ina.arg7,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,ina.arg2,nullptr,ina.arg4,ina.arg5,nullptr,ina.arg7,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,ina.arg2,nullptr,ina.arg4,ina.arg5,nullptr,nullptr,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,ina.arg2,nullptr,ina.arg4,ina.arg5,nullptr,nullptr,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,ina.arg2,nullptr,ina.arg4,nullptr,ina.arg6,ina.arg7,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,ina.arg2,nullptr,ina.arg4,nullptr,ina.arg6,ina.arg7,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,ina.arg2,nullptr,ina.arg4,nullptr,ina.arg6,nullptr,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,ina.arg2,nullptr,ina.arg4,nullptr,ina.arg6,nullptr,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,ina.arg2,nullptr,ina.arg4,nullptr,nullptr,ina.arg7,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,ina.arg2,nullptr,ina.arg4,nullptr,nullptr,ina.arg7,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,ina.arg2,nullptr,ina.arg4,nullptr,nullptr,nullptr,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,ina.arg2,nullptr,ina.arg4,nullptr,nullptr,nullptr,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,ina.arg2,nullptr,nullptr,ina.arg5,ina.arg6,ina.arg7,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,ina.arg2,nullptr,nullptr,ina.arg5,ina.arg6,ina.arg7,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,ina.arg2,nullptr,nullptr,ina.arg5,ina.arg6,nullptr,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,ina.arg2,nullptr,nullptr,ina.arg5,ina.arg6,nullptr,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,ina.arg2,nullptr,nullptr,ina.arg5,nullptr,ina.arg7,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,ina.arg2,nullptr,nullptr,ina.arg5,nullptr,ina.arg7,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,ina.arg2,nullptr,nullptr,ina.arg5,nullptr,nullptr,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,ina.arg2,nullptr,nullptr,ina.arg5,nullptr,nullptr,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,ina.arg2,nullptr,nullptr,nullptr,ina.arg6,ina.arg7,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,ina.arg2,nullptr,nullptr,nullptr,ina.arg6,ina.arg7,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,ina.arg2,nullptr,nullptr,nullptr,ina.arg6,nullptr,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,ina.arg2,nullptr,nullptr,nullptr,ina.arg6,nullptr,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,ina.arg2,nullptr,nullptr,nullptr,nullptr,ina.arg7,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,ina.arg2,nullptr,nullptr,nullptr,nullptr,ina.arg7,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,ina.arg2,nullptr,nullptr,nullptr,nullptr,nullptr,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,ina.arg2,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,ina.arg3,ina.arg4,ina.arg5,ina.arg6,ina.arg7,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,nullptr,ina.arg3,ina.arg4,ina.arg5,ina.arg6,ina.arg7,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,ina.arg3,ina.arg4,ina.arg5,ina.arg6,nullptr,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,nullptr,ina.arg3,ina.arg4,ina.arg5,ina.arg6,nullptr,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,ina.arg3,ina.arg4,ina.arg5,nullptr,ina.arg7,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,nullptr,ina.arg3,ina.arg4,ina.arg5,nullptr,ina.arg7,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,ina.arg3,ina.arg4,ina.arg5,nullptr,nullptr,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,nullptr,ina.arg3,ina.arg4,ina.arg5,nullptr,nullptr,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,ina.arg3,ina.arg4,nullptr,ina.arg6,ina.arg7,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,nullptr,ina.arg3,ina.arg4,nullptr,ina.arg6,ina.arg7,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,ina.arg3,ina.arg4,nullptr,ina.arg6,nullptr,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,nullptr,ina.arg3,ina.arg4,nullptr,ina.arg6,nullptr,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,ina.arg3,ina.arg4,nullptr,nullptr,ina.arg7,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,nullptr,ina.arg3,ina.arg4,nullptr,nullptr,ina.arg7,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,ina.arg3,ina.arg4,nullptr,nullptr,nullptr,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,nullptr,ina.arg3,ina.arg4,nullptr,nullptr,nullptr,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,ina.arg3,nullptr,ina.arg5,ina.arg6,ina.arg7,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,nullptr,ina.arg3,nullptr,ina.arg5,ina.arg6,ina.arg7,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,ina.arg3,nullptr,ina.arg5,ina.arg6,nullptr,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,nullptr,ina.arg3,nullptr,ina.arg5,ina.arg6,nullptr,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,ina.arg3,nullptr,ina.arg5,nullptr,ina.arg7,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,nullptr,ina.arg3,nullptr,ina.arg5,nullptr,ina.arg7,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,ina.arg3,nullptr,ina.arg5,nullptr,nullptr,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,nullptr,ina.arg3,nullptr,ina.arg5,nullptr,nullptr,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,ina.arg3,nullptr,nullptr,ina.arg6,ina.arg7,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,nullptr,ina.arg3,nullptr,nullptr,ina.arg6,ina.arg7,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,ina.arg3,nullptr,nullptr,ina.arg6,nullptr,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,nullptr,ina.arg3,nullptr,nullptr,ina.arg6,nullptr,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,ina.arg3,nullptr,nullptr,nullptr,ina.arg7,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,nullptr,ina.arg3,nullptr,nullptr,nullptr,ina.arg7,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,ina.arg3,nullptr,nullptr,nullptr,nullptr,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,nullptr,ina.arg3,nullptr,nullptr,nullptr,nullptr,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,nullptr,ina.arg4,ina.arg5,ina.arg6,ina.arg7,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,nullptr,nullptr,ina.arg4,ina.arg5,ina.arg6,ina.arg7,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,nullptr,ina.arg4,ina.arg5,ina.arg6,nullptr,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,nullptr,nullptr,ina.arg4,ina.arg5,ina.arg6,nullptr,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,nullptr,ina.arg4,ina.arg5,nullptr,ina.arg7,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,nullptr,nullptr,ina.arg4,ina.arg5,nullptr,ina.arg7,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,nullptr,ina.arg4,ina.arg5,nullptr,nullptr,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,nullptr,nullptr,ina.arg4,ina.arg5,nullptr,nullptr,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,nullptr,ina.arg4,nullptr,ina.arg6,ina.arg7,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,nullptr,nullptr,ina.arg4,nullptr,ina.arg6,ina.arg7,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,nullptr,ina.arg4,nullptr,ina.arg6,nullptr,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,nullptr,nullptr,ina.arg4,nullptr,ina.arg6,nullptr,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,nullptr,ina.arg4,nullptr,nullptr,ina.arg7,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,nullptr,nullptr,ina.arg4,nullptr,nullptr,ina.arg7,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,nullptr,ina.arg4,nullptr,nullptr,nullptr,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,nullptr,nullptr,ina.arg4,nullptr,nullptr,nullptr,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,nullptr,nullptr,ina.arg5,ina.arg6,ina.arg7,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,nullptr,nullptr,nullptr,ina.arg5,ina.arg6,ina.arg7,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,nullptr,nullptr,ina.arg5,ina.arg6,nullptr,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,nullptr,nullptr,nullptr,ina.arg5,ina.arg6,nullptr,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,nullptr,nullptr,ina.arg5,nullptr,ina.arg7,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,nullptr,nullptr,nullptr,ina.arg5,nullptr,ina.arg7,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,nullptr,nullptr,ina.arg5,nullptr,nullptr,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,nullptr,nullptr,nullptr,ina.arg5,nullptr,nullptr,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,nullptr,nullptr,nullptr,ina.arg6,ina.arg7,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,nullptr,nullptr,nullptr,nullptr,ina.arg6,ina.arg7,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,nullptr,nullptr,nullptr,ina.arg6,nullptr,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,nullptr,nullptr,nullptr,nullptr,ina.arg6,nullptr,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,ina.arg7,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,ina.arg7,nullptr); }
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,ina.arg8); }
                    static void call(_FuncT &fp, _arguments &,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr); }
                };

                template<class _FuncT>
                void push(_FuncT &fp)
                {
                    _call_helper<_FuncT>::call(fp, *this,
                        _type_is_nullptr<_Arg1, typename _function_traits<_FuncT>::arg1_type>::value, 
                        _type_is_nullptr<_Arg2, typename _function_traits<_FuncT>::arg2_type>::value, 
                        _type_is_nullptr<_Arg3, typename _function_traits<_FuncT>::arg3_type>::value,
                        _type_is_nullptr<_Arg4, typename _function_traits<_FuncT>::arg4_type>::value,
                        _type_is_nullptr<_Arg5, typename _function_traits<_FuncT>::arg5_type>::value,
                        _type_is_nullptr<_Arg6, typename _function_traits<_FuncT>::arg6_type>::value,
                        _type_is_nullptr<_Arg7, typename _function_traits<_FuncT>::arg7_type>::value,
                        _type_is_nullptr<_Arg8, typename _function_traits<_FuncT>::arg8_type>::value);
                }

                template<class _ObjectT>
                struct _functor
                {
                    /*template<class _ReturnT>
                    struct _member_function
                    {
                        typedef _ReturnT(_ObjectT::*type)(_Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8);
                        typedef _ReturnT(_ObjectT::*type_const)(_Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8) const;
                    };*/

                    static 
                    void push(const thread_detail::_priority_tag<4>&, 
                        typename
                        conditional<
                            intern::_has_feature<intern::_stdex_could_pass_nullptr_with_comma_operator>::value,
                            _arguments&, 
                            int
                        >::type args, _ObjectT &fp)
                    {
                        typedef intern::thread_asserts check;

                        typedef typename check::operator_comma_is_user_defined_for_this_type_assert<
                            thread_detail::_has_no_user_defined_operator_comma_op<_Arg1>::value &&
                            thread_detail::_has_no_user_defined_operator_comma_op<_Arg2>::value &&
                            thread_detail::_has_no_user_defined_operator_comma_op<_Arg3>::value &&
                            thread_detail::_has_no_user_defined_operator_comma_op<_Arg4>::value &&
                            thread_detail::_has_no_user_defined_operator_comma_op<_Arg5>::value && 
                            thread_detail::_has_no_user_defined_operator_comma_op<_Arg6>::value &&
                            thread_detail::_has_no_user_defined_operator_comma_op<_Arg7>::value &&
                            thread_detail::_has_no_user_defined_operator_comma_op<_Arg8>::value
                        >::operator_comma_is_user_defined_for_this_type_assert_failed
                        check1; // if you are there means there is user defined operator comma somewhere in global namespace
                        
                        using thread_detail::operator,;
                        using thread_detail::_arg;
                        
                        fp((_arg(args.arg1), nullptr),
                           (_arg(args.arg2), nullptr),
                           (_arg(args.arg3), nullptr),
                           (_arg(args.arg4), nullptr),
                           (_arg(args.arg5), nullptr),
                           (_arg(args.arg6), nullptr),
                           (_arg(args.arg7), nullptr),
                           (_arg(args.arg8), nullptr));
                    }

                    static 
                    void push(const thread_detail::_priority_tag<4>&, 
                        typename
                        conditional<
                            intern::_has_bug<intern::_stdex_could_not_pass_nullptr_with_comma_operator>::value,
                            _arguments&, 
                            int
                        >::type args, _ObjectT &fp)
                    {
                        _call_helper<_ObjectT>::call(fp, args,
                            _type_is_nullptr<_Arg1>::value,
                            _type_is_nullptr<_Arg2>::value,
                            _type_is_nullptr<_Arg3>::value,
                            _type_is_nullptr<_Arg4>::value,
                            _type_is_nullptr<_Arg5>::value,
                            _type_is_nullptr<_Arg6>::value,
                            _type_is_nullptr<_Arg7>::value,
                            _type_is_nullptr<_Arg8>::value);
                    }
                };
            };

            typedef _arguments arguments_type;
        };

        enum eTypeIsClass { _type_is_class };
        enum eTypeIsNotClass { _type_is_not_class };

        template<bool>
        struct _choose_class_overload
        {
            static const eTypeIsClass tag = 
                _type_is_class;
        };

        template<>
        struct _choose_class_overload<false>
        {
            static const eTypeIsNotClass tag = 
                _type_is_not_class;
        };

        template<class _FuncT, class _ArgT>
        struct _thread_function_proxy
        {
            typedef _ArgT arguments_type;
            typedef _FuncT function_type;

            function_type fp;
            arguments_type args;

            _thread_function_proxy(function_type fp_, arguments_type args_) :
                fp(fp_),
                args(args_)
            { }

            ~_thread_function_proxy()
            { }

            void call()
            {
                call(
                    _choose_class_overload<
                        is_class<_FuncT>::value && 
                        (arguments_type::count > 0) == bool(true)
                    >::tag);
            }

            void call(eTypeIsClass)
            {
                arguments_type::template _functor<function_type>::
                    push(thread_detail::_priority_tag<5>(), args, fp);
            }

            void call(eTypeIsNotClass)
            {
                args.push(fp);
            }

            static void gproxy(void *data)
            {
                _thread_function_proxy *pf = reinterpret_cast<_thread_function_proxy*>(data);

                pf->call();

                delete pf;
            }

        private:
            void operator=(const _thread_function_proxy&) _STDEX_DELETED_FUNCTION;
        };
}

    //! Thread class.
    class thread {

        void init(void(*aFunction)(void *), void * aArg);

    public:
        typedef pthread_t native_handle_type;

        class id {
        public:
            
            id() _STDEX_NOEXCEPT_FUNCTION :
                _uid(invalid_id)
            { }

            id(const id &aId) :
                _uid(aId._uid)
            { }

            inline id& operator=(const id &aId)
            {
                _uid = aId._uid;

                return *this;
            }

            bool operator==(const id &other) const _STDEX_NOEXCEPT_FUNCTION
            {
                return _uid == other._uid;
            }

            inline friend bool operator!=(const id &aId1, const id &aId2) _STDEX_NOEXCEPT_FUNCTION
            {
                return !(aId1 == aId2);
            }

            inline friend bool operator<=(const id &aId1, const id &aId2) _STDEX_NOEXCEPT_FUNCTION
            {
                return aId1._uid <= aId2._uid;
            }

            bool operator<(const id &other) const _STDEX_NOEXCEPT_FUNCTION
            {
                return _uid < other._uid;
            }

            inline friend bool operator>=(const id &aId1, const id &aId2) _STDEX_NOEXCEPT_FUNCTION
            {
                return aId1._uid >= aId2._uid;
            }

            inline friend bool operator>(const id &aId1, const id &aId2) _STDEX_NOEXCEPT_FUNCTION
            {
                return aId1._uid > aId2._uid;
            }

            ::std::ostream& print(::std::ostream &out) const
            {
                if (*this == id())
                    return out << "stdex::thread::id of a non-executing thread";
                else
                    return out << _uid;
            }

        protected:
            typedef stdex::uintmax_t id_type;
            static const id_type invalid_id =
                id_type(-1);

            explicit id(const id_type& uid) :
                _uid(uid)
            { }

        private:
            friend class thread;
            id_type _uid;
        };

        //! Default constructor.
        //! Construct a @c thread object without an associated thread of execution
        //! (i.e. non-joinable).
        thread() _STDEX_NOEXCEPT_FUNCTION
            : _handle() 
        { }

        template<class _FuncT>
        explicit thread(_FuncT fx)
            : _handle() 
        {
            typedef typename detail::_thread_args_helper<0>::arguments_type args_t;
            typedef _FuncT func_t;
            init(&detail::_thread_function_proxy<func_t, args_t>::gproxy, new detail::_thread_function_proxy<func_t, args_t>(fx, args_t()));
        }

        template<class _FuncT, class _Arg0T>
        explicit thread(_FuncT fx, _Arg0T t0)
            : _handle() 
        {
            typedef typename detail::_thread_args_helper<1, _Arg0T>::arguments_type args_t;
            typedef _FuncT func_t;
            init(&detail::_thread_function_proxy<func_t, args_t>::gproxy, new detail::_thread_function_proxy<func_t, args_t>(fx, args_t(t0)));
        }

        template<class _FuncT, class _Arg0T, class _Arg1T>
        explicit thread(_FuncT fx, _Arg0T t0, _Arg1T t1)
            : _handle() 
        {
            typedef typename detail::_thread_args_helper<2, _Arg0T, _Arg1T>::arguments_type args_t;
            typedef _FuncT func_t;
            init(&detail::_thread_function_proxy<func_t, args_t>::gproxy, new detail::_thread_function_proxy<func_t, args_t>(fx, args_t(t0, t1)));
        }

        template<class _FuncT, class _Arg0T, class _Arg1T, class _Arg2T>
        explicit thread(_FuncT fx, _Arg0T t0, _Arg1T t1, _Arg2T t2)
            : _handle() 
        {
            typedef typename detail::_thread_args_helper<3, _Arg0T, _Arg1T, _Arg2T>::arguments_type args_t;
            typedef _FuncT func_t;
            init(&detail::_thread_function_proxy<func_t, args_t>::gproxy, new detail::_thread_function_proxy<func_t, args_t>(fx, args_t(t0, t1, t2)));
        }

        template<class _FuncT, class _Arg0T, class _Arg1T, class _Arg2T, class _Arg3T>
        explicit thread(_FuncT fx, _Arg0T t0, _Arg1T t1, _Arg2T t2, _Arg3T t3)
            : _handle() 
        {
            typedef typename detail::_thread_args_helper<4, _Arg0T, _Arg1T, _Arg2T, _Arg3T>::arguments_type args_t;
            typedef _FuncT func_t;
            init(&detail::_thread_function_proxy<func_t, args_t>::gproxy, new detail::_thread_function_proxy<func_t, args_t>(fx, args_t(t0, t1, t2, t3)));
        }

        template<class _FuncT, class _Arg0T, class _Arg1T, class _Arg2T, class _Arg3T, class _Arg4T>
        explicit thread(_FuncT fx, _Arg0T t0, _Arg1T t1, _Arg2T t2, _Arg3T t3, _Arg4T t4)
            : _handle() 
        {
            typedef typename detail::_thread_args_helper<5, _Arg0T, _Arg1T, _Arg2T, _Arg3T, _Arg4T>::arguments_type args_t;
            typedef _FuncT func_t;
            init(&detail::_thread_function_proxy<func_t, args_t>::gproxy, new detail::_thread_function_proxy<func_t, args_t>(fx, args_t(t0, t1, t2, t3, t4)));
        }

        template<class _FuncT, class _Arg0T, class _Arg1T, class _Arg2T, class _Arg3T, class _Arg4T, class _Arg5T>
        explicit thread(_FuncT fx, _Arg0T t0, _Arg1T t1, _Arg2T t2, _Arg3T t3, _Arg4T t4, _Arg5T t5)
            : _handle() 
        {
            typedef typename detail::_thread_args_helper<6, _Arg0T, _Arg1T, _Arg2T, _Arg3T, _Arg4T, _Arg5T>::arguments_type args_t;
            typedef _FuncT func_t;
            init(&detail::_thread_function_proxy<func_t, args_t>::gproxy, new detail::_thread_function_proxy<func_t, args_t>(fx, args_t(t0, t1, t2, t3, t4, t5)));
        }

        template<class _FuncT, class _Arg0T, class _Arg1T, class _Arg2T, class _Arg3T, class _Arg4T, class _Arg5T, class _Arg6T>
        explicit thread(_FuncT fx, _Arg0T t0, _Arg1T t1, _Arg2T t2, _Arg3T t3, _Arg4T t4, _Arg5T t5, _Arg6T t6)
            : _handle() 
        {
            typedef typename detail::_thread_args_helper<7, _Arg0T, _Arg1T, _Arg2T, _Arg3T, _Arg4T, _Arg5T, _Arg6T>::arguments_type args_t;
            typedef _FuncT func_t;
            init(&detail::_thread_function_proxy<func_t, args_t>::gproxy, new detail::_thread_function_proxy<func_t, args_t>(fx, args_t(t0, t1, t2, t3, t4, t5, t6)));
        }

        template<class _FuncT, class _Arg0T, class _Arg1T, class _Arg2T, class _Arg3T, class _Arg4T, class _Arg5T, class _Arg6T, class _Arg7T>
        explicit thread(_FuncT fx, _Arg0T t0, _Arg1T t1, _Arg2T t2, _Arg3T t3, _Arg4T t4, _Arg5T t5, _Arg6T t6, _Arg7T t7)
            : _handle() 
        {
            typedef typename detail::_thread_args_helper<8, _Arg0T, _Arg1T, _Arg2T, _Arg3T, _Arg4T, _Arg5T, _Arg6T, _Arg7T>::arguments_type args_t;
            typedef _FuncT func_t;
            init(&detail::_thread_function_proxy<func_t, args_t>::gproxy, new detail::_thread_function_proxy<func_t, args_t>(fx, args_t(t0, t1, t2, t3, t4, t5, t6, t7)));
        }

        //! Destructor.
        //! @note If the thread is joinable upon destruction, @c std::terminate()
        //! will be called, which terminates the process. It is always wise to do
        //! @c join() before deleting a thread object.
        ~thread();

        //! Wait for the thread to finish (join execution flows).
        //! After calling @c join(), the thread object is no longer associated with
        //! a thread of execution (i.e. it is not joinable, and you may not join
        //! with it nor detach from it).
        void join();

        //! Check if the thread is joinable.
        //! A thread object is joinable if it has an associated thread of execution.
        bool joinable() const _STDEX_NOEXCEPT_FUNCTION;

        //! Detach from the thread.
        //! After calling @c detach(), the thread object is no longer associated with
        //! a thread of execution (i.e. it is not joinable). The thread continues
        //! execution without the calling thread blocking, and when the thread
        //! ends execution, any owned resources are released.
        void detach();

        //! Return the thread ID of a thread object.
        id get_id() const _STDEX_NOEXCEPT_FUNCTION;

        //! Get the native handle for this thread.
        inline native_handle_type native_handle()
        {
            return _handle;
        }

        //! Determine the number of threads which can possibly execute concurrently.
        //! This function is useful for determining the optimal number of threads to
        //! use for a task.
        //! @return The number of hardware thread contexts in the system.
        //! @note If this value is not defined, the function returns zero.
        static unsigned hardware_concurrency() _STDEX_NOEXCEPT_FUNCTION;

        void swap(thread &other) _STDEX_NOEXCEPT_FUNCTION;

    private:

        id _id;
        native_handle_type _handle;

        //thread(thread&) _STDEX_DELETED_FUNCTION;
        thread(const thread&) _STDEX_DELETED_FUNCTION;
        thread& operator=(const thread&) _STDEX_DELETED_FUNCTION;

        // This is the internal thread wrapper function.
        static void* wrapper_function(void *aArg);
    };

    namespace detail
    {
        void sleep_for_impl(const stdex::timespec *reltime);
    }

    //! The namespace @c this_thread provides methods for dealing with the
    //! calling thread.
    namespace this_thread 
    {
        //! Return the thread ID of the calling thread.
        thread::id get_id() _STDEX_NOEXCEPT_FUNCTION;

        //! Yield execution to another thread.
        //! Offers the operating system the opportunity to schedule another thread
        //! that is ready to run on the current processor.
        inline static void yield() _STDEX_NOEXCEPT_FUNCTION
        {
            sched_yield();
        }


        //! Blocks the calling thread for a period of time.
        //! @param[in] aTime Minimum time to put the thread to sleep.
        //! Example usage:
        //! @code
        //! // Sleep for 100 milliseconds
        //! this_thread::sleep_for(chrono::milliseconds(100));
        //! @endcode
        //! @note Supported duration types are: nanoseconds, microseconds,
        //! milliseconds, seconds, minutes and hours.

        template <class _Rep, class _Period>
        inline void sleep_for(const chrono::duration<_Rep, _Period> &_dur)
        {
            chrono::seconds _sec = chrono::duration_cast<chrono::seconds>(_dur);
            chrono::nanoseconds _ns = chrono::duration_cast<chrono::nanoseconds>(_dur - _sec);
            
            if(_sec.count() < 0 || _ns.count() < 0)
                return;
            if(_sec.count() == 0 && _ns.count() == 0)
                return;

            chrono::seconds::rep _s_count = _sec.count();

            stdex::timespec _ts;

            const stdex::time_t _ts_sec_max = 
                (std::numeric_limits<stdex::time_t>::max)();
            if (_s_count < _ts_sec_max)
            {
                _ts.tv_sec = static_cast<stdex::time_t>(_s_count > 0 ? _s_count : 0);
                _ts.tv_nsec = static_cast<long>(_ns.count());
            }
            else
            {
                _ts.tv_sec = _ts_sec_max;
                _ts.tv_nsec = 999999999;
            }

            detail::sleep_for_impl(&_ts);
        }

        template <class _Clock, class _Duration>
        inline void sleep_until(const chrono::time_point<_Clock, _Duration> &_tp)
        {
            chrono::time_point<_Clock> _now = _Clock::now();

            if (_Clock::is_steady)
            {
                if (_now < _tp)
                    sleep_for(_tp - _now);
                return;
            }

            while (_now < _tp)
            {
                sleep_for(_tp - _now);
                _now = _Clock::now();
            }

        }
    }
} // namespace stdex



namespace
{
    inline ::std::ostream& operator<<(::std::ostream &out, const stdex::thread::id &id)
    {
        return id.print(out);
    }
}

namespace std
{
    //template<>
    inline void swap(stdex::thread &lhs, stdex::thread &rhs)
    {
        lhs.swap(rhs);
    }
} // namespace std

namespace stdex
{
    using std::swap;
} // namespace stdex


#undef _STDEX_DELETED_FUNCTION
#undef _STDEX_NOEXCEPT_FUNCTION

#endif // _STDEX_THREAD_H

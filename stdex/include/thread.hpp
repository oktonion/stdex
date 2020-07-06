#ifndef _STDEX_THREAD_H
#define _STDEX_THREAD_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// stdex includes
#include "./chrono.hpp"
#include "./condition_variable.hpp"

// POSIX includes
#include <pthread.h>

// std includes
#include <ostream>
#include <memory>
#include <algorithm> // std::swap
#include <utility> // std::swap

#ifdef _STDEX_NATIVE_CPP11_SUPPORT

#define _STDEX_DELETED_FUNCTION =delete
#define _STDEX_NOEXCEPT_FUNCTION noexcept 

#else

#define _STDEX_DELETED_FUNCTION 
#define _STDEX_NOEXCEPT_FUNCTION throw()

#endif

namespace stdex
{
    namespace detail
    {
        template<unsigned _Rank> struct priority_tag : priority_tag < _Rank - 1 > {};
        template<> struct priority_tag<0> {};

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
        struct _type_is_nullptr_base_chooser
        {
            static const bool _is_nullptr =
                _is_nullptr_t<_Tp>::value == (true) && 
                (is_pointer<_ArgT>::value == (true) || is_member_function_pointer<_ArgT>::value == (true));

            typedef _type_is_nullptr_helper<_type_is_nullptr_base_chooser::_is_nullptr == (true)> type;
        };
        template<class _Tp, class _ArgT = void*>
        struct _type_is_nullptr:
            _type_is_nullptr_base_chooser<_Tp, _ArgT>::type
        { };

        template<class _Tp>
        struct _thread_nullptr_arg_helper
        {
            //static 
        };

        template<class _T0 = detail::void_type, class _T1 = detail::void_type, class _T2 = detail::void_type, class _T3 = detail::void_type, class _T4 = detail::void_type, class _T5 = detail::void_type, class _T6 = detail::void_type, class _T7 = detail::void_type, class _T8 = detail::void_type, class _T9 = detail::void_type, class _T10 = detail::void_type, class _T11 = detail::void_type, class _T12 = detail::void_type, class _T13 = detail::void_type, class _T14 = detail::void_type, class _T15 = detail::void_type, class _T16 = detail::void_type, class _T17 = detail::void_type, class _T18 = detail::void_type, class _T19 = detail::void_type, class _T20 = detail::void_type, class _T21 = detail::void_type, class _T22 = detail::void_type, class _T23 = detail::void_type, class _T24 = detail::void_type>
        struct _thread_args_helper;

        template<>
        struct _thread_args_helper<>
        {
            struct _arguments
            {
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
                    template<class _ReturnT>
                    struct _member_function
                    {
                        typedef _ReturnT(_ObjectT::*type)();
                        typedef _ReturnT(_ObjectT::*type_const)() const;
                    };

                    template<class _ReturnT>
                    static void push(const priority_tag<3>&, _arguments &, const _ObjectT &fp, _ReturnT(_ObjectT::*)() const)
                    {
                        _call_helper<const _ObjectT>::call(fp);
                    }

                    template<class _ReturnT>
                    static void push(const priority_tag<2>&, _arguments &, _ObjectT &fp, _ReturnT(_ObjectT::*)())
                    {
                        _call_helper<_ObjectT>::call(fp);
                    }
                };
                
            };

            typedef _arguments arguments_type;
        };

        template<class _Arg1>
        struct _thread_args_helper<_Arg1>
        {
            struct _arguments
            {
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

                template<class _FuncT>
                void push(_FuncT &fp)
                {
                    _call_helper<_FuncT>::call(fp, *this,
                        _type_is_nullptr<_Arg1, typename _function_traits<_FuncT>::arg1_type>::value);
                }
                
                template<class _ObjectT>
                struct _functor
                {
                    template<class _ReturnT>
                    struct _member_function
                    {
                        typedef _ReturnT(_ObjectT::*type)(_Arg1);
                        typedef _ReturnT(_ObjectT::*type_const)(_Arg1) const;
                    };

                    template<class _ReturnT>
                    static void push(const priority_tag<5>&, _arguments &args, const _ObjectT &fp, _ReturnT(_ObjectT::*)(_Arg1) const)
                    {
                        _call_helper<const _ObjectT>::call(fp, args,
                            _type_is_nullptr<_Arg1, _Arg1>::value);
                    }

                    template<class _ReturnT>
                    static void push(const priority_tag<4>&, _arguments &args, _ObjectT &fp, _ReturnT(_ObjectT::*)(_Arg1))
                    {
                        _call_helper<_ObjectT>::call(fp, args,
                            _type_is_nullptr<_Arg1, _Arg1>::value);
                    }

                    template<class _ReturnT, class _FArg1>
                    static void push(const priority_tag<3>&, _arguments &args, const _ObjectT &fp, _ReturnT(_ObjectT::*)(_FArg1) const)
                    {
                        _call_helper<const _ObjectT>::call(fp, args,
                            _type_is_nullptr<_Arg1, _FArg1>::value);
                    }

                    template<class _ReturnT, class _FArg1>
                    static void push(const priority_tag<2>&, _arguments &args, _ObjectT &fp, _ReturnT(_ObjectT::*)(_FArg1))
                    {
                        _call_helper<_ObjectT>::call(fp, args,
                            _type_is_nullptr<_Arg1, _FArg1>::value);
                    }
                };
            };

            typedef _arguments arguments_type;
        };

        template<class _Arg1, class _Arg2>
        struct _thread_args_helper<_Arg1, _Arg2>
        {
            struct _arguments
            {
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
                    template<class _ReturnT>
                    struct _member_function
                    {
                        typedef _ReturnT(_ObjectT::*type)(_Arg1, _Arg2);
                        typedef _ReturnT(_ObjectT::*type_const)(_Arg1, _Arg2) const;
                    };

                    template<class _ReturnT>
                    static void push(const priority_tag<5>&, _arguments &args, const _ObjectT &fp, _ReturnT(_ObjectT::*)(_Arg1, _Arg2) const)
                    {
                        _call_helper<const _ObjectT>::call(fp, args,
                            _type_is_nullptr<_Arg1, _Arg1>::value,
                            _type_is_nullptr<_Arg2, _Arg2>::value);
                    }

                    template<class _ReturnT>
                    static void push(const priority_tag<4>&, _arguments &args, _ObjectT &fp, _ReturnT(_ObjectT::*)(_Arg1, _Arg2))
                    {
                        _call_helper<_ObjectT>::call(fp, args,
                            _type_is_nullptr<_Arg1, _Arg1>::value,
                            _type_is_nullptr<_Arg2, _Arg2>::value);
                    }

                    template<class _ReturnT, class _FArg1, class _FArg2>
                    static void push(const priority_tag<3>&, _arguments &args, const _ObjectT &fp, _ReturnT(_ObjectT::*)(_FArg1, _FArg2) const)
                    {
                        _call_helper<const _ObjectT>::call(fp, args,
                            _type_is_nullptr<_Arg1, _FArg1>::value,
                            _type_is_nullptr<_Arg2, _FArg2>::value);
                    }

                    template<class _ReturnT, class _FArg1, class _FArg2>
                    static void push(const priority_tag<2>&, _arguments &args, _ObjectT &fp, _ReturnT(_ObjectT::*)(_FArg1, _FArg2))
                    {
                        _call_helper<_ObjectT>::call(fp, args,
                            _type_is_nullptr<_Arg1, _FArg1>::value,
                            _type_is_nullptr<_Arg2, _FArg2>::value);
                    }
                };
            };

            typedef _arguments arguments_type;
        };

        template<class _Arg1, class _Arg2, class _Arg3>
        struct _thread_args_helper<_Arg1, _Arg2, _Arg3>
        {
            struct _arguments
            {
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
                    template<class _ReturnT>
                    struct _member_function
                    {
                        typedef _ReturnT(_ObjectT::*type)(_Arg1, _Arg2, _Arg3);
                        typedef _ReturnT(_ObjectT::*type_const)(_Arg1, _Arg2, _Arg3) const;
                    };

                    template<class _ReturnT>
                    static void push(const priority_tag<5>&, _arguments &args, const _ObjectT &fp, _ReturnT(_ObjectT::*)(_Arg1, _Arg2, _Arg3) const)
                    {
                        _call_helper<const _ObjectT>::call(fp, args,
                            _type_is_nullptr<_Arg1, _Arg1>::value,
                            _type_is_nullptr<_Arg2, _Arg2>::value,
                            _type_is_nullptr<_Arg3, _Arg3>::value);
                    }

                    template<class _ReturnT>
                    static void push(const priority_tag<4>&, _arguments &args, _ObjectT &fp, _ReturnT(_ObjectT::*)(_Arg1, _Arg2, _Arg3))
                    {
                        _call_helper<_ObjectT>::call(fp, args,
                            _type_is_nullptr<_Arg1, _Arg1>::value,
                            _type_is_nullptr<_Arg2, _Arg2>::value,
                            _type_is_nullptr<_Arg3, _Arg3>::value);
                    }

                    template<class _ReturnT, class _FArg1, class _FArg2, class _FArg3>
                    static void push(const priority_tag<3>&, _arguments &args, const _ObjectT &fp, _ReturnT(_ObjectT::*)(_FArg1, _FArg2, _FArg3) const)
                    {
                        _call_helper<const _ObjectT>::call(fp, args,
                            _type_is_nullptr<_Arg1, _FArg1>::value,
                            _type_is_nullptr<_Arg2, _FArg2>::value,
                            _type_is_nullptr<_Arg3, _FArg3>::value);
                    }

                    template<class _ReturnT, class _FArg1, class _FArg2, class _FArg3>
                    static void push(const priority_tag<2>&, _arguments &args, _ObjectT &fp, _ReturnT(_ObjectT::*)(_FArg1, _FArg2, _FArg3))
                    {
                        _call_helper<_ObjectT>::call(fp, args,
                            _type_is_nullptr<_Arg1, _FArg1>::value,
                            _type_is_nullptr<_Arg2, _FArg2>::value,
                            _type_is_nullptr<_Arg3, _FArg3>::value);
                    }
                };
            };

            typedef _arguments arguments_type;
        };

        template<class _Arg1, class _Arg2, class _Arg3, class _Arg4>
        struct _thread_args_helper<_Arg1, _Arg2, _Arg3, _Arg4>
        {
            struct _arguments
            {
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
                    template<class _ReturnT>
                    struct _member_function
                    {
                        typedef _ReturnT(_ObjectT::*type)(_Arg1, _Arg2, _Arg3, _Arg4);
                        typedef _ReturnT(_ObjectT::*type_const)(_Arg1, _Arg2, _Arg3, _Arg4) const;
                    };

                    template<class _ReturnT>
                    void push(const priority_tag<5>&, _arguments &args, const _ObjectT &fp, _ReturnT(_ObjectT::*)(_Arg1, _Arg2, _Arg3, _Arg4) const)
                    {
                        _call_helper<const _ObjectT>::call(fp, args,
                            _type_is_nullptr<_Arg1, _Arg1>::value,
                            _type_is_nullptr<_Arg2, _Arg2>::value,
                            _type_is_nullptr<_Arg3, _Arg3>::value,
                            _type_is_nullptr<_Arg4, _Arg4>::value);
                    }

                    template<class _ReturnT>
                    void push(const priority_tag<4>&, _arguments &args, _ObjectT &fp, _ReturnT(_ObjectT::*)(_Arg1, _Arg2, _Arg3, _Arg4))
                    {
                        _call_helper<_ObjectT>::call(fp, args,
                            _type_is_nullptr<_Arg1, _Arg1>::value,
                            _type_is_nullptr<_Arg2, _Arg2>::value,
                            _type_is_nullptr<_Arg3, _Arg3>::value,
                            _type_is_nullptr<_Arg4, _Arg4>::value);
                    }

                    template<class _ReturnT, class _FArg1, class _FArg2, class _FArg3, class _FArg4>
                    void push(const priority_tag<3>&, _arguments &args, const _ObjectT &fp, _ReturnT(_ObjectT::*)(_FArg1, _FArg2, _FArg3, _FArg4) const)
                    {
                        _call_helper<const _ObjectT>::call(fp, args,
                            _type_is_nullptr<_Arg1, _FArg1>::value,
                            _type_is_nullptr<_Arg2, _FArg2>::value,
                            _type_is_nullptr<_Arg3, _FArg3>::value,
                            _type_is_nullptr<_Arg4, _FArg4>::value);
                    }

                    template<class _ReturnT, class _FArg1, class _FArg2, class _FArg3, class _FArg4>
                    void push(const priority_tag<2>&, _arguments &args, _ObjectT &fp, _ReturnT(_ObjectT::*)(_FArg1, _FArg2, _FArg3, _FArg4))
                    {
                        _call_helper<_ObjectT>::call(fp, args,
                            _type_is_nullptr<_Arg1, _FArg1>::value,
                            _type_is_nullptr<_Arg2, _FArg2>::value,
                            _type_is_nullptr<_Arg3, _FArg3>::value,
                            _type_is_nullptr<_Arg4, _FArg4>::value);
                    }
                };
            };

            typedef _arguments arguments_type;
        };

        template<class _Arg1, class _Arg2, class _Arg3, class _Arg4, class _Arg5>
        struct _thread_args_helper<_Arg1, _Arg2, _Arg3, _Arg4, _Arg5>
        {
            struct _arguments
            {
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
                    template<class _ReturnT>
                    struct object_member_func
                    {
                        typedef _ReturnT(_ObjectT::*type)(_Arg1, _Arg2, _Arg3, _Arg4, _Arg5);
                        typedef _ReturnT(_ObjectT::*type_const)(_Arg1, _Arg2, _Arg3, _Arg4, _Arg5) const;
                    };

                    template<class _ReturnT>
                    static void push(const priority_tag<5>&, _arguments &args, const _ObjectT &fp, _ReturnT(_ObjectT::*)(_Arg1, _Arg2, _Arg3, _Arg4, _Arg5) const)
                    {
                        _call_helper<const _ObjectT>::call(fp, args,
                            _type_is_nullptr<_Arg1, _Arg1>::value,
                            _type_is_nullptr<_Arg2, _Arg2>::value,
                            _type_is_nullptr<_Arg3, _Arg3>::value,
                            _type_is_nullptr<_Arg4, _Arg4>::value,
                            _type_is_nullptr<_Arg5, _Arg5>::value);
                    }

                    template<class _ReturnT>
                    static void push(const priority_tag<4>&, _arguments &args, _ObjectT &fp, _ReturnT(_ObjectT::*)(_Arg1, _Arg2, _Arg3, _Arg4, _Arg5))
                    {
                        _call_helper<_ObjectT>::call(fp, args,
                            _type_is_nullptr<_Arg1, _Arg1>::value,
                            _type_is_nullptr<_Arg2, _Arg2>::value,
                            _type_is_nullptr<_Arg3, _Arg3>::value,
                            _type_is_nullptr<_Arg4, _Arg4>::value,
                            _type_is_nullptr<_Arg5, _Arg5>::value);
                    }

                    template<class _ReturnT, class _FArg1, class _FArg2, class _FArg3, class _FArg4, class _FArg5>
                    static void push(const priority_tag<3>&, _arguments &args, const _ObjectT &fp, _ReturnT(_ObjectT::*)(_FArg1, _FArg2, _FArg3, _FArg4, _FArg5) const)
                    {
                        _call_helper<const _ObjectT>::call(fp, args,
                            _type_is_nullptr<_Arg1, _FArg1>::value,
                            _type_is_nullptr<_Arg2, _FArg2>::value,
                            _type_is_nullptr<_Arg3, _FArg3>::value,
                            _type_is_nullptr<_Arg4, _FArg4>::value,
                            _type_is_nullptr<_Arg5, _FArg5>::value);
                    }

                    template<class _ReturnT, class _FArg1, class _FArg2, class _FArg3, class _FArg4, class _FArg5>
                    static void push(const priority_tag<2>&, _arguments &args, _ObjectT &fp, _ReturnT(_ObjectT::*)(_FArg1, _FArg2, _FArg3, _FArg4, _FArg5))
                    {
                        _call_helper<_ObjectT>::call(fp, args,
                            _type_is_nullptr<_Arg1, _FArg1>::value,
                            _type_is_nullptr<_Arg2, _FArg2>::value,
                            _type_is_nullptr<_Arg3, _FArg3>::value,
                            _type_is_nullptr<_Arg4, _FArg4>::value,
                            _type_is_nullptr<_Arg5, _FArg5>::value);
                    }
                };
            };

            typedef _arguments arguments_type;
        };

        template<class _Arg1, class _Arg2, class _Arg3, class _Arg4, class _Arg5, class _Arg6>
        struct _thread_args_helper<_Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6>
        {
            struct _arguments
            {
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
                    static void call(_FuncT &fp, _arguments &ina,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,nullptr,nullptr,nullptr,nullptr,nullptr); }
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
                    template<class _ReturnT>
                    struct _member_function
                    {
                        typedef _ReturnT(_ObjectT::*type)(_Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6);
                        typedef _ReturnT(_ObjectT::*type_const)(_Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6) const;
                    };

                    template<class _ReturnT>
                    static void push(const priority_tag<5>&, _arguments &args, const _ObjectT &fp, _ReturnT(_ObjectT::*)(_Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6) const)
                    {
                        _call_helper<const _ObjectT>::call(fp, args,
                            _type_is_nullptr<_Arg1, _Arg1>::value,
                            _type_is_nullptr<_Arg2, _Arg2>::value,
                            _type_is_nullptr<_Arg3, _Arg3>::value,
                            _type_is_nullptr<_Arg4, _Arg4>::value,
                            _type_is_nullptr<_Arg5, _Arg5>::value,
                            _type_is_nullptr<_Arg6, _Arg6>::value);
                    }

                    template<class _ReturnT>
                    static void push(const priority_tag<4>&, _arguments &args, _ObjectT &fp, _ReturnT(_ObjectT::*)(_Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6))
                    {
                        _call_helper<_ObjectT>::call(fp, args,
                            _type_is_nullptr<_Arg1, _Arg1>::value,
                            _type_is_nullptr<_Arg2, _Arg2>::value,
                            _type_is_nullptr<_Arg3, _Arg3>::value,
                            _type_is_nullptr<_Arg4, _Arg4>::value,
                            _type_is_nullptr<_Arg5, _Arg5>::value,
                            _type_is_nullptr<_Arg6, _Arg6>::value);
                    }

                    template<class _ReturnT, class _FArg1, class _FArg2, class _FArg3, class _FArg4, class _FArg5, class _FArg6>
                    static void push(const priority_tag<3>&, _arguments &args, const _ObjectT &fp, _ReturnT(_ObjectT::*)(_FArg1, _FArg2, _FArg3, _FArg4, _FArg5, _FArg6) const)
                    {
                        _call_helper<const _ObjectT>::call(fp, args,
                            _type_is_nullptr<_Arg1, _FArg1>::value,
                            _type_is_nullptr<_Arg2, _FArg2>::value,
                            _type_is_nullptr<_Arg3, _FArg3>::value,
                            _type_is_nullptr<_Arg4, _FArg4>::value,
                            _type_is_nullptr<_Arg5, _FArg5>::value,
                            _type_is_nullptr<_Arg6, _FArg6>::value);
                    }

                    template<class _ReturnT, class _FArg1, class _FArg2, class _FArg3, class _FArg4, class _FArg5, class _FArg6>
                    static void push(const priority_tag<2>&, _arguments &args, _ObjectT &fp, _ReturnT(_ObjectT::*)(_FArg1, _FArg2, _FArg3, _FArg4, _FArg5, _FArg6))
                    {
                        _call_helper<_ObjectT>::call(fp, args,
                            _type_is_nullptr<_Arg1, _FArg1>::value,
                            _type_is_nullptr<_Arg2, _FArg2>::value,
                            _type_is_nullptr<_Arg3, _FArg3>::value,
                            _type_is_nullptr<_Arg4, _FArg4>::value,
                            _type_is_nullptr<_Arg5, _FArg5>::value,
                            _type_is_nullptr<_Arg6, _FArg6>::value);
                    }
                };
            };

            typedef _arguments arguments_type;
        };

        template<class _Arg1, class _Arg2, class _Arg3, class _Arg4, class _Arg5, class _Arg6, class _Arg7>
        struct _thread_args_helper<_Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7>
        {
            struct _arguments
            {
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
                    template<class _ReturnT>
                    struct _member_function
                    {
                        typedef _ReturnT(_ObjectT::*type)(_Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7);
                        typedef _ReturnT(_ObjectT::*type_const)(_Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7) const;
                    };

                    template<class _ReturnT>
                    static void push(const priority_tag<5>&, _arguments &args, const _ObjectT &fp, _ReturnT(_ObjectT::*)(_Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7) const)
                    {
                        _call_helper<const _ObjectT>::call(fp, args, 
                            _type_is_nullptr<_Arg1, _Arg1>::value,
                            _type_is_nullptr<_Arg2, _Arg2>::value,
                            _type_is_nullptr<_Arg3, _Arg3>::value,
                            _type_is_nullptr<_Arg4, _Arg4>::value,
                            _type_is_nullptr<_Arg5, _Arg5>::value,
                            _type_is_nullptr<_Arg6, _Arg6>::value,
                            _type_is_nullptr<_Arg7, _Arg7>::value);
                    }

                    template<class _ReturnT>
                    static void push(const priority_tag<4>&, _arguments &args, _ObjectT &fp, _ReturnT(_ObjectT::*)(_Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7))
                    {
                        _call_helper<_ObjectT>::call(fp, args, 
                            _type_is_nullptr<_Arg1, _Arg1>::value,
                            _type_is_nullptr<_Arg2, _Arg2>::value,
                            _type_is_nullptr<_Arg3, _Arg3>::value,
                            _type_is_nullptr<_Arg4, _Arg4>::value,
                            _type_is_nullptr<_Arg5, _Arg5>::value,
                            _type_is_nullptr<_Arg6, _Arg6>::value,
                            _type_is_nullptr<_Arg7, _Arg7>::value);
                    }

                    template<class _ReturnT, class _FArg1, class _FArg2, class _FArg3, class _FArg4, class _FArg5, class _FArg6, class _FArg7>
                    static void push(const priority_tag<3>&, _arguments &args, const _ObjectT &fp, _ReturnT(_ObjectT::*)(_FArg1, _FArg2, _FArg3, _FArg4, _FArg5, _FArg6, _FArg7) const)
                    {
                        _call_helper<const _ObjectT>::call(fp, args, 
                            _type_is_nullptr<_Arg1, _FArg1>::value,
                            _type_is_nullptr<_Arg2, _FArg2>::value,
                            _type_is_nullptr<_Arg3, _FArg3>::value,
                            _type_is_nullptr<_Arg4, _FArg4>::value,
                            _type_is_nullptr<_Arg5, _FArg5>::value,
                            _type_is_nullptr<_Arg6, _FArg6>::value,
                            _type_is_nullptr<_Arg7, _FArg7>::value);
                    }

                    template<class _ReturnT, class _FArg1, class _FArg2, class _FArg3, class _FArg4, class _FArg5, class _FArg6, class _FArg7>
                    static void push(const priority_tag<2>&, _arguments &args, _ObjectT &fp, _ReturnT(_ObjectT::*)(_FArg1, _FArg2, _FArg3, _FArg4, _FArg5, _FArg6, _FArg7))
                    {
                        _call_helper<_ObjectT>::call(fp, args, 
                            _type_is_nullptr<_Arg1, _FArg1>::value,
                            _type_is_nullptr<_Arg2, _FArg2>::value,
                            _type_is_nullptr<_Arg3, _FArg3>::value,
                            _type_is_nullptr<_Arg4, _FArg4>::value,
                            _type_is_nullptr<_Arg5, _FArg5>::value,
                            _type_is_nullptr<_Arg6, _FArg6>::value,
                            _type_is_nullptr<_Arg7, _FArg7>::value);
                    }
                };
            };

            typedef _arguments arguments_type;
        };

        template<class _Arg1, class _Arg2, class _Arg3, class _Arg4, class _Arg5, class _Arg6, class _Arg7, class _Arg8>
        struct _thread_args_helper<_Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8>
        {
            struct _arguments
            {
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
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ain.arg1,ain.arg2,ain.arg3,ain.arg4,ain.arg5,ain.arg6,ain.arg7,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ain.arg1,ain.arg2,ain.arg3,ain.arg4,ain.arg5,ain.arg6,ain.arg7,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ain.arg1,ain.arg2,ain.arg3,ain.arg4,ain.arg5,ain.arg6,nullptr,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(ain.arg1,ain.arg2,ain.arg3,ain.arg4,ain.arg5,ain.arg6,nullptr,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ain.arg1,ain.arg2,ain.arg3,ain.arg4,ain.arg5,nullptr,ain.arg7,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ain.arg1,ain.arg2,ain.arg3,ain.arg4,ain.arg5,nullptr,ain.arg7,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ain.arg1,ain.arg2,ain.arg3,ain.arg4,ain.arg5,nullptr,nullptr,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(ain.arg1,ain.arg2,ain.arg3,ain.arg4,ain.arg5,nullptr,nullptr,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ain.arg1,ain.arg2,ain.arg3,ain.arg4,nullptr,ain.arg6,ain.arg7,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ain.arg1,ain.arg2,ain.arg3,ain.arg4,nullptr,ain.arg6,ain.arg7,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ain.arg1,ain.arg2,ain.arg3,ain.arg4,nullptr,ain.arg6,nullptr,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(ain.arg1,ain.arg2,ain.arg3,ain.arg4,nullptr,ain.arg6,nullptr,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ain.arg1,ain.arg2,ain.arg3,ain.arg4,nullptr,nullptr,ain.arg7,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ain.arg1,ain.arg2,ain.arg3,ain.arg4,nullptr,nullptr,ain.arg7,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ain.arg1,ain.arg2,ain.arg3,ain.arg4,nullptr,nullptr,nullptr,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(ain.arg1,ain.arg2,ain.arg3,ain.arg4,nullptr,nullptr,nullptr,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ain.arg1,ain.arg2,ain.arg3,nullptr,ain.arg5,ain.arg6,ain.arg7,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ain.arg1,ain.arg2,ain.arg3,nullptr,ain.arg5,ain.arg6,ain.arg7,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ain.arg1,ain.arg2,ain.arg3,nullptr,ain.arg5,ain.arg6,nullptr,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(ain.arg1,ain.arg2,ain.arg3,nullptr,ain.arg5,ain.arg6,nullptr,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ain.arg1,ain.arg2,ain.arg3,nullptr,ain.arg5,nullptr,ain.arg7,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ain.arg1,ain.arg2,ain.arg3,nullptr,ain.arg5,nullptr,ain.arg7,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ain.arg1,ain.arg2,ain.arg3,nullptr,ain.arg5,nullptr,nullptr,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(ain.arg1,ain.arg2,ain.arg3,nullptr,ain.arg5,nullptr,nullptr,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ain.arg1,ain.arg2,ain.arg3,nullptr,nullptr,ain.arg6,ain.arg7,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ain.arg1,ain.arg2,ain.arg3,nullptr,nullptr,ain.arg6,ain.arg7,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ain.arg1,ain.arg2,ain.arg3,nullptr,nullptr,ain.arg6,nullptr,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(ain.arg1,ain.arg2,ain.arg3,nullptr,nullptr,ain.arg6,nullptr,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ain.arg1,ain.arg2,ain.arg3,nullptr,nullptr,nullptr,ain.arg7,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ain.arg1,ain.arg2,ain.arg3,nullptr,nullptr,nullptr,ain.arg7,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ain.arg1,ain.arg2,ain.arg3,nullptr,nullptr,nullptr,nullptr,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(ain.arg1,ain.arg2,ain.arg3,nullptr,nullptr,nullptr,nullptr,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ain.arg1,ain.arg2,nullptr,ain.arg4,ain.arg5,ain.arg6,ain.arg7,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ain.arg1,ain.arg2,nullptr,ain.arg4,ain.arg5,ain.arg6,ain.arg7,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ain.arg1,ain.arg2,nullptr,ain.arg4,ain.arg5,ain.arg6,nullptr,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(ain.arg1,ain.arg2,nullptr,ain.arg4,ain.arg5,ain.arg6,nullptr,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ain.arg1,ain.arg2,nullptr,ain.arg4,ain.arg5,nullptr,ain.arg7,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ain.arg1,ain.arg2,nullptr,ain.arg4,ain.arg5,nullptr,ain.arg7,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ain.arg1,ain.arg2,nullptr,ain.arg4,ain.arg5,nullptr,nullptr,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(ain.arg1,ain.arg2,nullptr,ain.arg4,ain.arg5,nullptr,nullptr,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ain.arg1,ain.arg2,nullptr,ain.arg4,nullptr,ain.arg6,ain.arg7,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ain.arg1,ain.arg2,nullptr,ain.arg4,nullptr,ain.arg6,ain.arg7,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ain.arg1,ain.arg2,nullptr,ain.arg4,nullptr,ain.arg6,nullptr,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(ain.arg1,ain.arg2,nullptr,ain.arg4,nullptr,ain.arg6,nullptr,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ain.arg1,ain.arg2,nullptr,ain.arg4,nullptr,nullptr,ain.arg7,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ain.arg1,ain.arg2,nullptr,ain.arg4,nullptr,nullptr,ain.arg7,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ain.arg1,ain.arg2,nullptr,ain.arg4,nullptr,nullptr,nullptr,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(ain.arg1,ain.arg2,nullptr,ain.arg4,nullptr,nullptr,nullptr,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ain.arg1,ain.arg2,nullptr,nullptr,ain.arg5,ain.arg6,ain.arg7,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ain.arg1,ain.arg2,nullptr,nullptr,ain.arg5,ain.arg6,ain.arg7,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ain.arg1,ain.arg2,nullptr,nullptr,ain.arg5,ain.arg6,nullptr,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(ain.arg1,ain.arg2,nullptr,nullptr,ain.arg5,ain.arg6,nullptr,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ain.arg1,ain.arg2,nullptr,nullptr,ain.arg5,nullptr,ain.arg7,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ain.arg1,ain.arg2,nullptr,nullptr,ain.arg5,nullptr,ain.arg7,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ain.arg1,ain.arg2,nullptr,nullptr,ain.arg5,nullptr,nullptr,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(ain.arg1,ain.arg2,nullptr,nullptr,ain.arg5,nullptr,nullptr,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ain.arg1,ain.arg2,nullptr,nullptr,nullptr,ain.arg6,ain.arg7,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ain.arg1,ain.arg2,nullptr,nullptr,nullptr,ain.arg6,ain.arg7,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ain.arg1,ain.arg2,nullptr,nullptr,nullptr,ain.arg6,nullptr,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(ain.arg1,ain.arg2,nullptr,nullptr,nullptr,ain.arg6,nullptr,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ain.arg1,ain.arg2,nullptr,nullptr,nullptr,nullptr,ain.arg7,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ain.arg1,ain.arg2,nullptr,nullptr,nullptr,nullptr,ain.arg7,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ain.arg1,ain.arg2,nullptr,nullptr,nullptr,nullptr,nullptr,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(ain.arg1,ain.arg2,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ain.arg1,nullptr,ain.arg3,ain.arg4,ain.arg5,ain.arg6,ain.arg7,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ain.arg1,nullptr,ain.arg3,ain.arg4,ain.arg5,ain.arg6,ain.arg7,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ain.arg1,nullptr,ain.arg3,ain.arg4,ain.arg5,ain.arg6,nullptr,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(ain.arg1,nullptr,ain.arg3,ain.arg4,ain.arg5,ain.arg6,nullptr,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ain.arg1,nullptr,ain.arg3,ain.arg4,ain.arg5,nullptr,ain.arg7,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ain.arg1,nullptr,ain.arg3,ain.arg4,ain.arg5,nullptr,ain.arg7,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ain.arg1,nullptr,ain.arg3,ain.arg4,ain.arg5,nullptr,nullptr,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(ain.arg1,nullptr,ain.arg3,ain.arg4,ain.arg5,nullptr,nullptr,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ain.arg1,nullptr,ain.arg3,ain.arg4,nullptr,ain.arg6,ain.arg7,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ain.arg1,nullptr,ain.arg3,ain.arg4,nullptr,ain.arg6,ain.arg7,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ain.arg1,nullptr,ain.arg3,ain.arg4,nullptr,ain.arg6,nullptr,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(ain.arg1,nullptr,ain.arg3,ain.arg4,nullptr,ain.arg6,nullptr,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ain.arg1,nullptr,ain.arg3,ain.arg4,nullptr,nullptr,ain.arg7,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ain.arg1,nullptr,ain.arg3,ain.arg4,nullptr,nullptr,ain.arg7,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ain.arg1,nullptr,ain.arg3,ain.arg4,nullptr,nullptr,nullptr,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(ain.arg1,nullptr,ain.arg3,ain.arg4,nullptr,nullptr,nullptr,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ain.arg1,nullptr,ain.arg3,nullptr,ain.arg5,ain.arg6,ain.arg7,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ain.arg1,nullptr,ain.arg3,nullptr,ain.arg5,ain.arg6,ain.arg7,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ain.arg1,nullptr,ain.arg3,nullptr,ain.arg5,ain.arg6,nullptr,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(ain.arg1,nullptr,ain.arg3,nullptr,ain.arg5,ain.arg6,nullptr,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ain.arg1,nullptr,ain.arg3,nullptr,ain.arg5,nullptr,ain.arg7,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ain.arg1,nullptr,ain.arg3,nullptr,ain.arg5,nullptr,ain.arg7,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ain.arg1,nullptr,ain.arg3,nullptr,ain.arg5,nullptr,nullptr,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(ain.arg1,nullptr,ain.arg3,nullptr,ain.arg5,nullptr,nullptr,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ain.arg1,nullptr,ain.arg3,nullptr,nullptr,ain.arg6,ain.arg7,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ain.arg1,nullptr,ain.arg3,nullptr,nullptr,ain.arg6,ain.arg7,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ain.arg1,nullptr,ain.arg3,nullptr,nullptr,ain.arg6,nullptr,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(ain.arg1,nullptr,ain.arg3,nullptr,nullptr,ain.arg6,nullptr,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ain.arg1,nullptr,ain.arg3,nullptr,nullptr,nullptr,ain.arg7,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ain.arg1,nullptr,ain.arg3,nullptr,nullptr,nullptr,ain.arg7,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ain.arg1,nullptr,ain.arg3,nullptr,nullptr,nullptr,nullptr,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(ain.arg1,nullptr,ain.arg3,nullptr,nullptr,nullptr,nullptr,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ain.arg1,nullptr,nullptr,ain.arg4,ain.arg5,ain.arg6,ain.arg7,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ain.arg1,nullptr,nullptr,ain.arg4,ain.arg5,ain.arg6,ain.arg7,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ain.arg1,nullptr,nullptr,ain.arg4,ain.arg5,ain.arg6,nullptr,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(ain.arg1,nullptr,nullptr,ain.arg4,ain.arg5,ain.arg6,nullptr,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ain.arg1,nullptr,nullptr,ain.arg4,ain.arg5,nullptr,ain.arg7,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ain.arg1,nullptr,nullptr,ain.arg4,ain.arg5,nullptr,ain.arg7,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ain.arg1,nullptr,nullptr,ain.arg4,ain.arg5,nullptr,nullptr,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(ain.arg1,nullptr,nullptr,ain.arg4,ain.arg5,nullptr,nullptr,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ain.arg1,nullptr,nullptr,ain.arg4,nullptr,ain.arg6,ain.arg7,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ain.arg1,nullptr,nullptr,ain.arg4,nullptr,ain.arg6,ain.arg7,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ain.arg1,nullptr,nullptr,ain.arg4,nullptr,ain.arg6,nullptr,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(ain.arg1,nullptr,nullptr,ain.arg4,nullptr,ain.arg6,nullptr,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ain.arg1,nullptr,nullptr,ain.arg4,nullptr,nullptr,ain.arg7,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ain.arg1,nullptr,nullptr,ain.arg4,nullptr,nullptr,ain.arg7,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ain.arg1,nullptr,nullptr,ain.arg4,nullptr,nullptr,nullptr,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(ain.arg1,nullptr,nullptr,ain.arg4,nullptr,nullptr,nullptr,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ain.arg1,nullptr,nullptr,nullptr,ain.arg5,ain.arg6,ain.arg7,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ain.arg1,nullptr,nullptr,nullptr,ain.arg5,ain.arg6,ain.arg7,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ain.arg1,nullptr,nullptr,nullptr,ain.arg5,ain.arg6,nullptr,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(ain.arg1,nullptr,nullptr,nullptr,ain.arg5,ain.arg6,nullptr,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ain.arg1,nullptr,nullptr,nullptr,ain.arg5,nullptr,ain.arg7,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ain.arg1,nullptr,nullptr,nullptr,ain.arg5,nullptr,ain.arg7,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ain.arg1,nullptr,nullptr,nullptr,ain.arg5,nullptr,nullptr,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(ain.arg1,nullptr,nullptr,nullptr,ain.arg5,nullptr,nullptr,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ain.arg1,nullptr,nullptr,nullptr,nullptr,ain.arg6,ain.arg7,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ain.arg1,nullptr,nullptr,nullptr,nullptr,ain.arg6,ain.arg7,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ain.arg1,nullptr,nullptr,nullptr,nullptr,ain.arg6,nullptr,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(ain.arg1,nullptr,nullptr,nullptr,nullptr,ain.arg6,nullptr,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(ain.arg1,nullptr,nullptr,nullptr,nullptr,nullptr,ain.arg7,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(ain.arg1,nullptr,nullptr,nullptr,nullptr,nullptr,ain.arg7,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(ain.arg1,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(ain.arg1,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,ain.arg2,ain.arg3,ain.arg4,ain.arg5,ain.arg6,ain.arg7,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,ain.arg2,ain.arg3,ain.arg4,ain.arg5,ain.arg6,ain.arg7,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,ain.arg2,ain.arg3,ain.arg4,ain.arg5,ain.arg6,nullptr,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,ain.arg2,ain.arg3,ain.arg4,ain.arg5,ain.arg6,nullptr,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,ain.arg2,ain.arg3,ain.arg4,ain.arg5,nullptr,ain.arg7,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,ain.arg2,ain.arg3,ain.arg4,ain.arg5,nullptr,ain.arg7,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,ain.arg2,ain.arg3,ain.arg4,ain.arg5,nullptr,nullptr,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,ain.arg2,ain.arg3,ain.arg4,ain.arg5,nullptr,nullptr,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,ain.arg2,ain.arg3,ain.arg4,nullptr,ain.arg6,ain.arg7,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,ain.arg2,ain.arg3,ain.arg4,nullptr,ain.arg6,ain.arg7,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,ain.arg2,ain.arg3,ain.arg4,nullptr,ain.arg6,nullptr,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,ain.arg2,ain.arg3,ain.arg4,nullptr,ain.arg6,nullptr,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,ain.arg2,ain.arg3,ain.arg4,nullptr,nullptr,ain.arg7,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,ain.arg2,ain.arg3,ain.arg4,nullptr,nullptr,ain.arg7,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,ain.arg2,ain.arg3,ain.arg4,nullptr,nullptr,nullptr,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,ain.arg2,ain.arg3,ain.arg4,nullptr,nullptr,nullptr,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,ain.arg2,ain.arg3,nullptr,ain.arg5,ain.arg6,ain.arg7,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,ain.arg2,ain.arg3,nullptr,ain.arg5,ain.arg6,ain.arg7,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,ain.arg2,ain.arg3,nullptr,ain.arg5,ain.arg6,nullptr,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,ain.arg2,ain.arg3,nullptr,ain.arg5,ain.arg6,nullptr,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,ain.arg2,ain.arg3,nullptr,ain.arg5,nullptr,ain.arg7,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,ain.arg2,ain.arg3,nullptr,ain.arg5,nullptr,ain.arg7,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,ain.arg2,ain.arg3,nullptr,ain.arg5,nullptr,nullptr,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,ain.arg2,ain.arg3,nullptr,ain.arg5,nullptr,nullptr,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,ain.arg2,ain.arg3,nullptr,nullptr,ain.arg6,ain.arg7,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,ain.arg2,ain.arg3,nullptr,nullptr,ain.arg6,ain.arg7,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,ain.arg2,ain.arg3,nullptr,nullptr,ain.arg6,nullptr,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,ain.arg2,ain.arg3,nullptr,nullptr,ain.arg6,nullptr,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,ain.arg2,ain.arg3,nullptr,nullptr,nullptr,ain.arg7,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,ain.arg2,ain.arg3,nullptr,nullptr,nullptr,ain.arg7,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,ain.arg2,ain.arg3,nullptr,nullptr,nullptr,nullptr,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,ain.arg2,ain.arg3,nullptr,nullptr,nullptr,nullptr,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,ain.arg2,nullptr,ain.arg4,ain.arg5,ain.arg6,ain.arg7,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,ain.arg2,nullptr,ain.arg4,ain.arg5,ain.arg6,ain.arg7,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,ain.arg2,nullptr,ain.arg4,ain.arg5,ain.arg6,nullptr,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,ain.arg2,nullptr,ain.arg4,ain.arg5,ain.arg6,nullptr,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,ain.arg2,nullptr,ain.arg4,ain.arg5,nullptr,ain.arg7,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,ain.arg2,nullptr,ain.arg4,ain.arg5,nullptr,ain.arg7,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,ain.arg2,nullptr,ain.arg4,ain.arg5,nullptr,nullptr,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,ain.arg2,nullptr,ain.arg4,ain.arg5,nullptr,nullptr,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,ain.arg2,nullptr,ain.arg4,nullptr,ain.arg6,ain.arg7,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,ain.arg2,nullptr,ain.arg4,nullptr,ain.arg6,ain.arg7,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,ain.arg2,nullptr,ain.arg4,nullptr,ain.arg6,nullptr,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,ain.arg2,nullptr,ain.arg4,nullptr,ain.arg6,nullptr,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,ain.arg2,nullptr,ain.arg4,nullptr,nullptr,ain.arg7,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,ain.arg2,nullptr,ain.arg4,nullptr,nullptr,ain.arg7,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,ain.arg2,nullptr,ain.arg4,nullptr,nullptr,nullptr,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,ain.arg2,nullptr,ain.arg4,nullptr,nullptr,nullptr,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,ain.arg2,nullptr,nullptr,ain.arg5,ain.arg6,ain.arg7,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,ain.arg2,nullptr,nullptr,ain.arg5,ain.arg6,ain.arg7,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,ain.arg2,nullptr,nullptr,ain.arg5,ain.arg6,nullptr,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,ain.arg2,nullptr,nullptr,ain.arg5,ain.arg6,nullptr,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,ain.arg2,nullptr,nullptr,ain.arg5,nullptr,ain.arg7,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,ain.arg2,nullptr,nullptr,ain.arg5,nullptr,ain.arg7,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,ain.arg2,nullptr,nullptr,ain.arg5,nullptr,nullptr,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,ain.arg2,nullptr,nullptr,ain.arg5,nullptr,nullptr,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,ain.arg2,nullptr,nullptr,nullptr,ain.arg6,ain.arg7,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,ain.arg2,nullptr,nullptr,nullptr,ain.arg6,ain.arg7,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,ain.arg2,nullptr,nullptr,nullptr,ain.arg6,nullptr,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,ain.arg2,nullptr,nullptr,nullptr,ain.arg6,nullptr,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,ain.arg2,nullptr,nullptr,nullptr,nullptr,ain.arg7,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,ain.arg2,nullptr,nullptr,nullptr,nullptr,ain.arg7,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,ain.arg2,nullptr,nullptr,nullptr,nullptr,nullptr,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,ain.arg2,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,ain.arg3,ain.arg4,ain.arg5,ain.arg6,ain.arg7,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,nullptr,ain.arg3,ain.arg4,ain.arg5,ain.arg6,ain.arg7,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,ain.arg3,ain.arg4,ain.arg5,ain.arg6,nullptr,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,nullptr,ain.arg3,ain.arg4,ain.arg5,ain.arg6,nullptr,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,ain.arg3,ain.arg4,ain.arg5,nullptr,ain.arg7,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,nullptr,ain.arg3,ain.arg4,ain.arg5,nullptr,ain.arg7,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,ain.arg3,ain.arg4,ain.arg5,nullptr,nullptr,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,nullptr,ain.arg3,ain.arg4,ain.arg5,nullptr,nullptr,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,ain.arg3,ain.arg4,nullptr,ain.arg6,ain.arg7,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,nullptr,ain.arg3,ain.arg4,nullptr,ain.arg6,ain.arg7,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,ain.arg3,ain.arg4,nullptr,ain.arg6,nullptr,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,nullptr,ain.arg3,ain.arg4,nullptr,ain.arg6,nullptr,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,ain.arg3,ain.arg4,nullptr,nullptr,ain.arg7,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,nullptr,ain.arg3,ain.arg4,nullptr,nullptr,ain.arg7,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,ain.arg3,ain.arg4,nullptr,nullptr,nullptr,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,nullptr,ain.arg3,ain.arg4,nullptr,nullptr,nullptr,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,ain.arg3,nullptr,ain.arg5,ain.arg6,ain.arg7,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,nullptr,ain.arg3,nullptr,ain.arg5,ain.arg6,ain.arg7,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,ain.arg3,nullptr,ain.arg5,ain.arg6,nullptr,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,nullptr,ain.arg3,nullptr,ain.arg5,ain.arg6,nullptr,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,ain.arg3,nullptr,ain.arg5,nullptr,ain.arg7,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,nullptr,ain.arg3,nullptr,ain.arg5,nullptr,ain.arg7,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,ain.arg3,nullptr,ain.arg5,nullptr,nullptr,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,nullptr,ain.arg3,nullptr,ain.arg5,nullptr,nullptr,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,ain.arg3,nullptr,nullptr,ain.arg6,ain.arg7,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,nullptr,ain.arg3,nullptr,nullptr,ain.arg6,ain.arg7,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,ain.arg3,nullptr,nullptr,ain.arg6,nullptr,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,nullptr,ain.arg3,nullptr,nullptr,ain.arg6,nullptr,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,ain.arg3,nullptr,nullptr,nullptr,ain.arg7,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,nullptr,ain.arg3,nullptr,nullptr,nullptr,ain.arg7,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,ain.arg3,nullptr,nullptr,nullptr,nullptr,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,nullptr,ain.arg3,nullptr,nullptr,nullptr,nullptr,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,nullptr,ain.arg4,ain.arg5,ain.arg6,ain.arg7,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,nullptr,nullptr,ain.arg4,ain.arg5,ain.arg6,ain.arg7,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,nullptr,ain.arg4,ain.arg5,ain.arg6,nullptr,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,nullptr,nullptr,ain.arg4,ain.arg5,ain.arg6,nullptr,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,nullptr,ain.arg4,ain.arg5,nullptr,ain.arg7,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,nullptr,nullptr,ain.arg4,ain.arg5,nullptr,ain.arg7,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,nullptr,ain.arg4,ain.arg5,nullptr,nullptr,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,nullptr,nullptr,ain.arg4,ain.arg5,nullptr,nullptr,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,nullptr,ain.arg4,nullptr,ain.arg6,ain.arg7,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,nullptr,nullptr,ain.arg4,nullptr,ain.arg6,ain.arg7,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,nullptr,ain.arg4,nullptr,ain.arg6,nullptr,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,nullptr,nullptr,ain.arg4,nullptr,ain.arg6,nullptr,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,nullptr,ain.arg4,nullptr,nullptr,ain.arg7,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,nullptr,nullptr,ain.arg4,nullptr,nullptr,ain.arg7,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,nullptr,ain.arg4,nullptr,nullptr,nullptr,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,nullptr,nullptr,ain.arg4,nullptr,nullptr,nullptr,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,nullptr,nullptr,ain.arg5,ain.arg6,ain.arg7,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,nullptr,nullptr,nullptr,ain.arg5,ain.arg6,ain.arg7,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,nullptr,nullptr,ain.arg5,ain.arg6,nullptr,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,nullptr,nullptr,nullptr,ain.arg5,ain.arg6,nullptr,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,nullptr,nullptr,ain.arg5,nullptr,ain.arg7,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,nullptr,nullptr,nullptr,ain.arg5,nullptr,ain.arg7,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,nullptr,nullptr,ain.arg5,nullptr,nullptr,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,nullptr,nullptr,nullptr,ain.arg5,nullptr,nullptr,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,nullptr,nullptr,nullptr,ain.arg6,ain.arg7,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,nullptr,nullptr,nullptr,nullptr,ain.arg6,ain.arg7,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,nullptr,nullptr,nullptr,ain.arg6,nullptr,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,nullptr,nullptr,nullptr,nullptr,ain.arg6,nullptr,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,ain.arg7,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr,eTypeNullptr) { fp(nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,ain.arg7,nullptr); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNotNullptr) { fp(nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,ain.arg8); }
					static void call(_FuncT &fp, _arguments &ain,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr,eTypeNullptr) { fp(nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr); }
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
                    template<class _ReturnT>
                    struct _member_function
                    {
                        typedef _ReturnT(_ObjectT::*type)(_Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8);
                        typedef _ReturnT(_ObjectT::*type_const)(_Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8) const;
                    };

                    template<class _ReturnT>
                    static void push(const priority_tag<5>&, _arguments &args, const _ObjectT &fp, _ReturnT(_ObjectT::*)(_Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8) const)
                    {
                        _call_helper<const _ObjectT>::call(fp, args,
                            _type_is_nullptr<_Arg1, _Arg1>::value,
                            _type_is_nullptr<_Arg2, _Arg2>::value,
                            _type_is_nullptr<_Arg3, _Arg3>::value,
                            _type_is_nullptr<_Arg4, _Arg4>::value,
                            _type_is_nullptr<_Arg5, _Arg5>::value,
                            _type_is_nullptr<_Arg6, _Arg6>::value,
                            _type_is_nullptr<_Arg7, _Arg7>::value,
                            _type_is_nullptr<_Arg8, _Arg8>::value);
                    }

                    template<class _ReturnT>
                    static void push(const priority_tag<4>&, _arguments &args, _ObjectT &fp, _ReturnT(_ObjectT::*)(_Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8))
                    {
                        _call_helper<_ObjectT>::call(fp, args,
                            _type_is_nullptr<_Arg1, _Arg1>::value,
                            _type_is_nullptr<_Arg2, _Arg2>::value,
                            _type_is_nullptr<_Arg3, _Arg3>::value,
                            _type_is_nullptr<_Arg4, _Arg4>::value,
                            _type_is_nullptr<_Arg5, _Arg5>::value,
                            _type_is_nullptr<_Arg6, _Arg6>::value,
                            _type_is_nullptr<_Arg7, _Arg7>::value,
                            _type_is_nullptr<_Arg8, _Arg8>::value);
                    }

                    template<class _ReturnT, class _FArg1, class _FArg2, class _FArg3, class _FArg4, class _FArg5, class _FArg6, class _FArg7, class _FArg8>
                    static void push(const priority_tag<3>&, _arguments &args, const _ObjectT &fp, _ReturnT(_ObjectT::*)(_FArg1, _FArg2, _FArg3, _FArg4, _FArg5, _FArg6, _FArg7, _FArg8) const)
                    {
                        _call_helper<const _ObjectT>::call(fp, args,
                            _type_is_nullptr<_Arg1, _FArg1>::value,
                            _type_is_nullptr<_Arg2, _FArg2>::value,
                            _type_is_nullptr<_Arg3, _FArg3>::value,
                            _type_is_nullptr<_Arg4, _FArg4>::value,
                            _type_is_nullptr<_Arg5, _FArg5>::value,
                            _type_is_nullptr<_Arg6, _FArg6>::value,
                            _type_is_nullptr<_Arg7, _FArg7>::value,
                            _type_is_nullptr<_Arg8, _FArg8>::value);
                    }

                    template<class _ReturnT, class _FArg1, class _FArg2, class _FArg3, class _FArg4, class _FArg5, class _FArg6, class _FArg7, class _FArg8>
                    static void push(const priority_tag<2>&, _arguments &args, _ObjectT &fp, _ReturnT(_ObjectT::*)(_FArg1, _FArg2, _FArg3, _FArg4, _FArg5, _FArg6, _FArg7, _FArg8))
                    {
                        _call_helper<_ObjectT>::call(fp, args,
                            _type_is_nullptr<_Arg1, _FArg1>::value,
                            _type_is_nullptr<_Arg2, _FArg2>::value,
                            _type_is_nullptr<_Arg3, _FArg3>::value,
                            _type_is_nullptr<_Arg4, _FArg4>::value,
                            _type_is_nullptr<_Arg5, _FArg5>::value,
                            _type_is_nullptr<_Arg6, _FArg6>::value,
                            _type_is_nullptr<_Arg7, _FArg7>::value,
                            _type_is_nullptr<_Arg8, _FArg8>::value);
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

        template<class _ObjectT, class _ArgT>
        struct _has_functor_operator_overload
        {
            template<class _ReturnT>
            static _yes_type _has_functor_operator_overload_tester(
                _ReturnT,
                typename _ArgT::template object_member_func<_ObjectT, _ReturnT>::type_const, const priority_tag<3>&);
            template<class _ReturnT>
            static _yes_type _has_functor_operator_overload_tester(
                _ReturnT,
                typename _ArgT::template object_member_func<_ObjectT, _ReturnT>::type, const priority_tag<2>&);
            template<class _ReturnT, class _Tp>
            static _no_type _has_functor_operator_overload_tester(
                _ReturnT, 
                _Tp, const priority_tag<1>&);

            static const bool value = 
                sizeof(_has_functor_operator_overload<_ObjectT, _ArgT>::
                    _has_functor_operator_overload_tester(
                        &_ObjectT::operator(), priority_tag<4>()
                    )
                ) == sizeof(_yes_type);
        };

        enum ePerformFunctorArgsChecks { _perform_functor_args_checks_tag };
        enum eDoNotPerformFunctorArgsChecks { _do_not_perform_functor_args_checks_tag }; 

        template<bool>
        struct _perform_functor_args_checks
        {
            static const ePerformFunctorArgsChecks tag = 
                _perform_functor_args_checks_tag;
        };

        template<>
        struct _perform_functor_args_checks<true>
        {
            static const eDoNotPerformFunctorArgsChecks tag = 
                _do_not_perform_functor_args_checks_tag;
        };

        template<class _FuncT, class _ArgT>
        struct _thread_function_proxy
        {
            typedef _ArgT arguments_type;
            typedef _FuncT function_type;

            arguments_type args;
            function_type fp;

            _thread_function_proxy(function_type fp_, arguments_type args_) :
                fp(fp_),
                args(args_)
            { }

            ~_thread_function_proxy()
            { }

            void call()
            {
                call(_choose_class_overload<is_class<_FuncT>::value>::tag);
            }

            void call(eTypeIsClass)
            {
                //call_class(_perform_functor_args_checks<_has_functor_operator_overload<_FuncT, _ArgT>::value>::tag);
                arguments_type::template _functor<function_type>::push(priority_tag<5>(), args, fp, &function_type::operator());
            }

            void call_class(ePerformFunctorArgsChecks)
            {
                args.push_functor(priority_tag<4>(), fp, &_FuncT::operator());
            }

            void call_class(eDoNotPerformFunctorArgsChecks)
            {
                args.push_functor(priority_tag<4>(), fp);
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
        };
}

    //! Thread class.
    class thread {

        void init(void(*aFunction)(void *), void * aArg);

    public:
        typedef pthread_t native_handle_type;

        //! Thread ID.
        //! The thread ID is a unique identifier for each thread.
        //! @see thread::get_id()
        class id {
        public:
            //! Default constructor.
            //! The default constructed ID is that of thread without a thread of
            //! execution.
            id() _STDEX_NOEXCEPT_FUNCTION :
                _uid()
            { }

            explicit id(const stdex::uintmax_t &uid) :
                _uid(uid)
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
                    return out << "thread::id of a non-executing thread";
                else
                    return out << _uid;
            }

        private:
            friend class thread;
            stdex::uintmax_t _uid;
        };

        //! Default constructor.
        //! Construct a @c thread object without an associated thread of execution
        //! (i.e. non-joinable).
        thread() _STDEX_NOEXCEPT_FUNCTION:
            _handle() 
        { }

        template<class _FuncT>
        explicit thread(_FuncT fx)
        {
            typedef typename detail::_thread_args_helper<>::arguments_type args_t;
            typedef _FuncT func_t;
            init(&detail::_thread_function_proxy<func_t, args_t>::gproxy, new detail::_thread_function_proxy<func_t, args_t>(fx, args_t()));
        }

        template<class _FuncT, class _Arg0T>
        explicit thread(_FuncT fx, _Arg0T t0)
        {
            typedef typename detail::_thread_args_helper<_Arg0T>::arguments_type args_t;
            typedef _FuncT func_t;
            init(&detail::_thread_function_proxy<func_t, args_t>::gproxy, new detail::_thread_function_proxy<func_t, args_t>(fx, args_t(t0)));
        }

        template<class _FuncT, class _Arg0T, class _Arg1T>
        explicit thread(_FuncT fx, _Arg0T t0, _Arg1T t1)
        {
            typedef typename detail::_thread_args_helper<_Arg0T, _Arg1T>::arguments_type args_t;
            typedef _FuncT func_t;
            init(&detail::_thread_function_proxy<func_t, args_t>::gproxy, new detail::_thread_function_proxy<func_t, args_t>(fx, args_t(t0, t1)));
        }

        template<class _FuncT, class _Arg0T, class _Arg1T, class _Arg2T>
        explicit thread(_FuncT fx, _Arg0T t0, _Arg1T t1, _Arg2T t2)
        {
            typedef typename detail::_thread_args_helper<_Arg0T, _Arg1T, _Arg2T>::arguments_type args_t;
            typedef _FuncT func_t;
            init(&detail::_thread_function_proxy<func_t, args_t>::gproxy, new detail::_thread_function_proxy<func_t, args_t>(fx, args_t(t0, t1, t2)));
        }

        template<class _FuncT, class _Arg0T, class _Arg1T, class _Arg2T, class _Arg3T>
        explicit thread(_FuncT fx, _Arg0T t0, _Arg1T t1, _Arg2T t2, _Arg3T t3)
        {
            typedef typename detail::_thread_args_helper<_Arg0T, _Arg1T, _Arg2T, _Arg3T>::arguments_type args_t;
            typedef _FuncT func_t;
            init(&detail::_thread_function_proxy<func_t, args_t>::gproxy, new detail::_thread_function_proxy<func_t, args_t>(fx, args_t(t0, t1, t2, t3)));
        }

        template<class _FuncT, class _Arg0T, class _Arg1T, class _Arg2T, class _Arg3T, class _Arg4T>
        explicit thread(_FuncT fx, _Arg0T t0, _Arg1T t1, _Arg2T t2, _Arg3T t3, _Arg4T t4)
        {
            typedef typename detail::_thread_args_helper<_Arg0T, _Arg1T, _Arg2T, _Arg3T, _Arg4T>::arguments_type args_t;
            typedef _FuncT func_t;
            init(&detail::_thread_function_proxy<func_t, args_t>::gproxy, new detail::_thread_function_proxy<func_t, args_t>(fx, args_t(t0, t1, t2, t3, t4)));
        }

        template<class _FuncT, class _Arg0T, class _Arg1T, class _Arg2T, class _Arg3T, class _Arg4T, class _Arg5T>
        explicit thread(_FuncT fx, _Arg0T t0, _Arg1T t1, _Arg2T t2, _Arg3T t3, _Arg4T t4, _Arg5T t5)
        {
            typedef typename detail::_thread_args_helper<_Arg0T, _Arg1T, _Arg2T, _Arg3T, _Arg4T, _Arg5T>::arguments_type args_t;
            typedef _FuncT func_t;
            init(&detail::_thread_function_proxy<func_t, args_t>::gproxy, new detail::_thread_function_proxy<func_t, args_t>(fx, args_t(t0, t1, t2, t3, t4, t5)));
        }

        template<class _FuncT, class _Arg0T, class _Arg1T, class _Arg2T, class _Arg3T, class _Arg4T, class _Arg5T, class _Arg6T>
        explicit thread(_FuncT fx, _Arg0T t0, _Arg1T t1, _Arg2T t2, _Arg3T t3, _Arg4T t4, _Arg5T t5, _Arg6T t6)
        {
            typedef typename detail::_thread_args_helper<_Arg0T, _Arg1T, _Arg2T, _Arg3T, _Arg4T, _Arg5T, _Arg6T>::arguments_type args_t;
            typedef _FuncT func_t;
            init(&detail::_thread_function_proxy<func_t, args_t>::gproxy, new detail::_thread_function_proxy<func_t, args_t>(fx, args_t(t0, t1, t2, t3, t4, t5, t6)));
        }

        template<class _FuncT, class _Arg0T, class _Arg1T, class _Arg2T, class _Arg3T, class _Arg4T, class _Arg5T, class _Arg6T, class _Arg7T>
        explicit thread(_FuncT fx, _Arg0T t0, _Arg1T t1, _Arg2T t2, _Arg3T t3, _Arg4T t4, _Arg5T t5, _Arg6T t6, _Arg7T t7)
        {
            typedef typename detail::_thread_args_helper<_Arg0T, _Arg1T, _Arg2T, _Arg3T, _Arg4T, _Arg5T, _Arg6T, _Arg7T>::arguments_type args_t;
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
        //! @note If this value is not defined, the function returns zero (0).
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
        void sleep_for_impl(const struct timespec *reltime);
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
            
            timespec ts;
            ts.tv_sec = static_cast<stdex::time_t>(_sec.count());
            ts.tv_nsec = static_cast<long>(_ns.count());

            detail::sleep_for_impl(&ts);
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

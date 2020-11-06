#ifndef _STDEX_FUNCTIONAL_H
#define _STDEX_FUNCTIONAL_H
 
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// stdex includes
#include "./core.h"
#include "./cstdint.hpp"
#include "./type_traits.hpp"
#include "./string.hpp"

// POSIX includes

// std includes
#include <functional>
#include <bitset> // for hash
#include <vector> // for hash
#include <algorithm>

#ifdef _STDEX_NATIVE_CPP11_SUPPORT

#define _STDEX_DELETED_FUNCTION =delete
#define _STDEX_NOEXCEPT_FUNCTION noexcept

#else

#define _STDEX_DELETED_FUNCTION 
#define _STDEX_NOEXCEPT_FUNCTION throw()

#endif

namespace stdex
{
    namespace functional_cpp11
    {
    #ifndef STDEX_DO_NOT_ADD_CPP11_STD // define to exclude std implementations
        using namespace std;
    #endif
    }

    namespace placeholders
    {

    }

    // Classes

    namespace detail
    {
        template<class _Tp, int _N>
        struct _arg
        {
            typedef _Tp type;
            _Tp value;
            _arg(_Tp value_): value(value_) {}
            template<class _OtherTp, int _OtherN>
            _arg(const _arg<_OtherTp, _OtherN> &other) : value(const_cast<_OtherTp>(other.value)) {}
        };

        template<class _NullptrT, bool>
        struct _nullptr_place_holder_impl{
            static const _NullptrT value = nullptr;
        };


        template<class _NullptrT>
        struct _nullptr_place_holder_impl<_NullptrT, true>{
            static _NullptrT value;
        };

        template<class _NullptrT>
        _NullptrT _nullptr_place_holder_impl<_NullptrT, true>::value = _NullptrT();

        _yes_type _rt_nullptr_can_be_ptr_checker(class _dummy*);
        _no_type _rt_nullptr_can_be_ptr_checker(...);

        struct _rt_nullptr_can_be_ptr
        {
            static stdex::nullptr_t declnull();
            static const bool value = 
                sizeof(_rt_nullptr_can_be_ptr_checker(declnull())) == sizeof(_yes_type);
        };
    } // namespace detail

    namespace intern
    {
        template<class>
        struct _has_feature;

        template<>
        struct _has_feature<class _stdex_has_native_nullptr>:
            integral_constant<
                bool,
                detail::_rt_nullptr_can_be_ptr::value == bool(true) &&
                is_class<stdex::nullptr_t>::value == bool(false)
            >
        { };

        template<class>
        struct _has_feature;

        template<>
        struct _has_feature<class _stdex_nullptr_implemented_as_distinct_type>:
            integral_constant<
                bool,
                _has_feature<_stdex_has_native_nullptr>::value == bool(true) ||
                is_union<stdex::nullptr_t>::value == bool(true) ||
                (is_arithmetic<stdex::nullptr_t>::value == bool(false) &&
                 is_arithmetic<remove_pointer<stdex::nullptr_t>::type>::value == bool(false) &&
                 is_void<remove_pointer<stdex::nullptr_t>::type>::value == bool(false) )
            >
        { };
    } // namespace intern

    namespace detail
    {
        struct _nullptr_place_holder:
            _nullptr_place_holder_impl<
                stdex::nullptr_t,
                intern::_has_feature<intern::_stdex_has_native_nullptr>::value>
        { };

        template<int _N>
        struct _arg<_nullptr_place_holder, _N>:
            _nullptr_place_holder
        { 
            typedef stdex::nullptr_t type;
            _arg(stdex::nullptr_t){}
            _arg(const _arg&){}
            template<int _OtherN>
            _arg(const _arg<stdex::nullptr_t, _OtherN>&){}
        };

        template<class, int _End>
        struct _get_args_impl;

        template<class _ArgsT, int _End>
        struct _get_args;

        template<int _Index, class _ArgsT>
        typename _get_args<_ArgsT, _Index>::arg&
        _get_arg(_ArgsT &args);

        template<int _Index, class _ArgsT>
        const
        typename _get_args<_ArgsT, _Index>::arg&
        _get_const_arg(const _ArgsT &args);

        template<class _ArgsT, class _ArgT, int _N>
        struct _args;

        template<class _OtherArgsT, class _OtherArgT>
        struct _other_args_helper 
        {
            typedef const _OtherArgsT& type;
        };

        template<class _OtherArgT>
        struct _other_args_helper<void, _OtherArgT>
        {
            typedef const _args<void, _OtherArgT, 0>& type;
        };

        template<class _ArgsT, class _ArgT, int _N>
        struct _args: _ArgsT, _arg<_ArgT, _N>
        {
            typedef _args type;
            static const int count = _N + 1;

            template<class _OtherArgsT, class _OtherArgT>
            _args(const _args<_OtherArgsT, _OtherArgT, _N - 1> &other, _ArgT arg):
                _ArgsT(
                    static_cast<
                    typename
                    _other_args_helper<
                        _OtherArgsT,
                        _OtherArgT
                    >::type >(other)
                ), 
                _arg<_ArgT, _N>(arg) 
            { }
            template<class _OtherArgsT, class _OtherArgT>
            _args(const _args<_OtherArgsT, _OtherArgT, _N> &other):
                _ArgsT(
                    static_cast<
                    typename
                    _other_args_helper<
                        _OtherArgsT,
                        _OtherArgT
                    >::type >(other)
                ), 
                _arg<_ArgT, _N>(_get_const_arg<_N>(other)) 
            { }

            template<class _NextArgT>
            _args<type, _NextArgT, _N + 1> make(_NextArgT arg) const {return _args<type, _NextArgT, _N + 1>(*this, arg);}

            template<class>
            const type& make(void_type) const { return *this; }
        };

        template<class _ArgT>
        struct _args<void, _ArgT, 0> : _arg<_ArgT, 0>
        {
            typedef _args type;
            static const int count = 1;

            _args(_ArgT arg):
                _arg<_ArgT, 0>(arg) {}
            template<class _OtherArgT>
            _args(const _args<void, _OtherArgT, 0> &other):
                _arg<_ArgT, 0>(_get_const_arg<0>(other)) {}

            template<class _NextArgT>
            _args<type, _NextArgT, 1> make(_NextArgT arg) const {return _args<type, _NextArgT, 1>(*this, arg);}

            template<class>
            const type& make(void_type) const { return *this; }
        };

        namespace functional_std {
            // since there is no move-semantic
            template<class _Tp>
            _Tp& move(_Tp &ref)
            {
                using namespace std;
                using namespace stdex;

                return ref;
            }

            template<class _Tp>
            struct _forward_params
            {
                typedef typename remove_reference<_Tp>::type return_type;
                typedef const typename remove_reference<_Tp>::type& arg_type;
            };

            template<class _Tp>
            struct _forward_params<_Tp&>
            {
                typedef _Tp& return_type;
                typedef _Tp& arg_type;
            };

            template<class _Tp>
            struct _forward_params<const _Tp&>
            {
                typedef const _Tp& return_type;
                typedef const _Tp& arg_type;
            };

            template<class _Tp>
            struct _forward_params<const _Tp>
            {
                typedef const typename remove_reference<_Tp>::type& return_type;
                typedef const typename remove_reference<_Tp>::type& arg_type;
            };

            template<>
            struct _forward_params<void>
            {
                typedef void return_type;
                typedef void arg_type;
            };

            template<class _Tp>
            struct _forward
            {
                static
                typename _forward_params<_Tp>::return_type call(typename _forward_params<_Tp>::arg_type ref)
                {
                    using namespace std;
                    using namespace stdex;

                    typedef typename _forward_params<_Tp>::return_type return_type;

                    return (ref);
                }
            };
        }

        namespace functional_detail
        {
            template<class _Tp>
            static
            typename
            conditional<
                is_void<_Tp>::value,
                void_type,
                _Tp
            >::type operator,(_Tp value, void_type&) { return functional_std::_forward<_Tp>::call(value); }
        }

        template<class _ArgsT, int _End>
        struct _get_args_impl;

        template<class _ArgsT, int _End>
        struct _get_args :
            _get_args_impl<_ArgsT, _End>
        { };

        template<class _ArgsT, class _ArgT, int _Index, int _End>
        struct _get_args_impl<_args<_ArgsT, _ArgT, _Index>, _End>:
            _get_args<_ArgsT, _End>
        { };

        template<class _ArgsT, class _ArgT, int _End>
        struct _get_args<_args<_ArgsT, _ArgT, _End>, _End>
        { 
            typedef _ArgsT base_args;
            typedef _args<_ArgsT, _ArgT, _End> args;
            typedef _arg<_ArgT, _End> arg;
        };

        template<int _Index, class _ArgsT>
        typename _get_args<_ArgsT, _Index>::arg&
        _get_arg(_ArgsT &args)
        {
            return args;
        }

        template<int _Index, class _ArgsT>
        const
        typename _get_args<_ArgsT, _Index>::arg&
        _get_const_arg(const _ArgsT &args)
        {
            return args;
        }

        template<class _R, class _FuncT, int _Index, int _Count>
        struct _check_args_for_null;

        template<class _R, class _FuncT, int _Index, int _Count>
        struct _func_invoker_impl;

        template<class _CheckedArgsT>
        struct _checked_args
        {
            typedef _CheckedArgsT type;
        };

        template<class _R, class _FuncT, int _Index, int _Count, bool>
        struct _check_args_for_null_impl_helper
        {
            template<class _CheckedArgsT, class _RawArgsT>
            static _R call(_FuncT &fx, _RawArgsT &args, 
                const _CheckedArgsT &checked_args)
            {
                typedef _func_invoker_impl<_R, _FuncT, _Index + 1, _Count> func_invoker;
                return functional_std::_forward<_R>::call(
                    func_invoker::call(fx, args, checked_args) );
            }
        };

        template<class _R, class _FuncT, int _Index, int _Count>
        struct _check_args_for_null_impl_helper<_R, _FuncT, _Index, _Count, true>
        {
            template<class _CheckedArgsT, class _RawArgsT>
            static _R call(_FuncT &fx, _RawArgsT &args, 
                const _checked_args<_CheckedArgsT> &)
            {
                    typedef _args<_CheckedArgsT, _nullptr_place_holder, _Index> checked_args_t;
                    typedef _func_invoker_impl<_R, _FuncT, _Index + 1, _Count> func_invoker;

                    return
                    functional_std::_forward<_R>::call(
                        func_invoker::call(fx, args, _checked_args<checked_args_t>()) );
            }

            template<class _RawArgsT>
            static _R call(_FuncT& fx, _RawArgsT& args,
                const _checked_args<_RawArgsT>& )
            {
                typedef 
                typename _get_args<_RawArgsT, _Index>::base_args args_type;

                typedef _args<args_type, _nullptr_place_holder, _Index> checked_args_t;
                typedef _func_invoker_impl<_R, _FuncT, _Index + 1, _Count> func_invoker;

                return
                    functional_std::_forward<_R>::call(
                        func_invoker::call(fx, args, _checked_args<checked_args_t>()));
            }
        };

        template<class _R, class _FuncT, int _Index, int _Count, bool>
        struct _check_args_for_null_impl
        {
            template<class _RawArgsT, class _CheckedArgsT>
            static _R call(_FuncT &fx, _RawArgsT &args, const _checked_args<_CheckedArgsT>& checked_args)
            {
                typedef _check_args_for_null_impl_helper<
                    _R, _FuncT, _Index, _Count,
                    intern::_has_feature<intern::_stdex_nullptr_implemented_as_distinct_type>::value == bool(true)
                > helper;
                return
                functional_std::_forward<_R>::call(
                    helper::call(fx, args, checked_args) );
            }
        };

        template<class _R, class _FuncT, int _Index, int _Count>
        struct _check_args_for_null_impl<_R, _FuncT, _Index, _Count, false>
        {
            typedef _func_invoker_impl<_R, _FuncT, _Index + 1, _Count> func_invoker;
            
            template<class _RawArgsT, class _CheckedArgsT>
            static _R call(_FuncT &fx, _RawArgsT &args, const _checked_args<_CheckedArgsT>& checked_args)
            {
                typedef 
                typename _get_args<_RawArgsT, _Index>::arg arg;

                typedef 
                typename arg::type arg_type;

                typedef _args<_CheckedArgsT, arg_type, _Index> checked_args_t;

                return
                    functional_std::_forward<_R>::call(
                        func_invoker::call(fx, args, _checked_args<checked_args_t>()));
            }

            template<class _RawArgsT>
            static _R call(_FuncT &fx, _RawArgsT &args, const _checked_args<_RawArgsT>& checked_args)
            {
                return
                functional_std::_forward<_R>::call(
                    func_invoker::call(fx, args, checked_args) );
            }
        };

        template<class _R, class _FuncT, int _Index, int _Count>
        struct _check_args_for_null
        {
            template<class _RawArgsT, class _CheckedArgsT>
            static _R call(_FuncT &fx, _RawArgsT &args, const _checked_args<_CheckedArgsT>& checked_args)
            {
                typedef 
                typename _get_args<_RawArgsT, _Index>::arg arg;

                typedef 
                typename arg::type arg_type;

                return
                functional_std::_forward<_R>::call(
                    _check_args_for_null_impl
                    <
                        _R, _FuncT, _Index, _Count,
                        is_null_pointer<typename remove_reference<arg_type>::type>::value == bool(true)
                        && intern::_has_feature<intern::_stdex_has_native_nullptr>::value == bool(false)
                    >::call(fx, args, checked_args) );
            }
        };


        template<class _R, class _FuncT, int _Index, int _Count>
        struct _func_invoker_impl
        {
            typedef _check_args_for_null<_R, _FuncT, _Index, _Count> null_checker;

            template<class _RawArgsT, class _CheckedArgsT>
            static _R call(_FuncT &fx, _RawArgsT &args, const _checked_args<_CheckedArgsT>& checked_args)
            {
                return
                functional_std::_forward<_R>::call(
                    null_checker::call(fx, args, checked_args) );
            }
        };

        template<class _R, class _FuncT>
        struct _func_invoker_impl<_R, _FuncT, 0, 0>
        {
            static _R call(_FuncT &fx)
            {
                struct _functor
                {
                    typedef _R return_type;

                    return_type operator()(_FuncT &fx) {
                        using ::stdex::detail::functional_detail::operator,;
                        ::stdex::detail::void_type dummy;
                        return 
                            (fx(), dummy);
                    }
                }; 
                _functor _f;
                return functional_std::_forward<_R>::call( _f(fx) );
            }
        };

        template<class _R, class _FuncT>
        struct _func_invoker_impl<_R, _FuncT, 2, 2>
        {
            template<class _ArgT0, class _ArgT1, class _ResArgsT>
            static _R func(_FuncT &fx, _arg<_ArgT0, 0>&, _arg<_ArgT1, 1>&, _ResArgsT &res)
            {
                struct _functor: _ResArgsT
                {
                    typedef _ResArgsT base_type;
                    typedef _R return_type;
                    typedef _ArgT0 arg0_type;
                    typedef _ArgT1 arg1_type;
                    typedef return_type(&disable)(...);

                    _functor(const base_type &other) : base_type(other) {}
                    return_type operator()(
                        typename 
                        conditional<
                            is_same<return_type, void_type>::value,
                            disable,
                            _FuncT &>::type fx) 
                    {
                        using ::stdex::detail::_arg;

                        return fx(_arg<arg0_type, 0>::value, _arg<arg1_type, 1>::value);
                    }

                    return_type operator()(
                        typename 
                        conditional<
                            is_same<return_type, void_type>::value,
                            _FuncT &,
                            disable>::type fx) 
                    {
                        using ::stdex::detail::_arg;

                        fx(_arg<arg0_type, 0>::value, _arg<arg1_type, 1>::value);
                        return_type dummy;
                        return dummy;
                    }
                };
                
                _functor _f = res;
                return functional_std::_forward<_R>::call( _f(fx) );
            }
            
            template<class _RawArgsT, class _CheckedArgsT>
            static _R call(_FuncT &fx, _RawArgsT &args, const _checked_args<_CheckedArgsT>&)
            {
                _CheckedArgsT checked_args(functional_std::move(args));
                return functional_std::_forward<_R>::call(
                    func(fx, _get_arg<0>(checked_args), _get_arg<1>(checked_args), checked_args));
            }

            template<class _RawArgsT>
            static _R call(_FuncT &fx, _RawArgsT &args, const _checked_args<_RawArgsT>&)
            {
                return functional_std::_forward<_R>::call(
                    func(fx, _get_arg<0>(args), _get_arg<1>(args), args) );
            }
        };

        template<class _R, class _FuncT, int _Index, int _Count>
        struct _func_invoker
        {
            template<class _RawArgsT>
            static _R call(_FuncT &fx, _RawArgsT &args)
            {
                return 
                    functional_std::_forward<_R>::call(
                        _func_invoker_impl<_R, _FuncT, _Index, _Count>::call(fx, args, _checked_args<_RawArgsT>()) );
            }
        };

        template<class _FuncT, int _Index, int _Count>
        struct _func_invoker<void, _FuncT, _Index, _Count>
        {
            template<class _RawArgsT>
            static void call(_FuncT &fx, _RawArgsT &args)
            {
                _func_invoker_impl<void_type, _FuncT, _Index, _Count>::call(fx, args, _checked_args<_RawArgsT>());
            }
        };
    } // namespace detail

    namespace detail{

        template<class _ArgT>
        struct _arg_is_void:
            bool_constant<
                is_same<_ArgT, void_type>::value == bool(true) ||
                is_void<_ArgT>::value == bool(true)
            >
        { };

        template<class _ArgsT, class _ArgT, int _Index, bool>
        struct _make_args_impl;

        template<class _ArgsT, class _ArgT, class _NextArgT, int _Index>
        struct _add;

        template<class _ArgsT>
        struct _make_args_impl1;

        template<class _ArgsT, class _ArgT, int _Index>
        struct _make_args_impl1<_args<_ArgsT, _ArgT, _Index>/**/>
        {
            template<class _NextArgT>
            struct add
            {
                typedef _add<_ArgsT, _ArgT, _NextArgT, _Index> type;
            };
        };

        template<class _ArgsT, class _ArgT, int _Index, bool VVV>
        struct _make_args_impl:
            _make_args_impl1<_ArgsT>
        {
            typedef _make_args_impl<_ArgsT, _ArgT, _Index, VVV> type;
            typedef _ArgsT args;
        };

        template<class _ArgsT, class _ArgT, int _Index>
        struct _make_args_impl<_ArgsT, _ArgT, _Index, false>:
            _make_args_impl1<_args<_ArgsT, _ArgT, _Index>/**/>
        { 
            typedef _make_args_impl<_ArgsT, _ArgT, _Index, false> type;
            typedef _args<_ArgsT, _ArgT, _Index> args;
        };

        template<class _ArgsT, class _ArgT, class _NextArgT, int _Index>
        struct _add :
            _make_args_impl<_args<_ArgsT, _ArgT, _Index>, _NextArgT, _Index + 1, _arg_is_void<_NextArgT>::value == bool(true)>
        { };

        template<class _ArgT>
        struct _make_args:
            _make_args_impl<void, _ArgT, 0, _arg_is_void<_ArgT>::value>
        { };

        template<class func_base, class _FuncT, class return_type, class args_type>
        struct _functor :
            func_base
        {
            typedef _FuncT func_type;
            typedef return_type function_return_type;
            typedef func_base function_func_base;
            typedef args_type function_args_type;
            typedef _functor type;

            _functor(func_type func) :
                _func(stdex::detail::functional_std::move(func)) {}

            virtual function_func_base* _copy() const { return (new type(_func)); }
            virtual function_func_base* _move() _STDEX_NOEXCEPT_FUNCTION { return (new type(stdex::detail::functional_std::move(_func))); }
            virtual return_type _co_call(function_args_type& args)
            {
                typedef
                    stdex::detail::_func_invoker<function_return_type, func_type, 0, function_args_type::count>
                    func_invoker_with_null_checks;
                typedef
                    stdex::detail::_func_invoker<function_return_type, func_type, function_args_type::count, function_args_type::count>
                    func_invoker_without_null_checks;

                return func_invoker_with_null_checks::call(_func, args);
            }
            virtual void _delete_this() _STDEX_NOEXCEPT_FUNCTION { delete this; }
            // dtor non-virtual due to _delete_this()

            func_type _func;
        };

        template<
            class _R, 
            class _Arg0T  = void_type, 
            class _Arg1T  = void_type, 
            class _Arg2T  = void_type,
            class _Arg3T  = void_type,
            class _Arg4T  = void_type,
            class _Arg5T  = void_type,
            class _Arg6T  = void_type,
            class _Arg7T  = void_type,
            class _Arg8T  = void_type,
            class _Arg9T  = void_type,
            class _Arg10T = void_type,
            class _Arg11T = void_type,
            class _Arg12T = void_type,
            class _Arg13T = void_type,
            class _Arg14T = void_type,
            class _Arg15T = void_type,
            class _Arg16T = void_type,
            class _Arg17T = void_type,
            class _Arg18T = void_type,
            class _Arg19T = void_type,
            class _Arg20T = void_type,
            class _Arg21T = void_type,
            class _Arg22T = void_type,
            class _Arg23T = void_type // up to 24 args
        >
        class function
        {
            typedef
                typename _make_args< _Arg0T >::type::
                template add       < _Arg1T >::type::
                template add       < _Arg2T >::type::
                template add       < _Arg3T >::type::
                template add       < _Arg4T >::type::
                template add       < _Arg5T >::type::
                template add       < _Arg6T >::type::
                template add       < _Arg7T >::type::
                template add       < _Arg8T >::type::
                template add       < _Arg9T >::type::
                template add       < _Arg10T>::type::
                template add       < _Arg11T>::type::
                template add       < _Arg12T>::type::
                template add       < _Arg13T>::type::
                template add       < _Arg14T>::type::
                template add       < _Arg15T>::type::
                template add       < _Arg16T>::type::
                template add       < _Arg17T>::type::
                template add       < _Arg18T>::type::
                template add       < _Arg19T>::type::
                template add       < _Arg20T>::type::
                template add       < _Arg21T>::type::
                template add       < _Arg22T>::type::
                template add       < _Arg23T>::type::
            args args_type;

            struct func_base {
                virtual func_base* _copy() const = 0;
                virtual func_base* _move() _STDEX_NOEXCEPT_FUNCTION = 0;
                virtual _R _co_call(args_type&) = 0;
                virtual void _delete_this() _STDEX_NOEXCEPT_FUNCTION = 0;

                func_base() {}
                virtual ~func_base() {}
            private:
                func_base(const func_base&) _STDEX_DELETED_FUNCTION;
                func_base& operator=(const func_base&) _STDEX_DELETED_FUNCTION;
            };

        public:
            typedef _R return_type;

            function() _STDEX_NOEXCEPT_FUNCTION : _fx(nullptr)  {}
            function(stdex::nullptr_t) _STDEX_NOEXCEPT_FUNCTION : _fx(nullptr)  {}

            function(const function& other) :
                _fx(nullptr)
            { 
                if (!other._fx)
                    return;
                _fx = other._fx->_copy();
            }
            //function(function&& other) _STDEX_NOEXCEPT_FUNCTION;


            template<class _FuncT>
            function(_FuncT func)
            {
                _fx = new _functor<func_base, _FuncT, return_type, args_type>(stdex::detail::functional_std::move(func));
            }

            _R operator()(
                _Arg0T  arg0  = void_type(),
                _Arg1T  arg1  = void_type(),
                _Arg2T  arg2  = void_type(),
                _Arg3T  arg3  = void_type(),
                _Arg4T  arg4  = void_type(),
                _Arg5T  arg5  = void_type(),
                _Arg6T  arg6  = void_type(),
                _Arg7T  arg7  = void_type(),
                _Arg8T  arg8  = void_type(),
                _Arg9T  arg9  = void_type(),
                _Arg10T arg10 = void_type(),
                _Arg11T arg11 = void_type(),
                _Arg12T arg12 = void_type(),
                _Arg13T arg13 = void_type(),
                _Arg14T arg14 = void_type(),
                _Arg15T arg15 = void_type(),
                _Arg16T arg16 = void_type(),
                _Arg17T arg17 = void_type(),
                _Arg18T arg18 = void_type(),
                _Arg19T arg19 = void_type(),
                _Arg20T arg20 = void_type(),
                _Arg21T arg21 = void_type(),
                _Arg22T arg22 = void_type(),
                _Arg23T arg23 = void_type() ) const
            {
                typedef _args<void,    _Arg0T, 0> args_x1;               

                using stdex::detail::functional_std::move;

                args_type args = 
                    args_x1(functional_std::_forward< _Arg0T >::call(arg0))
                    .template make< _Arg1T >(functional_std::_forward< _Arg1T >::call(arg1))
                    .template make< _Arg2T >(functional_std::_forward< _Arg2T >::call(arg2))
                    .template make< _Arg3T >(functional_std::_forward< _Arg3T >::call(arg3))
                    .template make< _Arg4T >(functional_std::_forward< _Arg4T >::call(arg4))
                    .template make< _Arg5T >(functional_std::_forward< _Arg5T >::call(arg5))
                    .template make< _Arg6T >(functional_std::_forward< _Arg6T >::call(arg6))
                    .template make< _Arg7T >(functional_std::_forward< _Arg7T >::call(arg7))
                    .template make< _Arg8T >(functional_std::_forward< _Arg8T >::call(arg8))
                    .template make< _Arg9T >(functional_std::_forward< _Arg9T >::call(arg9))
                    .template make< _Arg10T>(functional_std::_forward< _Arg10T>::call(arg10))
                    .template make< _Arg11T>(functional_std::_forward< _Arg11T>::call(arg11))
                    .template make< _Arg12T>(functional_std::_forward< _Arg12T>::call(arg12))
                    .template make< _Arg13T>(functional_std::_forward< _Arg13T>::call(arg13))
                    .template make< _Arg14T>(functional_std::_forward< _Arg14T>::call(arg14))
                    .template make< _Arg15T>(functional_std::_forward< _Arg15T>::call(arg15))
                    .template make< _Arg16T>(functional_std::_forward< _Arg16T>::call(arg16))
                    .template make< _Arg17T>(functional_std::_forward< _Arg17T>::call(arg17))
                    .template make< _Arg18T>(functional_std::_forward< _Arg18T>::call(arg18))
                    .template make< _Arg19T>(functional_std::_forward< _Arg19T>::call(arg19))
                    .template make< _Arg20T>(functional_std::_forward< _Arg20T>::call(arg20))
                    .template make< _Arg21T>(functional_std::_forward< _Arg21T>::call(arg21))
                    .template make< _Arg22T>(functional_std::_forward< _Arg22T>::call(arg22))
                    .template make< _Arg23T>(functional_std::_forward< _Arg23T>::call(arg23))
                    ;

                return _fx->_co_call(move(args));
            }

        private:


            func_base* _fx;
        };

    } // namespace detail


    template<class _FuncSignatureT>
    class function:
        public function<_FuncSignatureT*>
    { 
        typedef function<_FuncSignatureT*> base_type;
    public:
        typedef typename base_type::return_type return_type;

        function() _STDEX_NOEXCEPT_FUNCTION : base_type() {}
        function(stdex::nullptr_t) _STDEX_NOEXCEPT_FUNCTION : base_type(nullptr) {}

        function(const function& other) : base_type(other) { }
        //function(function&& other) _STDEX_NOEXCEPT_FUNCTION;

        template<class _FuncT>
        function(_FuncT func) : base_type(func) { }

        //using base_type::operator();
    };

    template<class _FuncSignatureT>
    class function<_FuncSignatureT&> :
        public function<_FuncSignatureT*>
    {
        typedef function<_FuncSignatureT*> base_type;
    public:
        typedef typename base_type::return_type return_type;

        function() _STDEX_NOEXCEPT_FUNCTION : base_type() {}
        function(stdex::nullptr_t) _STDEX_NOEXCEPT_FUNCTION : base_type(nullptr) {}

        function(const function& other) : base_type(other) { }
        //function(function&& other) _STDEX_NOEXCEPT_FUNCTION;

        template<class _FuncT>
        function(_FuncT func) : base_type(func) { }

        //using base_type::operator();
    };

    template<class _R>
    class function<_R(*)()>:
        detail::function<_R>
    {
        typedef detail::function<_R> base_type;
    public:
        typedef typename base_type::return_type return_type;

        function() _STDEX_NOEXCEPT_FUNCTION : base_type()  {}
        function(stdex::nullptr_t) _STDEX_NOEXCEPT_FUNCTION : base_type(nullptr)  {}

        function(const function& other) : base_type(other) { }
        //function(function&& other) _STDEX_NOEXCEPT_FUNCTION;

        template<class _FuncT>
        function(_FuncT func): base_type(func) { }

        return_type operator()() { return base_type::operator()(); }
    };

    template<class _R, class _Arg0T, class _Arg1T>
    class function<_R(*)(_Arg0T, _Arg1T)>:
        detail::function<_R, _Arg0T, _Arg1T>
    {
        typedef detail::function<_R, _Arg0T, _Arg1T> base_type;
    public:
        typedef typename base_type::return_type return_type;

        function() _STDEX_NOEXCEPT_FUNCTION : base_type()  {}
        function(stdex::nullptr_t) _STDEX_NOEXCEPT_FUNCTION : base_type(nullptr)  {}

        function(const function& other) : base_type(other) { }
        //function(function&& other) _STDEX_NOEXCEPT_FUNCTION;


        template<class _FuncT>
        function(_FuncT func): base_type(func) { }

        return_type operator()(_Arg0T arg0, _Arg1T arg1) { return base_type::operator()(arg0, arg1); }
    };

    // Hashing

    namespace intern
    {
        // since we have no static_assert in pre-C++11 we just compile-time assert this way:
        struct functional_asserts
        {
            template<bool>
            struct the_cpp_standard_does_not_provide_a_hash_for_this_type_assert;

            template<bool>
            struct the_cpp_standard_does_not_provide_a_hash_for_non_trivial_types_array_assert;
        };

        template<>
        struct functional_asserts::the_cpp_standard_does_not_provide_a_hash_for_this_type_assert<true>
        {
            typedef bool the_cpp_standard_does_not_provide_a_hash_for_this_type_assert_failed;
        };

        template<>
        struct functional_asserts::the_cpp_standard_does_not_provide_a_hash_for_non_trivial_types_array_assert<true>
        {
            typedef bool the_cpp_standard_does_not_provide_a_hash_for_non_trivial_types_array_assert_failed;
        };
    } // namespace intern

    namespace detail
    {
        template<class _SizeT, int>
        struct _bitwise_hash_seq;

        template<class _SizeT>
        struct _bitwise_hash_seq<_SizeT, 8>
        {
            static inline _SizeT call(const unsigned char *_first, _SizeT _count)
            {
                const _SizeT _FNV_offset_basis = 14695981039346656037ULL;
                const _SizeT _FNV_prime = 1099511628211ULL;

                _SizeT _val = _FNV_offset_basis;
                for (_SizeT _next = 0; _next < _count; ++_next)
                    {    // fold in another byte
                    _val ^= static_cast<_SizeT>(_first[_next]);
                    _val *= _FNV_prime;
                    }
                return (_val);
            }
        };

        template<class _SizeT>
        struct _bitwise_hash_seq<_SizeT, 4>
        {
            static inline _SizeT call(const unsigned char *_first, _SizeT _count)
            {
                const _SizeT _FNV_offset_basis = 2166136261U;
                const _SizeT _FNV_prime = 16777619U;

                _SizeT _val = _FNV_offset_basis;
                for (_SizeT _next = 0; _next < _count; ++_next)
                    {    // fold in another byte
                    _val ^= static_cast<_SizeT>(_first[_next]);
                    _val *= _FNV_prime;
                    }
                return (_val);
            }
        };

        template<class _KeyT, class>
        struct _bitwise_hash_impl
        {    // hash functor for plain old data
            typedef _KeyT argument_type;
            typedef std::size_t result_type;

            std::size_t operator()(const _KeyT& _keyval) const
            {    // hash _keyval to std::size_t value by pseudorandomizing transform
                return (_bitwise_hash_seq<std::size_t, sizeof(std::size_t)>::call(&reinterpret_cast<const unsigned char&>(_keyval), sizeof (argument_type)));
            }
        };
        
        template<class _KeyT>
        struct _bitwise_hash_impl<float, _KeyT>
        {
            typedef _KeyT argument_type;
            typedef std::size_t result_type;

            std::size_t operator()(const argument_type& _keyval) const
            {    // hash _keyval to std::size_t value by pseudorandomizing transform
                struct lambdas{
                    static std::size_t hash_seq(const argument_type &keyval)
                    {
                        return (_bitwise_hash_seq<std::size_t, sizeof(std::size_t)>::call(&reinterpret_cast<const unsigned char&>(keyval), sizeof (_KeyT)));
                    }
                };

                return (lambdas::hash_seq(
                    _keyval == 0 ? 0 : _keyval));    // map -0 to 0
            }
        };

        template<class _KeyT>
        struct _bitwise_hash_impl<double, _KeyT>:
            _bitwise_hash_impl<float, _KeyT>
        {
        };

        template<class _KeyT>
        struct _bitwise_hash_impl<long double, _KeyT>:
            _bitwise_hash_impl<float, _KeyT>
        {
        };

        template<class _KeyT>
        struct _hash_impl
        { 
            typedef _bitwise_hash_impl<_KeyT, _KeyT> type;
        };

        template<class _KeyT>
        struct _hash_array_impl
        {
            typedef intern::functional_asserts check;
            typedef typename check::the_cpp_standard_does_not_provide_a_hash_for_non_trivial_types_array_assert<is_class<_KeyT>::value == bool(false)>::
                the_cpp_standard_does_not_provide_a_hash_for_non_trivial_types_array_assert_failed
                check1; // if you are there means you tried to calculate hash for unsupported type

            static std::size_t call(const _KeyT* const _first, const std::size_t _count)
            {
                return (_bitwise_hash_seq<std::size_t, sizeof(std::size_t)>::call(reinterpret_cast<const unsigned char*>(_first), _count * sizeof(_KeyT)));
            }
        };

        template<class _KeyT, bool>
        struct _hash_base
            : public detail::_hash_impl<_KeyT>::type
        {    // hash functor for enums
            typedef intern::functional_asserts check;
            typedef typename check::the_cpp_standard_does_not_provide_a_hash_for_this_type_assert<
                is_const<_KeyT>::value == bool(false) && is_volatile<_KeyT>::value == bool(false) &&
                (is_enum<_KeyT>::value == bool(true) || is_integral<_KeyT>::value == bool(true) || is_pointer<_KeyT>::value == bool(true))
            >::the_cpp_standard_does_not_provide_a_hash_for_this_type_assert_failed
                check1; // if you are there means you tried to calculate hash for unsupported type
        };

        template<class _BasicStringT>
        struct _hash_basic_string
        {    // hash functor for basic_string
            typedef _BasicStringT argument_type;
            typedef std::size_t result_type;

            std::size_t operator()(const argument_type& _keyval) const
            {
                typedef typename _BasicStringT::value_type value_type;
                return detail::_hash_array_impl<value_type>::call(_keyval.c_str(), _keyval.size());
            }
        };

        template<class _KeyT>
        struct _hash_base<_KeyT, true>:
            _hash_basic_string<_KeyT>
        { };

        template<class _ElementT, class _TraitsT, class _AllocatorT>
        _yes_type _is_convertable_to_basic_string_tester(basic_string<_ElementT, _TraitsT, _AllocatorT>*);
        _no_type _is_convertable_to_basic_string_tester(...);

        template<class _KeyT>
        struct _is_convertable_to_basic_string
        {
            static const bool value =
                sizeof(_is_convertable_to_basic_string_tester(_declptr<_KeyT>())) == sizeof(_yes_type);
        };
    }


    template<class _KeyT>
    struct hash
        : public detail::_hash_base<_KeyT, 
            detail::_is_convertable_to_basic_string<_KeyT>::value>
    { };


    template<>
    struct hash<void>;

    template<>
    struct hash<bool>
        : public detail::_hash_impl<bool>::type
    {    // hash functor for bool
    };

    template<>
    struct hash<char>
        : public detail::_hash_impl<char>::type
    {    // hash functor for char
    };

    template<>
    struct hash<signed char>
        : public detail::_hash_impl<signed char>::type
    {    // hash functor for signed char
    };

    template<>
    struct hash<unsigned char>
        : public detail::_hash_impl<unsigned char>::type
    {    // hash functor for unsigned char
    };

    template<>
    struct hash<wchar_t>
        : public detail::_hash_impl<wchar_t>::type
    {    // hash functor for wchar_t
    };

    template<>
    struct hash<short>
        : public detail::_hash_impl<short>::type
    {    // hash functor for short
    };

    template<>
    struct hash<unsigned short>
        : public detail::_hash_impl<unsigned short>::type
    {    // hash functor for unsigned short
    };

    template<>
    struct hash<int>
        : public detail::_hash_impl<int>::type
    {    // hash functor for int
    };

    template<>
    struct hash<unsigned int>
        : public detail::_hash_impl<unsigned int>::type
    {    // hash functor for unsigned int
    };

    template<>
    struct hash<long>
        : public detail::_hash_impl<long>::type
    {    // hash functor for long
    };

    template<>
    struct hash<unsigned long>
        : public detail::_hash_impl<unsigned long>::type
    {    // hash functor for unsigned long
    };

    template<>
    struct hash<long long>
        : public detail::_hash_impl<long long>::type
    {    // hash functor for long long
    };

    template<>
    struct hash<unsigned long long>
        : public detail::_hash_impl<unsigned long long>::type
    {    // hash functor for unsigned long long
    };

    template<>
    struct hash<float>
        : public detail::_hash_impl<float>::type
    {    // hash functor for float
    };

    template<>
    struct hash<double>
        : public detail::_hash_impl<double>::type
    {    // hash functor for double
    };

    template<>
    struct hash<long double>
        : public detail::_hash_impl<long double>::type
    {    // hash functor for long double
    };

    template<class _KeyT>
    struct hash<_KeyT *>
        : public detail::_hash_impl<_KeyT *>::type
    {    // hash functor for _KeyT *
    };

    // standard hash overloads for std headers

    template<std::size_t _Bits>
    struct hash<std::bitset<_Bits>/**/>
    {    // hash functor for bitset<_Bits>
        typedef std::bitset<_Bits> argument_type;
        typedef std::size_t result_type;
    
        std::size_t operator()(const argument_type& _keyval) const
        {
            
            stringstream tmp; tmp << _keyval;
            return hash<std::string>()(tmp.str());
        }
    };

    template<class _ElementT, class _AllocatorT>
    struct hash<std::vector<_ElementT, _AllocatorT>/**/>
    {    // hash functor for vector
        typedef std::vector<_ElementT, _AllocatorT> argument_type;
        typedef std::size_t result_type;
    
        std::size_t operator()(const argument_type& _keyval) const
        {
            if(_keyval.size())
                return detail::_hash_array_impl<_ElementT>::call(&_keyval[0], _keyval.size());
            return 0;
        }
    };

    template<class _AllocatorT>
    struct hash<std::vector<bool, _AllocatorT>/**/>
    {    // hash functor for vector
        typedef std::vector<bool, _AllocatorT> argument_type;
        typedef std::size_t result_type;
    
        std::size_t operator()(const argument_type& _keyval) const
        {
            if(_keyval.empty())
                return 0;
            typedef std::vector<unsigned char> tr_argument_type;
            tr_argument_type tmp(_keyval.size());

            struct lambdas{
                static unsigned char bool_to_uchar(bool value){
                    return (value ? 1 : 0);
                }
            };

            std::transform(_keyval.begin(), _keyval.end(), tmp.begin(), 
                &lambdas::bool_to_uchar);
            
            return hash<tr_argument_type>()(tmp);
        }
    };

    template<class _ElementT, class _TraitsT, class _AllocatorT>
    struct hash<basic_string<_ElementT, _TraitsT, _AllocatorT>/**/>
    {    // hash functor for basic_string
        typedef basic_string<_ElementT, _TraitsT, _AllocatorT> argument_type;
        typedef std::size_t result_type;
    
        std::size_t operator()(const argument_type& _keyval) const
        {
            return detail::_hash_array_impl<_ElementT>::call(_keyval.c_str(), _keyval.size());
        }
    };


    // Arithmetic operations

    using std::plus;
    using std::minus;
    using std::multiplies;
    using std::divides;
    using std::modulus;
    using std::negate;

    // Comparisons

    using std::equal_to;
    using std::not_equal_to;
    using std::greater;
    using std::less;
    using std::greater_equal;
    using std::less_equal;

    // Logical operations

    using std::logical_and;
    using std::logical_or;
    using std::logical_not;

    // Bitwise operations

    template<class _Tp>
    struct bit_and
    {    // functor for operator&
        typedef _Tp first_argument_type;
        typedef _Tp second_argument_type;
        typedef _Tp result_type;

        _Tp operator()(const _Tp& _left, const _Tp& _right) const
        {    // apply operator& to operands
            return (_left & _right);
        }
    };

    template<class _Tp>
    struct bit_or
    {    // functor for operator|
        typedef _Tp first_argument_type;
        typedef _Tp second_argument_type;
        typedef _Tp result_type;

        _Tp operator()(const _Tp& _left, const _Tp& _right) const
        {    // apply operator| to operands
            return (_left | _right);
        }
    };

    template<class _Tp>
    struct bit_xor
    {    // functor for operator^
        typedef _Tp first_argument_type;
        typedef _Tp second_argument_type;
        typedef _Tp result_type;

        _Tp operator()(const _Tp& _left, const _Tp& _right) const
        {    // apply operator^ to operands
            return (_left ^ _right);
        }
    };

    template<class _Tp>
    struct bit_not
    {    // functor for unary operator~
        typedef _Tp argument_type;
        typedef _Tp result_type;

        _Tp operator()(const _Tp& _left) const
        {    // apply operator~ to operand
            return (~_left);
        }
    };

} // namespace stdex

#undef _STDEX_DELETED_FUNCTION
#undef _STDEX_NOEXCEPT_FUNCTION

#endif // _STDEX_FUNCTIONAL_H

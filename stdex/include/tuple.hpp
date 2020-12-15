#ifndef _STDEX_TUPLE_H
#define _STDEX_TUPLE_H
 
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// stdex includes
#include "./type_traits.hpp"

#define _STDEX_PARAMETER_PACK_DEFINE
#include "./parameter_pack.h"
#undef _STDEX_PARAMETER_PACK_DEFINE

// POSIX includes

// std includes
#include <utility> // std::pair, std::swap
#include <algorithm> // std::swap
#include <cstddef> // std::size_t

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
        template<class _Tp, int _N>
        struct _arg
        {
            typedef _Tp value_type;
            _Tp value;
            _arg(_Tp value_): value(value_) {}
            template<class _OtherTp, int _OtherN>
            _arg(const _arg<_OtherTp, _OtherN> &other) : value(const_cast<_OtherTp>(other.value)) {}

            template<class _NextTp, int _NextN>
            static _arg<_NextTp, _NextN> &find(_arg<_NextTp, _NextN> &value) {return value;}
        };

        template<int _N>
        struct _arg<void_type, _N>
        {
            typedef void_type value_type;
            void_type value;
            _arg(void_type value_ = void_type()): value(value_) {}
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
            typedef stdex::nullptr_t value_type;
            _arg(stdex::nullptr_t){}
            _arg(const _arg&){}
            template<int _OtherN>
            _arg(const _arg<stdex::nullptr_t, _OtherN>&){}
        };

        template<class, int _End>
        struct _get_args_traits_impl;

        template<class _ArgsT, int _End>
        struct _get_args_traits;

        template<int _Index, class _ArgsT>
        typename _get_args_traits<_ArgsT, _Index>::arg_type&
        _get_arg(_ArgsT &args);

        template<int _Index, class _ArgsT>
        const
        typename _get_args_traits<_ArgsT, _Index>::arg_type&
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

        namespace tuple_detail
        {
            template<class _ArgT>
            struct _arg_is_void
            : bool_constant<
                is_same<_ArgT, void_type>::value == bool(true) ||
                is_void<_ArgT>::value == bool(true)
            >
            {}; 
        }

        template<class _ArgT>
        struct _args_count_incr
        {
            static const int value = 
                tuple_detail::_arg_is_void<_ArgT>::value ? 0 : 1;
        };

        template<int _N, class _ArgsT>
        struct _args_ctor_helper_cond
        {
            static const bool value = _N >= _ArgsT::count;
        };

        template<class _ArgsT, int _N,
            bool _Disabled>
        struct _args_ctor_helper_impl
        {
            typedef typename _get_args_traits<_ArgsT, _N>::arg_type const& type;
        };

        namespace tuple_detail
        {
            struct disabled_type: void_type
            {
                template<class _ArgT, int _ArgN>
                disabled_type(const _arg<_ArgT, _ArgN>&){}
                disabled_type(const void_type&){}
                disabled_type(const disabled_type&){}
            };
        } // namespace tuple_detail

        template<class _ArgsT, int _N>
        struct _args_ctor_helper_impl<_ArgsT, _N, true>
        {

            typedef tuple_detail::disabled_type type;
        };

        template<class _ArgsT, int _N>
        struct _args_ctor_helper: 
            _args_ctor_helper_impl<_ArgsT, _N, _args_ctor_helper_cond<_N,  _ArgsT>::value>
        { };

        template<class _ArgT, int _N, class _AnyT>
        const _arg<_ArgT, _N>& operator,(const _arg<_ArgT, _N>& value, const _AnyT&) { return value; }
        template<class _LArgT, int _LArgN, class _RArgT, int _RArgN>
        const _arg<_RArgT, _RArgN>& operator,(const _arg<_LArgT, _LArgN>&, const _arg<_RArgT, _RArgN>& value) { return value; }

        template<class _ArgsT, class _ArgT, int _N>
        struct _args: _ArgsT, _arg<_ArgT, _N>
        {
            typedef _args type;
            static const int count = _N + _args_count_incr<_ArgT>::value;

#define _STDEX_PARAMS_TYPE_CUSTOM(arg_n) typename _args_ctor_helper<type, arg_n>::type
#define _STDEX_ARGS_CTOR_PARAMS \
    _STDEX_PARAMS_MAX_IMPL(_STDEX_BLANK, _STDEX_BLANK, _STDEX_BLANK, =void_type(), _STDEX_PARAMS_TYPE_CUSTOM, _STDEX_PARAMS_ARG_DEFAULT) 
    
            _args(_STDEX_ARGS_CTOR_PARAMS)
                : _ArgsT( _STDEX_ARGS_MAX(_STDEX_BLANK, _STDEX_BLANK) ), 
                  _arg<_ArgT, _N>( (_STDEX_ARGS_MAX(_STDEX_BLANK, _STDEX_BLANK)) )
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
        };

        template<class _ArgsT, class _ArgT>
        struct _args<_ArgsT, _ArgT, 0>: _arg<_ArgT, 0>
        {
            typedef _args type;
            static const int count = _args_count_incr<_ArgT>::value;

            _args(_STDEX_ARGS_CTOR_PARAMS)
                : _arg<_ArgT, 0>( arg0 )    
            { }

#undef _STDEX_PARAMS_TYPE_CUSTOM
#undef _STDEX_ARGS_CTOR_PARAMS

            template<class _OtherArgsT, class _OtherArgT>
            _args(const _args<_OtherArgsT, _OtherArgT, 0> &other): 
                _arg<_ArgT, 0>(_get_const_arg<0>(other)) 
            { }
        };

        template<class _ArgsT, int _End>
        struct _get_args_traits_impl;

        template<class _ArgsT, int _End>
        struct _get_args_traits :
            _get_args_traits_impl<_ArgsT, _End>
        { };

        template<class _ArgsT, class _ArgT, int _Index, int _End>
        struct _get_args_traits_impl<_args<_ArgsT, _ArgT, _Index>, _End>:
            _get_args_traits<_ArgsT, _End>
        { };

        template<class _ArgsT, class _ArgT, int _End>
        struct _get_args_traits_impl<_args<_ArgsT, _ArgT, _End>, _End>:
            _get_args_traits<_args<_ArgsT, _ArgT, _End>, _End>
        { };

        template<class _ArgsT, class _ArgT, int _End>
        struct _get_args_traits<_args<_ArgsT, _ArgT, _End>, _End>
        { 
            typedef _ArgsT base_type;
            typedef _args<_ArgsT, _ArgT, _End> type;
            typedef _arg<_ArgT, _End> arg_type;
            typedef typename arg_type::value_type value_type;
        };

        template<int _Index, class _ArgsT>
        typename _get_args_traits<_ArgsT, _Index>::arg_type&
        _get_arg(_ArgsT &args)
        {
            return args;
        }

        template<int _Index, class _ArgsT>
        const
        typename _get_args_traits<_ArgsT, _Index>::arg_type&
        _get_const_arg(const _ArgsT &args)
        {
            return args;
        }

        
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

        typedef _args<void, void_type, 0> _empty_args;

        template<class _ArgsT, class _ArgT>
        struct _make_args_impl<_ArgsT, _ArgT, 0, true>
        {
            typedef _empty_args args;
            typedef _make_args_impl1<args> type;
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
            _make_args_impl<_args<_ArgsT, _ArgT, _Index>, _NextArgT, _Index + 1, tuple_detail::_arg_is_void<_NextArgT>::value == bool(true)>
        { };

        struct _make_args
        { 
            template<class _ArgT>
            struct add :
                _make_args_impl<void, _ArgT, 0, tuple_detail::_arg_is_void<_ArgT>::value> {};
        };
    } // namespace detail

    template<std::size_t, class>
    class tuple_element;

    template<
        _STDEX_TMPL_ARGS_MAX(_STDEX_BLANK, = ::stdex::detail::void_type)
    >
    class tuple
    {
        typedef
        typename detail::_make_args::
#undef _STDEX_DELIM
#define _STDEX_DELIM ::
        _STDEX_TYPES_MAX(template add <, >::type)::
#undef _STDEX_DELIM
#define _STDEX_DELIM _STDEX_DELIM_DEFAULT
        args args_type;

    public:
#define _STDEX_PARAMS_ARG_CUSTOM(arg_n) _STDEX_PARAMS_ARG_DEFAULT(arg_n) = _STDEX_PARAMS_TYPE_DEFAULT(arg_n)()
        tuple(_STDEX_PARAMS_MAX_IMPL(_STDEX_BLANK, _STDEX_BLANK, _STDEX_BLANK, _STDEX_BLANK, _STDEX_PARAMS_TYPE_DEFAULT, _STDEX_PARAMS_ARG_CUSTOM)):
#undef _STDEX_PARAMS_ARG_CUSTOM
            args( _STDEX_ARGS_MAX(_STDEX_BLANK, _STDEX_BLANK) )
        { }

#define _STDEX_TYPE_CUSTOM(arg_n) _OtherArg##arg_n##T
        template<
            _STDEX_TMPL_ARGS_MAX_IMPL(_STDEX_BLANK, _STDEX_BLANK, _STDEX_TYPE_CUSTOM)
        >
        tuple(const tuple<_STDEX_TYPES_MAX_IMPL(_STDEX_BLANK, _STDEX_BLANK, _STDEX_TYPE_CUSTOM)> &other):
            args(other.args)
        { }

        template<
            _STDEX_TMPL_ARGS_MAX_IMPL(_STDEX_BLANK, _STDEX_BLANK, _STDEX_TYPE_CUSTOM)
        >
        tuple& operator=(const tuple<_STDEX_TYPES_MAX_IMPL(_STDEX_BLANK, _STDEX_BLANK, _STDEX_TYPE_CUSTOM)> &other)
#undef _STDEX_TYPE_CUSTOM
        { 
            args = other.args;
            return *this;
        }

        void swap(tuple& other)
        {
            using std::swap;

            swap(args, other.args);
        }

    private:
        args_type args;

        friend class tuple;
        template<std::size_t, class> friend class tuple_element;
    };

    template<std::size_t _N, class _TupleT>
    class tuple_element
    {
    public:
        typedef 
        typename 
            detail::_get_args_traits<typename _TupleT::args_type, _N>::value_type type;
    };

    template<std::size_t _N, _STDEX_TMPL_ARGS_MAX(_STDEX_BLANK, _STDEX_BLANK)>
    typename tuple_element<_N, tuple<_STDEX_TYPES_MAX(_STDEX_BLANK, _STDEX_BLANK)>/**/>::type& 
    get(tuple<_STDEX_TYPES_MAX(_STDEX_BLANK, _STDEX_BLANK)> &value) _STDEX_NOEXCEPT_FUNCTION
    {
        return detail::_get_arg<_N>(value.args).value;
    }

    template<std::size_t _N, _STDEX_TMPL_ARGS_MAX(_STDEX_BLANK, _STDEX_BLANK)> 
    typename tuple_element<_N, tuple<_STDEX_TYPES_MAX(_STDEX_BLANK, _STDEX_BLANK)>/**/>::type const& 
    get(const tuple<_STDEX_TYPES_MAX(_STDEX_BLANK, _STDEX_BLANK)> &value) _STDEX_NOEXCEPT_FUNCTION
    {
        return detail::_get_arg<_N>(value.args).value;
    }

#define _STDEX_TYPE_OTHER(arg_n) _OtherArg##arg_n##T
#define _STDEX_TMPL_ARGS_MAX_LHS \
    _STDEX_TMPL_ARGS_MAX(_STDEX_BLANK, _STDEX_BLANK)
#define _STDEX_TMPL_ARGS_MAX_RHS \
    _STDEX_TMPL_ARGS_MAX_IMPL(_STDEX_BLANK, _STDEX_BLANK, _STDEX_TYPE_OTHER)
#define _STDEX_TYPES_MAX_LHS\
    _STDEX_TYPES_MAX(_STDEX_BLANK, _STDEX_BLANK)
#define _STDEX_TYPES_MAX_RHS\
    _STDEX_TYPES_MAX_IMPL(_STDEX_BLANK, _STDEX_BLANK, _STDEX_TYPE_OTHER)

#define _STDEX_ARG_GET_EQ(arg_n) if(!( get<arg_n>(lhs) == get<arg_n>(rhs) )) return false
#define _STDEX_ARG_GET_NEQ(arg_n) if(get<arg_n>(lhs) != get<arg_n>(rhs)) return true
#define _STDEX_ARG_GET_CMP_IMPL(arg_n, op) if(get<arg_n>(lhs) op get<arg_n>(rhs)) return true; \
                                           if(get<arg_n>(rhs) op get<arg_n>(lhs)) return false 

    template<_STDEX_TMPL_ARGS_MAX_LHS, _STDEX_TMPL_ARGS_MAX_RHS>
    bool operator==(const tuple<_STDEX_TYPES_MAX_LHS> &lhs, const tuple<_STDEX_TYPES_MAX_RHS> &rhs)
    {
#undef _STDEX_DELIM
#define _STDEX_DELIM ;
        _STDEX_ARGS_MAX_IMPL(_STDEX_BLANK, _STDEX_BLANK, _STDEX_ARG_GET_EQ);
        return true;
#undef _STDEX_DELIM
#define _STDEX_DELIM _STDEX_DELIM_DEFAULT
    }

    template<_STDEX_TMPL_ARGS_MAX_LHS, _STDEX_TMPL_ARGS_MAX_RHS>
    bool operator!=(const tuple<_STDEX_TYPES_MAX_LHS> &lhs, const tuple<_STDEX_TYPES_MAX_RHS> &rhs)
    {
#undef _STDEX_DELIM
#define _STDEX_DELIM ;
        _STDEX_ARGS_MAX_IMPL(_STDEX_BLANK, _STDEX_BLANK, _STDEX_ARG_GET_NEQ);
        return true;
#undef _STDEX_DELIM
#define _STDEX_DELIM _STDEX_DELIM_DEFAULT
    }

    template<_STDEX_TMPL_ARGS_MAX_LHS, _STDEX_TMPL_ARGS_MAX_RHS>
    bool operator<(const tuple<_STDEX_TYPES_MAX_LHS> &lhs, const tuple<_STDEX_TYPES_MAX_RHS> &rhs)
    {
#undef _STDEX_DELIM
#define _STDEX_DELIM ;
#define _STDEX_ARG_GET_CMP(arg_n) _STDEX_ARG_GET_CMP_IMPL(arg_n, <)
        _STDEX_ARGS31_IMPL(_STDEX_BLANK, _STDEX_BLANK, _STDEX_ARG_GET_CMP);
        return true;
#undef _STDEX_ARG_GET_CMP
#undef _STDEX_DELIM
#define _STDEX_DELIM _STDEX_DELIM_DEFAULT
    }

    template<_STDEX_TMPL_ARGS_MAX_LHS, _STDEX_TMPL_ARGS_MAX_RHS>
    bool operator>(const tuple<_STDEX_TYPES_MAX_LHS> &lhs, const tuple<_STDEX_TYPES_MAX_RHS> &rhs)
    {
#undef _STDEX_DELIM
#define _STDEX_DELIM ;
#define _STDEX_ARG_GET_CMP(arg_n) _STDEX_ARG_GET_CMP_IMPL(arg_n, >)
        _STDEX_ARGS31_IMPL(_STDEX_BLANK, _STDEX_BLANK, _STDEX_ARG_GET_CMP);
        return true;
#undef _STDEX_ARG_GET_CMP
#undef _STDEX_DELIM
#define _STDEX_DELIM _STDEX_DELIM_DEFAULT
    }

    template<class _Arg0T, class _Arg1T>
    class tuple<_Arg0T, _Arg1T>
    {
        typedef
        typename detail::_make_args::
#undef _STDEX_DELIM
#define _STDEX_DELIM ::
        _STDEX_TYPES1(template add <, >::type)::
#undef _STDEX_DELIM
#define _STDEX_DELIM _STDEX_DELIM_DEFAULT
        args args_type;

    public:
        tuple(_Arg0T arg0 = _Arg0T(), _Arg1T arg1 = _Arg1T()):
            args( arg0, arg1 )
        { }

        template<
            class _OtherArg0T, class _OtherArg1T
        >
        tuple(const tuple<_OtherArg0T, _OtherArg1T> &other):
            args(other.args)
        { }

        template<class _FirstT, class _SecondT>
        tuple(std::pair<_FirstT, _SecondT> other):
            args(detail::_arg<_FirstT, 0>(other.first), detail::_arg<_SecondT, 1>(other.second))
        { }

        template<
            class _OtherArg0T, class _OtherArg1T
        >
        tuple& operator=(const tuple<_OtherArg0T, _OtherArg1T> &other)
        { 
            args = other.args;
            return *this;
        }


    private:
        args_type args;

        template<_STDEX_REPEAT_TOKEN_MAX(class)> friend class tuple;

        template<std::size_t, class> friend class tuple_element;
    };

    template<class>
    class reference_wrapper;

    namespace detail
    {
        template<class _Tp>
        struct _unwrap_refwrapper
        {
            typedef _Tp type;
        };
        
        template<class _Tp>
        struct _unwrap_refwrapper<stdex::reference_wrapper<_Tp>/**/>
        {
            typedef _Tp& type;
        };

        template <class _Tp>
        struct _special_decay :
            _unwrap_refwrapper<typename stdex::decay<_Tp>::type>
        { };
        
        struct _ignore {
            template <class _Tp>
            const _ignore& operator=(const _Tp&) const { return *this; }

            static _ignore &instance(){static _ignore value; return value;}
        };
    }

    const detail::_ignore &ignore = detail::_ignore::instance();

#define _STDEX_MAKE_TUPLE_IMPL(count) \
    template<_STDEX_TMPL_ARGS##count(_STDEX_BLANK, _STDEX_BLANK)> \
    stdex::tuple<_STDEX_TYPES##count##_IMPL(_STDEX_BLANK, _STDEX_BLANK, _STDEX_TYPE_CUSTOM)> \
    make_tuple( _STDEX_PARAMS##count(_STDEX_BLANK, _STDEX_BLANK, _STDEX_BLANK, _STDEX_BLANK) ) \
    { \
        return \
            stdex::tuple<_STDEX_TYPES##count##_IMPL(_STDEX_BLANK, _STDEX_BLANK, _STDEX_TYPE_CUSTOM)>( \
                 _STDEX_ARGS##count(_STDEX_BLANK, _STDEX_BLANK) ); \
    }

#define _STDEX_TIE_IMPL(count) \
    template<_STDEX_TMPL_ARGS##count(_STDEX_BLANK, _STDEX_BLANK)> \
    stdex::tuple<_STDEX_TYPES##count(_STDEX_BLANK, &)> \
    tie( _STDEX_PARAMS##count(_STDEX_BLANK, _STDEX_BLANK, &, _STDEX_BLANK) ) \
    { \
        return \
            stdex::tuple<_STDEX_TYPES##count(_STDEX_BLANK, &)>( \
                 _STDEX_ARGS##count(_STDEX_BLANK, _STDEX_BLANK) ); \
    }
    

#define _STDEX_MAKE_TUPLE_AND_TIE(count) \
    _STDEX_MAKE_TUPLE_IMPL(count) \
    _STDEX_TIE_IMPL(count)

#define _STDEX_TYPE_CUSTOM(arg_n) typename detail::_special_decay< _STDEX_TYPE_DEFAULT(arg_n) >::type

    _STDEX_MAKE_TUPLE_AND_TIE(0)
    _STDEX_MAKE_TUPLE_AND_TIE(1)
    _STDEX_MAKE_TUPLE_AND_TIE(2)
    _STDEX_MAKE_TUPLE_AND_TIE(3)
    _STDEX_MAKE_TUPLE_AND_TIE(4)
    _STDEX_MAKE_TUPLE_AND_TIE(5)
    _STDEX_MAKE_TUPLE_AND_TIE(6)
    _STDEX_MAKE_TUPLE_AND_TIE(7)
    _STDEX_MAKE_TUPLE_AND_TIE(8)
    _STDEX_MAKE_TUPLE_AND_TIE(9)
    _STDEX_MAKE_TUPLE_AND_TIE(10)
    _STDEX_MAKE_TUPLE_AND_TIE(11)
    _STDEX_MAKE_TUPLE_AND_TIE(12)
    _STDEX_MAKE_TUPLE_AND_TIE(13)
    _STDEX_MAKE_TUPLE_AND_TIE(14)
    _STDEX_MAKE_TUPLE_AND_TIE(15)
    _STDEX_MAKE_TUPLE_AND_TIE(16)
    _STDEX_MAKE_TUPLE_AND_TIE(17)
    _STDEX_MAKE_TUPLE_AND_TIE(18)
    _STDEX_MAKE_TUPLE_AND_TIE(19)
    _STDEX_MAKE_TUPLE_AND_TIE(20)
    _STDEX_MAKE_TUPLE_AND_TIE(21)
    _STDEX_MAKE_TUPLE_AND_TIE(22)
    _STDEX_MAKE_TUPLE_AND_TIE(23)
    _STDEX_MAKE_TUPLE_AND_TIE(24)
    _STDEX_MAKE_TUPLE_AND_TIE(25)
    _STDEX_MAKE_TUPLE_AND_TIE(26)
    _STDEX_MAKE_TUPLE_AND_TIE(27)
    _STDEX_MAKE_TUPLE_AND_TIE(28)
    _STDEX_MAKE_TUPLE_AND_TIE(29)
    _STDEX_MAKE_TUPLE_AND_TIE(30)
    _STDEX_MAKE_TUPLE_AND_TIE(31)

#undef _STDEX_TYPE_CUSTOM
#undef _STDEX_MAKE_TUPLE_AND_TIE 
#undef _STDEX_TIE_IMPL
#undef _STDEX_MAKE_TUPLE_IMPL

} // namespace stdex


#undef _STDEX_DELETED_FUNCTION
#undef _STDEX_NOEXCEPT_FUNCTION

#define _STDEX_PARAMETER_PACK_UNDEF
#include "./parameter_pack.h"
#undef _STDEX_PARAMETER_PACK_UNDEF

#endif // _STDEX_TUPLE_H

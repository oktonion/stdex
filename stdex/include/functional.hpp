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
#include "./memory.hpp"
#include "./tuple.hpp"

#define _STDEX_PARAMETER_PACK_DEFINE
#include "./parameter_pack.h"
#undef _STDEX_PARAMETER_PACK_DEFINE

// POSIX includes

// std includes
#include <functional>
#include <bitset> // for hash
#include <vector> // for hash
#include <algorithm>
#include <exception>
#include <typeinfo> // for function::target_type()

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

    namespace intern
    {
        // since we have no static_assert in pre-C++11 we just compile-time assert this way:
        struct functional_asserts
        {
            template<bool>
            struct the_cpp_standard_does_not_provide_a_hash_for_this_type_assert;

            template<bool>
            struct the_cpp_standard_does_not_provide_a_hash_for_non_trivial_types_array_assert;

            template<bool>
            struct reference_wrapper_requires_T_to_be_an_object_type_or_a_function_type_assert;
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

        template<>
        struct functional_asserts::reference_wrapper_requires_T_to_be_an_object_type_or_a_function_type_assert<true>
        {
            typedef bool reference_wrapper_requires_T_to_be_an_object_type_or_a_function_type_assert_failed;
        };
    } // namespace intern

    // Classes

    template<class _FuncSignatureT>
    class function;

    template<class, class>
    class member_function;

    template<class _Tp>
    class reference_wrapper;

    namespace detail
    {
        template<class _R, class _Tp>
        struct _return_type_is_void_if
        {
            typedef void yes_type;
            typedef _Tp no_type; 
        };
        template<class _Tp>
        struct _return_type_is_void_if<void, _Tp>
        {
            typedef _Tp yes_type;
            typedef void no_type; 
        };

        template<class _Tp, bool>
        struct _function_trait
        {
            typedef stdex::function<_Tp> function_base;
            struct type:
                public function_base
            {
                typedef function_base base;
                type(): base(0){}
            };
        };
        template<class _Tp>
        struct _function_trait<_Tp, true>
        {
            typedef stdex::function<_Tp> function_base;
            struct type:
                public function_base
            {
                typedef function_base base;
                template<class _FuncT>
                type(_FuncT &func): base(func) {}

                template<class _RefT>
                type(const reference_wrapper<_RefT> &func): base(func) {}

            private:
                template<class _FuncT>
                type(_FuncT *func);
            };
        };

        template<class _FuncSignatureT>
        struct _function_trait<stdex::function<_FuncSignatureT>/**/, true>
        {
            typedef const stdex::function<_FuncSignatureT> &type;
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
                typedef float arg_type;
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

                static void call() {}
            };

            template<>
            struct _forward<void>
            {
                static void call(void_type) {}
                static void call() {}
            };
        }

    } // namespace detail

    template<class _R>
    inline
        _R invoke(_R(*_func)())
    {
        return
            detail::functional_std::_forward<_R>::call(
                _func());
    }

    template<class _R, class _ObjectT>
    inline
        _R invoke(_R(_ObjectT::* _func)(), _ObjectT& _obj)
    {
        return
            detail::functional_std::_forward<_R>::call(
                (_obj.*_func)());
    }

    template<class _ObjectT>
    inline
        void invoke(void(_ObjectT::* _func)(), _ObjectT& _obj)
    {
        (_obj.*_func)();
    }

    template<class _R, class _ObjectT>
    inline
        _R invoke(_R(_ObjectT::* _func)() const, const _ObjectT& _obj)
    {
        return
            detail::functional_std::_forward<_R>::call(
                (_obj.*_func)());
    }

    template<class _ObjectT>
    inline
        void invoke(void(_ObjectT::* _func)() const, const _ObjectT& _obj)
    {
        (_obj.*_func)();
    }

    template<class _R, class _ObjectT>
    inline
        _R invoke(_R(_ObjectT::* _func)(), _ObjectT* _obj)
    {
        return
            detail::functional_std::_forward<_R>::call(
                ((*_obj).*_func)());
    }

    template<class _ObjectT>
    inline
        void invoke(void(_ObjectT::* _func)(), _ObjectT* _obj)
    {
        ((*_obj).*_func)();
    }

    template<class _R, class _ObjectT>
    inline
        _R invoke(_R(_ObjectT::* _func)() const, const _ObjectT* _obj)
    {
        return
            detail::functional_std::_forward<_R>::call(
                ((*_obj).*_func)());
    }

    template<class _ObjectT>
    inline
        void invoke(void(_ObjectT::* _func)() const, const _ObjectT* _obj)
    {
        ((*_obj).*_func)();
    }

    template<class _R, class _ObjectT>
    inline
        _R invoke(_R(_ObjectT::* _func)(), const reference_wrapper<_ObjectT>& _ref)
    {
        return
            detail::functional_std::_forward<_R>::call(
                (_ref.get().*_func)());
    }


    template<class _ObjectT>
    inline
        void invoke(void(_ObjectT::* _func)(), const reference_wrapper<_ObjectT>& _ref)
    {
        (_ref.get().*_func)();
    }

    template<class _R, class _ObjectT>
    inline
        _R invoke(_R(_ObjectT::* _func)() const, const reference_wrapper<const _ObjectT>& _ref)
    {
        return
            detail::functional_std::_forward<_R>::call(
                (_ref.get().*_func)());
    }

    template<class _ObjectT>
    inline
        void invoke(void(_ObjectT::* _func)() const, const reference_wrapper<const _ObjectT>& _ref)
    {
        (_ref.get().*_func)();
    }

    template<class _R>
    _R invoke(typename stdex::detail::_function_trait<_R(*)(), is_void<_R>::value == bool(false)>::type  _func)
    {
        return
        detail::functional_std::_forward<_R>::call(
            _func() );
    }

    template<class _R>
    _R invoke(typename stdex::detail::_function_trait<_R(*)(), is_void<_R>::value == bool(true)>::type _func)
    {
        _func();
    }

#define _STDEX_INVOKE_IMPL(count, vargs, elipsis_tmpl_args, elipsis_params, elipsis_args) \
    template<class _R, _STDEX_TMPL_ARGS##count(_STDEX_BLANK, _STDEX_BLANK) elipsis_tmpl_args> \
    _R invoke( _R(*_func)(_STDEX_TYPES##count(_STDEX_BLANK, _STDEX_BLANK) vargs ), _STDEX_PARAMS##count(_STDEX_ENABLE_IF_VOID, _STDEX_NO, _STDEX_BLANK, _STDEX_BLANK) elipsis_params)\
    {\
        return\
        detail::functional_std::_forward<_R>::call(\
            _func(_STDEX_ARGS##count##_IMPL(_STDEX_BLANK, _STDEX_BLANK, _STDEX_ARG_CUSTOM) elipsis_args) );\
    }\
\
    template<class _R, _STDEX_TMPL_ARGS##count(_STDEX_BLANK, _STDEX_BLANK) elipsis_tmpl_args> \
    _R invoke( _R(*_func)(_STDEX_TYPES##count(_STDEX_BLANK, _STDEX_BLANK) vargs ), _STDEX_PARAMS##count(_STDEX_ENABLE_IF_VOID, _STDEX_YES, _STDEX_BLANK, _STDEX_BLANK) elipsis_params)\
    {\
        _func(_STDEX_ARGS##count##_IMPL(_STDEX_BLANK, _STDEX_BLANK, _STDEX_ARG_CUSTOM) elipsis_args);\
    }\
\
    template<class _R, class _ObjectT, _STDEX_TMPL_ARGS##count(_STDEX_BLANK, _STDEX_BLANK) elipsis_tmpl_args>\
    _R invoke( _R(_ObjectT::*_func)(_STDEX_TYPES##count(_STDEX_BLANK, _STDEX_BLANK) vargs ), _ObjectT &_obj, _STDEX_PARAMS##count(_STDEX_ENABLE_IF_VOID, _STDEX_NO, _STDEX_BLANK, _STDEX_BLANK) elipsis_params)\
    {\
        return\
        detail::functional_std::_forward<_R>::call(\
            (_obj.*_func)(_STDEX_ARGS##count##_IMPL(_STDEX_BLANK, _STDEX_BLANK, _STDEX_ARG_CUSTOM) elipsis_args) );\
    }\
\
    template<class _R, class _ObjectT, _STDEX_TMPL_ARGS##count(_STDEX_BLANK, _STDEX_BLANK) elipsis_tmpl_args>\
    _R invoke( _R(_ObjectT::*_func)(_STDEX_TYPES##count(_STDEX_BLANK, _STDEX_BLANK) vargs ), _ObjectT &_obj, _STDEX_PARAMS##count(_STDEX_ENABLE_IF_VOID, _STDEX_YES, _STDEX_BLANK, _STDEX_BLANK) elipsis_params)\
    {\
        (_obj.*_func)(_STDEX_ARGS##count##_IMPL(_STDEX_BLANK, _STDEX_BLANK, _STDEX_ARG_CUSTOM) elipsis_args);\
    }\
\
    template<class _R, class _ObjectT, _STDEX_TMPL_ARGS##count(_STDEX_BLANK, _STDEX_BLANK) elipsis_tmpl_args>\
    _R invoke( _R(_ObjectT::*_func)(_STDEX_TYPES##count(_STDEX_BLANK, _STDEX_BLANK) vargs ) const, const _ObjectT &_obj, _STDEX_PARAMS##count(_STDEX_ENABLE_IF_VOID, _STDEX_NO, _STDEX_BLANK, _STDEX_BLANK) elipsis_params)\
    {\
        return\
        detail::functional_std::_forward<_R>::call(\
            (_obj.*_func)(_STDEX_ARGS##count##_IMPL(_STDEX_BLANK, _STDEX_BLANK, _STDEX_ARG_CUSTOM) elipsis_args) );\
    }\
\
    template<class _R, class _ObjectT, _STDEX_TMPL_ARGS##count(_STDEX_BLANK, _STDEX_BLANK) elipsis_tmpl_args>\
    _R invoke( _R(_ObjectT::*_func)(_STDEX_TYPES##count(_STDEX_BLANK, _STDEX_BLANK) vargs ) const, const _ObjectT &_obj, _STDEX_PARAMS##count(_STDEX_ENABLE_IF_VOID, _STDEX_YES, _STDEX_BLANK, _STDEX_BLANK) elipsis_params)\
    {\
        (_obj.*_func)(_STDEX_ARGS##count##_IMPL(_STDEX_BLANK, _STDEX_BLANK, _STDEX_ARG_CUSTOM) elipsis_args);\
    }\
\
    template<class _R, class _ObjectT, _STDEX_TMPL_ARGS##count(_STDEX_BLANK, _STDEX_BLANK) elipsis_tmpl_args>\
    _R invoke( _R(_ObjectT::*_func)(_STDEX_TYPES##count(_STDEX_BLANK, _STDEX_BLANK) vargs ), _ObjectT *_obj, _STDEX_PARAMS##count(_STDEX_ENABLE_IF_VOID, _STDEX_NO, _STDEX_BLANK, _STDEX_BLANK) elipsis_params)\
    {\
        return\
        detail::functional_std::_forward<_R>::call(\
            ((*_obj).*_func)(_STDEX_ARGS##count##_IMPL(_STDEX_BLANK, _STDEX_BLANK, _STDEX_ARG_CUSTOM) elipsis_args) );\
    }\
\
    template<class _R, class _ObjectT, _STDEX_TMPL_ARGS##count(_STDEX_BLANK, _STDEX_BLANK) elipsis_tmpl_args>\
    _R invoke( _R(_ObjectT::*_func)(_STDEX_TYPES##count(_STDEX_BLANK, _STDEX_BLANK) vargs ), _ObjectT *_obj, _STDEX_PARAMS##count(_STDEX_ENABLE_IF_VOID, _STDEX_YES, _STDEX_BLANK, _STDEX_BLANK) elipsis_params)\
    {\
        ((*_obj).*_func)(_STDEX_ARGS##count##_IMPL(_STDEX_BLANK, _STDEX_BLANK, _STDEX_ARG_CUSTOM) elipsis_args);\
    }\
\
    template<class _R, class _ObjectT, _STDEX_TMPL_ARGS##count(_STDEX_BLANK, _STDEX_BLANK) elipsis_tmpl_args>\
    _R invoke( _R(_ObjectT::*_func)(_STDEX_TYPES##count(_STDEX_BLANK, _STDEX_BLANK) vargs ) const, const _ObjectT *_obj, _STDEX_PARAMS##count(_STDEX_ENABLE_IF_VOID, _STDEX_NO, _STDEX_BLANK, _STDEX_BLANK) elipsis_params)\
    {\
        return\
        detail::functional_std::_forward<_R>::call(\
            ((*_obj).*_func)(_STDEX_ARGS##count##_IMPL(_STDEX_BLANK, _STDEX_BLANK, _STDEX_ARG_CUSTOM) elipsis_args) );\
    }\
\
    template<class _R, class _ObjectT, _STDEX_TMPL_ARGS##count(_STDEX_BLANK, _STDEX_BLANK) elipsis_tmpl_args>\
    _R invoke( _R(_ObjectT::*_func)(_STDEX_TYPES##count(_STDEX_BLANK, _STDEX_BLANK) vargs ) const, const _ObjectT *_obj, _STDEX_PARAMS##count(_STDEX_ENABLE_IF_VOID, _STDEX_YES, _STDEX_BLANK, _STDEX_BLANK) elipsis_params)\
    {\
        ((*_obj).*_func)(_STDEX_ARGS##count##_IMPL(_STDEX_BLANK, _STDEX_BLANK, _STDEX_ARG_CUSTOM) elipsis_args);\
    }\
\
    template<class _R, class _ObjectT, _STDEX_TMPL_ARGS##count(_STDEX_BLANK, _STDEX_BLANK) elipsis_tmpl_args>\
    _R invoke( _R(_ObjectT::*_func)(_STDEX_TYPES##count(_STDEX_BLANK, _STDEX_BLANK) vargs ), const reference_wrapper<_ObjectT>& _ref, _STDEX_PARAMS##count(_STDEX_ENABLE_IF_VOID, _STDEX_NO, _STDEX_BLANK, _STDEX_BLANK) elipsis_params)\
    {\
        return\
        detail::functional_std::_forward<_R>::call(\
            (_ref.get().*_func)(_STDEX_ARGS##count##_IMPL(_STDEX_BLANK, _STDEX_BLANK, _STDEX_ARG_CUSTOM) elipsis_args) );\
    }\
\
    template<class _R, class _ObjectT, _STDEX_TMPL_ARGS##count(_STDEX_BLANK, _STDEX_BLANK) elipsis_tmpl_args>\
    _R invoke( _R(_ObjectT::*_func)(_STDEX_TYPES##count(_STDEX_BLANK, _STDEX_BLANK) vargs ), const reference_wrapper<_ObjectT>& _ref, _STDEX_PARAMS##count(_STDEX_ENABLE_IF_VOID, _STDEX_YES, _STDEX_BLANK, _STDEX_BLANK) elipsis_params)\
    {\
        (_ref.get().*_func)(_STDEX_ARGS##count##_IMPL(_STDEX_BLANK, _STDEX_BLANK, _STDEX_ARG_CUSTOM) elipsis_args);\
    }\
\
    template<class _R, class _ObjectT, _STDEX_TMPL_ARGS##count(_STDEX_BLANK, _STDEX_BLANK) elipsis_tmpl_args>\
    _R invoke( _R(_ObjectT::*_func)(_STDEX_TYPES##count(_STDEX_BLANK, _STDEX_BLANK) vargs ) const, const reference_wrapper<const _ObjectT> &_ref, _STDEX_PARAMS##count(_STDEX_ENABLE_IF_VOID, _STDEX_NO, _STDEX_BLANK, _STDEX_BLANK) elipsis_params)\
    {\
        return\
        detail::functional_std::_forward<_R>::call(\
            (_ref.get().*_func)(_STDEX_ARGS##count##_IMPL(_STDEX_BLANK, _STDEX_BLANK, _STDEX_ARG_CUSTOM) elipsis_args) );\
    }\
\
    template<class _R, class _ObjectT, _STDEX_TMPL_ARGS##count(_STDEX_BLANK, _STDEX_BLANK) elipsis_tmpl_args>\
    _R invoke( _R(_ObjectT::*_func)(_STDEX_TYPES##count(_STDEX_BLANK, _STDEX_BLANK) vargs ) const, const reference_wrapper<const _ObjectT> &_ref, _STDEX_PARAMS##count(_STDEX_ENABLE_IF_VOID, _STDEX_YES, _STDEX_BLANK, _STDEX_BLANK) elipsis_params)\
    {\
        (_ref.get().*_func)(_STDEX_ARGS##count##_IMPL(_STDEX_BLANK, _STDEX_BLANK, _STDEX_ARG_CUSTOM) elipsis_args);\
    }

#define _STDEX_INVOKE_FALLBACK(count) \
    template<class _R, _STDEX_TMPL_ARGS##count(_STDEX_BLANK, _STDEX_BLANK)>\
    _R invoke(typename stdex::detail::_function_trait<_R(*)(_STDEX_TYPES##count(_STDEX_BLANK, _STDEX_BLANK) vargs ), is_void<_R>::value == bool(false)>::type  _func, _STDEX_PARAMS##count(_STDEX_BLANK, _STDEX_BLANK, _STDEX_BLANK, _STDEX_BLANK))\
    {\
        return\
        detail::functional_std::_forward<_R>::call(\
            _func(_STDEX_ARGS##count##_IMPL(_STDEX_BLANK, _STDEX_BLANK, _STDEX_ARG_CUSTOM)) );\
    } \
\
    template<class _R, _STDEX_TMPL_ARGS##count(_STDEX_BLANK, _STDEX_BLANK)>\
    _R invoke(typename stdex::detail::_function_trait<_R(*)(_STDEX_TYPES##count(_STDEX_BLANK, _STDEX_BLANK) vargs ), is_void<_R>::value == bool(true)>::type  _func, _STDEX_PARAMS##count(_STDEX_BLANK, _STDEX_BLANK, _STDEX_BLANK, _STDEX_BLANK))\
    {\
        _func(_STDEX_ARGS##count##_IMPL(_STDEX_BLANK, _STDEX_BLANK, _STDEX_ARG_CUSTOM));\
    }

#define _STDEX_MEM_FN(count) \
    template<class _R, class _ObjectT, _STDEX_TMPL_ARGS##count(_STDEX_BLANK, _STDEX_BLANK)>\
    stdex::member_function<_ObjectT, \
        _R(*)(_STDEX_TYPES##count(_STDEX_BLANK, _STDEX_BLANK))> \
    mem_fn( _R(_ObjectT::*_func)(_STDEX_TYPES##count(_STDEX_BLANK, _STDEX_BLANK) ) )\
    {\
        typedef stdex::member_function<_ObjectT, \
            _R(*)(_STDEX_TYPES##count(_STDEX_BLANK, _STDEX_BLANK))> result_type; \
        return\
        detail::functional_std::_forward<result_type>::call(\
            result_type(_func) );\
    }\
\
    template<class _R, class _ObjectT, _STDEX_TMPL_ARGS##count(_STDEX_BLANK, _STDEX_BLANK)>\
    stdex::member_function<const _ObjectT, \
        _R(*)(_STDEX_TYPES##count(_STDEX_BLANK, _STDEX_BLANK))> \
    mem_fn( _R(_ObjectT::*_func)(_STDEX_TYPES##count(_STDEX_BLANK, _STDEX_BLANK)) const)\
    {\
        typedef stdex::member_function<const _ObjectT, \
            _R(*)(_STDEX_TYPES##count(_STDEX_BLANK, _STDEX_BLANK))> result_type; \
        return\
        detail::functional_std::_forward<result_type>::call(\
            result_type(_func) );\
    }\
\
    template<class _R, class _ObjectT, _STDEX_TMPL_ARGS##count(_STDEX_BLANK, _STDEX_BLANK)>\
    stdex::member_function<_ObjectT, \
        _R(*)(_STDEX_TYPES##count(_STDEX_BLANK, _STDEX_BLANK)...)> \
    mem_fn( _R(_ObjectT::*_func)(_STDEX_TYPES##count(_STDEX_BLANK, _STDEX_BLANK)...))\
    {\
        typedef stdex::member_function<_ObjectT, \
            _R(*)(_STDEX_TYPES##count(_STDEX_BLANK, _STDEX_BLANK)...)> result_type; \
        return\
        detail::functional_std::_forward<result_type>::call(\
            result_type(_func) );\
    }\
\
    template<class _R, class _ObjectT, _STDEX_TMPL_ARGS##count(_STDEX_BLANK, _STDEX_BLANK)>\
    stdex::member_function<const _ObjectT, \
        _R(*)(_STDEX_TYPES##count(_STDEX_BLANK, _STDEX_BLANK)...)> \
    mem_fn( _R(_ObjectT::*_func)(_STDEX_TYPES##count(_STDEX_BLANK, _STDEX_BLANK)...) const)\
    {\
        typedef stdex::member_function<const _ObjectT, \
            _R(*)(_STDEX_TYPES##count(_STDEX_BLANK, _STDEX_BLANK)...)> result_type; \
        return\
        detail::functional_std::_forward<result_type>::call(\
            result_type(_func) );\
    }

#define _STDEX_ENABLE_IF_VOID typename stdex::detail::_return_type_is_void_if<_R,
#define _STDEX_YES >::yes_type
#define _STDEX_NO >::no_type

#define _STDEX_PARAMS_TYPE_CUSTOM(count) _ElipsisArg##count##T
#define _STDEX_PARAMS_ARG_CUSTOM(count) _elipsis_arg##count
#define _STDEX_ARG_CUSTOM(count) stdex::detail::functional_std::_forward< _STDEX_TYPE_DEFAULT(count) >::call( _STDEX_ARG_DEFAULT(count) )
#define _STDEX_ELIPSIS_TMPL_ARGS(count) _STDEX_TMPL_ARGS##count##_IMPL(_STDEX_BLANK, _STDEX_BLANK, _STDEX_PARAMS_TYPE_CUSTOM)
#define _STDEX_ELIPSIS_PARAMS(count) _STDEX_PARAMS##count##_IMPL(_STDEX_BLANK, _STDEX_BLANK, _STDEX_BLANK, _STDEX_BLANK, _STDEX_PARAMS_TYPE_CUSTOM, _STDEX_PARAMS_ARG_CUSTOM)
#define _STDEX_ELIPSIS_ARGS(count) _STDEX_ARGS##count##_IMPL(_STDEX_BLANK, _STDEX_BLANK, _STDEX_PARAMS_ARG_CUSTOM)

#define _STDEX_ELIPSIS_TYPE_WITH_COMMA ,...
#define _STDEX_ELIPSIS_TMPL_ARGS_WITH_COMMA(count) ,_STDEX_ELIPSIS_TMPL_ARGS(count)
#define _STDEX_ELIPSIS_PARAMS_WITH_COMMA(count) ,_STDEX_ELIPSIS_PARAMS(count) 
#define _STDEX_ELIPSIS_ARGS_WITH_COMMA(count) ,_STDEX_ELIPSIS_ARGS(count) 

#define _STDEX_INVOKE_ELIPSIS_IMPL(count, rcount) _STDEX_INVOKE_IMPL(count, _STDEX_ELIPSIS_TYPE_WITH_COMMA,  _STDEX_ELIPSIS_TMPL_ARGS_WITH_COMMA(rcount) , _STDEX_ELIPSIS_PARAMS_WITH_COMMA(rcount), _STDEX_ELIPSIS_ARGS_WITH_COMMA(rcount))

#define _STDEX_INVOKE_ELIPSIS_IMPL0(count) _STDEX_INVOKE_ELIPSIS_IMPL(count, 7) _STDEX_INVOKE_ELIPSIS_IMPL(count, 6) _STDEX_INVOKE_ELIPSIS_IMPL(count, 5) _STDEX_INVOKE_ELIPSIS_IMPL(count, 4) _STDEX_INVOKE_ELIPSIS_IMPL(count, 3) \
                                           _STDEX_INVOKE_ELIPSIS_IMPL(count, 2) _STDEX_INVOKE_ELIPSIS_IMPL(count, 1) _STDEX_INVOKE_ELIPSIS_IMPL(count, 0)
#define _STDEX_INVOKE_ELIPSIS_IMPL1(count) _STDEX_INVOKE_ELIPSIS_IMPL(count, 6) _STDEX_INVOKE_ELIPSIS_IMPL(count, 5) _STDEX_INVOKE_ELIPSIS_IMPL(count, 4) _STDEX_INVOKE_ELIPSIS_IMPL(count, 3) \
                                           _STDEX_INVOKE_ELIPSIS_IMPL(count, 2) _STDEX_INVOKE_ELIPSIS_IMPL(count, 1) _STDEX_INVOKE_ELIPSIS_IMPL(count, 0)
#define _STDEX_INVOKE_ELIPSIS_IMPL2(count) _STDEX_INVOKE_ELIPSIS_IMPL(count, 5) _STDEX_INVOKE_ELIPSIS_IMPL(count, 4) _STDEX_INVOKE_ELIPSIS_IMPL(count, 3) \
                                           _STDEX_INVOKE_ELIPSIS_IMPL(count, 2) _STDEX_INVOKE_ELIPSIS_IMPL(count, 1) _STDEX_INVOKE_ELIPSIS_IMPL(count, 0)
#define _STDEX_INVOKE_ELIPSIS_IMPL3(count) _STDEX_INVOKE_ELIPSIS_IMPL(count, 4) _STDEX_INVOKE_ELIPSIS_IMPL(count, 3) \
                                           _STDEX_INVOKE_ELIPSIS_IMPL(count, 2) _STDEX_INVOKE_ELIPSIS_IMPL(count, 1) _STDEX_INVOKE_ELIPSIS_IMPL(count, 0)
#define _STDEX_INVOKE_ELIPSIS_IMPL4(count) _STDEX_INVOKE_ELIPSIS_IMPL(count, 3) \
                                           _STDEX_INVOKE_ELIPSIS_IMPL(count, 2) _STDEX_INVOKE_ELIPSIS_IMPL(count, 1) _STDEX_INVOKE_ELIPSIS_IMPL(count, 0)
#define _STDEX_INVOKE_ELIPSIS_IMPL5(count) _STDEX_INVOKE_ELIPSIS_IMPL(count, 2) _STDEX_INVOKE_ELIPSIS_IMPL(count, 1) _STDEX_INVOKE_ELIPSIS_IMPL(count, 0)
#define _STDEX_INVOKE_ELIPSIS_IMPL6(count) _STDEX_INVOKE_ELIPSIS_IMPL(count, 1) _STDEX_INVOKE_ELIPSIS_IMPL(count, 0)
#define _STDEX_INVOKE_ELIPSIS_IMPL7(count) _STDEX_INVOKE_ELIPSIS_IMPL(count, 0)


#define _STDEX_INVOKE_AND_MEM_FN(count) \
        _STDEX_INVOKE_IMPL(count, _STDEX_BLANK, _STDEX_BLANK, _STDEX_BLANK, _STDEX_BLANK)\
        _STDEX_INVOKE_IMPL(count, _STDEX_ELIPSIS_TYPE_WITH_COMMA, _STDEX_BLANK, _STDEX_BLANK, _STDEX_BLANK) \
        _STDEX_INVOKE_FALLBACK(count) \
        _STDEX_MEM_FN(count)

#define _STDEX_INVOKE_ELIPSIS(count) \
        _STDEX_INVOKE_ELIPSIS_IMPL##count(count)
        
        
        _STDEX_INVOKE_ELIPSIS(0)
        _STDEX_INVOKE_ELIPSIS(1)
        _STDEX_INVOKE_ELIPSIS(2)
        _STDEX_INVOKE_ELIPSIS(3)
        _STDEX_INVOKE_ELIPSIS(4)
        _STDEX_INVOKE_ELIPSIS(5)
        _STDEX_INVOKE_ELIPSIS(6)
        _STDEX_INVOKE_ELIPSIS(7)
        //_STDEX_INVOKE_ELIPSIS(8)
        //_STDEX_INVOKE_ELIPSIS(9)
        //_STDEX_INVOKE_ELIPSIS(10)
        //_STDEX_INVOKE_ELIPSIS(11)
        //_STDEX_INVOKE_ELIPSIS(12)
        //_STDEX_INVOKE_ELIPSIS(13)
        //_STDEX_INVOKE_ELIPSIS(14)
        //_STDEX_INVOKE_ELIPSIS(15)
        //_STDEX_INVOKE_ELIPSIS(16)
        //_STDEX_INVOKE_ELIPSIS(17)
        //_STDEX_INVOKE_ELIPSIS(18)
        //_STDEX_INVOKE_ELIPSIS(19)
        //_STDEX_INVOKE_ELIPSIS(20)
        //_STDEX_INVOKE_ELIPSIS(21)
        //_STDEX_INVOKE_ELIPSIS(22)
        //_STDEX_INVOKE_ELIPSIS(23)
        //_STDEX_INVOKE_ELIPSIS(24)
        //_STDEX_INVOKE_ELIPSIS(25)
        //_STDEX_INVOKE_ELIPSIS(26)
        //_STDEX_INVOKE_ELIPSIS(27)
        //_STDEX_INVOKE_ELIPSIS(28)
        //_STDEX_INVOKE_ELIPSIS(29)
        //_STDEX_INVOKE_ELIPSIS(30)
        //_STDEX_INVOKE_ELIPSIS(31)

        _STDEX_INVOKE_AND_MEM_FN(0 )
        _STDEX_INVOKE_AND_MEM_FN(1 )
        _STDEX_INVOKE_AND_MEM_FN(2 )
        _STDEX_INVOKE_AND_MEM_FN(3 )
        _STDEX_INVOKE_AND_MEM_FN(4 )
        _STDEX_INVOKE_AND_MEM_FN(5 )
        _STDEX_INVOKE_AND_MEM_FN(6 )
        _STDEX_INVOKE_AND_MEM_FN(7 )
        _STDEX_INVOKE_AND_MEM_FN(8 )
        _STDEX_INVOKE_AND_MEM_FN(9 )
        _STDEX_INVOKE_AND_MEM_FN(10)
        _STDEX_INVOKE_AND_MEM_FN(11)
        _STDEX_INVOKE_AND_MEM_FN(12)
        _STDEX_INVOKE_AND_MEM_FN(13)
        _STDEX_INVOKE_AND_MEM_FN(14)
        _STDEX_INVOKE_AND_MEM_FN(15)
        _STDEX_INVOKE_AND_MEM_FN(16)
        _STDEX_INVOKE_AND_MEM_FN(17)
        _STDEX_INVOKE_AND_MEM_FN(18)
        _STDEX_INVOKE_AND_MEM_FN(19)
        _STDEX_INVOKE_AND_MEM_FN(20)
        _STDEX_INVOKE_AND_MEM_FN(21)
        _STDEX_INVOKE_AND_MEM_FN(22)
        _STDEX_INVOKE_AND_MEM_FN(23)
        _STDEX_INVOKE_AND_MEM_FN(24)
        _STDEX_INVOKE_AND_MEM_FN(25)
        _STDEX_INVOKE_AND_MEM_FN(26)
        _STDEX_INVOKE_AND_MEM_FN(27)
        _STDEX_INVOKE_AND_MEM_FN(28)
        _STDEX_INVOKE_AND_MEM_FN(29)
        _STDEX_INVOKE_AND_MEM_FN(30)
        _STDEX_INVOKE_AND_MEM_FN(31)

#undef _STDEX_INVOKE_ELIPSIS_IMPL
#undef _STDEX_INVOKE_ELIPSIS_IMPL0
#undef _STDEX_INVOKE_ELIPSIS_IMPL1
#undef _STDEX_INVOKE_ELIPSIS_IMPL2
#undef _STDEX_INVOKE_ELIPSIS_IMPL3
#undef _STDEX_INVOKE_ELIPSIS_IMPL4
#undef _STDEX_INVOKE_ELIPSIS_IMPL5
#undef _STDEX_INVOKE_ELIPSIS_IMPL6
#undef _STDEX_INVOKE_ELIPSIS_IMPL7
#undef _STDEX_INVOKE_ELIPSIS_IMPL8
#undef _STDEX_INVOKE_ELIPSIS_IMPL9
#undef _STDEX_INVOKE_ELIPSIS_IMPL10
#undef _STDEX_INVOKE_ELIPSIS_IMPL11
#undef _STDEX_INVOKE_ELIPSIS_IMPL12
#undef _STDEX_INVOKE_ELIPSIS_IMPL13
#undef _STDEX_INVOKE_ELIPSIS_IMPL14
#undef _STDEX_INVOKE_ELIPSIS_IMPL15
#undef _STDEX_INVOKE_ELIPSIS_IMPL16
#undef _STDEX_INVOKE_ELIPSIS_IMPL17
#undef _STDEX_INVOKE_ELIPSIS_IMPL18
#undef _STDEX_INVOKE_ELIPSIS_IMPL19
#undef _STDEX_INVOKE_ELIPSIS_IMPL20
#undef _STDEX_INVOKE_ELIPSIS_IMPL21
#undef _STDEX_INVOKE_ELIPSIS_IMPL22
#undef _STDEX_INVOKE_ELIPSIS_IMPL23
#undef _STDEX_INVOKE_ELIPSIS_IMPL24
#undef _STDEX_INVOKE_ELIPSIS_IMPL25
#undef _STDEX_INVOKE_ELIPSIS_IMPL26
#undef _STDEX_INVOKE_ELIPSIS_IMPL27
#undef _STDEX_INVOKE_ELIPSIS_IMPL28
#undef _STDEX_INVOKE_ELIPSIS_IMPL29
#undef _STDEX_INVOKE_ELIPSIS_IMPL30
#undef _STDEX_INVOKE_ELIPSIS_IMPL31

#undef _STDEX_ENABLE_IF_VOID
#undef _STDEX_YES
#undef _STDEX_NO

#undef _STDEX_INVOKE_AND_MEM_FN 
#undef _STDEX_INVOKE_IMPL
#undef _STDEX_MEM_FN
#undef _STDEX_INVOKE_ELIPSIS
#undef _STDEX_INVOKE_FALLBACK

#undef _STDEX_ARG_CUSTOM
#undef _STDEX_PARAMS_TYPE_CUSTOM
#undef _STDEX_PARAMS_ARG_CUSTOM
#undef _STDEX_ELIPSIS_PARAMS
#undef _STDEX_ELIPSIS_ARGS

#undef _STDEX_ELIPSIS_TMPL_ARGS_WITH_COMMA
#undef _STDEX_ELIPSIS_PARAMS_WITH_COMMA
#undef _STDEX_ELIPSIS_ARGS_WITH_COMMA

    namespace detail
    {

        template<class _R, class _FuncT, int _Index, int _Count>
        struct _check_args_for_null;

        template<class _R, class _FuncT, int _Index, int _Count>
        struct _func_invoker_impl;

        template<class _CheckedArgsT>
        struct _checked_args
        {
            typedef _CheckedArgsT type;
        };

        template<class _R>
        struct _return_arg
        {
            mutable _R* _ptr;

            _return_arg(const _R& ref_) :_ptr( new _R(ref_)) {}
            _return_arg() :_ptr(0) {}
            void swap(_return_arg& other) { using std::swap; swap(_ptr, other._ptr); }
            ~_return_arg() { delete _ptr; }

            _R* release() { _R* _tmp = _ptr; _ptr = 0; return _tmp; }
            _R& get() { return *_ptr; }

            _return_arg(const _return_arg& other):
                _ptr(0)
            { 
                using std::swap;
                swap(other._ptr, _ptr);
            }

            _return_arg& operator=(const _return_arg& other)
            {
                using std::swap;
                swap(other._ptr, _ptr);
                return *this;
            }
        };

        template<class _R>
        struct _return_arg<_R*>
        {
            _R* _ptr;

            _return_arg(_R* ptr_ = 0) :_ptr(ptr_) {}

            _R* release() { return _ptr; }
            _R* get() { return release(); }

        private:
            //_return_arg(const _return_arg&) _STDEX_DELETED_FUNCTION;
        };

        template<class _R>
        struct _return_arg<_R&>
        {
            _R* _ptr;

            _return_arg(_R& ref_) :_ptr(&ref_) {}
            _return_arg() :_ptr(0) {}

            _R& release() { return *_ptr; }
            _R& get() { return release(); }

        private:
            //_return_arg(const _return_arg&) _STDEX_DELETED_FUNCTION;
        };

        template<>
        struct _return_arg<void>
        {
            _return_arg() {}
            _return_arg(void*) {}
            _return_arg(void_type) {}
            _return_arg(const _return_arg<void_type>&) {}

            void_type &release() { static void_type dummy;  return dummy; }
            void_type &get() { return release(); }

        private:
            //_return_arg(const _return_arg&) _STDEX_DELETED_FUNCTION;
        };

        template<>
        struct _return_arg<void_type>
        {
            _return_arg() {}
            _return_arg(void*) {}
            _return_arg(void_type) {}
            _return_arg(const _return_arg<void>&) {}

            void_type &release() { static void_type dummy;  return dummy; }
            void_type &get() { return release(); }

        private:
            //_return_arg(const _return_arg&) _STDEX_DELETED_FUNCTION;
        };

        template<class>
        struct _callable_args;

        namespace functional_detail
        {
            struct _any
            {
                _any(...){}
            };
        } // namespace functional_detail

        template<class _R, class _FuncT, int _Index, int _Count, bool>
        struct _check_args_for_null_impl_helper
        {
            template<class _CheckedArgsT, class _RawArgsT>
            static void call(_FuncT &fx, _callable_args<_RawArgsT> &args,
                const _CheckedArgsT &checked_args, _return_arg<_R> &result, functional_detail::_any)
            {
                typedef _func_invoker_impl<_R, _FuncT, _Index + 1, _Count> func_invoker;
                func_invoker::call(fx, args, checked_args, result);
            }
        };

        template<class _R, class _FuncT, int _Index, int _Count>
        struct _check_args_for_null_impl_helper<_R, _FuncT, _Index, _Count, true>
        {
            template<class _CheckedArgsT, class _RawArgsT>
            static void call(_FuncT &fx, _callable_args<_RawArgsT> &args,
                const _checked_args<_CheckedArgsT> &, _return_arg<_R> &result, void*)
            {
                typedef _args<_CheckedArgsT, _nullptr_place_holder, _Index> checked_args_t;
                typedef _func_invoker_impl<_R, _FuncT, _Index + 1, _Count> func_invoker;

                func_invoker::call(fx, args, _checked_args<checked_args_t>(), result);
            }

            template<class _CheckedArgsT, class _RawArgsT>
            static void call(_FuncT &fx, _callable_args<_RawArgsT> &args,
                const _checked_args<_CheckedArgsT> &checked_args, _return_arg<_R> &result, functional_detail::_any)
            {
                typedef _func_invoker_impl<_R, _FuncT, _Index + 1, _Count> func_invoker;
                func_invoker::call(fx, args, checked_args, result);
            }

            template<class _RawArgsT>
            static void call(_FuncT& fx, _callable_args<_RawArgsT>& args,
                const _checked_args<_RawArgsT>&, _return_arg<_R> &result, void*)
            {
                typedef 
                typename _get_args_traits<_RawArgsT, _Index>::base_type args_type;

                typedef _args<args_type, _nullptr_place_holder, _Index> checked_args_t;
                typedef _func_invoker_impl<_R, _FuncT, _Index + 1, _Count> func_invoker;

                func_invoker::call(fx, args, _checked_args<checked_args_t>(), result);
            }

            template<class _RawArgsT>
            static void call(_FuncT& fx, _callable_args<_RawArgsT>& args,
                const _checked_args<_RawArgsT>& checked_args, _return_arg<_R> &result, functional_detail::_any)
            {
                typedef _func_invoker_impl<_R, _FuncT, _Index + 1, _Count> func_invoker;
                func_invoker::call(fx, args, checked_args, result);
            }
        };

        template<class _R, class _FuncT, int _Index, int _Count>
        struct _callable_with_nullptr_tester: _arg<_nullptr_place_holder, 0>
        {
            typedef _arg<_nullptr_place_holder, 0> base_type;

            _callable_with_nullptr_tester():
                base_type(nullptr){}

            template<class _RawArgsT, class _CheckedArgsT>
            void call(_FuncT &fx, _callable_args<_RawArgsT> &args, 
                const _checked_args<_CheckedArgsT>& checked_args, _return_arg<_R> &result)
            {
                typedef _check_args_for_null_impl_helper<
                    _R, _FuncT, _Index, _Count,
                    intern::_has_feature<intern::_stdex_nullptr_implemented_as_distinct_type>::value == bool(true)
                    //|| is_class<_FuncT>::value == bool(false)
                > helper;
                helper::call(fx, args, checked_args, result, base_type::value);                        
            }
        };

        template<class _R, class _FuncT, int _Index, int _Count, bool>
        struct _check_args_for_null_impl
        {
            template<class _RawArgsT, class _CheckedArgsT>
            static void call(_FuncT &fx, _callable_args<_RawArgsT> &args, 
                const _checked_args<_CheckedArgsT>& checked_args, _return_arg<_R> &result)
            {                
                _callable_with_nullptr_tester<_R, _FuncT, _Index, _Count> tt;
                tt.call(fx, args, checked_args, result);
            }
        };

        template<class _R, class _FuncT, int _Index, int _Count>
        struct _check_args_for_null_impl<_R, _FuncT, _Index, _Count, false>
        {
            typedef _func_invoker_impl<_R, _FuncT, _Index + 1, _Count> func_invoker;
            
            template<class _RawArgsT, class _CheckedArgsT>
            static void call(_FuncT &fx, _callable_args<_RawArgsT> &args, 
                const _checked_args<_CheckedArgsT>& checked_args, _return_arg<_R> &result)
            {
                typedef 
                typename _get_args_traits<_RawArgsT, _Index>::value_type value_type;

                typedef _args<_CheckedArgsT, value_type, _Index> checked_args_t;

                func_invoker::call(fx, args, _checked_args<checked_args_t>(), result);
            }

            template<class _RawArgsT>
            static void call(_FuncT &fx, _callable_args<_RawArgsT> &args, 
                const _checked_args<_RawArgsT>& checked_args, _return_arg<_R> &result)
            {
                func_invoker::call(fx, args, checked_args, result);
            }
        };

        template<class _R, class _FuncT, int _Index, int _Count>
        struct _check_args_for_null
        {
            template<class _RawArgsT, class _CheckedArgsT>
            static  void call(_FuncT &fx, _callable_args<_RawArgsT> &args, 
                const _checked_args<_CheckedArgsT>& checked_args, _return_arg<_R> &result)
            {
                typedef 
                typename _get_args_traits<_RawArgsT, _Index>::value_type value_type;

                _check_args_for_null_impl
                <
                    _R, _FuncT, _Index, _Count,
                    is_null_pointer<typename remove_reference<value_type>::type>::value == bool(true)
                    && intern::_has_feature<intern::_stdex_has_native_nullptr>::value == bool(false)
                >::call(fx, args, checked_args, result);
            }
        };


        template<class _R, class _FuncT, int _Index, int _Count>
        struct _func_invoker_impl
        {
            typedef _check_args_for_null<_R, _FuncT, _Index, _Count> null_checker;

            template<class _RawArgsT, class _CheckedArgsT>
            static void call(_FuncT &fx, _callable_args<_RawArgsT> &args, 
                const _checked_args<_CheckedArgsT>& checked_args, _return_arg<_R> &result)
            {
                null_checker::call(fx, args, checked_args, result);
            }
        };

        namespace functional_detail
        {
            template<bool>
            struct _invokable_tag{};
        }

        template<class _FuncT, class _ArgsT, class _R>
        void _invoke_clear(_FuncT &fx, _callable_args<_ArgsT> &args, _return_arg<_R> &result)
        {
            args.call(fx, result, 
                functional_detail::_invokable_tag< is_class<_FuncT>::value == bool(false) >());
        }

        template<class _R, class _FuncT, int _N>
        struct _func_invoker_impl<_R, _FuncT, _N, _N>
        {
            template<class _RawArgsT, class _CheckedArgsT>
            static void call(_FuncT &fx, _callable_args<_RawArgsT> &args, 
                const _checked_args<_CheckedArgsT>&, _return_arg<_R> &result)
            {
                _callable_args<_CheckedArgsT> checked_args(functional_std::move(args));
                _invoke_clear(fx, checked_args, result);
            }

            template<class _RawArgsT>
            static void call(_FuncT &fx, _callable_args<_RawArgsT> &args, 
                const _checked_args<_RawArgsT>&, _return_arg<_R> &result)
            {
                _invoke_clear(fx, args, result);
            }
        };

        template<class _R, class _FuncT, int _Index, int _Count>
        struct _func_invoker
        {
            template<class _RawArgsT>
            static void call(_FuncT &fx, _callable_args<_RawArgsT> &args, _return_arg<_R> &result)
            {
                _func_invoker_impl<_R, _FuncT, _Index, _Count>::
                    call(fx, args, _checked_args<_RawArgsT>(), result);
            }
        };

        template<class _FuncT, class _ArgsT, class _R>
        void _invoke(_FuncT &fx, _callable_args<_ArgsT> &args, _return_arg<_R> &result)
        {
            _func_invoker<_R, _FuncT, 0, _callable_args<_ArgsT>::count>::
                call(fx, args, result);
        }

        template<>
        struct _callable_args<_empty_args> : _empty_args
        {
            typedef _empty_args base_type;
            
            _callable_args(const _empty_args &other)
                : base_type(other) {}

            template<class _R, class _FuncT, class _Invokable>
            void call(_FuncT &fx, _return_arg<_R> &result, _Invokable)
            {result = fx();}

            template<class _FuncT, class _Invokable>
            void call(_FuncT &fx, _return_arg<void> &, _Invokable)
            {fx();}
        };

#define _STDEX_ARG_VALUE(arg_n) _get_args_traits<base_type, arg_n>::arg_type::value

#define _STDEX_CALLABLE_ARGS(arg_n) \
        template<class _ArgsT, class _ArgT> \
        struct _callable_args<_args<_ArgsT, _ArgT, arg_n>/**/> : _args<_ArgsT, _ArgT, arg_n> \
        { \
            typedef _args<_ArgsT, _ArgT, arg_n> base_type; \
        \
            template<class _OtherArgsT, class _OtherArgT> \
            _callable_args(const _args<_OtherArgsT, _OtherArgT, base_type::count - 1> &other) \
                : base_type(other) {} \
        \
            template<class _R, class _FuncT> \
            void call(_FuncT &fx, _return_arg<_R> &result, functional_detail::_invokable_tag<false>) \
            {result = _return_arg<_R>(fx( \
                _STDEX_ARGS##arg_n##_IMPL(_STDEX_BLANK, _STDEX_BLANK, _STDEX_ARG_VALUE) ));} \
        \
            template<class _FuncT> \
            void call(_FuncT &fx, _return_arg<void> &, functional_detail::_invokable_tag<false>) \
            {fx( \
                _STDEX_ARGS##arg_n##_IMPL(_STDEX_BLANK, _STDEX_BLANK, _STDEX_ARG_VALUE) );} \
        \
            template<class _R, class _FuncT> \
            void call(_FuncT &fx, _return_arg<_R> &result, functional_detail::_invokable_tag<true>) \
            {result = _return_arg<_R>(stdex::invoke(fx, \
                _STDEX_ARGS##arg_n##_IMPL(_STDEX_BLANK, _STDEX_BLANK, _STDEX_ARG_VALUE) ));} \
        \
            template<class _FuncT> \
            void call(_FuncT &fx, _return_arg<void> &, functional_detail::_invokable_tag<true>) \
            {stdex::invoke(fx, \
                _STDEX_ARGS##arg_n##_IMPL(_STDEX_BLANK, _STDEX_BLANK, _STDEX_ARG_VALUE) );} \
        };

    _STDEX_CALLABLE_ARGS(0)
    _STDEX_CALLABLE_ARGS(1)
    _STDEX_CALLABLE_ARGS(2)
    _STDEX_CALLABLE_ARGS(3)
    _STDEX_CALLABLE_ARGS(4)
    _STDEX_CALLABLE_ARGS(5)
    _STDEX_CALLABLE_ARGS(6)
    _STDEX_CALLABLE_ARGS(7)
    _STDEX_CALLABLE_ARGS(8)
    _STDEX_CALLABLE_ARGS(9)
    _STDEX_CALLABLE_ARGS(10)
    _STDEX_CALLABLE_ARGS(11)
    _STDEX_CALLABLE_ARGS(12)
    _STDEX_CALLABLE_ARGS(13)
    _STDEX_CALLABLE_ARGS(14)
    _STDEX_CALLABLE_ARGS(15)
    _STDEX_CALLABLE_ARGS(16)
    _STDEX_CALLABLE_ARGS(17)
    _STDEX_CALLABLE_ARGS(18)
    _STDEX_CALLABLE_ARGS(19)
    _STDEX_CALLABLE_ARGS(20)
    _STDEX_CALLABLE_ARGS(21)
    _STDEX_CALLABLE_ARGS(22)
    _STDEX_CALLABLE_ARGS(23)
    _STDEX_CALLABLE_ARGS(24)
    _STDEX_CALLABLE_ARGS(25)
    _STDEX_CALLABLE_ARGS(26)
    _STDEX_CALLABLE_ARGS(27)
    _STDEX_CALLABLE_ARGS(28)
    _STDEX_CALLABLE_ARGS(29)
    _STDEX_CALLABLE_ARGS(30)
    _STDEX_CALLABLE_ARGS(31)

#undef _STDEX_ARG_VALUE
#undef _STDEX_CALLABLE_ARGS

    } // namespace detail

    class bad_function_call : public std::exception { // exception thrown when an empty std::function is called
    public:
        bad_function_call() _STDEX_NOEXCEPT_FUNCTION {}

        virtual const char* what() const _STDEX_NOEXCEPT_FUNCTION {
            // return pointer to message string
            return "bad function call";
        }
    };

    namespace detail
    {
        template<class _FuncT>
        struct _functor_pointer_copy
        {
            typedef
            typename
            conditional<
                is_pointer<_FuncT>::value == bool(true) ||
                is_member_function_pointer<_FuncT>::value == bool(true),
                _FuncT,
                class _disable&
            >::type func_ptr_type;

            typedef
            typename
            conditional<
                is_pointer<_FuncT>::value == bool(false) &&
                is_member_function_pointer<_FuncT>::value == bool(false),
                _FuncT&,
                class _disable
            >::type func_obj_type;

            static std::size_t call(void* &_dst,
                func_ptr_type _src)
            {
                _FuncT *_ptr_to_ptr = new _FuncT(0);
                (*_ptr_to_ptr) = _src;
                _dst = _ptr_to_ptr;
                return sizeof(_FuncT);
            }

            static std::size_t call(void* &_dst,
                func_obj_type _src)
            {
                _FuncT** _ptr_to_ptr = new _FuncT *();
                (*_ptr_to_ptr) = &_src;
                _dst = _ptr_to_ptr;
                return sizeof(_FuncT);
            }

            static std::size_t call(const void* &_dst,
                func_ptr_type _src)
            {
                typedef typename remove_const<_FuncT>::type ctype;
                ctype *_ptr_to_ptr = new ctype(0);
                (*_ptr_to_ptr) = _src;
                _dst = _ptr_to_ptr;
                return sizeof(ctype);
            }

            static std::size_t call(const void* &_dst,
                func_obj_type _src)
            {
                typedef typename add_const<_FuncT>::type ctype;
                ctype** _ptr_to_ptr = new ctype *();
                (*_ptr_to_ptr) = &_src;
                _dst = _ptr_to_ptr;
                return sizeof(ctype);
            }
        };

        template<class _FuncT>
        struct _functor_pointer_copy<_FuncT&>
        {
            static std::size_t call(const void* &_dst,
                _FuncT& _src)
            {
                _dst = &_src;
                return sizeof(_FuncT);
            }

            static std::size_t call(void* &_dst,
                _FuncT& _src)
            {
                _dst = &_src;
                return sizeof(_FuncT);
            }
        };

        template<>
        struct _functor_pointer_copy<void>
        {
            template<class _DstT, class _SrcT> 
            static std::size_t call(const _DstT&, const _SrcT&)
            {
                return 0;
            }
        };

        template<class func_base, class _FuncT, class args_type>
        struct _functor :
            func_base
        {
            typedef _FuncT func_type;
            typedef func_base function_func_base;
            typedef args_type function_args_type;
            typedef typename func_base::_return_type function_return_type;
            typedef _functor type;

            _functor(func_type func) :
                _func(stdex::detail::functional_std::move(func)) {}

            virtual function_func_base* _copy() const { return (new type(_func)); }
            virtual function_func_base* _move() _STDEX_NOEXCEPT_FUNCTION { return (new type(stdex::detail::functional_std::move(_func))); }
            virtual _return_arg<function_return_type> _co_call(function_args_type& args)
            {
                _return_arg<function_return_type> result;
                //return _func_invoker<function_return_type, func_type, 0, function_args_type::count>::call(_func, args);
                _invoke(_func, args, result);

                return result;
            }
            virtual void _delete_this() _STDEX_NOEXCEPT_FUNCTION { delete this; }


            virtual std::size_t _target(void* &_dst) _STDEX_NOEXCEPT_FUNCTION
            {
                typedef typename conditional<is_const<typename remove_reference<func_type>::type>::value,
                    void,
                    func_type>::type ctype;
                return _functor_pointer_copy<ctype>::call(_dst, _func);
            }

            virtual std::size_t _target(const void* &_dst) const _STDEX_NOEXCEPT_FUNCTION
            {
                typedef typename add_const<func_type>::type ctype;
                typedef typename remove_reference<ctype>::type& ctype_ref;
                return _functor_pointer_copy<ctype>::call(_dst, const_cast<ctype_ref>(_func));
            }

            virtual const std::type_info& _target_type() const _STDEX_NOEXCEPT_FUNCTION
            {
                return typeid(typename remove_pointer<func_type>::type);
            }

            func_type _func;
        };

        namespace functional_detail
        {
            template<unsigned _Rank> struct _priority_tag : _priority_tag < _Rank - 1 > {};
            template<> struct _priority_tag<0> {};
        }

        template<
            class _R, 
            _STDEX_TMPL_ARGS_MAX(_STDEX_BLANK, = ::stdex::detail::void_type)
        >
        class _function_impl
        {
            typedef
                typename _make_args::
#undef _STDEX_DELIM
#define _STDEX_DELIM ::
                _STDEX_TYPES_MAX(template add <, >::type)::
#undef _STDEX_DELIM
#define _STDEX_DELIM _STDEX_DELIM_DEFAULT
            args _args_type_raw;

            typedef _callable_args<_args_type_raw> _args_type;

            struct _func_base {
                typedef 
                typename 
                conditional<
                    is_void<_R>::value,
                    void, 
                    _R
                >::type _return_type;

                virtual _func_base* _copy() const = 0;
                virtual _func_base* _move() _STDEX_NOEXCEPT_FUNCTION = 0;
                virtual _return_arg<_return_type> _co_call(_args_type&) = 0;
                virtual void _delete_this() _STDEX_NOEXCEPT_FUNCTION = 0;
                virtual std::size_t _target(void*&) _STDEX_NOEXCEPT_FUNCTION = 0;
                virtual std::size_t _target(const void*&) const _STDEX_NOEXCEPT_FUNCTION = 0;
                virtual const std::type_info& _target_type() const _STDEX_NOEXCEPT_FUNCTION = 0;

                _func_base() {}
                virtual ~_func_base() {}
            private:
                _func_base(const _func_base&) _STDEX_DELETED_FUNCTION;
                _func_base& operator=(const _func_base&) _STDEX_DELETED_FUNCTION;
            };

        protected:
            typedef _R return_type;


            _function_impl() _STDEX_NOEXCEPT_FUNCTION : _fx(nullptr)  {}
            _function_impl(stdex::nullptr_t) _STDEX_NOEXCEPT_FUNCTION : _fx(nullptr)  {}

            _function_impl(const _function_impl& other) :
                _fx(nullptr)
            { 
                if (!other._fx)
                    return;
                _fx = other._fx->_copy();
            }
            //function(function&& other) _STDEX_NOEXCEPT_FUNCTION;


            template<class _FuncT>
            _function_impl(_FuncT *func, 
                functional_detail::_priority_tag<0>)
            {
                _fx = new _functor<_func_base, _FuncT, _args_type>(stdex::detail::functional_std::move(func));
            }

            template<class _FuncT>
            _function_impl(_FuncT &func, 
                functional_detail::_priority_tag<1>)
            {
                _fx = new _functor<_func_base, _FuncT, _args_type>(stdex::detail::functional_std::move(func));
            }

            template<class _FuncT>
            _function_impl(reference_wrapper<_FuncT> func,
                functional_detail::_priority_tag<2>)
            {
                _fx = new _functor<_func_base, _FuncT, _args_type>(stdex::detail::functional_std::move(func.get()));
            }

            ~_function_impl()
            {
                delete _fx;
            }

            _return_arg<return_type>
                operator()(
                _STDEX_PARAMS_MAX(_STDEX_BLANK, _STDEX_BLANK, _STDEX_BLANK, = void_type())
                    ) const
            {
                if (!_fx)
                    throw(bad_function_call());             

                using stdex::detail::functional_std::move;

                _args_type args =
                    _args_type_raw( 
#define _STDEX_ARG_CUSTOM(arg_n) \
    _arg<_STDEX_TYPE_DEFAULT(arg_n), arg_n> \
        ( _STDEX_ARG_DEFAULT(arg_n) )
                    _STDEX_ARGS_MAX_IMPL(_STDEX_BLANK, _STDEX_BLANK, _STDEX_ARG_CUSTOM)
#undef _STDEX_ARG_CUSTOM
                    );

                return 
                    _fx->_co_call(move(args));
            }

            void swap(_function_impl& other) _STDEX_NOEXCEPT_FUNCTION
            {
                using std::swap;
                swap(_fx, other._fx);
            }

            operator bool() const _STDEX_NOEXCEPT_FUNCTION
            {
                return _fx != nullptr;
            }

            const std::type_info& target_type() const _STDEX_NOEXCEPT_FUNCTION
            {
                if(_fx)
                    return _fx->_target_type();
                return typeid(void);
            }

            template<class _FuncT>
            _FuncT* target() _STDEX_NOEXCEPT_FUNCTION
            {
                if (_fx && typeid(_FuncT) == target_type())
                {
                    typename 
                    conditional<
                        is_const<_FuncT>::value, const void*, void*>::type _ptr_to_ptr = 0;
                    std::size_t _size = _fx->_target(_ptr_to_ptr);

                    if (_ptr_to_ptr && _size == sizeof(_FuncT*))
                    {
                        _FuncT *_result = *static_cast<_FuncT**>(_ptr_to_ptr);
                        
                        return _result;
                    }
                }

                return 0;
            }

        private:


            _func_base* _fx;
        };

    } // namespace detail

    namespace detail
    {
        template<class _Tp>
        class _reference_wrapper_impl
        {
            explicit _reference_wrapper_impl(_Tp& ref) _STDEX_NOEXCEPT_FUNCTION :
                _ptr(addressof(ref))
            { }

        protected:
            _Tp* _ptr;

        };

        template<class _R>
        class _reference_wrapper_impl<_R(&)()>
        {
            typedef _R(type)();

            explicit _reference_wrapper_impl(type& ref) _STDEX_NOEXCEPT_FUNCTION :
                _ptr(addressof(ref))
            { }

        protected:
            type* _ptr;

        public:
            _R operator()() { return invoke(*_ptr); }
        };
    }
    template<class _Tp>
    class reference_wrapper
    {
    public:
        typedef intern::functional_asserts check;
        typedef typename check::reference_wrapper_requires_T_to_be_an_object_type_or_a_function_type_assert<is_object<_Tp>::value == bool(true) || is_function<_Tp>::value == bool(true)>::
            reference_wrapper_requires_T_to_be_an_object_type_or_a_function_type_assert_failed
            check1; // if you are there means you tried to use reference_wrapper for unsupported type

        typedef _Tp type;

        explicit reference_wrapper(type &ref) _STDEX_NOEXCEPT_FUNCTION:
            _ptr( addressof(ref) )
        { }

        operator _Tp&() const _STDEX_NOEXCEPT_FUNCTION {
            return *_ptr;
        }

        _Tp& get() const _STDEX_NOEXCEPT_FUNCTION {
            return *_ptr;
        }

    private:
        _Tp* _ptr;

        //reference_wrapper(type&&) _STDEX_DELETED_FUNCTION;
    };

    


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

        void swap(function& other) _STDEX_NOEXCEPT_FUNCTION
        {
            base_type::swap(other);
        }

        operator bool() const _STDEX_NOEXCEPT_FUNCTION
        {
            return static_cast<const base_type&>(*this);
        }

        const std::type_info& target_type() const _STDEX_NOEXCEPT_FUNCTION
        {
            return base_type::target_type();
        }

        template<class _FuncT>
        _FuncT* target() _STDEX_NOEXCEPT_FUNCTION
        {
            return base_type::template target<_FuncT>();
        }

        template<class _FuncT>
        const _FuncT* target() const _STDEX_NOEXCEPT_FUNCTION
        {
            return base_type::template target<_FuncT>();
        }
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

        void swap(function& other) _STDEX_NOEXCEPT_FUNCTION
        {
            base_type::swap(other);
        }

        operator bool() const _STDEX_NOEXCEPT_FUNCTION
        {
            return static_cast<const base_type&>(*this);
        }

        const std::type_info& target_type() const _STDEX_NOEXCEPT_FUNCTION
        {
            return base_type::target_type();
        }

        template<class _FuncT>
        _FuncT* target() _STDEX_NOEXCEPT_FUNCTION
        {
            return base_type::template target<_FuncT>();
        }

        template<class _FuncT>
        const _FuncT* target() const _STDEX_NOEXCEPT_FUNCTION
        {
            return base_type::template target<_FuncT>();
        }
    };

    template<class _R>
    class function<_R(*)()>:
        detail::_function_impl<_R>
    {
        typedef detail::_function_impl<_R> base_type;
        typedef typename stdex::remove_pointer<_R(*)()>::type function_signature;
    public:
        typedef typename base_type::return_type return_type;

        function() _STDEX_NOEXCEPT_FUNCTION : base_type()  {}
        function(stdex::nullptr_t) _STDEX_NOEXCEPT_FUNCTION : base_type(nullptr)  {}

        function(const function& other) : base_type(other) { }
        //function(function&& other) _STDEX_NOEXCEPT_FUNCTION;

        template<class _FuncT>
        function(_FuncT func): base_type(func, detail::functional_detail::_priority_tag<8>()) { }

        return_type operator()() const {
            return 
            detail::functional_std::_forward<return_type>::call(
                base_type::operator()().get() ); 
        }

        void swap(function& other) _STDEX_NOEXCEPT_FUNCTION
        {
            base_type::swap(other);
        }

        operator bool() const _STDEX_NOEXCEPT_FUNCTION
        {
            return static_cast<const base_type&>(*this);
        }

        const std::type_info& target_type() const _STDEX_NOEXCEPT_FUNCTION
        {
            return base_type::target_type();
        }

        template<class _FuncT>
        _FuncT* target() _STDEX_NOEXCEPT_FUNCTION
        {
            return base_type::template target<_FuncT>();
        }

        template<class _FuncT>
        const _FuncT* target() const _STDEX_NOEXCEPT_FUNCTION
        {
            return base_type::template target<_FuncT>();
        }
    };

    namespace detail
    {
        template<class _Tp, bool, bool>
        struct _reference_or_pointer_impl
        { 
            typedef _Tp type;
        };

        template<class _Tp>
        struct _reference_or_pointer_impl<_Tp, false, false>
        {
            typedef _Tp& type;
        };

        template<class _Tp>
        struct _reference_or_pointer:
            _reference_or_pointer_impl<_Tp, is_pointer<_Tp>::value, is_reference<_Tp>::value>
        { };
    }

#define _STDEX_FUNCTION_IMPL(count) \
    template<class _R,  \
        _STDEX_TMPL_ARGS##count(_STDEX_BLANK, _STDEX_BLANK) \
    > \
    class function<_R(*)( \
        _STDEX_TYPES##count(_STDEX_BLANK, _STDEX_BLANK) \
        )>: \
        detail::_function_impl<_R,  \
            _STDEX_TYPES##count(typename detail::_reference_or_pointer <, >::type) \
        > \
    { \
        typedef  \
        detail::_function_impl<_R, \
            _STDEX_TYPES##count(typename detail::_reference_or_pointer <, >::type) \
        > base_type; \
 \
        typedef typename stdex::remove_pointer< \
            _R(*)( _STDEX_TYPES##count(_STDEX_BLANK, _STDEX_BLANK) ) \
        >::type function_signature; \
    public: \
        typedef typename base_type::return_type return_type; \
 \
        function() _STDEX_NOEXCEPT_FUNCTION : base_type()  {} \
        function(stdex::nullptr_t) _STDEX_NOEXCEPT_FUNCTION : base_type(nullptr)  {} \
 \
        function(const function& other) : base_type(other) { } \
        /*function(function&& other) _STDEX_NOEXCEPT_FUNCTION;*/ \
 \
 \
        template<class _FuncT> \
        function(_FuncT func): base_type(func, detail::functional_detail::_priority_tag<8>()) { } \
 \
        return_type operator()( \
            _STDEX_PARAMS##count(_STDEX_BLANK, _STDEX_BLANK, _STDEX_BLANK, _STDEX_BLANK) \
            ) const {  \
            return  \
            detail::functional_std::_forward<return_type>::call( \
                base_type::operator()( \
                    _STDEX_ARGS##count(_STDEX_BLANK, _STDEX_BLANK) \
                    ).get() ); \
        } \
 \
        void swap(function& other) _STDEX_NOEXCEPT_FUNCTION \
        { \
            base_type::swap(other); \
        } \
 \
        operator bool() const _STDEX_NOEXCEPT_FUNCTION \
        { \
            return static_cast<const base_type&>(*this); \
        } \
 \
        const std::type_info& target_type() const _STDEX_NOEXCEPT_FUNCTION \
        { \
            return base_type::target_type(); \
        } \
 \
        template<class _FuncT> \
        _FuncT* target() _STDEX_NOEXCEPT_FUNCTION \
        { \
            return base_type::template target<_FuncT>(); \
        } \
 \
        template<class _FuncT> \
        const _FuncT* target() const _STDEX_NOEXCEPT_FUNCTION \
        { \
            return base_type::template target<_FuncT>(); \
        } \
    };

    template<class _ObjectT, class _FuncT>
    class member_function:
        public member_function<_ObjectT, _FuncT*>
    {
        typedef typename member_function<_ObjectT, _FuncT*>::pointer_type pointer_type;
        typedef member_function<_ObjectT, _FuncT*> base_type;
    public:
        explicit member_function(pointer_type mem_ptr) _STDEX_NOEXCEPT_FUNCTION:
            base_type(mem_ptr) {}      
    };

#define _STDEX_MEM_FN(count) \
    template<class _ObjectT, class _R, _STDEX_TMPL_ARGS##count(_STDEX_BLANK, _STDEX_BLANK)> \
    class member_function<_ObjectT, _R(*)(_STDEX_TYPES##count(_STDEX_BLANK, _STDEX_BLANK))> \
    { \
    protected: \
        typedef _R(_ObjectT::*pointer_type)(_STDEX_TYPES##count(_STDEX_BLANK, _STDEX_BLANK)); \
        typedef _R return_type; \
\
        pointer_type _mem_ptr; \
    public: \
        explicit member_function(pointer_type mem_ptr) _STDEX_NOEXCEPT_FUNCTION: \
            _mem_ptr(mem_ptr) {}\
\
        return_type operator()(_ObjectT& _obj, \
            _STDEX_PARAMS##count(_STDEX_BLANK, _STDEX_BLANK, _STDEX_BLANK, _STDEX_BLANK) \
            ) const {  \
            return  \
            stdex::invoke(\
                    _mem_ptr, _obj, \
                    _STDEX_ARGS##count(_STDEX_BLANK, _STDEX_BLANK) \
                    ); \
        } \
\
        return_type operator()(const reference_wrapper<_ObjectT>& _obj, \
            _STDEX_PARAMS##count(_STDEX_BLANK, _STDEX_BLANK, _STDEX_BLANK, _STDEX_BLANK) \
            ) const {  \
            return  \
            stdex::invoke(\
                    _mem_ptr, _obj, \
                    _STDEX_ARGS##count(_STDEX_BLANK, _STDEX_BLANK) \
                    ); \
        } \
\
        return_type operator()(_ObjectT* _obj_ptr, \
            _STDEX_PARAMS##count(_STDEX_BLANK, _STDEX_BLANK, _STDEX_BLANK, _STDEX_BLANK) \
            ) const {  \
            return (*this)(*_obj_ptr, _STDEX_ARGS##count(_STDEX_BLANK, _STDEX_BLANK)); }\
    }; \
\
    template<class _ObjectT, class _R, _STDEX_TMPL_ARGS##count(_STDEX_BLANK, _STDEX_BLANK)> \
    class member_function<const _ObjectT, _R(*)(_STDEX_TYPES##count(_STDEX_BLANK, _STDEX_BLANK))> \
    { \
    protected: \
        typedef _R(_ObjectT::*pointer_type)(_STDEX_TYPES##count(_STDEX_BLANK, _STDEX_BLANK)) const; \
        typedef _R return_type; \
\
        pointer_type _mem_ptr; \
    public: \
        explicit member_function(pointer_type mem_ptr) _STDEX_NOEXCEPT_FUNCTION: \
            _mem_ptr(mem_ptr) {}\
\
        return_type operator()(const _ObjectT& _obj, \
            _STDEX_PARAMS##count(_STDEX_BLANK, _STDEX_BLANK, _STDEX_BLANK, _STDEX_BLANK) \
            ) const {  \
            return  \
            stdex::invoke(\
                    _mem_ptr, _obj, \
                    _STDEX_ARGS##count(_STDEX_BLANK, _STDEX_BLANK) \
                    ); \
        } \
\
        return_type operator()(const reference_wrapper<const _ObjectT>& _obj, \
            _STDEX_PARAMS##count(_STDEX_BLANK, _STDEX_BLANK, _STDEX_BLANK, _STDEX_BLANK) \
            ) const {  \
            return  \
            stdex::invoke(\
                    _mem_ptr, _obj, \
                    _STDEX_ARGS##count(_STDEX_BLANK, _STDEX_BLANK) \
                    ); \
        } \
\
        return_type operator()(const _ObjectT* _obj_ptr, \
            _STDEX_PARAMS##count(_STDEX_BLANK, _STDEX_BLANK, _STDEX_BLANK, _STDEX_BLANK) \
            ) const {  \
            return (*this)(*_obj_ptr, _STDEX_ARGS##count(_STDEX_BLANK, _STDEX_BLANK)); }\
    };

#define _STDEX_FUNCTION(count) \
    _STDEX_FUNCTION_IMPL(count)\
    _STDEX_MEM_FN(count)

    _STDEX_FUNCTION(0)
    _STDEX_FUNCTION(1)
    _STDEX_FUNCTION(2)
    _STDEX_FUNCTION(3)
    _STDEX_FUNCTION(4)
    _STDEX_FUNCTION(5)
    _STDEX_FUNCTION(6)
    _STDEX_FUNCTION(7)
    _STDEX_FUNCTION(8)
    _STDEX_FUNCTION(9)
    _STDEX_FUNCTION(10)
    _STDEX_FUNCTION(11)
    _STDEX_FUNCTION(12)
    _STDEX_FUNCTION(13)
    _STDEX_FUNCTION(14)
    _STDEX_FUNCTION(15)
    _STDEX_FUNCTION(16)
    _STDEX_FUNCTION(17)
    _STDEX_FUNCTION(18)
    _STDEX_FUNCTION(19)
    _STDEX_FUNCTION(20)
    _STDEX_FUNCTION(21)
    _STDEX_FUNCTION(22)
    _STDEX_FUNCTION(23)
    _STDEX_FUNCTION(24)
    _STDEX_FUNCTION(25)
    _STDEX_FUNCTION(26)
    _STDEX_FUNCTION(27)
    _STDEX_FUNCTION(28)
    _STDEX_FUNCTION(29)
    _STDEX_FUNCTION(30)
    _STDEX_FUNCTION(31)

#undef _STDEX_FUNCTION 
#undef _STDEX_FUNCTION_IMPL
#undef _STDEX_MEM_FN

    // Bind

    template<class>
    struct is_placeholder
        : integral_constant<int, 0>
    { };

    template<class>
    struct is_bind_expression
        : false_type
    { };

    namespace detail
    {
        template<class _ReplArgsT, class, int _PhIndex>
        struct _replace_ph_args_helper_impl
        {
            typedef
            typename _get_args_traits<_ReplArgsT, _PhIndex - 1>::arg_type type;
        };

        template<class _ReplArgsT, class _ArgT>
        struct _replace_ph_args_helper_impl<_ReplArgsT, _ArgT, 0>
        {
            typedef _ArgT type;
        };

        template<class _ArgsT, class _ReplArgsT, int _Index>
        struct _replace_ph_args_helper
        {
            typedef
            typename _get_args_traits<_ArgsT, _Index>::arg_type _arg_type;

            typedef
            typename _replace_ph_args_helper_impl<
                _ReplArgsT, 
                _arg_type, 
                is_placeholder<typename _arg_type::value_type>::value
            >::type _result_arg_type;

            typedef typename _result_arg_type::value_type type;
        };

        template<class _ArgsT, class _ReplArgsT, int _Index, class _ResultArgsT>
        struct _replace_ph_args;

        template<class _ArgsT, class _ReplArgsT, int _Index, class _ResultArgsT>
        struct _replace_ph_args2:
            _replace_ph_args<_ArgsT, _ReplArgsT, _Index + 1, _ResultArgsT>{};

        template<
            class _ArgsT, class _ArgT, int _ArgsCount, 
            class _ReplArgsT, class _ReplArgT, int _ReplArgsCount, 
            int _Index, class _ResultArgsT>
        struct _replace_ph_args<
            _args<_ArgsT, _ArgT, _ArgsCount>, 
            _args<_ReplArgsT, _ReplArgT, _ReplArgsCount>, 
            _Index, _ResultArgsT>
            : _replace_ph_args2<
                _args<_ArgsT, _ArgT, _ArgsCount>, 
                _args<_ReplArgsT, _ReplArgT, _ReplArgsCount>, 
                _Index, 
                _args<
                    _ResultArgsT, 
                    typename 
                    _replace_ph_args_helper<
                        _args<_ArgsT, _ArgT, _ArgsCount>,
                        _args<_ReplArgsT, _ReplArgT, _ReplArgsCount>,
                        _Index
                    >::type,
                    _Index
                >
            >
        {
        };

        template<class _ArgsT, class _ArgT, int _Index, class _ReplArgsT, class _ResultArgsT>
        struct _replace_ph_args2<
            _args<_ArgsT, _ArgT, _Index>, _ReplArgsT, _Index, _ResultArgsT>
        {
            typedef
            _ResultArgsT type;
        };

        template<class>
        struct _ph_max_index;
        
        template<class _Tp>
        struct _ph_max_index_impl
        {
            template<class _NextTp>
            struct get
            {
                typedef _Tp prev_type;
                typedef 
                typename
                conditional<
                    (is_placeholder<prev_type>::value < is_placeholder<_NextTp>::value),
                    _ph_max_index<_NextTp>,
                    _ph_max_index<prev_type>
                >::type type;
            };
        };

        template<class _Tp>
        struct _ph_max_index:
            is_placeholder<_Tp>,
            _ph_max_index_impl<_Tp>
        { 

        };

        template<
            _STDEX_TMPL_ARGS_MAX(_STDEX_BLANK, = ::stdex::detail::void_type)
        >
        struct _binder_traits
        {
            typedef
            typename detail::_make_args::
#undef _STDEX_DELIM
#define _STDEX_DELIM ::
            _STDEX_TYPES_MAX(template add <, >::type)::
#undef _STDEX_DELIM
#define _STDEX_DELIM _STDEX_DELIM_DEFAULT
            args args_type;

            static const std::size_t ph_max_index = 
            detail::_ph_max_index<void>::
#undef _STDEX_DELIM
#define _STDEX_DELIM ::
            _STDEX_TYPES_MAX(template get <, >::type)::
#undef _STDEX_DELIM
#define _STDEX_DELIM _STDEX_DELIM_DEFAULT
            value;
        };

        template<class _ArgsT, class _MissingArgsT, int _N,
            int _IsPh>
        struct _get_ph_args_impl
        {
            typedef
            typename 
            stdex::detail::_get_args_traits<_MissingArgsT, _IsPh - 1>::arg_type arg_type;

            template<class _ArgT>
            static arg_type &call(const _ArgT &, arg_type &missing_arg)
            {
                return missing_arg;
            }
        };

        template<class _ArgsT, class _MissingArgsT, int _N>
        struct _get_ph_args_impl<_ArgsT, _MissingArgsT, _N, 0>
        {
            typedef
            typename 
            _get_args_traits<_ArgsT, _N>::arg_type arg_type;

            template<class _MissingArgT>
            static arg_type &call(arg_type &arg, const _MissingArgT &)
            {
                return arg;
            }
        };

        template<class _ArgsT, class _MissingArgsT, int _N>
        struct _get_ph_args_impl<_ArgsT, _MissingArgsT, _N, -1>
        {
            template<class _MissingArgT>
            static void_type call(_ArgsT &, const _MissingArgT &)
            {
                return void_type();
            }
        };

        template<class _ArgsT, int _N, bool>
        struct _get_ph_args_helper_impl:
            integral_constant<int, -1>
        { };

        template<class _ArgsT, int _N>
        struct _get_ph_args_helper_impl<_ArgsT, _N, true>:
            is_placeholder<
                typename _get_args_traits<_ArgsT, _N>::value_type>
        { };

        template<class _ArgsT, int _N>
        struct _get_ph_args_helper:
            _get_ph_args_helper_impl<_ArgsT, _N, (_ArgsT::count > _N)>
        { };

        template<class _ArgsT, class _MissingArgsT, int _N>
        struct _get_ph_args:
            _get_ph_args_impl<_ArgsT, _MissingArgsT, _N,
                _get_ph_args_helper<
                    _ArgsT, _N>::value>
        { };

        template<class _R>
        inline
        _R& _get_return(_return_arg<_R> &arg)
        {
            return arg.get();
        }

        template<class _R>
        inline
        _R& _get_return(_return_arg<_R&> &arg)
        {
            return arg.get();
        }

        inline
        void _get_return(_return_arg<void>&)
        {
        }

        template<
            class _R,
            class _FuncT,
            class _TraitsT,
            std::size_t _PhMaxIndex
        >
        class _binder_impl;

        namespace functional_detail
        {
            struct _any_type: void_type
            {
                template<class _Tp>
                _any_type(const _Tp&){}
            };
        } // namespace tuple_detail

        template<class _FuncT, class _ArgsT, class _R>
        _R _get_bind_expression_impl(_FuncT &fx, _callable_args<_ArgsT> &args,
            typename
            conditional<
                is_bind_expression<_FuncT>::value,
                functional_detail::_priority_tag<1>, 
                functional_detail::_priority_tag<0>/**/>::type)
        {
            _return_arg<_R> result;

            _invoke(fx, args, result);

            return _get_return(result);
        }

        template<class _ArgsT, class _R>
        _R& _get_bind_expression_impl(_R &result, _callable_args<_ArgsT> &,
            typename
            conditional<
                is_bind_expression<_R>::value,
                functional_detail::_priority_tag<0>, 
                functional_detail::_priority_tag<1>/**/>::type)
        {
            return result;
        }

        template<class _FuncT, class _ArgsT, class _R>
        _R _get_bind_expression(_FuncT &fx, _callable_args<_ArgsT> &args)
        {
            return _get_bind_expression_impl<_R>(fx, args, functional_detail::_priority_tag<2>());
        }

#define _STDEX_TYPE_MISSING(arg_n) _MissingArg##arg_n##T
#define _STDEX_TYPE_DISABLED(arg_n) ::stdex::detail::functional_detail::_any_type
#define _STDEX_ARG_DISABLED(arg_n) disabled_arg##arg_n
#define _STDEX_MERGE_ARGS(arg_n) detail::_get_ph_args<_ArgsT, missing_args_type, arg_n>::call(args, missing_args)
#define _STDEX_MERGE_BIND_EXPR_ARGS(arg_n) \
            _get_bind_expression< \
            typename \
            _get_args_traits<_ArgsT, arg_n>::value_type>( _STDEX_MERGE_ARGS(arg_n), missing_args )

        template<
            class _R,
            class _FuncT,
            class _TraitsT
        >
        class _binder_impl<_R, _FuncT, _TraitsT, std::size_t(-1)>
        {
            typedef typename _TraitsT::args_type _ArgsT;

        protected:
            _binder_impl(
                _FuncT &fx_,
                _ArgsT &args_
            )
            : args( args_ )
            , fx( fx_ )
            { }

        public:
            template<_STDEX_TMPL_ARGS0_IMPL(_STDEX_BLANK, _STDEX_BLANK, _STDEX_TYPE_MISSING)>
            _R operator()(
                _STDEX_PARAMS0_IMPL(_STDEX_BLANK, _STDEX_BLANK, _STDEX_BLANK, _STDEX_BLANK, _STDEX_TYPE_MISSING, _STDEX_ARG_DEFAULT)
            )
            {
                typedef 
                typename
                detail::_binder_traits<
                    _STDEX_TYPES0_IMPL(_STDEX_BLANK, _STDEX_BLANK, _STDEX_TYPE_MISSING)
                >::args_type missing_args_type; 

                missing_args_type missing_args = missing_args_type(
                    _STDEX_ARGS0(_STDEX_BLANK, _STDEX_BLANK)
                    );

                typedef
                typename
                detail::_replace_ph_args<_ArgsT, missing_args_type, 0, void>::type result_args;

                detail::_callable_args<result_args> callable_args = 
                    result_args(
                        _STDEX_ARGS_MAX_IMPL(_STDEX_BLANK, _STDEX_BLANK, _STDEX_MERGE_BIND_EXPR_ARGS)
                    );
                
                detail::_return_arg<_R> result;

                detail::_invoke(fx, callable_args, result);

                return detail::_get_return(result);
            }

        private:
            _ArgsT &args;
            _FuncT &fx;
        };

#undef _STDEX_MERGE_BIND_EXPR_ARGS

        template<
            class _R,
            class _FuncT,
            class _TraitsT
        >
        class _binder_impl< _R, _FuncT, _TraitsT, 0>
        {
            typedef typename _TraitsT::args_type _ArgsT;
            typedef _R return_type;

        protected:
            _binder_impl(
                _FuncT &fx_,
                _ArgsT &args_
            )
            : args( args_ )
            , fx( fx_ )
            { }

        public:
            return_type operator()(
                _STDEX_PARAMS_MAX_IMPL(_STDEX_BLANK, _STDEX_BLANK, _STDEX_BLANK, = ::stdex::detail::void_type(), _STDEX_TYPE_DISABLED, _STDEX_ARG_DISABLED)
            )
            {
                
                detail::_return_arg<return_type> result;

                detail::_invoke(fx, args, result);

                return detail::_get_return(result);
            }

        private:
            detail::_callable_args<_ArgsT> args;
            _FuncT &fx;
        };

        template<
            class _R,
            class _FuncT,
            class _TraitsT
        >
        class _binder_impl<_R, _FuncT, _TraitsT, 1>
        {
            typedef typename _TraitsT::args_type _ArgsT;
            typedef _R return_type;

        protected:
            _binder_impl(
                _FuncT &fx_,
                _ArgsT &args_
            )
            : args( args_ )
            , fx( fx_ )
            { }

        public:
            template<_STDEX_TMPL_ARGS0_IMPL(_STDEX_BLANK, _STDEX_BLANK, _STDEX_TYPE_MISSING)>
            return_type operator()(
                _STDEX_PARAMS0_IMPL(_STDEX_BLANK, _STDEX_BLANK, _STDEX_BLANK, _STDEX_BLANK, _STDEX_TYPE_MISSING, _STDEX_ARG_DEFAULT),
                _STDEX_PARAMS30_IMPL(_STDEX_BLANK, _STDEX_BLANK, _STDEX_BLANK, = ::stdex::detail::void_type(), _STDEX_TYPE_DISABLED, _STDEX_ARG_DISABLED)
            )
            {
                typedef 
                typename
                detail::_binder_traits<
                    _STDEX_TYPES0_IMPL(_STDEX_BLANK, _STDEX_BLANK, _STDEX_TYPE_MISSING)
                >::args_type missing_args_type; 

                missing_args_type missing_args = missing_args_type(
                    _STDEX_ARGS0(_STDEX_BLANK, _STDEX_BLANK)
                    );

                typedef
                typename
                detail::_replace_ph_args<_ArgsT, missing_args_type, 0, void>::type result_args;

                detail::_callable_args<result_args> callable_args = 
                    result_args(
                        _STDEX_ARGS_MAX_IMPL(_STDEX_BLANK, _STDEX_BLANK, _STDEX_MERGE_ARGS)
                    );
                
                detail::_return_arg<return_type> result;

                detail::_invoke(fx, callable_args, result);

                return detail::_get_return(result);
            }

        private:
            _ArgsT &args;
            _FuncT &fx;
        };

#undef _STDEX_TYPE_MISSING
#undef _STDEX_TYPE_DISABLED
#undef _STDEX_ARG_DISABLED

    } // namespace detail

#define _STDEX_ARG_CUSTOM(arg_n) ::stdex::detail::_arg<_STDEX_TYPE_DEFAULT(arg_n), arg_n>( _STDEX_ARG_DEFAULT(arg_n) )
    template<
        class _R,
        class _FuncT,
        _STDEX_TMPL_ARGS_MAX(_STDEX_BLANK, = ::stdex::detail::void_type)
    >
    class binder:
        public detail::_binder_impl<
            _R, 
            typename decay<_FuncT>::type,
            ::stdex::detail::_binder_traits<_STDEX_TYPES_MAX(_STDEX_BLANK, _STDEX_BLANK)>,
            ::stdex::detail::_binder_traits<_STDEX_TYPES_MAX(_STDEX_BLANK, _STDEX_BLANK)>::ph_max_index>
    { 
        typedef detail::_binder_traits<_STDEX_TYPES_MAX(_STDEX_BLANK, _STDEX_BLANK)> traits;

        typedef 
        typename
        traits::args_type args_type;

        typedef typename decay<_FuncT>::type func_type;

        typedef 
        detail::_binder_impl<_R, func_type,
            traits,
            traits::ph_max_index> base_type;

    public:
        binder(
            func_type fx_,
            _STDEX_PARAMS_MAX(_STDEX_BLANK, _STDEX_BLANK, _STDEX_BLANK, = ::stdex::detail::void_type())
        )
        : base_type(fx, args) 
        , args( _STDEX_ARGS_MAX_IMPL(_STDEX_BLANK, _STDEX_BLANK, _STDEX_ARG_CUSTOM) )
        , fx( fx_ )
        { }

    private:
        args_type args;
        func_type fx;
    };

#undef _STDEX_ARG_CUSTOM

    template<
        class _R,
        class _FuncT,
        _STDEX_TMPL_ARGS_MAX(_STDEX_BLANK, _STDEX_BLANK)
    >
    struct is_bind_expression<
        binder<_R, _FuncT, _STDEX_TYPES_MAX(_STDEX_BLANK, _STDEX_BLANK)>
    > : true_type
    { };

    namespace detail
    {
        template<class _FuncT, bool>
        struct _function_return_impl
        {
            typedef typename function<_FuncT>::return_type type;
        };

        template<class _FuncT>
        struct _function_return_impl<_FuncT, false>
        {
            typedef void type;
        };
        template<class _FuncT>
        struct _function_return:
            _function_return_impl<
                _FuncT,
                is_function<typename remove_pointer<_FuncT>::type>::value
            >
        { };

    } // namespace detail

    template<class _FuncT>
    binder<typename detail::_function_return<_FuncT>::type, _FuncT>
    bind(_FuncT fx)
    {
        return binder<typename detail::_function_return<_FuncT>::type, _FuncT>(fx);
    }

    template<class _R, class _FuncT>
    binder<_R, _FuncT>
    bind(_FuncT fx)
    {
        return binder<_R, _FuncT>(fx);
    }

    template<class _FuncT, class _Arg0T>
    binder<typename detail::_function_return<_FuncT>::type, _FuncT, _Arg0T>
    bind(_FuncT fx, _Arg0T arg0)
    {
        return binder<typename detail::_function_return<_FuncT>::type, _FuncT, _Arg0T>(fx, arg0);
    }

    template<class _R, class _FuncT, class _Arg0T>
    binder<_R, _FuncT, _Arg0T>
    bind(_FuncT fx, _Arg0T arg0)
    {
        return binder<_R, _FuncT, _Arg0T>(fx, arg0);
    }

    template<class _FuncT, class _Arg0T, class _Arg1T>
    binder<typename detail::_function_return<_FuncT>::type, _FuncT, _Arg0T, _Arg1T>
    bind(_FuncT fx, _Arg0T arg0, _Arg1T arg1)
    {
        return binder<typename detail::_function_return<_FuncT>::type, _FuncT, _Arg0T, _Arg1T>(fx, arg0, arg1);
    }

    template<class _R, class _FuncT, class _Arg0T, class _Arg1T>
    binder<_R, _FuncT, _Arg0T, _Arg1T> 
    bind(_FuncT fx, _Arg0T arg0, _Arg1T arg1)
    {
        return binder<_R, _FuncT, _Arg0T, _Arg1T>(fx, arg0, arg1);
    }

    // Hashing



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
            tr_argument_type _tmp(_keyval.size());

            struct lambdas{
                static unsigned char bool_to_uchar(bool value){
                    return (value ? 1 : 0);
                }
            };

            std::transform(_keyval.begin(), _keyval.end(), _tmp.begin(), 
                &lambdas::bool_to_uchar);
            
            return hash<tr_argument_type>()(_tmp);
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
    
    template<std::size_t _Bits>
    struct hash<std::bitset<_Bits>/**/>
    {    // hash functor for bitset<_Bits>
        typedef std::bitset<_Bits> argument_type;
        typedef std::size_t result_type;
    
        std::size_t operator()(const argument_type& _keyval) const
        {
            
            stringstream _tmp; _tmp << _keyval;
            return hash<std::string>()(_tmp.str());
        }
    };

    // functions

    template<class _Tp>
    stdex::reference_wrapper<_Tp> ref(_Tp& _ref) _STDEX_NOEXCEPT_FUNCTION
    {
        return stdex::reference_wrapper<_Tp>(_ref);
    }

    template<class _Tp>
    stdex::reference_wrapper<_Tp> ref(stdex::reference_wrapper<_Tp> _ref) _STDEX_NOEXCEPT_FUNCTION
    {
        return _ref;
    }

    template<class _Tp>
    stdex::reference_wrapper<const _Tp> cref(const _Tp& _ref) _STDEX_NOEXCEPT_FUNCTION
    {
        return stdex::reference_wrapper<const _Tp>(_ref);
    }

    template<class _Tp>
    stdex::reference_wrapper<const _Tp> cref(stdex::reference_wrapper<_Tp> _ref) _STDEX_NOEXCEPT_FUNCTION
    {
        return stdex::reference_wrapper<const _Tp>(_ref.get());
    }

    namespace placeholders
    {
        namespace detail
        {
            template<int _N>
            struct _ph
            {	// placeholder
                static const _ph<_N> &instance(){static _ph<_N> inst; return inst;}
            };
        }

        static const detail::_ph<1>  &_1  = detail::_ph<1 >::instance();
        static const detail::_ph<2>  &_2  = detail::_ph<2 >::instance();
        static const detail::_ph<3>  &_3  = detail::_ph<3 >::instance();
        static const detail::_ph<4>  &_4  = detail::_ph<4 >::instance();
        static const detail::_ph<5>  &_5  = detail::_ph<5 >::instance();
        static const detail::_ph<6>  &_6  = detail::_ph<6 >::instance();
        static const detail::_ph<7>  &_7  = detail::_ph<7 >::instance();
        static const detail::_ph<8>  &_8  = detail::_ph<8 >::instance();
        static const detail::_ph<9>  &_9  = detail::_ph<9 >::instance();
        static const detail::_ph<10> &_10 = detail::_ph<10>::instance();
        static const detail::_ph<11> &_11 = detail::_ph<11>::instance();
        static const detail::_ph<12> &_12 = detail::_ph<12>::instance();
        static const detail::_ph<13> &_13 = detail::_ph<13>::instance();
        static const detail::_ph<14> &_14 = detail::_ph<14>::instance();
        static const detail::_ph<15> &_15 = detail::_ph<15>::instance();
        static const detail::_ph<16> &_16 = detail::_ph<16>::instance();
        static const detail::_ph<17> &_17 = detail::_ph<17>::instance();
        static const detail::_ph<18> &_18 = detail::_ph<18>::instance();
        static const detail::_ph<19> &_19 = detail::_ph<19>::instance();
        static const detail::_ph<20> &_20 = detail::_ph<20>::instance();
        static const detail::_ph<21> &_21 = detail::_ph<21>::instance();
        static const detail::_ph<22> &_22 = detail::_ph<22>::instance();
        static const detail::_ph<23> &_23 = detail::_ph<23>::instance();
        static const detail::_ph<24> &_24 = detail::_ph<24>::instance();
        static const detail::_ph<25> &_25 = detail::_ph<25>::instance();
        static const detail::_ph<26> &_26 = detail::_ph<26>::instance();
        static const detail::_ph<27> &_27 = detail::_ph<27>::instance();
        static const detail::_ph<28> &_28 = detail::_ph<28>::instance();
        static const detail::_ph<29> &_29 = detail::_ph<29>::instance();
        static const detail::_ph<30> &_30 = detail::_ph<30>::instance();
        static const detail::_ph<31> &_31 = detail::_ph<31>::instance();
        static const detail::_ph<32> &_32 = detail::_ph<32>::instance();
    }

    template<int _N>
    struct is_placeholder<placeholders::detail::_ph<_N>/**/>
        : integral_constant<int, _N>
    { };

    template<class _Tp>
    struct is_placeholder<const _Tp>
        : is_placeholder<_Tp>
    { };

    template<class _Tp>
    struct is_placeholder<volatile _Tp>
        : is_placeholder<_Tp>
    { };

    template<class _Tp>
    struct is_placeholder<const volatile _Tp>
        : is_placeholder<_Tp>
    { };


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

#define _STDEX_PARAMETER_PACK_UNDEF
#include "./parameter_pack.h"
#undef _STDEX_PARAMETER_PACK_UNDEF

#endif // _STDEX_FUNCTIONAL_H

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

    template<class _FuncSignatureT>
    class function;

    template<class _Tp>
    class reference_wrapper;

    namespace detail
    {
        template<class _Tp>
        struct _dummy_trait
        {
            typedef _Tp type;
        };

        template<class _Tp>
        struct _dummy_trait<_Tp*>
        {
            typedef _Tp* type;
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

    namespace functional_cpp11
    {

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
            _R invoke(_R(_ObjectT::* _func)(), reference_wrapper<_ObjectT>& _ref)
        {
            return
                detail::functional_std::_forward<_R>::call(
                    (_ref.get().*_func)());
        }

        template<class _ObjectT>
        inline
            void invoke(void(_ObjectT::* _func)(), reference_wrapper<_ObjectT>& _ref)
        {
            (_ref.get().*_func)();
        }

        template<class _R, class _ObjectT>
        inline
            _R invoke(_R(_ObjectT::* _func)() const, reference_wrapper<_ObjectT>& _ref)
        {
            return
                detail::functional_std::_forward<_R>::call(
                    (_ref.get().*_func)());
        }

        template<class _ObjectT>
        inline
            void invoke(void(_ObjectT::* _func)() const, reference_wrapper<_ObjectT>& _ref)
        {
            (_ref.get().*_func)();
        }

        template<class _R, class _FuncT>
        inline
            _R invoke(_FuncT& _func)
        {
            return _func();
        }

        template<>
        inline
            void invoke(void(*_func)())
        {
            _func();
        }

#define _STDEX_INVOKE_IMPL(N) \
    template<class _R, _STDEX_TMPL_ARGS##N(_STDEX_BLANK, _STDEX_BLANK)> \
    _R invoke( _R(*_func)(_STDEX_TYPES##N(_STDEX_BLANK, _STDEX_BLANK)), _STDEX_PARAMS##N(typename stdex::detail::_dummy_trait<, >::type, _STDEX_BLANK, _STDEX_BLANK))\
    {\
        return\
        detail::functional_std::_forward<_R>::call(\
            _func(_STDEX_ARGS##N(_STDEX_BLANK, _STDEX_BLANK)) );\
    }\
\
    template<_STDEX_TMPL_ARGS##N(_STDEX_BLANK, _STDEX_BLANK)> \
    void invoke( void(*_func)(_STDEX_TYPES##N(_STDEX_BLANK, _STDEX_BLANK)), _STDEX_PARAMS##N(typename stdex::detail::_dummy_trait<, >::type, _STDEX_BLANK, _STDEX_BLANK))\
    {\
        _func(_STDEX_ARGS##N(_STDEX_BLANK, _STDEX_BLANK));\
    }\
\
    template<class _R, class _ObjectT, _STDEX_TMPL_ARGS##N(_STDEX_BLANK, _STDEX_BLANK)>\
    _R invoke( _R(_ObjectT::*_func)(_STDEX_TYPES##N(_STDEX_BLANK, _STDEX_BLANK)), _ObjectT &_obj, _STDEX_PARAMS##N(typename stdex::detail::_dummy_trait<, >::type, _STDEX_BLANK, _STDEX_BLANK))\
    {\
        return\
        detail::functional_std::_forward<_R>::call(\
            (_obj.*_func)(_STDEX_ARGS##N(_STDEX_BLANK, _STDEX_BLANK)) );\
    }\
\
    template<class _ObjectT, _STDEX_TMPL_ARGS##N(_STDEX_BLANK, _STDEX_BLANK)>\
    void invoke( void(_ObjectT::*_func)(_STDEX_TYPES##N(_STDEX_BLANK, _STDEX_BLANK)), _ObjectT &_obj, _STDEX_PARAMS##N(typename stdex::detail::_dummy_trait<, >::type, _STDEX_BLANK, _STDEX_BLANK))\
    {\
        (_obj.*_func)(_STDEX_ARGS##N(_STDEX_BLANK, _STDEX_BLANK));\
    }\
\
    template<class _R, class _ObjectT, _STDEX_TMPL_ARGS##N(_STDEX_BLANK, _STDEX_BLANK)>\
    _R invoke( _R(_ObjectT::*_func)(_STDEX_TYPES##N(_STDEX_BLANK, _STDEX_BLANK)) const, const _ObjectT &_obj, _STDEX_PARAMS##N(typename stdex::detail::_dummy_trait<, >::type, _STDEX_BLANK, _STDEX_BLANK))\
    {\
        return\
        detail::functional_std::_forward<_R>::call(\
            (_obj.*_func)(_STDEX_ARGS##N(_STDEX_BLANK, _STDEX_BLANK)) );\
    }\
\
    template<class _ObjectT, _STDEX_TMPL_ARGS##N(_STDEX_BLANK, _STDEX_BLANK)>\
    void invoke( void(_ObjectT::*_func)(_STDEX_TYPES##N(_STDEX_BLANK, _STDEX_BLANK)) const, const _ObjectT &_obj, _STDEX_PARAMS##N(typename stdex::detail::_dummy_trait<, >::type, _STDEX_BLANK, _STDEX_BLANK))\
    {\
        (_obj.*_func)(_STDEX_ARGS##N(_STDEX_BLANK, _STDEX_BLANK));\
    }\
\
    template<class _R, class _ObjectT, _STDEX_TMPL_ARGS##N(_STDEX_BLANK, _STDEX_BLANK)>\
    _R invoke( _R(_ObjectT::*_func)(_STDEX_TYPES##N(_STDEX_BLANK, _STDEX_BLANK)), _ObjectT *_obj, _STDEX_PARAMS##N(typename stdex::detail::_dummy_trait<, >::type, _STDEX_BLANK, _STDEX_BLANK))\
    {\
        return\
        detail::functional_std::_forward<_R>::call(\
            ((*_obj).*_func)(_STDEX_ARGS##N(_STDEX_BLANK, _STDEX_BLANK)) );\
    }\
\
    template<class _ObjectT, _STDEX_TMPL_ARGS##N(_STDEX_BLANK, _STDEX_BLANK)>\
    void invoke( void(_ObjectT::*_func)(_STDEX_TYPES##N(_STDEX_BLANK, _STDEX_BLANK)), _ObjectT *_obj, _STDEX_PARAMS##N(typename stdex::detail::_dummy_trait<, >::type, _STDEX_BLANK, _STDEX_BLANK))\
    {\
        ((*_obj).*_func)(_STDEX_ARGS##N(_STDEX_BLANK, _STDEX_BLANK));\
    }\
\
    template<class _R, class _ObjectT, _STDEX_TMPL_ARGS##N(_STDEX_BLANK, _STDEX_BLANK)>\
    _R invoke( _R(_ObjectT::*_func)(_STDEX_TYPES##N(_STDEX_BLANK, _STDEX_BLANK)) const, const _ObjectT *_obj, _STDEX_PARAMS##N(typename stdex::detail::_dummy_trait<, >::type, _STDEX_BLANK, _STDEX_BLANK))\
    {\
        return\
        detail::functional_std::_forward<_R>::call(\
            ((*_obj).*_func)(_STDEX_ARGS##N(_STDEX_BLANK, _STDEX_BLANK)) );\
    }\
\
    template<class _ObjectT, _STDEX_TMPL_ARGS##N(_STDEX_BLANK, _STDEX_BLANK)>\
    void invoke( void(_ObjectT::*_func)(_STDEX_TYPES##N(_STDEX_BLANK, _STDEX_BLANK)) const, const _ObjectT *_obj, _STDEX_PARAMS##N(typename stdex::detail::_dummy_trait<, >::type, _STDEX_BLANK, _STDEX_BLANK))\
    {\
        ((*_obj).*_func)(_STDEX_ARGS##N(_STDEX_BLANK, _STDEX_BLANK));\
    }\
\
    template<class _R, class _ObjectT, _STDEX_TMPL_ARGS##N(_STDEX_BLANK, _STDEX_BLANK)>\
    _R invoke( _R(_ObjectT::*_func)(_STDEX_TYPES##N(_STDEX_BLANK, _STDEX_BLANK)), reference_wrapper<_ObjectT> &_ref, _STDEX_PARAMS##N(typename stdex::detail::_dummy_trait<, >::type, _STDEX_BLANK, _STDEX_BLANK))\
    {\
        return\
        detail::functional_std::_forward<_R>::call(\
            (_ref.get().*_func)(_STDEX_ARGS##N(_STDEX_BLANK, _STDEX_BLANK)) );\
    }\
\
    template<class _ObjectT, _STDEX_TMPL_ARGS##N(_STDEX_BLANK, _STDEX_BLANK)>\
    void invoke( void(_ObjectT::*_func)(_STDEX_TYPES##N(_STDEX_BLANK, _STDEX_BLANK)), reference_wrapper<_ObjectT> &_ref, _STDEX_PARAMS##N(typename stdex::detail::_dummy_trait<, >::type, _STDEX_BLANK, _STDEX_BLANK))\
    {\
        (_ref.get().*_func)(_STDEX_ARGS##N(_STDEX_BLANK, _STDEX_BLANK));\
    }\
\
    template<class _R, class _ObjectT, _STDEX_TMPL_ARGS##N(_STDEX_BLANK, _STDEX_BLANK)>\
    _R invoke( _R(_ObjectT::*_func)(_STDEX_TYPES##N(_STDEX_BLANK, _STDEX_BLANK)) const, reference_wrapper<_ObjectT> &_ref, _STDEX_PARAMS##N(typename stdex::detail::_dummy_trait<, >::type, _STDEX_BLANK, _STDEX_BLANK))\
    {\
        return\
        detail::functional_std::_forward<_R>::call(\
            (_ref.get().*_func)(_STDEX_ARGS##N(_STDEX_BLANK, _STDEX_BLANK)) );\
    }\
\
    template<class _ObjectT, _STDEX_TMPL_ARGS##N(_STDEX_BLANK, _STDEX_BLANK)>\
    void invoke( void(_ObjectT::*_func)(_STDEX_TYPES##N(_STDEX_BLANK, _STDEX_BLANK)) const, reference_wrapper<_ObjectT> &_ref, _STDEX_PARAMS##N(typename stdex::detail::_dummy_trait<, >::type, _STDEX_BLANK, _STDEX_BLANK))\
    {\
        (_ref.get().*_func)(_STDEX_ARGS##N(_STDEX_BLANK, _STDEX_BLANK));\
    }\
\
    template<class _R, class _FuncT, _STDEX_TMPL_ARGS##N(_STDEX_BLANK, _STDEX_BLANK)>\
    _R invoke(_FuncT &_func, _STDEX_PARAMS##N(_STDEX_BLANK, _STDEX_BLANK, _STDEX_BLANK, _STDEX_BLANK))\
    {\
        stdex::function<_FuncT> _f(_func); \
        return\
            _f(_STDEX_ARGS##N(_STDEX_BLANK, _STDEX_BLANK));\
    }\


#define _STDEX_INVOKE(N) _STDEX_INVOKE_IMPL(N)

#define _STDEX_TMPL_ARGS_IMPL(prefix, N, postfix) class prefix _Arg##N##T postfix
#define _STDEX_TYPES_IMPL(prefix, N, postfix) prefix _Arg##N##T postfix
#define _STDEX_PARAMS_IMPL(prefix1, prefix2, N, postfix1, postfix2) prefix1 _Arg##N##T postfix1 prefix2 arg##N postfix2
#define _STDEX_ARGS_IMPL(prefix, N, postfix) prefix arg##N postfix
#define _STDEX_BLANK

#undef _STDEX_TMPL_ARGS
#undef _STDEX_TYPES
#undef _STDEX_PARAMS
#undef _STDEX_ARGS

#define _STDEX_TMPL_ARGS(prefix, N, postfix) _STDEX_TMPL_ARGS_IMPL(prefix, N, postfix)
#define _STDEX_TYPES(prefix, N, postfix) _STDEX_TYPES_IMPL(prefix, N, postfix)
#define _STDEX_PARAMS(prefix1, prefix2, N, postfix1, postfix2) _STDEX_PARAMS_IMPL(prefix1, prefix2, N, postfix1, postfix2)
#define _STDEX_ARGS(prefix, N, postfix) _STDEX_ARGS_IMPL(prefix, N, postfix)

#define _STDEX_TMPL_ARGS0(prefix, postfix) _STDEX_TMPL_ARGS(prefix, 0, postfix)
#define _STDEX_TYPES0(prefix, postfix) _STDEX_TYPES(prefix, 0, postfix)
#define _STDEX_PARAMS0(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS(prefix1, prefix2,0, postfix1, postfix2)
#define _STDEX_ARGS0(prefix, postfix) _STDEX_ARGS(prefix, 0, postfix)

#define _STDEX_TMPL_ARGS1(prefix, postfix) _STDEX_TMPL_ARGS0(prefix, postfix) _STDEX_DELIM _STDEX_TMPL_ARGS(prefix, 1, postfix)
#define _STDEX_TYPES1(prefix, postfix) _STDEX_TYPES0(prefix, postfix) _STDEX_DELIM _STDEX_TYPES(prefix, 1, postfix)
#define _STDEX_PARAMS1(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS0(prefix1, postfix1, prefix2, postfix2) _STDEX_DELIM _STDEX_PARAMS(prefix1, prefix2,1, postfix1, postfix2)
#define _STDEX_ARGS1(prefix, postfix) _STDEX_ARGS0(prefix, postfix) _STDEX_DELIM _STDEX_ARGS(prefix, 1, postfix)

#define _STDEX_TMPL_ARGS2(prefix, postfix) _STDEX_TMPL_ARGS1(prefix, postfix) _STDEX_DELIM _STDEX_TMPL_ARGS(prefix, 2, postfix)
#define _STDEX_TYPES2(prefix, postfix) _STDEX_TYPES1(prefix, postfix) _STDEX_DELIM _STDEX_TYPES(prefix, 2, postfix)
#define _STDEX_PARAMS2(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS1(prefix1, postfix1, prefix2, postfix2) _STDEX_DELIM _STDEX_PARAMS(prefix1, prefix2,2, postfix1, postfix2)
#define _STDEX_ARGS2(prefix, postfix) _STDEX_ARGS1(prefix, postfix) _STDEX_DELIM _STDEX_ARGS(prefix, 2, postfix)

#define _STDEX_TMPL_ARGS3(prefix, postfix) _STDEX_TMPL_ARGS2(prefix, postfix) _STDEX_DELIM _STDEX_TMPL_ARGS(prefix, 3, postfix)
#define _STDEX_TYPES3(prefix, postfix) _STDEX_TYPES2(prefix, postfix) _STDEX_DELIM _STDEX_TYPES(prefix, 3, postfix)
#define _STDEX_PARAMS3(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS2(prefix1, postfix1, prefix2, postfix2) _STDEX_DELIM _STDEX_PARAMS(prefix1, prefix2,3, postfix1, postfix2)
#define _STDEX_ARGS3(prefix, postfix) _STDEX_ARGS2(prefix, postfix) _STDEX_DELIM _STDEX_ARGS(prefix, 3, postfix)

#define _STDEX_TMPL_ARGS4(prefix, postfix) _STDEX_TMPL_ARGS3(prefix, postfix) _STDEX_DELIM _STDEX_TMPL_ARGS(prefix, 4, postfix)
#define _STDEX_TYPES4(prefix, postfix) _STDEX_TYPES3(prefix, postfix) _STDEX_DELIM _STDEX_TYPES(prefix, 4, postfix)
#define _STDEX_PARAMS4(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS3(prefix1, postfix1, prefix2, postfix2) _STDEX_DELIM _STDEX_PARAMS(prefix1, prefix2,4, postfix1, postfix2)
#define _STDEX_ARGS4(prefix, postfix) _STDEX_ARGS3(prefix, postfix) _STDEX_DELIM _STDEX_ARGS(prefix, 4, postfix)

#define _STDEX_TMPL_ARGS5(prefix, postfix) _STDEX_TMPL_ARGS4(prefix, postfix) _STDEX_DELIM _STDEX_TMPL_ARGS(prefix, 5, postfix)
#define _STDEX_TYPES5(prefix, postfix) _STDEX_TYPES4(prefix, postfix) _STDEX_DELIM _STDEX_TYPES(prefix, 5, postfix)
#define _STDEX_PARAMS5(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS4(prefix1, postfix1, prefix2, postfix2) _STDEX_DELIM _STDEX_PARAMS(prefix1, prefix2,5, postfix1, postfix2)
#define _STDEX_ARGS5(prefix, postfix) _STDEX_ARGS4(prefix, postfix) _STDEX_DELIM _STDEX_ARGS(prefix, 5, postfix)

#define _STDEX_TMPL_ARGS6(prefix, postfix) _STDEX_TMPL_ARGS5(prefix, postfix) _STDEX_DELIM _STDEX_TMPL_ARGS(prefix, 6, postfix)
#define _STDEX_TYPES6(prefix, postfix) _STDEX_TYPES5(prefix, postfix) _STDEX_DELIM _STDEX_TYPES(prefix, 6, postfix)
#define _STDEX_PARAMS6(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS5(prefix1, postfix1, prefix2, postfix2) _STDEX_DELIM _STDEX_PARAMS(prefix1, prefix2,6, postfix1, postfix2)
#define _STDEX_ARGS6(prefix, postfix) _STDEX_ARGS5(prefix, postfix) _STDEX_DELIM _STDEX_ARGS(prefix, 6, postfix)

#define _STDEX_TMPL_ARGS7(prefix, postfix) _STDEX_TMPL_ARGS6(prefix, postfix) _STDEX_DELIM _STDEX_TMPL_ARGS(prefix, 7, postfix)
#define _STDEX_TYPES7(prefix, postfix) _STDEX_TYPES6(prefix, postfix) _STDEX_DELIM _STDEX_TYPES(prefix, 7, postfix)
#define _STDEX_PARAMS7(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS6(prefix1, postfix1, prefix2, postfix2) _STDEX_DELIM _STDEX_PARAMS(prefix1, prefix2,7, postfix1, postfix2)
#define _STDEX_ARGS7(prefix, postfix) _STDEX_ARGS6(prefix, postfix) _STDEX_DELIM _STDEX_ARGS(prefix, 7, postfix)

#define _STDEX_TMPL_ARGS8(prefix, postfix) _STDEX_TMPL_ARGS7(prefix, postfix) _STDEX_DELIM _STDEX_TMPL_ARGS(prefix, 8, postfix)
#define _STDEX_TYPES8(prefix, postfix) _STDEX_TYPES7(prefix, postfix) _STDEX_DELIM _STDEX_TYPES(prefix, 8, postfix)
#define _STDEX_PARAMS8(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS7(prefix1, postfix1, prefix2, postfix2) _STDEX_DELIM _STDEX_PARAMS(prefix1, prefix2,8, postfix1, postfix2)
#define _STDEX_ARGS8(prefix, postfix) _STDEX_ARGS7(prefix, postfix) _STDEX_DELIM _STDEX_ARGS(prefix, 8, postfix)

#define _STDEX_TMPL_ARGS9(prefix, postfix) _STDEX_TMPL_ARGS8(prefix, postfix) _STDEX_DELIM _STDEX_TMPL_ARGS(prefix, 9, postfix)
#define _STDEX_TYPES9(prefix, postfix) _STDEX_TYPES8(prefix, postfix) _STDEX_DELIM _STDEX_TYPES(prefix, 9, postfix)
#define _STDEX_PARAMS9(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS8(prefix1, postfix1, prefix2, postfix2) _STDEX_DELIM _STDEX_PARAMS(prefix1, prefix2,9, postfix1, postfix2)
#define _STDEX_ARGS9(prefix, postfix) _STDEX_ARGS8(prefix, postfix) _STDEX_DELIM _STDEX_ARGS(prefix, 9, postfix)

#define _STDEX_TMPL_ARGS10(prefix, postfix) _STDEX_TMPL_ARGS9(prefix, postfix) _STDEX_DELIM _STDEX_TMPL_ARGS(prefix, 10, postfix)
#define _STDEX_TYPES10(prefix, postfix) _STDEX_TYPES9(prefix, postfix) _STDEX_DELIM _STDEX_TYPES(prefix, 10, postfix)
#define _STDEX_PARAMS10(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS9(prefix1, postfix1, prefix2, postfix2) _STDEX_DELIM _STDEX_PARAMS(prefix1, prefix2,10, postfix1, postfix2)
#define _STDEX_ARGS10(prefix, postfix) _STDEX_ARGS9(prefix, postfix) _STDEX_DELIM _STDEX_ARGS(prefix, 10, postfix)

#define _STDEX_TMPL_ARGS11(prefix, postfix) _STDEX_TMPL_ARGS10(prefix, postfix) _STDEX_DELIM _STDEX_TMPL_ARGS(prefix, 11, postfix)
#define _STDEX_TYPES11(prefix, postfix) _STDEX_TYPES10(prefix, postfix) _STDEX_DELIM _STDEX_TYPES(prefix, 11, postfix)
#define _STDEX_PARAMS11(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS10(prefix1, postfix1, prefix2, postfix2) _STDEX_DELIM _STDEX_PARAMS(prefix1, prefix2,11, postfix1, postfix2)
#define _STDEX_ARGS11(prefix, postfix) _STDEX_ARGS10(prefix, postfix) _STDEX_DELIM _STDEX_ARGS(prefix, 11, postfix)

#define _STDEX_TMPL_ARGS12(prefix, postfix) _STDEX_TMPL_ARGS11(prefix, postfix) _STDEX_DELIM _STDEX_TMPL_ARGS(prefix, 12, postfix)
#define _STDEX_TYPES12(prefix, postfix) _STDEX_TYPES11(prefix, postfix) _STDEX_DELIM _STDEX_TYPES(prefix, 12, postfix)
#define _STDEX_PARAMS12(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS11(prefix1, postfix1, prefix2, postfix2) _STDEX_DELIM _STDEX_PARAMS(prefix1, prefix2,12, postfix1, postfix2)
#define _STDEX_ARGS12(prefix, postfix) _STDEX_ARGS11(prefix, postfix) _STDEX_DELIM _STDEX_ARGS(prefix, 12, postfix)

#define _STDEX_TMPL_ARGS13(prefix, postfix) _STDEX_TMPL_ARGS12(prefix, postfix) _STDEX_DELIM _STDEX_TMPL_ARGS(prefix, 13, postfix)
#define _STDEX_TYPES13(prefix, postfix) _STDEX_TYPES12(prefix, postfix) _STDEX_DELIM _STDEX_TYPES(prefix, 13, postfix)
#define _STDEX_PARAMS13(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS12(prefix1, postfix1, prefix2, postfix2) _STDEX_DELIM _STDEX_PARAMS(prefix1, prefix2,13, postfix1, postfix2)
#define _STDEX_ARGS13(prefix, postfix) _STDEX_ARGS12(prefix, postfix) _STDEX_DELIM _STDEX_ARGS(prefix, 13, postfix)

#define _STDEX_TMPL_ARGS14(prefix, postfix) _STDEX_TMPL_ARGS13(prefix, postfix) _STDEX_DELIM _STDEX_TMPL_ARGS(prefix, 14, postfix)
#define _STDEX_TYPES14(prefix, postfix) _STDEX_TYPES13(prefix, postfix) _STDEX_DELIM _STDEX_TYPES(prefix, 14, postfix)
#define _STDEX_PARAMS14(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS13(prefix1, postfix1, prefix2, postfix2) _STDEX_DELIM _STDEX_PARAMS(prefix1, prefix2,14, postfix1, postfix2)
#define _STDEX_ARGS14(prefix, postfix) _STDEX_ARGS13(prefix, postfix) _STDEX_DELIM _STDEX_ARGS(prefix, 14, postfix)

#define _STDEX_TMPL_ARGS15(prefix, postfix) _STDEX_TMPL_ARGS14(prefix, postfix) _STDEX_DELIM _STDEX_TMPL_ARGS(prefix, 15, postfix)
#define _STDEX_TYPES15(prefix, postfix) _STDEX_TYPES14(prefix, postfix) _STDEX_DELIM _STDEX_TYPES(prefix, 15, postfix)
#define _STDEX_PARAMS15(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS14(prefix1, postfix1, prefix2, postfix2) _STDEX_DELIM _STDEX_PARAMS(prefix1, prefix2,15, postfix1, postfix2)
#define _STDEX_ARGS15(prefix, postfix) _STDEX_ARGS14(prefix, postfix) _STDEX_DELIM _STDEX_ARGS(prefix, 15, postfix)

#define _STDEX_TMPL_ARGS16(prefix, postfix) _STDEX_TMPL_ARGS15(prefix, postfix) _STDEX_DELIM _STDEX_TMPL_ARGS(prefix, 16, postfix)
#define _STDEX_TYPES16(prefix, postfix) _STDEX_TYPES15(prefix, postfix) _STDEX_DELIM _STDEX_TYPES(prefix, 16, postfix)
#define _STDEX_PARAMS16(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS15(prefix1, postfix1, prefix2, postfix2) _STDEX_DELIM _STDEX_PARAMS(prefix1, prefix2,16, postfix1, postfix2)
#define _STDEX_ARGS16(prefix, postfix) _STDEX_ARGS15(prefix, postfix) _STDEX_DELIM _STDEX_ARGS(prefix, 16, postfix)

#define _STDEX_TMPL_ARGS17(prefix, postfix) _STDEX_TMPL_ARGS16(prefix, postfix) _STDEX_DELIM _STDEX_TMPL_ARGS(prefix, 17, postfix)
#define _STDEX_TYPES17(prefix, postfix) _STDEX_TYPES16(prefix, postfix) _STDEX_DELIM _STDEX_TYPES(prefix, 17, postfix)
#define _STDEX_PARAMS17(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS16(prefix1, postfix1, prefix2, postfix2) _STDEX_DELIM _STDEX_PARAMS(prefix1, prefix2,17, postfix1, postfix2)
#define _STDEX_ARGS17(prefix, postfix) _STDEX_ARGS16(prefix, postfix) _STDEX_DELIM _STDEX_ARGS(prefix, 17, postfix)

#define _STDEX_TMPL_ARGS18(prefix, postfix) _STDEX_TMPL_ARGS17(prefix, postfix) _STDEX_DELIM _STDEX_TMPL_ARGS(prefix, 18, postfix)
#define _STDEX_TYPES18(prefix, postfix) _STDEX_TYPES17(prefix, postfix) _STDEX_DELIM _STDEX_TYPES(prefix, 18, postfix)
#define _STDEX_PARAMS18(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS17(prefix1, postfix1, prefix2, postfix2) _STDEX_DELIM _STDEX_PARAMS(prefix1, prefix2,18, postfix1, postfix2)
#define _STDEX_ARGS18(prefix, postfix) _STDEX_ARGS17(prefix, postfix) _STDEX_DELIM _STDEX_ARGS(prefix, 18, postfix)

#define _STDEX_TMPL_ARGS19(prefix, postfix) _STDEX_TMPL_ARGS18(prefix, postfix) _STDEX_DELIM _STDEX_TMPL_ARGS(prefix, 19, postfix)
#define _STDEX_TYPES19(prefix, postfix) _STDEX_TYPES18(prefix, postfix) _STDEX_DELIM _STDEX_TYPES(prefix, 19, postfix)
#define _STDEX_PARAMS19(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS18(prefix1, postfix1, prefix2, postfix2) _STDEX_DELIM _STDEX_PARAMS(prefix1, prefix2,19, postfix1, postfix2)
#define _STDEX_ARGS19(prefix, postfix) _STDEX_ARGS18(prefix, postfix) _STDEX_DELIM _STDEX_ARGS(prefix, 19, postfix)

#define _STDEX_TMPL_ARGS20(prefix, postfix) _STDEX_TMPL_ARGS19(prefix, postfix) _STDEX_DELIM _STDEX_TMPL_ARGS(prefix, 20, postfix)
#define _STDEX_TYPES20(prefix, postfix) _STDEX_TYPES19(prefix, postfix) _STDEX_DELIM _STDEX_TYPES(prefix, 20, postfix)
#define _STDEX_PARAMS20(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS19(prefix1, postfix1, prefix2, postfix2) _STDEX_DELIM _STDEX_PARAMS(prefix1, prefix2,20, postfix1, postfix2)
#define _STDEX_ARGS20(prefix, postfix) _STDEX_ARGS19(prefix, postfix) _STDEX_DELIM _STDEX_ARGS(prefix, 20, postfix)

#define _STDEX_TMPL_ARGS21(prefix, postfix) _STDEX_TMPL_ARGS20(prefix, postfix) _STDEX_DELIM _STDEX_TMPL_ARGS(prefix, 21, postfix)
#define _STDEX_TYPES21(prefix, postfix) _STDEX_TYPES20(prefix, postfix) _STDEX_DELIM _STDEX_TYPES(prefix, 21, postfix)
#define _STDEX_PARAMS21(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS20(prefix1, postfix1, prefix2, postfix2) _STDEX_DELIM _STDEX_PARAMS(prefix1, prefix2,21, postfix1, postfix2)
#define _STDEX_ARGS21(prefix, postfix) _STDEX_ARGS20(prefix, postfix) _STDEX_DELIM _STDEX_ARGS(prefix, 21, postfix)

#define _STDEX_TMPL_ARGS22(prefix, postfix) _STDEX_TMPL_ARGS21(prefix, postfix) _STDEX_DELIM _STDEX_TMPL_ARGS(prefix, 22, postfix)
#define _STDEX_TYPES22(prefix, postfix) _STDEX_TYPES21(prefix, postfix) _STDEX_DELIM _STDEX_TYPES(prefix, 22, postfix)
#define _STDEX_PARAMS22(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS21(prefix1, postfix1, prefix2, postfix2) _STDEX_DELIM _STDEX_PARAMS(prefix1, prefix2,22, postfix1, postfix2)
#define _STDEX_ARGS22(prefix, postfix) _STDEX_ARGS21(prefix, postfix) _STDEX_DELIM _STDEX_ARGS(prefix, 22, postfix)

#define _STDEX_TMPL_ARGS23(prefix, postfix) _STDEX_TMPL_ARGS22(prefix, postfix) _STDEX_DELIM _STDEX_TMPL_ARGS(prefix, 23, postfix)
#define _STDEX_TYPES23(prefix, postfix) _STDEX_TYPES22(prefix, postfix) _STDEX_DELIM _STDEX_TYPES(prefix, 23, postfix)
#define _STDEX_PARAMS23(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS22(prefix1, postfix1, prefix2, postfix2) _STDEX_DELIM _STDEX_PARAMS(prefix1, prefix2,23, postfix1, postfix2)
#define _STDEX_ARGS23(prefix, postfix) _STDEX_ARGS22(prefix, postfix) _STDEX_DELIM _STDEX_ARGS(prefix, 23, postfix)

#define _STDEX_TMPL_ARGS24(prefix, postfix) _STDEX_TMPL_ARGS23(prefix, postfix) _STDEX_DELIM _STDEX_TMPL_ARGS(prefix, 24, postfix)
#define _STDEX_TYPES24(prefix, postfix) _STDEX_TYPES23(prefix, postfix) _STDEX_DELIM _STDEX_TYPES(prefix, 24, postfix)
#define _STDEX_PARAMS24(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS23(prefix1, postfix1, prefix2, postfix2) _STDEX_DELIM _STDEX_PARAMS(prefix1, prefix2,24, postfix1, postfix2)
#define _STDEX_ARGS24(prefix, postfix) _STDEX_ARGS23(prefix, postfix) _STDEX_DELIM _STDEX_ARGS(prefix, 24, postfix)

#define _STDEX_TMPL_ARGS25(prefix, postfix) _STDEX_TMPL_ARGS24(prefix, postfix) _STDEX_DELIM _STDEX_TMPL_ARGS(prefix, 25, postfix)
#define _STDEX_TYPES25(prefix, postfix) _STDEX_TYPES24(prefix, postfix) _STDEX_DELIM _STDEX_TYPES(prefix, 25, postfix)
#define _STDEX_PARAMS25(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS24(prefix1, postfix1, prefix2, postfix2) _STDEX_DELIM _STDEX_PARAMS(prefix1, prefix2,25, postfix1, postfix2)
#define _STDEX_ARGS25(prefix, postfix) _STDEX_ARGS24(prefix, postfix) _STDEX_DELIM _STDEX_ARGS(prefix, 25, postfix)

#define _STDEX_TMPL_ARGS26(prefix, postfix) _STDEX_TMPL_ARGS25(prefix, postfix) _STDEX_DELIM _STDEX_TMPL_ARGS(prefix, 26, postfix)
#define _STDEX_TYPES26(prefix, postfix) _STDEX_TYPES25(prefix, postfix) _STDEX_DELIM _STDEX_TYPES(prefix, 26, postfix)
#define _STDEX_PARAMS26(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS25(prefix1, postfix1, prefix2, postfix2) _STDEX_DELIM _STDEX_PARAMS(prefix1, prefix2,26, postfix1, postfix2)
#define _STDEX_ARGS26(prefix, postfix) _STDEX_ARGS25(prefix, postfix) _STDEX_DELIM _STDEX_ARGS(prefix, 26, postfix)

#define _STDEX_TMPL_ARGS27(prefix, postfix) _STDEX_TMPL_ARGS26(prefix, postfix) _STDEX_DELIM _STDEX_TMPL_ARGS(prefix, 27, postfix)
#define _STDEX_TYPES27(prefix, postfix) _STDEX_TYPES26(prefix, postfix) _STDEX_DELIM _STDEX_TYPES(prefix, 27, postfix)
#define _STDEX_PARAMS27(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS26(prefix1, postfix1, prefix2, postfix2) _STDEX_DELIM _STDEX_PARAMS(prefix1, prefix2,27, postfix1, postfix2)
#define _STDEX_ARGS27(prefix, postfix) _STDEX_ARGS26(prefix, postfix) _STDEX_DELIM _STDEX_ARGS(prefix, 27, postfix)

#define _STDEX_TMPL_ARGS28(prefix, postfix) _STDEX_TMPL_ARGS27(prefix, postfix) _STDEX_DELIM _STDEX_TMPL_ARGS(prefix, 28, postfix)
#define _STDEX_TYPES28(prefix, postfix) _STDEX_TYPES27(prefix, postfix) _STDEX_DELIM _STDEX_TYPES(prefix, 28, postfix)
#define _STDEX_PARAMS28(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS27(prefix1, postfix1, prefix2, postfix2) _STDEX_DELIM _STDEX_PARAMS(prefix1, prefix2,28, postfix1, postfix2)
#define _STDEX_ARGS28(prefix, postfix) _STDEX_ARGS27(prefix, postfix) _STDEX_DELIM _STDEX_ARGS(prefix, 28, postfix)

#define _STDEX_TMPL_ARGS29(prefix, postfix) _STDEX_TMPL_ARGS28(prefix, postfix) _STDEX_DELIM _STDEX_TMPL_ARGS(prefix, 29, postfix)
#define _STDEX_TYPES29(prefix, postfix) _STDEX_TYPES28(prefix, postfix) _STDEX_DELIM _STDEX_TYPES(prefix, 29, postfix)
#define _STDEX_PARAMS29(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS28(prefix1, postfix1, prefix2, postfix2) _STDEX_DELIM _STDEX_PARAMS(prefix1, prefix2,29, postfix1, postfix2)
#define _STDEX_ARGS29(prefix, postfix) _STDEX_ARGS28(prefix, postfix) _STDEX_DELIM _STDEX_ARGS(prefix, 29, postfix)

#define _STDEX_TMPL_ARGS30(prefix, postfix) _STDEX_TMPL_ARGS29(prefix, postfix) _STDEX_DELIM _STDEX_TMPL_ARGS(prefix, 30, postfix)
#define _STDEX_TYPES30(prefix, postfix) _STDEX_TYPES29(prefix, postfix) _STDEX_DELIM _STDEX_TYPES(prefix, 30, postfix)
#define _STDEX_PARAMS30(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS29(prefix1, postfix1, prefix2, postfix2) _STDEX_DELIM _STDEX_PARAMS(prefix1, prefix2,30, postfix1, postfix2)
#define _STDEX_ARGS30(prefix, postfix) _STDEX_ARGS29(prefix, postfix) _STDEX_DELIM _STDEX_ARGS(prefix, 30, postfix)

#define _STDEX_TMPL_ARGS31(prefix, postfix) _STDEX_TMPL_ARGS30(prefix, postfix) _STDEX_DELIM _STDEX_TMPL_ARGS(prefix, 31, postfix)
#define _STDEX_TYPES31(prefix, postfix) _STDEX_TYPES30(prefix, postfix) _STDEX_DELIM _STDEX_TYPES(prefix, 31, postfix)
#define _STDEX_PARAMS31(prefix1, postfix1, prefix2, postfix2) _STDEX_PARAMS30(prefix1, postfix1, prefix2, postfix2) _STDEX_DELIM _STDEX_PARAMS(prefix1, prefix2,31, postfix1, postfix2)
#define _STDEX_ARGS31(prefix, postfix) _STDEX_ARGS30(prefix, postfix) _STDEX_DELIM _STDEX_ARGS(prefix, 31, postfix)

#define _STDEX_DELIM ,

_STDEX_INVOKE(0)
_STDEX_INVOKE(1)
_STDEX_INVOKE(2)
_STDEX_INVOKE(3)
_STDEX_INVOKE(4)
_STDEX_INVOKE(5)
_STDEX_INVOKE(6)
_STDEX_INVOKE(7)
_STDEX_INVOKE(8)
_STDEX_INVOKE(9)
_STDEX_INVOKE(10)
_STDEX_INVOKE(11)
_STDEX_INVOKE(12)
_STDEX_INVOKE(13)
_STDEX_INVOKE(14)
_STDEX_INVOKE(15)
_STDEX_INVOKE(16)
_STDEX_INVOKE(17)
_STDEX_INVOKE(18)
_STDEX_INVOKE(19)
_STDEX_INVOKE(20)
_STDEX_INVOKE(21)
_STDEX_INVOKE(22)
_STDEX_INVOKE(23)
_STDEX_INVOKE(24)
_STDEX_INVOKE(25)
_STDEX_INVOKE(26)
_STDEX_INVOKE(27)
_STDEX_INVOKE(28)
_STDEX_INVOKE(29)
_STDEX_INVOKE(30)
_STDEX_INVOKE(31)

#undef _STDEX_DELIM


    }


    using functional_cpp11::invoke;

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

        template<class _ArgT>
        struct _arg_is_void:
            bool_constant<
                is_same<_ArgT, void_type>::value == bool(true) ||
                is_void<_ArgT>::value == bool(true)
            >
        { };

        template<class _ArgT>
        struct _args_count_incr
        {
            static const int value = 
                _arg_is_void<_ArgT>::value ? 0 : 1;
        };

        template<class _ArgsT, class _ArgT, int _N>
        struct _args: _ArgsT, _arg<_ArgT, _N>
        {
            typedef _args type;
            static const int count = _N + _args_count_incr<_ArgT>::value;

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
            static const int count = 
                _args_count_incr<_ArgT>::value;

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

        template<class _R>
        struct _return_arg
        {
            mutable _R* _ptr;

            _return_arg(const _R& ref_) :_ptr( new _R(ref_)) {}
            _return_arg(_R* ptr_) :_ptr(ptr_) {}
            void swap(_return_arg& other) { using std::swap; swap(_ptr, other._ptr); }
            ~_return_arg() { delete _ptr; }
            _return_arg(_return_arg& other): _ptr(0) { swap(other); }

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

            _return_arg(_R* ptr_) :_ptr(ptr_) {}

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

            _R& release() { return *_ptr; }
            _R& get() { return release(); }

        private:
            //_return_arg(const _return_arg&) _STDEX_DELETED_FUNCTION;
        };

        template<>
        struct _return_arg<void>
        {
            _return_arg(void*) {}
            _return_arg(void_type) {}
            _return_arg(const _return_arg<void_type>&) {}

            void_type release() { void_type dummy;  return dummy; }
            void_type get() { return release(); }

        private:
            //_return_arg(const _return_arg&) _STDEX_DELETED_FUNCTION;
        };

        template<>
        struct _return_arg<void_type>
        {
            _return_arg(void*) {}
            _return_arg(void_type) {}
            _return_arg(const _return_arg<void>&) {}

            void_type release() { void_type dummy;  return dummy; }
            void_type get() { return release(); }

        private:
            //_return_arg(const _return_arg&) _STDEX_DELETED_FUNCTION;
        };

        template<class _R, class _FuncT, int _Index, int _Count, bool>
        struct _check_args_for_null_impl_helper
        {
            template<class _CheckedArgsT, class _RawArgsT>
            static _return_arg<_R> call(_FuncT &fx, _RawArgsT &args,
                const _CheckedArgsT &checked_args)
            {
                typedef _func_invoker_impl<_R, _FuncT, _Index + 1, _Count> func_invoker;
                return func_invoker::call(fx, args, checked_args);
            }
        };

        template<class _R, class _FuncT, int _Index, int _Count>
        struct _check_args_for_null_impl_helper<_R, _FuncT, _Index, _Count, true>
        {
            template<class _CheckedArgsT, class _RawArgsT>
            static _return_arg<_R> call(_FuncT &fx, _RawArgsT &args,
                const _checked_args<_CheckedArgsT> &)
            {
                    typedef _args<_CheckedArgsT, _nullptr_place_holder, _Index> checked_args_t;
                    typedef _func_invoker_impl<_R, _FuncT, _Index + 1, _Count> func_invoker;

                    return func_invoker::call(fx, args, _checked_args<checked_args_t>());
            }

            template<class _RawArgsT>
            static _return_arg<_R> call(_FuncT& fx, _RawArgsT& args,
                const _checked_args<_RawArgsT>& )
            {
                typedef 
                typename _get_args<_RawArgsT, _Index>::base_args args_type;

                typedef _args<args_type, _nullptr_place_holder, _Index> checked_args_t;
                typedef _func_invoker_impl<_R, _FuncT, _Index + 1, _Count> func_invoker;

                return func_invoker::call(fx, args, _checked_args<checked_args_t>());
            }
        };

        template<class _R, class _FuncT, int _Index, int _Count, bool>
        struct _check_args_for_null_impl
        {
            template<class _RawArgsT, class _CheckedArgsT>
            static _return_arg<_R> call(_FuncT &fx, _RawArgsT &args, const _checked_args<_CheckedArgsT>& checked_args)
            {
                typedef _check_args_for_null_impl_helper<
                    _R, _FuncT, _Index, _Count,
                    intern::_has_feature<intern::_stdex_nullptr_implemented_as_distinct_type>::value == bool(true)
                > helper;
                return helper::call(fx, args, checked_args);
            }
        };

        template<class _R, class _FuncT, int _Index, int _Count>
        struct _check_args_for_null_impl<_R, _FuncT, _Index, _Count, false>
        {
            typedef _func_invoker_impl<_R, _FuncT, _Index + 1, _Count> func_invoker;
            
            template<class _RawArgsT, class _CheckedArgsT>
            static _return_arg<_R> call(_FuncT &fx, _RawArgsT &args, const _checked_args<_CheckedArgsT>& checked_args)
            {
                typedef 
                typename _get_args<_RawArgsT, _Index>::arg arg;

                typedef 
                typename arg::type arg_type;

                typedef _args<_CheckedArgsT, arg_type, _Index> checked_args_t;

                return func_invoker::call(fx, args, _checked_args<checked_args_t>());
            }

            template<class _RawArgsT>
            static _return_arg<_R> call(_FuncT &fx, _RawArgsT &args, const _checked_args<_RawArgsT>& checked_args)
            {
                return func_invoker::call(fx, args, checked_args);
            }
        };

        template<class _R, class _FuncT, int _Index, int _Count>
        struct _check_args_for_null
        {
            template<class _RawArgsT, class _CheckedArgsT>
            static _return_arg<_R> call(_FuncT &fx, _RawArgsT &args, const _checked_args<_CheckedArgsT>& checked_args)
            {
                typedef 
                typename _get_args<_RawArgsT, _Index>::arg arg;

                typedef 
                typename arg::type arg_type;

                return
                _check_args_for_null_impl
                <
                    _R, _FuncT, _Index, _Count,
                    is_null_pointer<typename remove_reference<arg_type>::type>::value == bool(true)
                    && intern::_has_feature<intern::_stdex_has_native_nullptr>::value == bool(false)
                >::call(fx, args, checked_args);
            }
        };


        template<class _R, class _FuncT, int _Index, int _Count>
        struct _func_invoker_impl
        {
            typedef _check_args_for_null<_R, _FuncT, _Index, _Count> null_checker;

            template<class _RawArgsT, class _CheckedArgsT>
            static _return_arg<_R> call(_FuncT &fx, _RawArgsT &args, const _checked_args<_CheckedArgsT>& checked_args)
            {
                return null_checker::call(fx, args, checked_args);
            }
        };



        template<class _R, class _FuncT>
        struct _func_invoker_impl<_R, _FuncT, 0, 0>
        {
            template<class _RawArgsT>
            static _return_arg<_R> call(_FuncT &fx, _RawArgsT&, const _checked_args<_RawArgsT>&)
            {
                struct _functor
                {
                    typedef
                    typename 
                    conditional<_arg_is_void<_R>::value, void_type, _R>::type return_type;
                    typedef return_type(&disable)(...);
                    typedef
                    typename 
                    conditional<_arg_is_void<return_type>::value, disable, _FuncT&>::type func_return_type;
                    typedef
                    typename 
                    conditional<_arg_is_void<return_type>::value, _FuncT&, disable>::type func_noreturn_type;

                    _return_arg<return_type> operator()(func_return_type fx)
                    {
                        using ::stdex::detail::_return_arg;
                        return 
                        _return_arg<return_type>(
                            ::stdex::detail::functional_std::_forward<return_type>::call(fx()) );
                    }

                    _return_arg<return_type> operator()(func_noreturn_type fx)
                    {
                        using ::stdex::detail::_return_arg;
                        fx();
                        _return_arg<return_type> result = 0;
                        return  result;
                    }
                }; 
                _functor _f;
                return _f(fx);
            }
        };


        namespace func_invoker_calls
        {
            template<int>
            struct _any
            {
                template<class _Tp>
                _any(const _Tp&) {}
            };
            template<class _FuncT, class _R, class _Disable1, class _Disable2, class _Disable3, class _Disable4>
            struct _func_result
            {
                typedef
                typename 
                conditional<_arg_is_void<_R>::value, _Disable1, _FuncT&>::type non_void_func;
                typedef
                typename 
                conditional<_arg_is_void<_R>::value, _FuncT&, _Disable2>::type void_func;
                typedef
                typename 
                conditional<is_member_function_pointer<_FuncT>::value, _Disable3, non_void_func>::type non_void_plain_func;
                typedef
                typename 
                conditional<is_member_function_pointer<_FuncT>::value, _Disable4, void_func>::type void_plain_func;
                typedef
                typename 
                conditional<is_member_function_pointer<_FuncT>::value, non_void_func, _Disable3>::type non_void_member_func;
                typedef
                typename 
                conditional<is_member_function_pointer<_FuncT>::value, void_func, _Disable4>::type void_member_func;
            };

            template<class _R, class _FuncT, class _ArgT0, class _ResArgsT>
            static _return_arg<_R> func(_FuncT &fx, _arg<_ArgT0, 0>&, _ResArgsT &res)
            {
                struct _functor: _ResArgsT
                {
                    typedef _ResArgsT base_type;
                    typedef _ArgT0 arg0_type;
                    typedef
                    typename 
                    conditional<_arg_is_void<_R>::value, void_type, _R>::type return_type;
                    typedef
                    _func_result<_FuncT, return_type, return_type(&)(_any<1>), return_type(&)(_any<2>), return_type(&)(_any<3>), return_type(&)(_any<4>)> result;
                    typedef typename result::non_void_plain_func  non_void_plain_func;
                    typedef typename result::non_void_member_func non_void_member_func;
                    typedef typename result::void_plain_func      void_plain_func;
                    typedef typename result::void_member_func     void_member_func;

                    _functor(const base_type &other) : base_type(other) {}

                    _return_arg<return_type> operator()(non_void_plain_func fx)
                    {
                        using ::stdex::detail::_arg;
                        using ::stdex::detail::_return_arg;

                        return 
                        _return_arg<return_type>(
                            ::stdex::detail::functional_std::_forward<return_type>::call(
                                fx(_arg<arg0_type, 0>::value) ) 
                        );
                    }
                    _return_arg<return_type> operator()(non_void_member_func fx)
                    {
                        using ::stdex::detail::_arg;
                        using ::stdex::detail::_return_arg;

                        return 
                        _return_arg<return_type>(
                            ::stdex::detail::functional_std::_forward<return_type>::call(
                                stdex::invoke(fx, _arg<arg0_type, 0>::value) )
                        );
                    }

                    _return_arg<return_type> operator()(void_plain_func fx)
                    {
                        using ::stdex::detail::_arg;
                        using ::stdex::detail::_return_arg;

                        fx(_arg<arg0_type, 0>::value);
                        _return_arg<return_type> _result = 0;
                        return  _result;
                    }
                    _return_arg<return_type> operator()(void_member_func fx)
                    {
                        using ::stdex::detail::_arg;
                        using ::stdex::detail::_return_arg;

                        stdex::invoke(fx, _arg<arg0_type, 0>::value);
                        _return_arg<return_type> _result = 0;
                        return  _result;
                    }
                };
                
                _functor _f = res;
                return _f(fx);
            }

            template<class _R, class _FuncT, class _ArgT0, class _ArgT1, class _ResArgsT>
            static _return_arg<_R> func(_FuncT &fx, _arg<_ArgT0, 0>&, _arg<_ArgT1, 1>&, _ResArgsT &res)
            {
                struct _functor: _ResArgsT
                {
                    typedef _ResArgsT base_type;
                    typedef _ArgT0 arg0_type;
                    typedef _ArgT1 arg1_type;
                    typedef
                    typename 
                    conditional<_arg_is_void<_R>::value, void_type, _R>::type return_type;
                    typedef
                    _func_result<_FuncT, return_type, return_type(&)(_any<1>, _any<1>), return_type(&)(_any<2>, _any<2>), return_type(&)(_any<3>, _any<3>), return_type(&)(_any<4>, _any<4>)> result;
                    typedef typename result::non_void_plain_func  non_void_plain_func;
                    typedef typename result::non_void_member_func non_void_member_func;
                    typedef typename result::void_plain_func      void_plain_func;
                    typedef typename result::void_member_func     void_member_func;


                    _functor(const base_type &other) : base_type(other) {}
                    _return_arg<return_type> operator()(non_void_plain_func fx)
                    {
                        using ::stdex::detail::_arg;
                        using ::stdex::detail::_return_arg;

                        return 
                        _return_arg<return_type>(
                            ::stdex::detail::functional_std::_forward<return_type>::call(
                                fx(_arg<arg0_type, 0>::value, _arg<arg1_type, 1>::value) )
                        );
                    }

                    _return_arg<return_type> operator()(non_void_member_func fx)
                    {
                        using ::stdex::detail::_arg;
                        using ::stdex::detail::_return_arg;

                        return 
                        _return_arg<return_type>(
                            ::stdex::detail::functional_std::_forward<return_type>::call(
                                stdex::invoke(fx, _arg<arg0_type, 0>::value, _arg<arg1_type, 1>::value) )
                        );
                    }

                    _return_arg<return_type> operator()(void_plain_func fx)
                    {
                        using ::stdex::detail::_arg;
                        using ::stdex::detail::_return_arg;

                        fx(_arg<arg0_type, 0>::value, _arg<arg1_type, 1>::value);
                        _return_arg<return_type> result = 0;
                        return  result;
                    }

                    _return_arg<return_type> operator()(void_member_func fx)
                    {
                        using ::stdex::detail::_arg;
                        using ::stdex::detail::_return_arg;

                        stdex::invoke(fx, _arg<arg0_type, 0>::value, _arg<arg1_type, 1>::value);
                        _return_arg<return_type> result = 0;
                        return  result;
                    }

                };
                
                _functor _f = res;
                return _f(fx);
            }
        }

        template<class _R, class _FuncT>
        struct _func_invoker_impl<_R, _FuncT, 1, 1>
        {
            template<class _RawArgsT, class _CheckedArgsT>
            static _return_arg<_R> call(_FuncT &fx, _RawArgsT &args, const _checked_args<_CheckedArgsT>&)
            {
                _CheckedArgsT checked_args(functional_std::move(args));
                return func_invoker_calls::func<_R>(fx, _get_arg<0>(checked_args), checked_args);
            }

            template<class _RawArgsT>
            static _return_arg<_R> call(_FuncT &fx, _RawArgsT &args, const _checked_args<_RawArgsT>&)
            {
                return func_invoker_calls::func<_R>(fx, _get_arg<0>(args), args);
            }
        };


        template<class _R, class _FuncT>
        struct _func_invoker_impl<_R, _FuncT, 2, 2>
        {

            
            template<class _RawArgsT, class _CheckedArgsT>
            static _return_arg<_R> call(_FuncT &fx, _RawArgsT &args, const _checked_args<_CheckedArgsT>&)
            {
                _CheckedArgsT checked_args(functional_std::move(args));
                return 
                    func_invoker_calls::func<_R>(fx, _get_arg<0>(checked_args), _get_arg<1>(checked_args), checked_args);
            }

            template<class _RawArgsT>
            static _return_arg<_R> call(_FuncT &fx, _RawArgsT &args, const _checked_args<_RawArgsT>&)
            {
                return 
                    func_invoker_calls::func<_R>(fx, _get_arg<0>(args), _get_arg<1>(args), args);
            }
        };

        template<class _R, class _FuncT, int _Index, int _Count>
        struct _func_invoker
        {
            template<class _RawArgsT>
            static _return_arg<_R> call(_FuncT &fx, _RawArgsT &args)
            {
                return 
                    _func_invoker_impl<_R, _FuncT, _Index, _Count>::call(fx, args, _checked_args<_RawArgsT>());
            }
        };

        template<class _R, class _FuncT, class _ArgsT>
        _return_arg<_R>& _invoke(_FuncT& fx, _ArgsT& args, _return_arg<_R> &result)
        {
            typedef _func_invoker<_R, _FuncT, 0, _ArgsT::count> invoker;
            result = 
                invoker::call(fx, args).release();
            return result;
        }

        template<class _FuncT, class _ArgsT>
        _return_arg<void>& _invoke(_FuncT& fx, _ArgsT& args, _return_arg<void> &result)
        {
            _func_invoker<void, _FuncT, 0, _ArgsT::count>::call(fx, args);
            return result;
        }
    } // namespace detail

    class bad_function_call : public std::exception { // exception thrown when an empty std::function is called
    public:
        bad_function_call() _STDEX_NOEXCEPT_FUNCTION {}

        virtual const char* what() const _STDEX_NOEXCEPT_FUNCTION {
            // return pointer to message string
            return "bad function call";
        }
    };

    namespace detail{

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

        template<class _ArgsT, class _ArgT>
        struct _make_args_impl<_ArgsT, _ArgT, 0, true>
        {
            typedef _args<void, void_type, 0> args;
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
            _make_args_impl<_args<_ArgsT, _ArgT, _Index>, _NextArgT, _Index + 1, _arg_is_void<_NextArgT>::value == bool(true)>
        { };

        struct _make_args
        { 
            template<class _ArgT>
            struct add :
                _make_args_impl<void, _ArgT, 0, _arg_is_void<_ArgT>::value> {};
        };

        template<class _FuncT>
        struct _functor_pointer_copy
        {
            typedef
            typename
            conditional<
                is_pointer<_FuncT>::value == bool(true) ||
                is_member_function_pointer<_FuncT>::value == bool(true),
                _FuncT,
                int&
            >::type func_ptr_type;

            typedef
            typename
            conditional<
                is_pointer<_FuncT>::value == bool(false) &&
                is_member_function_pointer<_FuncT>::value == bool(false),
                _FuncT,
                int&
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
                _return_arg<function_return_type> result = 0;
                //return _func_invoker<function_return_type, func_type, 0, function_args_type::count>::call(_func, args);
                return _invoke(_func, args, result);
            }
            virtual void _delete_this() _STDEX_NOEXCEPT_FUNCTION { delete this; }


            virtual std::size_t _target(void* &_dst) const _STDEX_NOEXCEPT_FUNCTION
            {
                return _functor_pointer_copy<func_type>::call(_dst, _func);
            }

            virtual const std::type_info& _target_type() const _STDEX_NOEXCEPT_FUNCTION
            {
                return typeid(typename remove_pointer<func_type>::type);
            }

            func_type _func;
        };

        template<
            class _R, 
#define _STDEX_DELIM ,
            _STDEX_TMPL_ARGS31(_STDEX_BLANK, = void_type)
#undef _STDEX_DELIM
        >
        class _function_impl
        {
            typedef
                typename _make_args::
#define _STDEX_DELIM ::
                _STDEX_TYPES31(template add <, >::type)::
#undef _STDEX_DELIM
            args _args_type;

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
                virtual std::size_t _target(void*&) const _STDEX_NOEXCEPT_FUNCTION = 0;
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
            _function_impl(_FuncT func)
            {
                _fx = new _functor<_func_base, _FuncT, _args_type>(stdex::detail::functional_std::move(func));
            }

            ~_function_impl()
            {
                delete _fx;
            }

            _return_arg<return_type>
                operator()(
#define _STDEX_DELIM ,
                _STDEX_PARAMS31(_STDEX_BLANK, _STDEX_BLANK, _STDEX_BLANK, = void_type())
#undef _STDEX_DELIM 
                    ) const
            {
                if (!_fx)
                    throw(bad_function_call());

                typedef _args<void,    _Arg0T, 0> args_x1;               

                using stdex::detail::functional_std::move;

                _args_type args =
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
                    void* _ptr_to_ptr = nullptr;
                    std::size_t _size = _fx->_target(_ptr_to_ptr);

                    if (_ptr_to_ptr && _size == sizeof(_FuncT*))
                    {
                        _FuncT *_result = *static_cast<_FuncT**>(_ptr_to_ptr);
                        
                        return _result;
                    }
                }

                return nullptr;
            }

        private:


            _func_base* _fx;
        };

    } // namespace detail

    template<class _Tp>
    class reference_wrapper
    /*{
    public:
        static_assert(is_object_v<_Ty> || is_function_v<_Ty>,
            "reference_wrapper<T> requires T to be an object type or a function type.");

        using type = _Ty;

        template <class _Uty, enable_if_t<conjunction_v<negation<is_same<_Remove_cvref_t<_Uty>, reference_wrapper>>,
                                              _Refwrap_has_ctor_from<_Ty, _Uty>>,
                                  int> = 0>
        _CONSTEXPR20 reference_wrapper(_Uty&& _Val) noexcept(noexcept(_Refwrap_ctor_fun<_Ty>(_STD declval<_Uty>()))) {
            _Ty& _Ref = static_cast<_Uty&&>(_Val);
            _Ptr      = _STD addressof(_Ref);
        }

        _CONSTEXPR20 operator _Ty&() const noexcept {
            return *_Ptr;
        }

        _NODISCARD _CONSTEXPR20 _Ty& get() const noexcept {
            return *_Ptr;
        }

    private:
        _Ty* _Ptr{};

    public:
        template <class... _Types>
        _CONSTEXPR20 auto operator()(_Types&&... _Args) const
            noexcept(noexcept(_STD invoke(*_Ptr, static_cast<_Types&&>(_Args)...))) // strengthened
            -> decltype(_STD invoke(*_Ptr, static_cast<_Types&&>(_Args)...)) {
            return _STD invoke(*_Ptr, static_cast<_Types&&>(_Args)...);
        }
    }*/;

    


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
        function(_FuncT func): base_type(func) { }

        return_type operator()() { 
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

    template<class _R, class _Arg0T>
    class function<_R(*)(_Arg0T)>:
        detail::_function_impl<_R, typename remove_reference<_Arg0T>::type&>
    {
        typedef detail::_function_impl<_R, typename remove_reference<_Arg0T>::type&> base_type;
        typedef typename stdex::remove_pointer<_R(*)(_Arg0T)>::type function_signature;
    public:
        typedef typename base_type::return_type return_type;

        function() _STDEX_NOEXCEPT_FUNCTION : base_type()  {}
        function(stdex::nullptr_t) _STDEX_NOEXCEPT_FUNCTION : base_type(nullptr)  {}

        function(const function& other) : base_type(other) { }
        //function(function&& other) _STDEX_NOEXCEPT_FUNCTION;


        template<class _FuncT>
        function(_FuncT func): base_type(func) { }

        return_type operator()(_Arg0T arg0) { 
            return 
            detail::functional_std::_forward<return_type>::call(
                base_type::operator()(arg0).get() );
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

    template<class _R, class _Arg0T, class _Arg1T>
    class function<_R(*)(_Arg0T, _Arg1T)>:
        detail::_function_impl<_R,
            typename remove_reference<_Arg0T>::type&, typename remove_reference<_Arg1T>::type&>
    {
        typedef detail::_function_impl<_R,
            typename remove_reference<_Arg0T>::type&, typename remove_reference<_Arg1T>::type&> base_type;
        typedef typename stdex::remove_pointer<_R(*)(
            _Arg0T, _Arg1T)>::type function_signature;
    public:
        typedef typename base_type::return_type return_type;

        function() _STDEX_NOEXCEPT_FUNCTION : base_type()  {}
        function(stdex::nullptr_t) _STDEX_NOEXCEPT_FUNCTION : base_type(nullptr)  {}

        function(const function& other) : base_type(other) { }
        //function(function&& other) _STDEX_NOEXCEPT_FUNCTION;


        template<class _FuncT>
        function(_FuncT func): base_type(func) { }

        return_type operator()(_Arg0T arg0, _Arg1T arg1) { 
            return 
            detail::functional_std::_forward<return_type>::call(
                base_type::operator()(arg0, arg1).get() ); 
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

#undef _STDEX_BLANK

#undef _STDEX_TMPL_ARGS_IMPL
#undef _STDEX_TYPES_IMPL
#undef _STDEX_PARAMS_IMPL
#undef _STDEX_ARGS_IMPL

#undef _STDEX_TMPL_ARGS
#undef _STDEX_TYPES
#undef _STDEX_PARAMS
#undef _STDEX_ARGS

#undef _STDEX_INVOKE 
#undef _STDEX_INVOKE_IMPL

#undef       _STDEX_TMPL_ARGS0
#undef           _STDEX_TYPES0
#undef          _STDEX_PARAMS0
#undef            _STDEX_ARGS0
#undef       _STDEX_TMPL_ARGS1
#undef           _STDEX_TYPES1
#undef          _STDEX_PARAMS1
#undef            _STDEX_ARGS1
#undef       _STDEX_TMPL_ARGS2
#undef           _STDEX_TYPES2
#undef          _STDEX_PARAMS2
#undef            _STDEX_ARGS2
#undef       _STDEX_TMPL_ARGS3
#undef           _STDEX_TYPES3
#undef          _STDEX_PARAMS3
#undef            _STDEX_ARGS3
#undef       _STDEX_TMPL_ARGS4
#undef           _STDEX_TYPES4
#undef          _STDEX_PARAMS4
#undef            _STDEX_ARGS4
#undef       _STDEX_TMPL_ARGS5
#undef           _STDEX_TYPES5
#undef          _STDEX_PARAMS5
#undef            _STDEX_ARGS5
#undef       _STDEX_TMPL_ARGS6
#undef           _STDEX_TYPES6
#undef          _STDEX_PARAMS6
#undef            _STDEX_ARGS6
#undef       _STDEX_TMPL_ARGS7
#undef           _STDEX_TYPES7
#undef          _STDEX_PARAMS7
#undef            _STDEX_ARGS7
#undef       _STDEX_TMPL_ARGS8
#undef           _STDEX_TYPES8
#undef          _STDEX_PARAMS8
#undef            _STDEX_ARGS8
#undef       _STDEX_TMPL_ARGS9
#undef           _STDEX_TYPES9
#undef          _STDEX_PARAMS9
#undef            _STDEX_ARGS9
#undef       _STDEX_TMPL_ARGS10
#undef           _STDEX_TYPES10
#undef          _STDEX_PARAMS10
#undef            _STDEX_ARGS10
#undef       _STDEX_TMPL_ARGS11
#undef           _STDEX_TYPES11
#undef          _STDEX_PARAMS11
#undef            _STDEX_ARGS11
#undef       _STDEX_TMPL_ARGS12
#undef           _STDEX_TYPES12
#undef          _STDEX_PARAMS12
#undef            _STDEX_ARGS12
#undef       _STDEX_TMPL_ARGS13
#undef           _STDEX_TYPES13
#undef          _STDEX_PARAMS13
#undef            _STDEX_ARGS13
#undef       _STDEX_TMPL_ARGS14
#undef           _STDEX_TYPES14
#undef          _STDEX_PARAMS14
#undef            _STDEX_ARGS14
#undef       _STDEX_TMPL_ARGS15
#undef           _STDEX_TYPES15
#undef          _STDEX_PARAMS15
#undef            _STDEX_ARGS15
#undef       _STDEX_TMPL_ARGS16
#undef           _STDEX_TYPES16
#undef          _STDEX_PARAMS16
#undef            _STDEX_ARGS16
#undef       _STDEX_TMPL_ARGS17
#undef           _STDEX_TYPES17
#undef          _STDEX_PARAMS17
#undef            _STDEX_ARGS17
#undef       _STDEX_TMPL_ARGS18
#undef           _STDEX_TYPES18
#undef          _STDEX_PARAMS18
#undef            _STDEX_ARGS18
#undef       _STDEX_TMPL_ARGS19
#undef           _STDEX_TYPES19
#undef          _STDEX_PARAMS19
#undef            _STDEX_ARGS19
#undef       _STDEX_TMPL_ARGS20
#undef           _STDEX_TYPES20
#undef          _STDEX_PARAMS20
#undef            _STDEX_ARGS20
#undef       _STDEX_TMPL_ARGS21
#undef           _STDEX_TYPES21
#undef          _STDEX_PARAMS21
#undef            _STDEX_ARGS21
#undef       _STDEX_TMPL_ARGS22
#undef           _STDEX_TYPES22
#undef          _STDEX_PARAMS22
#undef            _STDEX_ARGS22
#undef       _STDEX_TMPL_ARGS23
#undef           _STDEX_TYPES23
#undef          _STDEX_PARAMS23
#undef            _STDEX_ARGS23
#undef       _STDEX_TMPL_ARGS24
#undef           _STDEX_TYPES24
#undef          _STDEX_PARAMS24
#undef            _STDEX_ARGS24
#undef       _STDEX_TMPL_ARGS25
#undef           _STDEX_TYPES25
#undef          _STDEX_PARAMS25
#undef            _STDEX_ARGS25
#undef       _STDEX_TMPL_ARGS26
#undef           _STDEX_TYPES26
#undef          _STDEX_PARAMS26
#undef            _STDEX_ARGS26
#undef       _STDEX_TMPL_ARGS27
#undef           _STDEX_TYPES27
#undef          _STDEX_PARAMS27
#undef            _STDEX_ARGS27
#undef       _STDEX_TMPL_ARGS28
#undef           _STDEX_TYPES28
#undef          _STDEX_PARAMS28
#undef            _STDEX_ARGS28
#undef       _STDEX_TMPL_ARGS29
#undef           _STDEX_TYPES29
#undef          _STDEX_PARAMS29
#undef            _STDEX_ARGS29
#undef       _STDEX_TMPL_ARGS30
#undef           _STDEX_TYPES30
#undef          _STDEX_PARAMS30
#undef            _STDEX_ARGS30
#undef       _STDEX_TMPL_ARGS31
#undef           _STDEX_TYPES31
#undef          _STDEX_PARAMS31
#undef            _STDEX_ARGS31


#undef _STDEX_DELETED_FUNCTION
#undef _STDEX_NOEXCEPT_FUNCTION

#endif // _STDEX_FUNCTIONAL_H

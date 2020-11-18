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
    template<class _R, _STDEX_TMPL_ARGS##N> \
    _R invoke( _R(*_func)(_STDEX_TYPES##N), _STDEX_FUNC_PARAMS##N)\
    {\
        return\
        detail::functional_std::_forward<_R>::call(\
            _func(_STDEX_ARGS##N) );\
    }\
\
    template<_STDEX_TMPL_ARGS##N> \
    void invoke( void(*_func)(_STDEX_TYPES##N), _STDEX_FUNC_PARAMS##N)\
    {\
        _func(_STDEX_ARGS##N);\
    }\
\
    template<class _R, class _ObjectT, _STDEX_TMPL_ARGS##N>\
    _R invoke( _R(_ObjectT::*_func)(_STDEX_TYPES##N), _ObjectT &_obj, _STDEX_FUNC_PARAMS##N)\
    {\
        return\
        detail::functional_std::_forward<_R>::call(\
            (_obj.*_func)(_STDEX_ARGS##N) );\
    }\
\
    template<class _ObjectT, _STDEX_TMPL_ARGS##N>\
    void invoke( void(_ObjectT::*_func)(_STDEX_TYPES##N), _ObjectT &_obj, _STDEX_FUNC_PARAMS##N)\
    {\
        (_obj.*_func)(_STDEX_ARGS##N);\
    }\
\
    template<class _R, class _ObjectT, _STDEX_TMPL_ARGS##N>\
    _R invoke( _R(_ObjectT::*_func)(_STDEX_TYPES##N) const, const _ObjectT &_obj, _STDEX_FUNC_PARAMS##N)\
    {\
        return\
        detail::functional_std::_forward<_R>::call(\
            (_obj.*_func)(_STDEX_ARGS##N) );\
    }\
\
    template<class _ObjectT, _STDEX_TMPL_ARGS##N>\
    void invoke( void(_ObjectT::*_func)(_STDEX_TYPES##N) const, const _ObjectT &_obj, _STDEX_FUNC_PARAMS##N)\
    {\
        (_obj.*_func)(_STDEX_ARGS##N);\
    }\
\
    template<class _R, class _ObjectT, _STDEX_TMPL_ARGS##N>\
    _R invoke( _R(_ObjectT::*_func)(_STDEX_TYPES##N), _ObjectT *_obj, _STDEX_FUNC_PARAMS##N)\
    {\
        return\
        detail::functional_std::_forward<_R>::call(\
            ((*_obj).*_func)(_STDEX_ARGS##N) );\
    }\
\
    template<class _ObjectT, _STDEX_TMPL_ARGS##N>\
    void invoke( void(_ObjectT::*_func)(_STDEX_TYPES##N), _ObjectT *_obj, _STDEX_FUNC_PARAMS##N)\
    {\
        ((*_obj).*_func)(_STDEX_ARGS##N);\
    }\
\
    template<class _R, class _ObjectT, _STDEX_TMPL_ARGS##N>\
    _R invoke( _R(_ObjectT::*_func)(_STDEX_TYPES##N) const, const _ObjectT *_obj, _STDEX_FUNC_PARAMS##N)\
    {\
        return\
        detail::functional_std::_forward<_R>::call(\
            ((*_obj).*_func)(_STDEX_ARGS##N) );\
    }\
\
    template<class _ObjectT, _STDEX_TMPL_ARGS##N>\
    void invoke( void(_ObjectT::*_func)(_STDEX_TYPES##N) const, const _ObjectT *_obj, _STDEX_FUNC_PARAMS##N)\
    {\
        ((*_obj).*_func)(_STDEX_ARGS##N);\
    }\
\
    template<class _R, class _ObjectT, _STDEX_TMPL_ARGS##N>\
    _R invoke( _R(_ObjectT::*_func)(_STDEX_TYPES##N), reference_wrapper<_ObjectT> &_ref, _STDEX_FUNC_PARAMS##N)\
    {\
        return\
        detail::functional_std::_forward<_R>::call(\
            (_ref.get().*_func)(_STDEX_ARGS##N) );\
    }\
\
    template<class _ObjectT, _STDEX_TMPL_ARGS##N>\
    void invoke( void(_ObjectT::*_func)(_STDEX_TYPES##N), reference_wrapper<_ObjectT> &_ref, _STDEX_FUNC_PARAMS##N)\
    {\
        (_ref.get().*_func)(_STDEX_ARGS##N);\
    }\
\
    template<class _R, class _ObjectT, _STDEX_TMPL_ARGS##N>\
    _R invoke( _R(_ObjectT::*_func)(_STDEX_TYPES##N) const, reference_wrapper<_ObjectT> &_ref, _STDEX_FUNC_PARAMS##N)\
    {\
        return\
        detail::functional_std::_forward<_R>::call(\
            (_ref.get().*_func)(_STDEX_ARGS##N) );\
    }\
\
    template<class _ObjectT, _STDEX_TMPL_ARGS##N>\
    void invoke( void(_ObjectT::*_func)(_STDEX_TYPES##N) const, reference_wrapper<_ObjectT> &_ref, _STDEX_FUNC_PARAMS##N)\
    {\
        (_ref.get().*_func)(_STDEX_ARGS##N);\
    }\
\
    template<class _R, class _FuncT, _STDEX_TMPL_ARGS##N>\
    _R invoke(_FuncT &_func, _STDEX_CLEAR_PARAMS##N)\
    {\
        stdex::function<_FuncT> _f(_func); \
        return\
            _f(_STDEX_ARGS##N);\
    }\


#define _STDEX_INVOKE(N) _STDEX_INVOKE_IMPL(N)

#define _STDEX_TMPL_ARGS_IMPL(N) ,class _Arg##N##T
#define _STDEX_TYPES_IMPL(N) ,_Arg##N##T 
#define _STDEX_FUNC_PARAMS_IMPL(N) ,typename stdex::detail::_dummy_trait<_Arg##N##T>::type arg##N
#define _STDEX_CLEAR_PARAMS_IMPL(N) ,_Arg##N##T arg##N
#define _STDEX_ARGS_IMPL(N) ,arg##N

#undef _STDEX_TMPL_ARGS
#undef _STDEX_TYPES
#undef _STDEX_ARGS
#undef _STDEX_PARAMS

#define _STDEX_TMPL_ARGS(N) _STDEX_TMPL_ARGS_IMPL(N)
#define _STDEX_TYPES(N) _STDEX_TYPES_IMPL(N)
#define _STDEX_FUNC_PARAMS(N) _STDEX_FUNC_PARAMS_IMPL(N)
#define _STDEX_CLEAR_PARAMS(N) _STDEX_CLEAR_PARAMS_IMPL(N)
#define _STDEX_ARGS(N) _STDEX_ARGS_IMPL(N)

#define _STDEX_TMPL_ARGS0 class _Arg0T
#define _STDEX_TYPES0 _Arg0T
#define _STDEX_FUNC_PARAMS0 typename stdex::detail::_dummy_trait<_Arg0T>::type arg0
#define _STDEX_CLEAR_PARAMS0 _Arg0T arg0
#define _STDEX_ARGS0 arg0
        _STDEX_INVOKE(0)

#define _STDEX_TMPL_ARGS1 _STDEX_TMPL_ARGS0 _STDEX_TMPL_ARGS(1)
#define _STDEX_TYPES1 _STDEX_TYPES0 _STDEX_TYPES(1)
#define _STDEX_FUNC_PARAMS1 _STDEX_FUNC_PARAMS0 _STDEX_FUNC_PARAMS(1)
#define _STDEX_CLEAR_PARAMS1 _STDEX_CLEAR_PARAMS0 _STDEX_CLEAR_PARAMS(1)
#define _STDEX_ARGS1 _STDEX_ARGS0 _STDEX_ARGS(1)
            _STDEX_INVOKE(1)

#define _STDEX_TMPL_ARGS2 _STDEX_TMPL_ARGS1 _STDEX_TMPL_ARGS(2)
#define _STDEX_TYPES2 _STDEX_TYPES1 _STDEX_TYPES(2)
#define _STDEX_FUNC_PARAMS2 _STDEX_FUNC_PARAMS1 _STDEX_FUNC_PARAMS(2)
#define _STDEX_CLEAR_PARAMS2 _STDEX_CLEAR_PARAMS1 _STDEX_CLEAR_PARAMS(2)
#define _STDEX_ARGS2 _STDEX_ARGS1 _STDEX_ARGS(2)
            _STDEX_INVOKE(2)

#define _STDEX_TMPL_ARGS3 _STDEX_TMPL_ARGS2 _STDEX_TMPL_ARGS(3)
#define _STDEX_TYPES3 _STDEX_TYPES2 _STDEX_TYPES(3)
#define _STDEX_FUNC_PARAMS3 _STDEX_FUNC_PARAMS2 _STDEX_FUNC_PARAMS(3)
#define _STDEX_CLEAR_PARAMS3 _STDEX_CLEAR_PARAMS2 _STDEX_CLEAR_PARAMS(3)
#define _STDEX_ARGS3 _STDEX_ARGS2 _STDEX_ARGS(3)
            _STDEX_INVOKE(3)

#define _STDEX_TMPL_ARGS4 _STDEX_TMPL_ARGS3 _STDEX_TMPL_ARGS(4)
#define _STDEX_TYPES4 _STDEX_TYPES3 _STDEX_TYPES(4)
#define _STDEX_FUNC_PARAMS4 _STDEX_FUNC_PARAMS3 _STDEX_FUNC_PARAMS(4)
#define _STDEX_CLEAR_PARAMS4 _STDEX_CLEAR_PARAMS3 _STDEX_CLEAR_PARAMS(4)
#define _STDEX_ARGS4 _STDEX_ARGS3 _STDEX_ARGS(4)
            _STDEX_INVOKE(4)

#define _STDEX_TMPL_ARGS5 _STDEX_TMPL_ARGS4 _STDEX_TMPL_ARGS(5)
#define _STDEX_TYPES5 _STDEX_TYPES4 _STDEX_TYPES(5)
#define _STDEX_FUNC_PARAMS5 _STDEX_FUNC_PARAMS4 _STDEX_FUNC_PARAMS(5)
#define _STDEX_CLEAR_PARAMS5 _STDEX_CLEAR_PARAMS4 _STDEX_CLEAR_PARAMS(5)
#define _STDEX_ARGS5 _STDEX_ARGS4 _STDEX_ARGS(5)
            _STDEX_INVOKE(5)

#define _STDEX_TMPL_ARGS6 _STDEX_TMPL_ARGS5 _STDEX_TMPL_ARGS(6)
#define _STDEX_TYPES6 _STDEX_TYPES5 _STDEX_TYPES(6)
#define _STDEX_FUNC_PARAMS6 _STDEX_FUNC_PARAMS5 _STDEX_FUNC_PARAMS(6)
#define _STDEX_CLEAR_PARAMS6 _STDEX_CLEAR_PARAMS5 _STDEX_CLEAR_PARAMS(6)
#define _STDEX_ARGS6 _STDEX_ARGS5 _STDEX_ARGS(6)
            _STDEX_INVOKE(6)

#define _STDEX_TMPL_ARGS7 _STDEX_TMPL_ARGS6 _STDEX_TMPL_ARGS(7)
#define _STDEX_TYPES7 _STDEX_TYPES6 _STDEX_TYPES(7)
#define _STDEX_FUNC_PARAMS7 _STDEX_FUNC_PARAMS6 _STDEX_FUNC_PARAMS(7)
#define _STDEX_CLEAR_PARAMS7 _STDEX_CLEAR_PARAMS6 _STDEX_CLEAR_PARAMS(7)
#define _STDEX_ARGS7 _STDEX_ARGS6 _STDEX_ARGS(7)
            _STDEX_INVOKE(7)

#define _STDEX_TMPL_ARGS8 _STDEX_TMPL_ARGS7 _STDEX_TMPL_ARGS(8)
#define _STDEX_TYPES8 _STDEX_TYPES7 _STDEX_TYPES(8)
#define _STDEX_FUNC_PARAMS8 _STDEX_FUNC_PARAMS7 _STDEX_FUNC_PARAMS(8)
#define _STDEX_CLEAR_PARAMS8 _STDEX_CLEAR_PARAMS7 _STDEX_CLEAR_PARAMS(8)
#define _STDEX_ARGS8 _STDEX_ARGS7 _STDEX_ARGS(8)
            _STDEX_INVOKE(8)

#define _STDEX_TMPL_ARGS9 _STDEX_TMPL_ARGS8 _STDEX_TMPL_ARGS(9)
#define _STDEX_TYPES9 _STDEX_TYPES8 _STDEX_TYPES(9)
#define _STDEX_FUNC_PARAMS9 _STDEX_FUNC_PARAMS8 _STDEX_FUNC_PARAMS(9)
#define _STDEX_CLEAR_PARAMS9 _STDEX_CLEAR_PARAMS8 _STDEX_CLEAR_PARAMS(9)
#define _STDEX_ARGS9 _STDEX_ARGS8 _STDEX_ARGS(9)
            _STDEX_INVOKE(9)

#define _STDEX_TMPL_ARGS10 _STDEX_TMPL_ARGS9 _STDEX_TMPL_ARGS(10)
#define _STDEX_TYPES10 _STDEX_TYPES9 _STDEX_TYPES(10)
#define _STDEX_FUNC_PARAMS10 _STDEX_FUNC_PARAMS9 _STDEX_FUNC_PARAMS(10)
#define _STDEX_CLEAR_PARAMS10 _STDEX_CLEAR_PARAMS9 _STDEX_CLEAR_PARAMS(10)
#define _STDEX_ARGS10 _STDEX_ARGS9 _STDEX_ARGS(10)
            _STDEX_INVOKE(10)

#define _STDEX_TMPL_ARGS11 _STDEX_TMPL_ARGS10 _STDEX_TMPL_ARGS(11)
#define _STDEX_TYPES11 _STDEX_TYPES10 _STDEX_TYPES(11)
#define _STDEX_FUNC_PARAMS11 _STDEX_FUNC_PARAMS10 _STDEX_FUNC_PARAMS(11)
#define _STDEX_CLEAR_PARAMS11 _STDEX_CLEAR_PARAMS10 _STDEX_CLEAR_PARAMS(11)
#define _STDEX_ARGS11 _STDEX_ARGS10 _STDEX_ARGS(11)
            _STDEX_INVOKE(11)

#define _STDEX_TMPL_ARGS12 _STDEX_TMPL_ARGS11 _STDEX_TMPL_ARGS(12)
#define _STDEX_TYPES12 _STDEX_TYPES11 _STDEX_TYPES(12)
#define _STDEX_FUNC_PARAMS12 _STDEX_FUNC_PARAMS11 _STDEX_FUNC_PARAMS(12)
#define _STDEX_CLEAR_PARAMS12 _STDEX_CLEAR_PARAMS11 _STDEX_CLEAR_PARAMS(12)
#define _STDEX_ARGS12 _STDEX_ARGS11 _STDEX_ARGS(12)
            _STDEX_INVOKE(12)

#define _STDEX_TMPL_ARGS13 _STDEX_TMPL_ARGS12 _STDEX_TMPL_ARGS(13)
#define _STDEX_TYPES13 _STDEX_TYPES12 _STDEX_TYPES(13)
#define _STDEX_FUNC_PARAMS13 _STDEX_FUNC_PARAMS12 _STDEX_FUNC_PARAMS(13)
#define _STDEX_CLEAR_PARAMS13 _STDEX_CLEAR_PARAMS12 _STDEX_CLEAR_PARAMS(13)
#define _STDEX_ARGS13 _STDEX_ARGS12 _STDEX_ARGS(13)
            _STDEX_INVOKE(13)

#define _STDEX_TMPL_ARGS14 _STDEX_TMPL_ARGS13 _STDEX_TMPL_ARGS(14)
#define _STDEX_TYPES14 _STDEX_TYPES13 _STDEX_TYPES(14)
#define _STDEX_FUNC_PARAMS14 _STDEX_FUNC_PARAMS13 _STDEX_FUNC_PARAMS(14)
#define _STDEX_CLEAR_PARAMS14 _STDEX_CLEAR_PARAMS13 _STDEX_CLEAR_PARAMS(14)
#define _STDEX_ARGS14 _STDEX_ARGS13 _STDEX_ARGS(14)
            _STDEX_INVOKE(14)

#define _STDEX_TMPL_ARGS15 _STDEX_TMPL_ARGS14 _STDEX_TMPL_ARGS(15)
#define _STDEX_TYPES15 _STDEX_TYPES14 _STDEX_TYPES(15)
#define _STDEX_FUNC_PARAMS15 _STDEX_FUNC_PARAMS14 _STDEX_FUNC_PARAMS(15)
#define _STDEX_CLEAR_PARAMS15 _STDEX_CLEAR_PARAMS14 _STDEX_CLEAR_PARAMS(15)
#define _STDEX_ARGS15 _STDEX_ARGS14 _STDEX_ARGS(15)
            _STDEX_INVOKE(15)

#define _STDEX_TMPL_ARGS16 _STDEX_TMPL_ARGS15 _STDEX_TMPL_ARGS(16)
#define _STDEX_TYPES16 _STDEX_TYPES15 _STDEX_TYPES(16)
#define _STDEX_FUNC_PARAMS16 _STDEX_FUNC_PARAMS15 _STDEX_FUNC_PARAMS(16)
#define _STDEX_CLEAR_PARAMS16 _STDEX_CLEAR_PARAMS15 _STDEX_CLEAR_PARAMS(16)
#define _STDEX_ARGS16 _STDEX_ARGS15 _STDEX_ARGS(16)
            _STDEX_INVOKE(16)

#define _STDEX_TMPL_ARGS17 _STDEX_TMPL_ARGS16 _STDEX_TMPL_ARGS(17)
#define _STDEX_TYPES17 _STDEX_TYPES16 _STDEX_TYPES(17)
#define _STDEX_FUNC_PARAMS17 _STDEX_FUNC_PARAMS16 _STDEX_FUNC_PARAMS(17)
#define _STDEX_CLEAR_PARAMS17 _STDEX_CLEAR_PARAMS16 _STDEX_CLEAR_PARAMS(17)
#define _STDEX_ARGS17 _STDEX_ARGS16 _STDEX_ARGS(17)
            _STDEX_INVOKE(17)

#define _STDEX_TMPL_ARGS18 _STDEX_TMPL_ARGS17 _STDEX_TMPL_ARGS(18)
#define _STDEX_TYPES18 _STDEX_TYPES17 _STDEX_TYPES(18)
#define _STDEX_FUNC_PARAMS18 _STDEX_FUNC_PARAMS17 _STDEX_FUNC_PARAMS(18)
#define _STDEX_CLEAR_PARAMS18 _STDEX_CLEAR_PARAMS17 _STDEX_CLEAR_PARAMS(18)
#define _STDEX_ARGS18 _STDEX_ARGS17 _STDEX_ARGS(18)
            _STDEX_INVOKE(18)

#define _STDEX_TMPL_ARGS19 _STDEX_TMPL_ARGS18 _STDEX_TMPL_ARGS(19)
#define _STDEX_TYPES19 _STDEX_TYPES18 _STDEX_TYPES(19)
#define _STDEX_FUNC_PARAMS19 _STDEX_FUNC_PARAMS18 _STDEX_FUNC_PARAMS(19)
#define _STDEX_CLEAR_PARAMS19 _STDEX_CLEAR_PARAMS18 _STDEX_CLEAR_PARAMS(19)
#define _STDEX_ARGS19 _STDEX_ARGS18 _STDEX_ARGS(19)
            _STDEX_INVOKE(19)

#define _STDEX_TMPL_ARGS20 _STDEX_TMPL_ARGS19 _STDEX_TMPL_ARGS(20)
#define _STDEX_TYPES20 _STDEX_TYPES19 _STDEX_TYPES(20)
#define _STDEX_FUNC_PARAMS20 _STDEX_FUNC_PARAMS19 _STDEX_FUNC_PARAMS(20)
#define _STDEX_CLEAR_PARAMS20 _STDEX_CLEAR_PARAMS19 _STDEX_CLEAR_PARAMS(20)
#define _STDEX_ARGS20 _STDEX_ARGS19 _STDEX_ARGS(20)
            _STDEX_INVOKE(20)

#define _STDEX_TMPL_ARGS21 _STDEX_TMPL_ARGS20 _STDEX_TMPL_ARGS(21)
#define _STDEX_TYPES21 _STDEX_TYPES20 _STDEX_TYPES(21)
#define _STDEX_FUNC_PARAMS21 _STDEX_FUNC_PARAMS20 _STDEX_FUNC_PARAMS(21)
#define _STDEX_CLEAR_PARAMS21 _STDEX_CLEAR_PARAMS20 _STDEX_CLEAR_PARAMS(21)
#define _STDEX_ARGS21 _STDEX_ARGS20 _STDEX_ARGS(21)
            _STDEX_INVOKE(21)

#define _STDEX_TMPL_ARGS22 _STDEX_TMPL_ARGS21 _STDEX_TMPL_ARGS(22)
#define _STDEX_TYPES22 _STDEX_TYPES21 _STDEX_TYPES(22)
#define _STDEX_FUNC_PARAMS22 _STDEX_FUNC_PARAMS21 _STDEX_FUNC_PARAMS(22)
#define _STDEX_CLEAR_PARAMS22 _STDEX_CLEAR_PARAMS21 _STDEX_CLEAR_PARAMS(22)
#define _STDEX_ARGS22 _STDEX_ARGS21 _STDEX_ARGS(22)
            _STDEX_INVOKE(22)

#define _STDEX_TMPL_ARGS23 _STDEX_TMPL_ARGS22 _STDEX_TMPL_ARGS(23)
#define _STDEX_TYPES23 _STDEX_TYPES22 _STDEX_TYPES(23)
#define _STDEX_FUNC_PARAMS23 _STDEX_FUNC_PARAMS22 _STDEX_FUNC_PARAMS(23)
#define _STDEX_CLEAR_PARAMS23 _STDEX_CLEAR_PARAMS22 _STDEX_CLEAR_PARAMS(23)
#define _STDEX_ARGS23 _STDEX_ARGS22 _STDEX_ARGS(23)
            _STDEX_INVOKE(23)

#define _STDEX_TMPL_ARGS24 _STDEX_TMPL_ARGS23 _STDEX_TMPL_ARGS(24)
#define _STDEX_TYPES24 _STDEX_TYPES23 _STDEX_TYPES(24)
#define _STDEX_FUNC_PARAMS24 _STDEX_FUNC_PARAMS23 _STDEX_FUNC_PARAMS(24)
#define _STDEX_CLEAR_PARAMS24 _STDEX_CLEAR_PARAMS23 _STDEX_CLEAR_PARAMS(24)
#define _STDEX_ARGS24 _STDEX_ARGS23 _STDEX_ARGS(24)
            _STDEX_INVOKE(24)

#define _STDEX_TMPL_ARGS25 _STDEX_TMPL_ARGS24 _STDEX_TMPL_ARGS(25)
#define _STDEX_TYPES25 _STDEX_TYPES24 _STDEX_TYPES(25)
#define _STDEX_FUNC_PARAMS25 _STDEX_FUNC_PARAMS24 _STDEX_FUNC_PARAMS(25)
#define _STDEX_CLEAR_PARAMS25 _STDEX_CLEAR_PARAMS24 _STDEX_CLEAR_PARAMS(25)
#define _STDEX_ARGS25 _STDEX_ARGS24 _STDEX_ARGS(25)
            _STDEX_INVOKE(25)

#define _STDEX_TMPL_ARGS26 _STDEX_TMPL_ARGS25 _STDEX_TMPL_ARGS(26)
#define _STDEX_TYPES26 _STDEX_TYPES25 _STDEX_TYPES(26)
#define _STDEX_FUNC_PARAMS26 _STDEX_FUNC_PARAMS25 _STDEX_FUNC_PARAMS(26)
#define _STDEX_CLEAR_PARAMS26 _STDEX_CLEAR_PARAMS25 _STDEX_CLEAR_PARAMS(26)
#define _STDEX_ARGS26 _STDEX_ARGS25 _STDEX_ARGS(26)
            _STDEX_INVOKE(26)

#define _STDEX_TMPL_ARGS27 _STDEX_TMPL_ARGS26 _STDEX_TMPL_ARGS(27)
#define _STDEX_TYPES27 _STDEX_TYPES26 _STDEX_TYPES(27)
#define _STDEX_FUNC_PARAMS27 _STDEX_FUNC_PARAMS26 _STDEX_FUNC_PARAMS(27)
#define _STDEX_CLEAR_PARAMS27 _STDEX_CLEAR_PARAMS26 _STDEX_CLEAR_PARAMS(27)
#define _STDEX_ARGS27 _STDEX_ARGS26 _STDEX_ARGS(27)
            _STDEX_INVOKE(27)

#define _STDEX_TMPL_ARGS28 _STDEX_TMPL_ARGS27 _STDEX_TMPL_ARGS(28)
#define _STDEX_TYPES28 _STDEX_TYPES27 _STDEX_TYPES(28)
#define _STDEX_FUNC_PARAMS28 _STDEX_FUNC_PARAMS27 _STDEX_FUNC_PARAMS(28)
#define _STDEX_CLEAR_PARAMS28 _STDEX_CLEAR_PARAMS27 _STDEX_CLEAR_PARAMS(28)
#define _STDEX_ARGS28 _STDEX_ARGS27 _STDEX_ARGS(28)
            _STDEX_INVOKE(28)

#define _STDEX_TMPL_ARGS29 _STDEX_TMPL_ARGS28 _STDEX_TMPL_ARGS(29)
#define _STDEX_TYPES29 _STDEX_TYPES28 _STDEX_TYPES(29)
#define _STDEX_FUNC_PARAMS29 _STDEX_FUNC_PARAMS28 _STDEX_FUNC_PARAMS(29)
#define _STDEX_CLEAR_PARAMS29 _STDEX_CLEAR_PARAMS28 _STDEX_CLEAR_PARAMS(29)
#define _STDEX_ARGS29 _STDEX_ARGS28 _STDEX_ARGS(29)
            _STDEX_INVOKE(29)

#define _STDEX_TMPL_ARGS30 _STDEX_TMPL_ARGS29 _STDEX_TMPL_ARGS(30)
#define _STDEX_TYPES30 _STDEX_TYPES29 _STDEX_TYPES(30)
#define _STDEX_FUNC_PARAMS30 _STDEX_FUNC_PARAMS29 _STDEX_FUNC_PARAMS(30)
#define _STDEX_CLEAR_PARAMS30 _STDEX_CLEAR_PARAMS29 _STDEX_CLEAR_PARAMS(30)
#define _STDEX_ARGS30 _STDEX_ARGS29 _STDEX_ARGS(30)
            _STDEX_INVOKE(30)

#define _STDEX_TMPL_ARGS31 _STDEX_TMPL_ARGS30 _STDEX_TMPL_ARGS(31)
#define _STDEX_TYPES31 _STDEX_TYPES30 _STDEX_TYPES(31)
#define _STDEX_FUNC_PARAMS31 _STDEX_FUNC_PARAMS30 _STDEX_FUNC_PARAMS(31)
#define _STDEX_CLEAR_PARAMS31 _STDEX_CLEAR_PARAMS30 _STDEX_CLEAR_PARAMS(31)
#define _STDEX_ARGS31 _STDEX_ARGS30 _STDEX_ARGS(31)
            _STDEX_INVOKE(31)
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

        template<class _ArgT>
        struct _make_args:
            _make_args_impl<void, _ArgT, 0, _arg_is_void<_ArgT>::value>
        { };

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

            static std::size_t func_ptr_copy(void* &_dst,
                typename
                conditional<
                    is_pointer<func_type>::value == bool(true) ||
                    is_member_function_pointer<func_type>::value == bool(true),
                    func_type,
                    class _dummy>::type _src)
            {
                func_type* _ptr_to_ptr = new func_type();
                (*_ptr_to_ptr) = _src;
                _dst = _ptr_to_ptr;
                return sizeof(func_type);
            }

            static std::size_t func_ptr_copy(void* &_dst,
                typename
                conditional<
                    is_pointer<func_type>::value,
                    class _dummy,
                    func_type&>::type _src)
            {
                func_type** _ptr_to_ptr = new func_type*();
                (*_ptr_to_ptr) = &_src;
                _dst = _ptr_to_ptr;
                return sizeof(func_type);
            }

            virtual std::size_t _target(void* &_dst) const _STDEX_NOEXCEPT_FUNCTION
            {
                return 0;//func_ptr_copy(_dst, _func);
            }

            virtual const std::type_info& _target_type() const _STDEX_NOEXCEPT_FUNCTION
            {
                return typeid(typename remove_pointer<func_type>::type);
            }

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
        class _function_impl
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

#undef _STDEX_TMPL_ARGS_IMPL
#undef _STDEX_TYPES_IMPL
#undef _STDEX_FUNC_PARAMS_IMPL
#undef _STDEX_CLEAR_PARAMS_IMPL
#undef _STDEX_ARGS_IMPL

#undef _STDEX_TMPL_ARGS
#undef _STDEX_TYPES
#undef _STDEX_ARGS
#undef _STDEX_PARAMS

#undef _STDEX_INVOKE 
#undef _STDEX_INVOKE_IMPL

#undef       _STDEX_TMPL_ARGS1
#undef           _STDEX_TYPES1
#undef     _STDEX_FUNC_PARAMS1
#undef    _STDEX_CLEAR_PARAMS1
#undef            _STDEX_ARGS1
#undef       _STDEX_TMPL_ARGS2
#undef           _STDEX_TYPES2
#undef     _STDEX_FUNC_PARAMS2
#undef    _STDEX_CLEAR_PARAMS2
#undef            _STDEX_ARGS2
#undef       _STDEX_TMPL_ARGS3
#undef           _STDEX_TYPES3
#undef     _STDEX_FUNC_PARAMS3
#undef    _STDEX_CLEAR_PARAMS3
#undef            _STDEX_ARGS3
#undef       _STDEX_TMPL_ARGS4
#undef           _STDEX_TYPES4
#undef     _STDEX_FUNC_PARAMS4
#undef    _STDEX_CLEAR_PARAMS4
#undef            _STDEX_ARGS4
#undef       _STDEX_TMPL_ARGS5
#undef           _STDEX_TYPES5
#undef     _STDEX_FUNC_PARAMS5
#undef    _STDEX_CLEAR_PARAMS5
#undef            _STDEX_ARGS5
#undef       _STDEX_TMPL_ARGS6
#undef           _STDEX_TYPES6
#undef     _STDEX_FUNC_PARAMS6
#undef    _STDEX_CLEAR_PARAMS6
#undef            _STDEX_ARGS6
#undef       _STDEX_TMPL_ARGS7
#undef           _STDEX_TYPES7
#undef     _STDEX_FUNC_PARAMS7
#undef    _STDEX_CLEAR_PARAMS7
#undef            _STDEX_ARGS7
#undef       _STDEX_TMPL_ARGS8
#undef           _STDEX_TYPES8
#undef     _STDEX_FUNC_PARAMS8
#undef    _STDEX_CLEAR_PARAMS8
#undef            _STDEX_ARGS8
#undef       _STDEX_TMPL_ARGS9
#undef           _STDEX_TYPES9
#undef     _STDEX_FUNC_PARAMS9
#undef    _STDEX_CLEAR_PARAMS9
#undef            _STDEX_ARGS9
#undef       _STDEX_TMPL_ARGS10
#undef           _STDEX_TYPES10
#undef     _STDEX_FUNC_PARAMS10
#undef    _STDEX_CLEAR_PARAMS10
#undef            _STDEX_ARGS10
#undef       _STDEX_TMPL_ARGS11
#undef           _STDEX_TYPES11
#undef     _STDEX_FUNC_PARAMS11
#undef    _STDEX_CLEAR_PARAMS11
#undef            _STDEX_ARGS11
#undef       _STDEX_TMPL_ARGS12
#undef           _STDEX_TYPES12
#undef     _STDEX_FUNC_PARAMS12
#undef    _STDEX_CLEAR_PARAMS12
#undef            _STDEX_ARGS12
#undef       _STDEX_TMPL_ARGS13
#undef           _STDEX_TYPES13
#undef     _STDEX_FUNC_PARAMS13
#undef    _STDEX_CLEAR_PARAMS13
#undef            _STDEX_ARGS13
#undef       _STDEX_TMPL_ARGS14
#undef           _STDEX_TYPES14
#undef     _STDEX_FUNC_PARAMS14
#undef    _STDEX_CLEAR_PARAMS14
#undef            _STDEX_ARGS14
#undef       _STDEX_TMPL_ARGS15
#undef           _STDEX_TYPES15
#undef     _STDEX_FUNC_PARAMS15
#undef    _STDEX_CLEAR_PARAMS15
#undef            _STDEX_ARGS15
#undef       _STDEX_TMPL_ARGS16
#undef           _STDEX_TYPES16
#undef     _STDEX_FUNC_PARAMS16
#undef    _STDEX_CLEAR_PARAMS16
#undef            _STDEX_ARGS16
#undef       _STDEX_TMPL_ARGS17
#undef           _STDEX_TYPES17
#undef     _STDEX_FUNC_PARAMS17
#undef    _STDEX_CLEAR_PARAMS17
#undef            _STDEX_ARGS17
#undef       _STDEX_TMPL_ARGS18
#undef           _STDEX_TYPES18
#undef     _STDEX_FUNC_PARAMS18
#undef    _STDEX_CLEAR_PARAMS18
#undef            _STDEX_ARGS18
#undef       _STDEX_TMPL_ARGS19
#undef           _STDEX_TYPES19
#undef     _STDEX_FUNC_PARAMS19
#undef    _STDEX_CLEAR_PARAMS19
#undef            _STDEX_ARGS19
#undef       _STDEX_TMPL_ARGS20
#undef           _STDEX_TYPES20
#undef     _STDEX_FUNC_PARAMS20
#undef    _STDEX_CLEAR_PARAMS20
#undef            _STDEX_ARGS20
#undef       _STDEX_TMPL_ARGS21
#undef           _STDEX_TYPES21
#undef     _STDEX_FUNC_PARAMS21
#undef    _STDEX_CLEAR_PARAMS21
#undef            _STDEX_ARGS21
#undef       _STDEX_TMPL_ARGS22
#undef           _STDEX_TYPES22
#undef     _STDEX_FUNC_PARAMS22
#undef    _STDEX_CLEAR_PARAMS22
#undef            _STDEX_ARGS22
#undef       _STDEX_TMPL_ARGS23
#undef           _STDEX_TYPES23
#undef     _STDEX_FUNC_PARAMS23
#undef    _STDEX_CLEAR_PARAMS23
#undef            _STDEX_ARGS23
#undef       _STDEX_TMPL_ARGS24
#undef           _STDEX_TYPES24
#undef     _STDEX_FUNC_PARAMS24
#undef    _STDEX_CLEAR_PARAMS24
#undef            _STDEX_ARGS24
#undef       _STDEX_TMPL_ARGS25
#undef           _STDEX_TYPES25
#undef     _STDEX_FUNC_PARAMS25
#undef    _STDEX_CLEAR_PARAMS25
#undef            _STDEX_ARGS25
#undef       _STDEX_TMPL_ARGS26
#undef           _STDEX_TYPES26
#undef     _STDEX_FUNC_PARAMS26
#undef    _STDEX_CLEAR_PARAMS26
#undef            _STDEX_ARGS26
#undef       _STDEX_TMPL_ARGS27
#undef           _STDEX_TYPES27
#undef     _STDEX_FUNC_PARAMS27
#undef    _STDEX_CLEAR_PARAMS27
#undef            _STDEX_ARGS27
#undef       _STDEX_TMPL_ARGS28
#undef           _STDEX_TYPES28
#undef     _STDEX_FUNC_PARAMS28
#undef    _STDEX_CLEAR_PARAMS28
#undef            _STDEX_ARGS28
#undef       _STDEX_TMPL_ARGS29
#undef           _STDEX_TYPES29
#undef     _STDEX_FUNC_PARAMS29
#undef    _STDEX_CLEAR_PARAMS29
#undef            _STDEX_ARGS29
#undef       _STDEX_TMPL_ARGS30
#undef           _STDEX_TYPES30
#undef     _STDEX_FUNC_PARAMS30
#undef    _STDEX_CLEAR_PARAMS30
#undef            _STDEX_ARGS30
#undef       _STDEX_TMPL_ARGS31
#undef           _STDEX_TYPES31
#undef     _STDEX_FUNC_PARAMS31
#undef    _STDEX_CLEAR_PARAMS31
#undef            _STDEX_ARGS31

#undef _STDEX_DELETED_FUNCTION
#undef _STDEX_NOEXCEPT_FUNCTION

#endif // _STDEX_FUNCTIONAL_H

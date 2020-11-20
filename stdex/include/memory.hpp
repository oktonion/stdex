#ifndef _STDEX_MEMORY_H
#define _STDEX_MEMORY_H
 
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// stdex includes

#define _STDEX_PARAMETER_PACK_DEFINE
#include "./parameter_pack.h"
#undef _STDEX_PARAMETER_PACK_DEFINE

// POSIX includes

// std includes
#include <memory>

#ifdef _STDEX_NATIVE_CPP11_SUPPORT

#define _STDEX_DELETED_FUNCTION =delete
#define _STDEX_NOEXCEPT_FUNCTION noexcept

#else

#define _STDEX_DELETED_FUNCTION 
#define _STDEX_NOEXCEPT_FUNCTION throw()

#endif

namespace stdex
{
    namespace memory_cpp11
    {
#ifndef STDEX_DO_NOT_ADD_CPP11_STD // define to exclude std implementations
        using namespace std;
#endif
    }

    namespace memory_detail
    {
        struct _has_addressof
        {
            static const bool value =
                false; // idk how to find templated function in another namespace
        };
    }

    namespace detail
    {
        template<bool>
        struct _addressof_impl
        {
            template<class _Tp>
            static _Tp* call(_Tp &ref) _STDEX_NOEXCEPT_FUNCTION
            {
                return std::addressof(ref);
            }
        };

        template<>
        struct _addressof_impl<false>
        {
            template<class _Tp>
            _Tp* call(_Tp &ref) _STDEX_NOEXCEPT_FUNCTION
            {
                struct opaque_type;
                return reinterpret_cast<_Tp*>(&const_cast<opaque_type&>(reinterpret_cast<const volatile opaque_type&>(value)));
            }
        };
    }

    namespace memory_cpp11
    {
        template<class _Tp>
        _Tp* addressof(_Tp& ref) _STDEX_NOEXCEPT_FUNCTION
        {
            return detail::_addressof_impl<memory_detail::_has_addressof::value>::call(ref);
        }
    } // namespace memory_cpp11


    using memory_cpp11::addressof;

    // Pointer traits
    namespace detail
    {
        template <class _Tp>
        struct _get_first_parameter;

#define _STDEX_GET_FIRST_PARAMETER(N) \
        template <template <_STDEX_REPEAT_TOKEN##N(class)> class _Tp, _STDEX_TMPL_ARGS##N(_STDEX_BLANK, _STDEX_BLANK)> \
        struct _get_first_parameter<_Tp<_STDEX_TYPES##N(_STDEX_BLANK, _STDEX_BLANK)>/**/> { \
            typedef _Arg0T type; \
        };

        template <class _NewArg0T, class _Tp>
        struct _replace_first_parameter;

#define _STDEX_REPLACE_FIRST_PARAMETER(N) \
        template <class _NewArg0T, template <_STDEX_REPEAT_TOKEN##N(class)> class _Tp, _STDEX_TMPL_ARGS##N(_STDEX_BLANK, _STDEX_BLANK)> \
        struct _replace_first_parameter<_NewArg0T, _Tp<_STDEX_TYPES##N(_STDEX_BLANK, _STDEX_BLANK)>/**/> { \
            typedef _NewArg0T _Arg0T; \
            typedef _Tp<_STDEX_TYPES##N(_STDEX_BLANK, _STDEX_BLANK)> type;\
        };

#define _STDEX_GET_AND_REPLACE_FIRST_PARAMETER(N)\
        _STDEX_GET_FIRST_PARAMETER(N)\
        _STDEX_REPLACE_FIRST_PARAMETER(N)
        

        _STDEX_GET_AND_REPLACE_FIRST_PARAMETER(0)
        _STDEX_GET_AND_REPLACE_FIRST_PARAMETER(1)
        _STDEX_GET_AND_REPLACE_FIRST_PARAMETER(2)
        _STDEX_GET_AND_REPLACE_FIRST_PARAMETER(3)
        _STDEX_GET_AND_REPLACE_FIRST_PARAMETER(4)
        _STDEX_GET_AND_REPLACE_FIRST_PARAMETER(5)
        _STDEX_GET_AND_REPLACE_FIRST_PARAMETER(6)
        _STDEX_GET_AND_REPLACE_FIRST_PARAMETER(7)
        _STDEX_GET_AND_REPLACE_FIRST_PARAMETER(8)
        _STDEX_GET_AND_REPLACE_FIRST_PARAMETER(9)
        _STDEX_GET_AND_REPLACE_FIRST_PARAMETER(10)
        _STDEX_GET_AND_REPLACE_FIRST_PARAMETER(11)
        _STDEX_GET_AND_REPLACE_FIRST_PARAMETER(12)
        _STDEX_GET_AND_REPLACE_FIRST_PARAMETER(13)
        _STDEX_GET_AND_REPLACE_FIRST_PARAMETER(14)
        _STDEX_GET_AND_REPLACE_FIRST_PARAMETER(15)
        _STDEX_GET_AND_REPLACE_FIRST_PARAMETER(16)
        _STDEX_GET_AND_REPLACE_FIRST_PARAMETER(17)
        _STDEX_GET_AND_REPLACE_FIRST_PARAMETER(18)
        _STDEX_GET_AND_REPLACE_FIRST_PARAMETER(19)
        _STDEX_GET_AND_REPLACE_FIRST_PARAMETER(20)
        _STDEX_GET_AND_REPLACE_FIRST_PARAMETER(21)
        _STDEX_GET_AND_REPLACE_FIRST_PARAMETER(22)
        _STDEX_GET_AND_REPLACE_FIRST_PARAMETER(23)
        _STDEX_GET_AND_REPLACE_FIRST_PARAMETER(24)
        _STDEX_GET_AND_REPLACE_FIRST_PARAMETER(25)
        _STDEX_GET_AND_REPLACE_FIRST_PARAMETER(26)
        _STDEX_GET_AND_REPLACE_FIRST_PARAMETER(27)
        _STDEX_GET_AND_REPLACE_FIRST_PARAMETER(28)
        _STDEX_GET_AND_REPLACE_FIRST_PARAMETER(29)
        _STDEX_GET_AND_REPLACE_FIRST_PARAMETER(30)
        _STDEX_GET_AND_REPLACE_FIRST_PARAMETER(31)

#undef _STDEX_GET_FIRST_PARAMETER 
#undef _STDEX_REPLACE_FIRST_PARAMETER
#undef _STDEX_GET_AND_REPLACE_FIRST_PARAMETER

        namespace memory_detail
        {
            struct void_type;
        }

        template <class _Tp, class _DummyT = memory_detail::void_type>
        struct _get_element_type {
            typedef typename _get_first_parameter<_Tp>::type type;
        };

        /*template <class _Tp>
        struct _get_element_type<_Tp, typename _Tp::element_type> {
            typedef typename _Tp::element_type type;
        };*/

        template <class _Tp, class _DummyT = memory_detail::void_type>
        struct _get_ptr_difference_type {
            typedef std::ptrdiff_t type;
        };

        /*template <class _Tp>
        struct _get_ptr_difference_type<_Tp, typename _Tp::difference_type> {
            typedef typename _Tp::difference_type type;
        };*/

        template <class _Tp, class _OtherT, class _DummyT = memory_detail::void_type>
        struct _get_rebind_alias {
            typedef typename _replace_first_parameter<_OtherT, _Tp>::type type;
        };

        //template <class _Tp, class _OtherT>
        //struct _get_rebind_alias<_Tp, _OtherT, typename _Tp::template rebind<_OtherT>/**/> {
        //    typedef typename _Tp::template rebind<_OtherT> type;
        //};
    }

    template <class _Tp>
    struct pointer_traits 
    {
        typedef _Tp pointer;
        typedef typename detail::_get_element_type<_Tp>::type element_type;
        typedef typename detail::_get_ptr_difference_type<_Tp>::type difference_type;

        template <class _OtherT>
        struct rebind: detail::_get_rebind_alias<_Tp, _OtherT>::type {};

        static pointer pointer_to(element_type &ref)
        {
            return _Tp::pointer_to(_Val);
        }
    };

    template <class _Tp>
    struct pointer_traits<_Tp*> 
    {
        typedef _Tp* pointer;
        typedef _Tp element_type;
        typedef std::ptrdiff_t difference_type;

        template <class _OtherT>
        struct rebind
        {
            typedef std::allocator<_OtherT*> other;
        };

        static pointer pointer_to(element_type &ref) _STDEX_NOEXCEPT_FUNCTION {
            return stdex::addressof(ref);
        }
    };

} // namespace stdex

#undef _STDEX_DELETED_FUNCTION
#undef _STDEX_NOEXCEPT_FUNCTION

#define _STDEX_PARAMETER_PACK_UNDEF
#include "./parameter_pack.h"
#undef _STDEX_PARAMETER_PACK_UNDEF

#endif // _STDEX_MEMORY_H

#ifndef _STDEX_ITERATOR_H
#define _STDEX_ITERATOR_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// stdex includes

// POSIX includes

// std includes
#include <iterator>
#include <cstddef> //std::size_t

#ifdef _STDEX_NATIVE_CPP11_SUPPORT

#define DELETED_FUNCTION =delete
#define NOEXCEPT_FUNCTION noexcept

#else

#define DELETED_FUNCTION 
#define NOEXCEPT_FUNCTION throw()

#endif

namespace stdex
{
    template<class ContainerType>
    inline
    typename ContainerType::iterator begin(ContainerType &value)
    {	// get beginning of sequence
        return (value.begin());
    }

    template<class ContainerType>
    inline
    typename ContainerType::iterator begin(const ContainerType &value)
    {	// get beginning of sequence
        return (value.begin());
    }

    template<class ContainerType>
    inline
    typename ContainerType::iterator end(ContainerType &value)
    {	// get end of sequence
        return (value.end());
    }

    template<class ContainerType>
    inline
    typename ContainerType::iterator end(const ContainerType &value)
    {	// get end of sequence
        return (value.end());
    }

    template<class T, std::size_t Size>
    inline
    T *begin(T(&value)[Size]) NOEXCEPT_FUNCTION
    {	// get beginning of array
        return (value);
    }

    template<class T, std::size_t Size>
    inline
    T *end(T(&value)[Size]) NOEXCEPT_FUNCTION
    {	// get end of array
        return (value + Size);
    }

    namespace detail
    {
        template <bool, class _Tp>
        struct _iterator_enable_if
        { };

        template <class _Tp>
        struct _iterator_enable_if<true, _Tp>
        {
            typedef _Tp type;
        };

        template<class, class>
        struct _iterator_is_same
        { 
            static const bool value = false;
        };

        template<class _Tp>
        struct _iterator_is_same<_Tp, _Tp>
        { 
            static const bool value = true;
        };
    }
    template<class ForwardIt>
    inline
    detail::_iterator_enable_if<
        detail::_iterator_is_same<
            typename ForwardIt::iterator_category, 
            std::forward_iterator_tag
        >::value == bool(true), 
        ForwardIt
    >::type next(ForwardIt it,
    typename std::iterator_traits<ForwardIt>::difference_type n = 1)
    {
        std::advance(it, n);
        return it;
    }

    template<class BidirIt>
    inline
    detail::_iterator_enable_if<
        detail::_iterator_is_same<
            typename BidirIt::iterator_category, 
            std::bidirectional_iterator_tag
        >::value == bool(true), 
        BidirIt
    >::type prev(BidirIt it,
    typename std::iterator_traits<BidirIt>::difference_type n = 1)
    {
        std::advance(it, -n);
        return it;
    }
}

#endif // _STDEX_ITERATOR_H
#ifndef _STDEX_ITERATOR_H
#define _STDEX_ITERATOR_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// stdex includes

// POSIX includes

// std includes
#include <iterator>

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

    template<class ForwardIt>
    inline
    ForwardIt next(ForwardIt it,
    typename std::iterator_traits<ForwardIt>::difference_type n = 1)
    {
        std::advance(it, n);
        return it;
    }
}

#endif // _STDEX_ITERATOR_H
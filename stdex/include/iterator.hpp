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
    T *begin(T(&value)[Size]) throw()
    {	// get beginning of array
        return (value);
    }

    template<class T, std::size_t Size>
    inline
    T *end(T(&value)[Size]) throw()
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

    template<class ForwardIt, class T>
    inline
    ForwardIt remove(ForwardIt first, ForwardIt last,
        const T& value)
    {
        ForwardIt result = first;
        for (; first != last; ++first) {
            if (!(*first == value)) {
                *result++ = *first;
            }
        }
        return result;
    }
}

#endif // _STDEX_ITERATOR_H
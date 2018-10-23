#ifndef _STDEX_ALGORITHM_H
#define _STDEX_ALGORITHM_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// stdex includes

// POSIX includes

// std includes
#include <algorithm>

#ifdef _STDEX_NATIVE_CPP11_SUPPORT

#define DELETED_FUNCTION =delete
#define NOEXCEPT_FUNCTION noexcept

#else

#define DELETED_FUNCTION 
#define NOEXCEPT_FUNCTION throw()

#endif

namespace stdex
{
    template< class InputIt, class UnaryPredicate >
    inline
    bool none_of(InputIt first, InputIt last, UnaryPredicate p)
    {
        for (; first != last; ++first) {
            if (p(*first)) return false;
        }
        return true;
    }
}

#endif // _STDEX_ALGORITHM_H
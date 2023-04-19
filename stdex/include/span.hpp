#ifndef _STDEX_SPAN_H
#define _STDEX_SPAN_H

#ifndef _STDEX_SPAN_BODY_DEFINE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// stdex includes
#include "./cstdint.hpp"
#include "./type_traits.hpp"

// POSIX includes

// std includes
#include <cstddef>
#include <iterator>

#ifdef _STDEX_NATIVE_CPP11_SUPPORT

#define _STDEX_DELETED_FUNCTION =delete
#define _STDEX_NOEXCEPT_FUNCTION noexcept

#else

#define _STDEX_DELETED_FUNCTION 
#define _STDEX_NOEXCEPT_FUNCTION throw()

#endif

namespace stdex
{
    enum {dynamic_extent = std::size_t(-1)};

    namespace detail
    {
        template<class T, std::size_t Size>
        struct span_internal {
            T* begin;
            enum {size = Size};
            span_internal() : begin(0), end(0) {}
            span_internal(T* begin) 
                : begin(begin) {}
            span_internal(T (&arr)[Size]) 
                : begin(arr) {}
        };

        template<class T>
        struct span_internal<T, dynamic_extent> {
            T* begin;
            std::size_t size;
            span_internal() : begin(0), size(0) {}
            span_internal(T* begin, T* end) 
                : begin(begin)
                , size(end - begin) {}
            span_internal(T* begin, std::size_t size) 
                : begin(begin)
                , size(size) {}
            template<class TT, std::size_t Size>
            span_internal(TT (&arr)[Size]) 
                : begin(arr)
                , size(Size) {}
        };

        template<class T>
        struct span_element
        {
            typedef T type;
        };
    }


    template <class T,
        std::size_t Extent = static_cast<const std::size_t>(stdex::dynamic_extent)>
    class span
    {
#endif // _STDEX_SPAN_BODY_DEFINE
        typedef span<false_type, 0>*& disabled_type;
        typedef void(*unspecified_bool_type)();
        static void unspecified_bool_true() {}
        typedef detail::span_internal<T, Extent> span_internal;
    public:
        static const std::size_t extent = Extent;
	
        typedef T element_type;
        typedef typename stdex::remove_cv<T>::type value_type;
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef T& reference;
        typedef const T& const_reference;
        typedef T* pointer;
        typedef const T* const_pointer;
        typedef T* iterator;
        typedef const T* const_iterator;
        typedef std::reverse_iterator<iterator> reverse_iterator;
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

        span() _STDEX_NOEXCEPT_FUNCTION {}

        // from range
        template <class U>
        span(U* begin,
            typename
            conditional<
               is_same<T, U>::value == bool(true) ||
               is_same<T, const U>::value == bool(true),
               U*, disabled_type
            >::type end)
            : internal(begin, end)
        {}

        template <class U>
        span(U* begin,
            typename
            conditional<
               is_same<T, U>::value == bool(true) ||
               is_same<T, const U>::value == bool(true),
               size_type, disabled_type
            >::type size)
            : internal(begin, size)
        {}

        span(
            typename
            conditional<
                is_same<typename remove_const<T>::type, T>::value,
                const span<T>&, 
                disabled_type // disable
            >::type other)
            : internal(other.data(), other.size())
        {}

        // span of const from span of non-const
        span(
            typename
            conditional<
                is_same<typename add_const<T>::type, T>::value, // instead of is_const
                disabled_type, // disable
                const span<typename add_const<T>::type>&
            >::type other)
            : internal(other.data(), other.size())
        {}

        span& operator=(const span &other) _STDEX_NOEXCEPT_FUNCTION
        {
            internal = other.internal;
            return *this;
        }

        // assign non-const span to const
        span& operator=(
            typename
            conditional<
                is_same<typename add_const<T>::type, T>::value, // instead of is_const
                const span<typename remove_const<T>::type>&,
                disabled_type // disable
            >::type other) _STDEX_NOEXCEPT_FUNCTION
        {
            internal = span_internal(other.begin(), other.end());
            return *this;
        }

        operator unspecified_bool_type() const _STDEX_NOEXCEPT_FUNCTION
        {
            return !!m_begin ? unspecified_bool_true : 0;
        }

        bool operator!() const _STDEX_NOEXCEPT_FUNCTION
        {
            return !!!m_begin;
        }

        reference operator[](size_type pos) const
        {
    #ifdef assert
            assert((pos) < this->size());
    #endif
            return *((internal.begin) + pos);
        }

        reference front() const
        {
            return operator[](0);
        }

        reference back() const
        {
            return operator[](size() - 1);
        }

        pointer data() const _STDEX_NOEXCEPT_FUNCTION
        {
            return (internal.begin);
        }

        // iterators
        iterator begin() const _STDEX_NOEXCEPT_FUNCTION
        {
            return (internal.begin);
        }

        const_iterator cbegin() const _STDEX_NOEXCEPT_FUNCTION
        {
            return (internal.begin);
        }

        iterator end() const _STDEX_NOEXCEPT_FUNCTION
        {
            return (internal.begin + internal.size);
        }

        const_iterator cend() const _STDEX_NOEXCEPT_FUNCTION
        {
            return (internal.begin + internal.size);
        }

        reverse_iterator rbegin() const _STDEX_NOEXCEPT_FUNCTION
        {
            return reverse_iterator(end());
        }

        reverse_iterator rend() const _STDEX_NOEXCEPT_FUNCTION
        {
            return reverse_iterator(begin());
        }

        // capacity
        bool empty() const _STDEX_NOEXCEPT_FUNCTION
        {
            return internal.size == 0;
        }

        size_type size() const _STDEX_NOEXCEPT_FUNCTION
        {
            return static_cast<size_type>(internal.size);
        }

        // byte access
        size_type size_bytes() const _STDEX_NOEXCEPT_FUNCTION
        {
            return size() * sizeof(element_type);
        }

        // slicing
        template<std::size_t Offset, std::size_t Count>
        span<element_type, Count> 
        subspan() const _STDEX_NOEXCEPT_FUNCTION
        {
            typedef span<element_type, Count> result_type;

            return result_type();
        }

        template<std::size_t Offset>
        span<element_type, 
            detail::_conditional_t<
                bool_constant<bool(dynamic_extent != Extent)>::value,
                integral_constant<size_type, Extent - Offset>,
                integral_constant<size_type, dynamic_extent>
            >::value> 
        subspan(
            typename
            enable_if<
                bool_constant<bool(Offset <= Extent)>::value,
                std::size_t
             >::type size = Size) const _STDEX_NOEXCEPT_FUNCTION
        {
            typedef
            span<element_type, 
            detail::_conditional_t<
                bool_constant<bool(dynamic_extent != Extent)>::value,
                integral_constant<size_type, Extent - Offset>,
                integral_constant<size_type, dynamic_extent>
            >::value> result_type;

            return result_type();
        }

        span<element_type> subspan(size_type off, size_type count = dynamic_extent) const _STDEX_NOEXCEPT_FUNCTION
        {
            typedef span<element_type> result_type;
            if (begin() + off > end()) return result_type(end(), 0);
            size_type newSize = size() - off;
            if (count > newSize) count = newSize;
            return result_type(begin() + off, count);
        }

        span first(size_type count) const _STDEX_NOEXCEPT_FUNCTION
        {
            return subspan(0, count);
        }

        span last(size_type count) const _STDEX_NOEXCEPT_FUNCTION
        {
            return subspan(size() - count, count);
        }

    private:
        span_internal internal;

#ifndef _STDEX_SPAN_BODY_DEFINE
    // just for non-const version
    public:
        template<class TT, size_type Size>
        span(TT (&arr)[Size], 
            typename
            enable_if<
                is_same<typename remove_cv<TT>::type, value_type>::value == bool(true),
                std::size_t
             >::type size = Size) _STDEX_NOEXCEPT_FUNCTION
            : internal(arr, size)
        { }
    };

    template <class StrippedConstT,
        std::size_t Extent>
    class span<const StrippedConstT, Extent>
    {
// we need this abomination of self-include
// to simply copy-paste code: 
// non-const version of span into span of const element type
#define _STDEX_SPAN_BODY_DEFINE
#undef _STDEX_SPAN_H
        typedef const StrippedConstT T;
    #include "./span.hpp"    
#define _STDEX_SPAN_H
#undef _STDEX_SPAN_BODY_DEFINE
    // just for const version
    public:
        template<class TT, size_type Size>
        span(const TT (&arr)[Size], 
            typename
            enable_if<
                is_same<typename remove_cv<TT>::type, value_type>::value == bool(true),
                std::size_t
             >::type size = Size) _STDEX_NOEXCEPT_FUNCTION
            : internal(arr, size)
        { }
    };
    
    template <class T>
    span<T> make_span(T* begin, T* end)
    {
        return span<T>(begin, end);
    }

    template <class T>
    span<T> make_span(T* begin, typename span<T>::size_type size)
    {
        return span<T>(begin, size);
    }

    template <class T, std::size_t N>
    span<T, N> make_span(T(&arr)[N])
    {
        return span<T, N>(arr);
    }

    template <class T>
    span<const stdex::uint8_t> as_bytes(const span<T> &that) _STDEX_NOEXCEPT_FUNCTION
    {
        return span<const stdex::uint8_t>(reinterpret_cast<const stdex::uint8_t*>(that.begin()), that.size_bytes());
    }

    template <class T>
    span<typename conditional<is_const<T>::value, const stdex::uint8_t, stdex::uint8_t>::type> 
    as_writable_bytes(const span<T> &that) _STDEX_NOEXCEPT_FUNCTION
    {
        // byte type with the same constness as T
        // can't have std::byte here with no c++17 guaranteed, so use the next best thing
        typedef typename conditional< is_const<T>::value, const stdex::uint8_t, stdex::uint8_t>::type byte_type;
        return span<byte_type>(reinterpret_cast<byte_type*>(that.begin()), that.size_bytes());
    }

} // namespace stdex

#undef _STDEX_DELETED_FUNCTION
#undef _STDEX_NOEXCEPT_FUNCTION

#endif // _STDEX_SPAN_BODY_DEFINE

#endif // _STDEX_SPAN_H

#ifndef _STDEX_SPAN_H
#define _STDEX_SPAN_H

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

    template <class T,
        std::size_t Extent = stdex::dynamic_extent>
    class span
    {
        typedef span<false_type, 0>*& disabled_type;
        typedef void(*unspecified_bool_type)();
        static void unspecified_bool_true() {}
    public:
        static const std::size_t extent = Extent;
	
        typedef T element_type;
        typedef typename std::remove_cv<T>::type value_type;
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
               is_same<T, U>::value ||
               is_same<T, const U>::value, 
               U*, disabled_type
            >::type end)
            : internal(begin, end)
        {}

        template <class U>
        span(U* begin,
            typename
            conditional<
               is_same<T, U>::value ||
               is_same<T, const U>::value,
               size_type, disabled_type
            >::type size)
            : internal(begin, begin + size)
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

        template <std::size_t N>
        span(T(&ar)[N])
            : internal(ar, ar + N)
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
            internal.begin = other.begin();
            internal.end = other.end();
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

        const_reference at(size_type pos) const
        {
    #ifdef assert
            assert((pos) < this->size());
    #endif
            return *(internal.begin + pos);
        }

        reference at(size_type pos)
        {
    #ifdef assert
            assert((pos) < this->size());
    #endif
            return *(m_begin + pos);
        }

        const_reference operator[](size_type pos) const
        {
            return at(pos);
        }

        reference operator[](size_type pos)
        {
            return at(pos);
        }

        const_reference front() const
        {
            return at(0);
        }

        reference front()
        {
            return at(0);
        }

        const_reference back() const
        {
            return at(size() - 1);
        }

        reference back()
        {
            return at(size() - 1);
        }

        const_pointer data() const _STDEX_NOEXCEPT_FUNCTION
        {
            return internal.begin;
        }

        pointer data() _STDEX_NOEXCEPT_FUNCTION
        {
            return internal.begin;
        }

        // iterators
        iterator begin() _STDEX_NOEXCEPT_FUNCTION
        {
            return internal.begin;
        }

        const_iterator begin() const _STDEX_NOEXCEPT_FUNCTION
        {
            return internal.begin;
        }

        const_iterator cbegin() const _STDEX_NOEXCEPT_FUNCTION
        {
            return internal.begin;
        }

        iterator end() _STDEX_NOEXCEPT_FUNCTION
        {
            return internal.end;
        }

        const_iterator end() const _STDEX_NOEXCEPT_FUNCTION
        {
            return internal.end;
        }

        const_iterator cend() const _STDEX_NOEXCEPT_FUNCTION
        {
            return internal.end;
        }

        reverse_iterator rbegin() _STDEX_NOEXCEPT_FUNCTION
        {
            return reverse_iterator(end());
        }

        const_reverse_iterator rbegin() const _STDEX_NOEXCEPT_FUNCTION
        {
            return const_reverse_iterator(end());
        }

        reverse_iterator rend() _STDEX_NOEXCEPT_FUNCTION
        {
            return reverse_iterator(begin());
        }

        const_reverse_iterator rend() const _STDEX_NOEXCEPT_FUNCTION
        {
            return const_reverse_iterator(begin());
        }

        // capacity
        bool empty() const _STDEX_NOEXCEPT_FUNCTION
        {
            return internal.begin == internal.end;
        }

        size_type size() const _STDEX_NOEXCEPT_FUNCTION
        {
            return internal.end - internal.begin;
        }

        // byte access
        size_type size_bytes() const _STDEX_NOEXCEPT_FUNCTION
        {
            return size() * sizeof(value_type);
        }

        // slicing
        span subspan(size_type off, size_type count = size_type(-1)) const _STDEX_NOEXCEPT_FUNCTION
        {
            if (internal.begin + off > internal.end) return span(internal.end, 0);
            size_type newSize = size() - off;
            if (count > newSize) count = newSize;
            return span(internal.begin + off, count);
        }

        span first(size_type count) const _STDEX_NOEXCEPT_FUNCTION
        {
            return subspan(0, count);
        }

        span last(size_type count) const _STDEX_NOEXCEPT_FUNCTION
        {
            return subspan(size() - count, count);
        }

        void remove_prefix(size_type count) _STDEX_NOEXCEPT_FUNCTION
        {
            internal.begin += count;
        }

        void remove_suffix(size_type count) _STDEX_NOEXCEPT_FUNCTION
        {
            internal.end -= count;
        }

    private:
        struct internal {
            T* begin;
            T* end;
            internal(T* begin = 0, T* end = 0) : begin(begin), end(end) {}
        } internal;
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

#endif // _STDEX_SPAN_H

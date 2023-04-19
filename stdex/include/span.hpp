#if !defined(_STDEX_SPAN_H) || defined(_STDEX_SPAN_BODY_DEFINE)
#define _STDEX_SPAN_H

#ifndef _STDEX_SPAN_BODY_DEFINE

#if _MSC_VER > 1000
//#pragma once
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
    static const std::size_t dynamic_extent =
        integral_constant<std::size_t, static_cast<std::size_t>(0) - static_cast<std::size_t>(1)>::value;

    namespace detail
    {
        template<class T, std::size_t Size>
        struct _span_internal {
            T* begin;
            enum {size = Size};
            _span_internal() : begin(0) {}
            _span_internal(T* begin, T*) 
                : begin(begin) {}
            _span_internal(T* begin, std::size_t)
                : begin(begin) {}
            _span_internal(T (&arr)[Size]) 
                : begin(arr) {}
        };

        template<class T>
        struct _span_internal<T, dynamic_extent> {
            T* begin;
            std::size_t size;
            _span_internal() : begin(0), size(0) {}
            _span_internal(T* begin, T* end) 
                : begin(begin)
                , size(end - begin) {}
            _span_internal(T* begin, std::size_t size) 
                : begin(begin)
                , size(size) {}
            template<class TT, std::size_t Size>
            _span_internal(TT (&arr)[Size]) 
                : begin(arr)
                , size(Size) {}
        };

        template<class T>
        struct _span_internal<T, 0> {
            T* begin;
            enum { size = 0 };
            _span_internal() : begin(0) {}
            _span_internal(T* begin, T*)
                : begin(begin) {}
            _span_internal(T* begin, std::size_t)
                : begin(begin) {}
            _span_internal(T* begin)
                : begin(begin) {}
        };

        struct _span_range
        {
            typedef detail::_span_internal<const void, dynamic_extent> _span_internal;
            _span_internal internal;
            template<class _Tp, class _AllocT, template<class, class> class _R>
            _span_range(_R<_Tp, _AllocT>& range) _STDEX_NOEXCEPT_FUNCTION
                : internal(range.data(), range.size())
            {}
        };
    }


    template <class T,
        std::size_t Extent = static_cast<const std::size_t>(stdex::dynamic_extent)>
    class span
    {
#endif // _STDEX_SPAN_BODY_DEFINE
        typedef void(*unspecified_bool_type)();
        static void unspecified_bool_true() {}
        typedef detail::_span_internal<T, Extent> _span_internal;
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
        span(pointer begin, pointer end)
            : internal(begin, end)
        {}

        span(pointer begin, size_type size)
            : internal(begin, size)
        {}

        span(const span& other) _STDEX_NOEXCEPT_FUNCTION
            : internal(other.internal)
        {}

        span(detail::_span_range range) _STDEX_NOEXCEPT_FUNCTION
            : internal(reinterpret_cast<pointer>(const_cast<void*>(range.internal.begin)), range.internal.size)
        {}

        span& operator=(const span &other) _STDEX_NOEXCEPT_FUNCTION
        {
            internal = other.internal;
            return *this;
        }

        operator unspecified_bool_type() const _STDEX_NOEXCEPT_FUNCTION
        {
            return !!internal.begin ? unspecified_bool_true : 0;
        }

        bool operator!() const _STDEX_NOEXCEPT_FUNCTION
        {
            return !!!internal.begin;
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
        typename
        conditional<
            bool_constant<bool(Offset > Extent)>::value == bool(false) &&
            bool_constant<bool(Count > (Extent - Offset))>::value == bool(false),
            span<element_type, Count>, void
        >::type
        subspan(typename
            enable_if<
                bool_constant<bool(Offset > Extent)>::value == bool(false) &&
                bool_constant<bool(Count > (Extent - Offset))>::value == bool(false),
                std::size_t
             >::type count = Count
        ) const _STDEX_NOEXCEPT_FUNCTION
        {
            return span<element_type, Count>(
                    this->data() + Offset, Count);
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
                bool_constant<bool(Offset > Extent)>::value == bool(false),
                std::size_t
             >::type offset = Offset) const _STDEX_NOEXCEPT_FUNCTION
        {
            typedef
            detail::_conditional_t<
                bool_constant<bool(dynamic_extent != Extent)>::value,
                integral_constant<size_type, Extent - Offset>,
                integral_constant<size_type, dynamic_extent>
            > span_count;

            return span<element_type, span_count::value>(
                    this->data() + Offset, this->size() - Offset);
        }

        span<element_type, dynamic_extent> subspan(size_type off, size_type count = dynamic_extent) const _STDEX_NOEXCEPT_FUNCTION
        {
            if ( begin() + off > end() )
            {
                iterator end = span::end();
                return span<element_type, dynamic_extent>(end, static_cast<size_type>(0));
            }
            size_type newSize = size() - off;
            if ( count > newSize ) count = newSize;
            iterator begin = span::begin();
            return span<element_type, dynamic_extent>(begin + off, count);
        }

        template<std::size_t Count>
        typename
        conditional<
            bool_constant<bool(Count > Extent)>::value == bool(false),
            span<element_type, Count>, void
        >::type first() const _STDEX_NOEXCEPT_FUNCTION
        {
            return subspan<0, Count>();
        }

        span<element_type, dynamic_extent> first(size_type count) const _STDEX_NOEXCEPT_FUNCTION
        {
            return subspan(0, count);
        }

        template<std::size_t Count>
        typename
        conditional<
            bool_constant<bool(Count > Extent)>::value == bool(false),
            span<element_type, Count>, void
        >::type last() const _STDEX_NOEXCEPT_FUNCTION
        {
            return subspan<Extent - Count, Count>();
        }

        span<element_type, dynamic_extent> last(size_type count) const _STDEX_NOEXCEPT_FUNCTION
        {
            return subspan(size() - count, count);
        }

    private:
        _span_internal internal;

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
        typedef const StrippedConstT T;
    #include "./span.hpp" 
#undef _STDEX_SPAN_BODY_DEFINE
    // just for const version
    public:
        template<class TT, size_type Size>
        span(TT const (&arr)[Size],
            typename
            enable_if<
                is_same<typename remove_cv<TT>::type, value_type>::value == bool(true),
                std::size_t
             >::type size = Size) _STDEX_NOEXCEPT_FUNCTION
            : internal(arr, size)
        { }

        // span of const from span of non-const
        span(const span<StrippedConstT>& other)
            : internal(other.data(), other.size())
        {}

        // assign non-const span to const
        span& operator=(const span<StrippedConstT> &other) _STDEX_NOEXCEPT_FUNCTION
        {
            internal = _span_internal(other.begin(), other.end());
            return *this;
        }
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
        return span<T, N>(arr, N);
    }

    template <class T, std::size_t N>
    span<const T, N> make_span(T const (&arr)[N], ...)
    {
        return span<const T, N>(arr);
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

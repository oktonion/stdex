#ifndef _STDEX_FUNCTIONAL_H
#define _STDEX_FUNCTIONAL_H
 
 #if _MSC_VER > 1000
 #pragma once
 #endif // _MSC_VER > 1000
 
 // stdex includes
 #include "./core.h"
 #include "./cstdint.hpp"
 #include "./type_traits.hpp"
 
 // POSIX includes
 
 // std includes
 #include <functional>
 #include <bitset> // for hash
 #include <vector> // for hash
 
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
                    {	// fold in another byte
                    _val ^= (_SizeT)_first[_next];
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
                    {	// fold in another byte
                    _val ^= (_SizeT)_first[_next];
                    _val *= _FNV_prime;
                    }
                return (_val);
            }
        };

        template<class _KeyT, class>
        struct _bitwise_hash_impl
        {	// hash functor for plain old data
            typedef _KeyT argument_type;
            typedef std::size_t result_type;

            std::size_t operator()(const _KeyT& _keyval) const
            {	// hash _keyval to std::size_t value by pseudorandomizing transform
                return (_bitwise_hash_seq<std::size_t, sizeof(std::size_t)>::call((const unsigned char *)&_keyval, sizeof (_KeyT)));
            }
        };
        
        template<class _KeyT>
        struct _bitwise_hash_impl<float, _KeyT>
        {
            typedef _KeyT argument_type;
            typedef std::size_t result_type;

            std::size_t operator()(const argument_type& _keyval) const
            {	// hash _keyval to std::size_t value by pseudorandomizing transform
                struct labdas{
                    static std::size_t hash_seq(const argument_type &keyval)
                    {
                        return (_bitwise_hash_seq<std::size_t, sizeof(std::size_t)>::call((const unsigned char *)&keyval, sizeof (argument_type)));
                    }
                };

                return (labdas::hash_seq(
                    _keyval == 0 ? 0 : _keyval));	// map -0 to 0
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
    }

    namespace intern
    {
        // since we have no static_assert in pre-C++11 we just compile-time assert this way:
        struct functional_asserts
        {
            template<bool>
            struct the_cpp_standard_does_not_provide_a_hash_for_enum_types_assert;
        };

        template<>
        struct functional_asserts::the_cpp_standard_does_not_provide_a_hash_for_enum_types_assert<true>
        {
            typedef bool the_cpp_standard_does_not_provide_a_hash_for_enum_types_assert_failed;
        };
    } // namespace intern

    template<class _KeyT>
    struct hash
        : public detail::_hash_impl<_KeyT>::type
    {	// hash functor for enums
        typedef intern::functional_asserts check;
        typedef typename check::the_cpp_standard_does_not_provide_a_hash_for_enum_types_assert<is_enum<_KeyT>::value == bool(false)>::
            the_cpp_standard_does_not_provide_a_hash_for_enum_types_assert_failed
        check1; // if you are there means you tried to calculate hash for enum type
    };

    template<>
    struct hash<void>;

    template<>
    struct hash<bool>
        : public detail::_hash_impl<bool>::type
    {	// hash functor for bool
    };

    template<>
    struct hash<char>
        : public detail::_hash_impl<char>::type
    {	// hash functor for char
    };

    template<>
    struct hash<signed char>
        : public detail::_hash_impl<signed char>::type
    {	// hash functor for signed char
    };

    template<>
    struct hash<unsigned char>
        : public detail::_hash_impl<unsigned char>::type
    {	// hash functor for unsigned char
    };

    template<>
    struct hash<wchar_t>
        : public detail::_hash_impl<wchar_t>::type
    {	// hash functor for wchar_t
    };

    template<>
    struct hash<short>
        : public detail::_hash_impl<short>::type
    {	// hash functor for short
    };

    template<>
    struct hash<unsigned short>
        : public detail::_hash_impl<unsigned short>::type
    {	// hash functor for unsigned short
    };

    template<>
    struct hash<int>
        : public detail::_hash_impl<int>::type
    {	// hash functor for int
    };

    template<>
    struct hash<unsigned int>
        : public detail::_hash_impl<unsigned int>::type
    {	// hash functor for unsigned int
    };

    template<>
    struct hash<long>
        : public detail::_hash_impl<long>::type
    {	// hash functor for long
    };

    template<>
    struct hash<unsigned long>
        : public detail::_hash_impl<unsigned long>::type
    {	// hash functor for unsigned long
    };

    template<>
    struct hash<long long>
        : public detail::_hash_impl<long long>::type
    {	// hash functor for long long
    };

    template<>
    struct hash<unsigned long long>
        : public detail::_hash_impl<unsigned long long>::type
    {	// hash functor for unsigned long long
    };

    template<>
    struct hash<float>
        : public detail::_hash_impl<float>::type
    {	// hash functor for float
    };

    template<>
    struct hash<double>
        : public detail::_hash_impl<double>::type
    {	// hash functor for double
    };

    template<>
    struct hash<long double>
        : public detail::_hash_impl<long double>::type
    {	// hash functor for long double
    };

    template<class _KeyT>
    struct hash<_KeyT *>
        : public detail::_hash_impl<_KeyT *>::type
    {	// hash functor for _KeyT *
    };

    // standard hash overloads for std headers

    template<std::size_t _Bits>
	struct hash<std::bitset<_Bits>/**/>;
	//{	// hash functor for bitset<_Bits>
    //    typedef std::bitset<_Bits> argument_type;
    //    typedef std::size_t result_type;
    //
    //    std::size_t operator()(const argument_type& _keyval) const
    //    {
    //        return (_keyval.hash());
    //    }
	//};

    template<class _AllocatorT>
	struct hash<std::vector<bool, _AllocatorT>/**/>
	{	// hash functor for vector<bool, _AllocatorT>
        typedef std::vector<bool, _AllocatorT> argument_type;
        typedef std::size_t result_type;

        std::size_t operator()(const argument_type& _keyval) const
        {
            const unsigned char *_first = nullptr;
            const std::size_t _size = _keyval.size();
            if(_size)
                _first = (const unsigned char *)&_keyval[0];
            
            return (detail::_bitwise_hash_seq<std::size_t, sizeof(std::size_t)>::call(_first, _size));
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
	{	// functor for operator&
        typedef _Tp first_argument_type;
        typedef _Tp second_argument_type;
        typedef _Tp result_type;

        _Tp operator()(const _Tp& _left, const _Tp& _right) const
        {	// apply operator& to operands
            return (_left & _right);
        }
	};

    template<class _Tp>
	struct bit_or
	{	// functor for operator|
        typedef _Tp first_argument_type;
        typedef _Tp second_argument_type;
        typedef _Tp result_type;

        _Tp operator()(const _Tp& _left, const _Tp& _right) const
        {	// apply operator| to operands
            return (_left | _right);
        }
	};

    template<class _Tp>
	struct bit_xor
	{	// functor for operator^
        typedef _Tp first_argument_type;
        typedef _Tp second_argument_type;
        typedef _Tp result_type;

        _Tp operator()(const _Tp& _left, const _Tp& _right) const
        {	// apply operator^ to operands
            return (_left ^ _right);
        }
	};

    template<class _Tp>
	struct bit_not
	{	// functor for unary operator~
        typedef _Tp argument_type;
        typedef _Tp result_type;

        _Tp operator()(const _Tp& _left) const
        {	// apply operator~ to operand
            return (~_left);
        }
	};
    
 } // namespace stdex
 
 #undef _STDEX_DELETED_FUNCTION
 #undef _STDEX_NOEXCEPT_FUNCTION
 
 #endif // _STDEX_FUNCTIONAL_H

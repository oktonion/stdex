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

    namespace placeholders
    {

    }

    // Classes

    namespace detail
    {
        template<int _Index>
        struct _arg_tag {};

        template<class _Tp, int _N>
        struct _arg
        {
            typedef _Tp type;
            _Tp value;
            _arg(_Tp value_): value(value_) {}
            _arg(const _arg &other) : value(other.value) {}

            _arg& self(_arg_tag<_N>&) { return *this; }
        };

        template<class _NullptrT, bool>
        struct _nullptr_place_holder_impl{
            static const _NullptrT value = nullptr;
        };


        template<class _NullptrT>
        struct _nullptr_place_holder_impl<_NullptrT, false>{
            static _NullptrT value;
        };

        template<class _NullptrT>
        _NullptrT _nullptr_place_holder_impl<_NullptrT, false>::value = _NullptrT();

        struct _nullptr_place_holder:
            _nullptr_place_holder_impl<
                stdex::nullptr_t,
                stdex::is_class<stdex::nullptr_t>::value == bool(false) &&
                (stdex::is_integral<stdex::nullptr_t>::value == bool(true) ||
                stdex::is_enum<stdex::nullptr_t>::value == bool(true))>
        { };

        template<int _N>
        struct _arg<stdex::nullptr_t, _N>:
            _nullptr_place_holder
        { 
            typedef stdex::nullptr_t type;
            _arg(stdex::nullptr_t){}
            
            _arg& self(_arg_tag<_N>&) { return *this; }
        };

        template<int>
        class void_type_n;
        #define _STDEX_VTN(N) class _T##N = void_type_n< N >
        template<_STDEX_VTN(0), _STDEX_VTN(1)>
        struct _derived: _T0, _T1{};

        template<class _ArgsT, class _ArgT, int _N>
        struct _args: _ArgsT, _arg<_ArgT, _N>
        {
            _arg<_ArgT, _N> &arg(const _arg_tag<_N>&) {return *this;}

            typedef _args type;
            _args(const _ArgsT &other, _ArgT arg):
                _ArgsT(other), _arg<_ArgT, _N>(arg) {}
            _args(const _args &other):
                _ArgsT(other), _arg<_ArgT, _N>(static_cast<const _arg<_ArgT, _N>&>(other)) {}
        };

        template<class _ArgT>
        struct _args<void, _ArgT, 0> : _arg<_ArgT, 0>
        {
            _arg<_ArgT, 0> &arg(const _arg_tag<0>&) {return *this;}
            
            typedef _args type;
            _args(_ArgT arg) :
                _arg<_ArgT, 0>(arg) {}
            _args(const _args& other) :
                _arg<_ArgT, 0>(other) {}
        };

        template<class, int _End>
        struct _get_args;

        template<class _ArgsT, class _ArgT, int _Index, int _End>
        struct _get_args<_args<_ArgsT, _ArgT, _Index>, _End>:
            _get_args<_ArgsT, _End>
        { };

        template<class _ArgsT, class _ArgT, int _End>
        struct _get_args<_args<_ArgsT, _ArgT, _End>, _End>
        { 
            typedef _args<_ArgsT, _ArgT, _End> type;
        };

        template<class _FuncT, int _Index, int _Count>
        struct _check_args_for_null
        {
            
            template<class _RawArgsT, class _Tp, class _ResArgsT>
            static void check(_FuncT &fx, _RawArgsT &args, _arg<_Tp, _Index> &arg, _ResArgsT &res)
            {
                typedef _Tp checked_arg_t;
                _check_args_for_null<_FuncT, _Index + 1, _Count>::call(fx, args, arg, res);
            }

            template<class _RawArgsT, class _CheckedArgT, class _ResArgsT>
            static void call(_FuncT &fx, _RawArgsT &args, _CheckedArgT &arg, _ResArgsT &res)
            {
                _arg_tag<_Index> tag;
                typedef typename _get_args<_RawArgsT, _Index>::type args_type;
                if(&arg != &args)
                {
                    _args<_ResArgsT, _CheckedArgT, _Index> checked_args(res, arg);
                    check(fx, args, static_cast<args_type&>(args).arg(tag), checked_args);
                }
                else
                    check(fx, args, static_cast<args_type&>(args).arg(tag), args);
            }
        };

        template<class _FuncT, int _Count>
        struct _check_args_for_null<_FuncT, 0, _Count>
        {
            template<class _RawArgsT, class _Tp, class _ResArgsT>
            static void check(_FuncT &fx, _RawArgsT &args, _arg<_Tp, 0> &arg, _ResArgsT &res)
            {
                typedef _Tp checked_arg_t;
                _check_args_for_null<_FuncT, 1, _Count>::call(fx, args, arg, res);
            }

            template<class _RawArgsT>
            static void call(_FuncT &fx, _RawArgsT &args)
            {
                check(fx, args, args, args);
            }
        };

        template<class _FuncT>
        struct _check_args_for_null<_FuncT, 0, 0>
        {
            static void call(_FuncT &fx)
            {
                struct _functor
                {
                    void operator()(_FuncT &fx) {fx();}
                }; 
                _functor()(fx);
            }
        };

        template<class _FuncT>
        struct _check_args_for_null<_FuncT, 2, 2>
        {
            template<class _ArgT0, class _ArgT1, class _ResArgsT>
            static void func(_FuncT &fx, _arg<_ArgT0, 0>&, _arg<_ArgT1, 1>&, _ResArgsT &res)
            {
                struct _functor: _ResArgsT
                {
                    _functor(_ResArgsT &other) : _ResArgsT(other) {}
                    void operator()(_FuncT &fx) {fx(_arg<_ArgT0, 0>::value, _arg<_ArgT1, 1>::value);}
                };
                
                _functor f = res;
                f(fx);
            }
            
            template<class _RawArgsT, class _CheckedArgT, class _ResArgsT>
            static void call(_FuncT &fx, _RawArgsT &args, _CheckedArgT &arg, _ResArgsT &res)
            {
                typedef typename _get_args<_RawArgsT, 0>::type args0_type;
                typedef typename _get_args<_RawArgsT, 1>::type args1_type;
                if(&arg != &args)
                {
                    _args<_ResArgsT, _CheckedArgT, 1> checked_args(res, arg);
                    func(fx, static_cast<args0_type&>(args).arg(_arg_tag<0>()), static_cast<args1_type&>(args).arg(_arg_tag<1>()), checked_args);
                }
                else
                    func(fx, static_cast<args0_type&>(args).arg(_arg_tag<0>()), static_cast<args1_type&>(args).arg(_arg_tag<1>()), args);
            }
        };

    } // namespace detail
    
    //template<class>
    class function
    {
    public:
        typedef void(*fx0_type)();
        
        function(fx0_type fx)
        {
            typedef detail::_check_args_for_null<fx0_type, 0, 0> functor;
            functor::call(fx);
        }

        typedef void(*fx2_type)(int&, void*);
        function(fx2_type fx, int& val, stdex::nullptr_t np)
        {
            typedef detail::_check_args_for_null<fx2_type, 0, 2> functor;
            typedef detail::_args<void, int&, 0> args1_type;
            typedef detail::_args<args1_type, stdex::nullptr_t, 1> args2_type;

            args2_type args = args2_type(args1_type(val), np);

            functor::call(fx, args);
        }

    private:

        
    };

    // Hashing

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
	struct hash<std::vector<bool, _AllocatorT>/**/>;
	//{	// hash functor for vector<bool, _AllocatorT>
    //    typedef std::vector<bool, _AllocatorT> argument_type;
    //    typedef std::size_t result_type;
    //
    //    std::size_t operator()(const argument_type& _keyval) const
    //    {
    //        return (_keyval.hash());
    //    }
	//};

    
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

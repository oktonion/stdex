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
        };

        template<class _NullptrT, bool>
        struct _nullptr_place_holder_impl{
            static const _NullptrT value = nullptr;
        };


        template<class _NullptrT>
        struct _nullptr_place_holder_impl<_NullptrT, true>{
            static _NullptrT value;
        };

        template<class _NullptrT>
        _NullptrT _nullptr_place_holder_impl<_NullptrT, true>::value = _NullptrT();

        _yes_type _rt_nullptr_can_be_ptr_checker(class _dummy*);
        _no_type _rt_nullptr_can_be_ptr_checker(...);

        struct _rt_nullptr_can_be_ptr
        {
            static stdex::nullptr_t declnull();
            static const bool value = 
                sizeof(_rt_nullptr_can_be_ptr_checker(declnull())) == sizeof(_yes_type);
        };
    } // namespace detail

    namespace intern
    {
        template<class>
        struct _has_feature;

        template<>
        struct _has_feature<class _stdex_has_native_nullptr>:
            integral_constant<
                bool,
                detail::_rt_nullptr_can_be_ptr::value == bool(true) &&
                is_class<stdex::nullptr_t>::value == bool(false)
            >
        { };
    } // namespace intern

    namespace detail
    {
        struct _nullptr_place_holder:
            _nullptr_place_holder_impl<
                stdex::nullptr_t,
                intern::_has_feature<intern::_stdex_has_native_nullptr>::value>
        { };

        template<int _N>
        struct _arg<_nullptr_place_holder, _N>:
            _nullptr_place_holder
        { 
            typedef stdex::nullptr_t type;
            _arg(stdex::nullptr_t){}
        };

        template<class _ArgsT, class _ArgT, int _N>
        struct _args: _ArgsT, _arg<_ArgT, _N>
        {
            typedef _args type;
            static const int count = _N + 1;
            _args(const _ArgsT &other, _ArgT arg):
                _ArgsT(other), _arg<_ArgT, _N>(arg) {}
            _args(const _args &other):
                _ArgsT(other), _arg<_ArgT, _N>(static_cast<const _arg<_ArgT, _N>&>(other)) {}
            template<class _NextArgT>
            _args<type, _NextArgT, _N + 1> make(_NextArgT arg) const {return _args<type, _NextArgT, _N + 1>(*this, arg);}

            const type& make(void_type) const { return *this; }
        };

        template<class _ArgT>
        struct _args<void, _ArgT, 0> : _arg<_ArgT, 0>
        {
            typedef _args type;
            static const int count = 1;
            _args(_ArgT arg) :
                _arg<_ArgT, 0>(arg) {}
            _args(const _args& other) :
                _arg<_ArgT, 0>(other) {}

            template<class _NextArgT>
            _args<type, _NextArgT, 1> make(_NextArgT arg) const {return _args<type, _NextArgT, 1>(*this, arg);}

            const type& make(void_type) const { return *this; }
        };

        /*template<class _ArgsT, int _N>
        struct _args<_ArgsT, void_type, _N> : _ArgsT
        {
            typedef _args type;
            _args(const _ArgsT& other) :
                _ArgsT(other) {}
            _args(const _args& other) :
                _ArgsT(other) {}
        };*/

        template<class, int _End>
        struct _get_args_impl;

        template<class _ArgsT, int _End>
        struct _get_args :
            _get_args_impl<_ArgsT, _End>
        { };

        template<class _ArgsT, class _ArgT, int _Index, int _End>
        struct _get_args_impl<_args<_ArgsT, _ArgT, _Index>, _End>:
            _get_args<_ArgsT, _End>
        { };

        template<class _ArgsT, class _ArgT, int _End>
        struct _get_args<_args<_ArgsT, _ArgT, _End>, _End>
        { 
            typedef _args<_ArgsT, _ArgT, _End> args;
            typedef _arg<_ArgT, _End> arg;
        };

        template<int _Index, class _ArgsT>
        typename _get_args<_ArgsT, _Index>::arg&
        _get_arg(_ArgsT &args)
        {
            return args;
        }

        template<class _FuncT, int _Index, int _Count>
        struct _check_args_for_null;

        template<class _FuncT, int _Index, int _Count>
        struct _func_invoker_impl;

        template<class _CheckedArgsT>
        struct _checked_args
        {
            typedef _CheckedArgsT type;
        };

        template<class _FuncT, int _Index, int _Count, bool>
        struct _check_args_for_null_impl
        {
            typedef _func_invoker_impl<_FuncT, _Index + 1, _Count> func_invoker;

            template<class _RawArgsT, class _CheckedArgsT>
            static void call(_FuncT &fx, _RawArgsT &args, const _checked_args<_CheckedArgsT>& checked_args)
            {
                _arg<stdex::nullptr_t, _Index> &arg = args;

                if(nullptr == arg.value)
                {
                    typedef _args<_CheckedArgsT, _nullptr_place_holder, _Index> checked_args_t;
                    func_invoker::call(fx, args, _checked_args<checked_args_t>());
                }
                else
                    func_invoker::call(fx, args, checked_args);
            }
        };

        template<class _FuncT, int _Index, int _Count>
        struct _check_args_for_null_impl<_FuncT, _Index, _Count, false>
        {
            typedef _func_invoker_impl<_FuncT, _Index + 1, _Count> func_invoker;

            template<class _RawArgsT, class _CheckedArgsT>
            static void call(_FuncT &fx, _RawArgsT &args, const _checked_args<_CheckedArgsT>& checked_args)
            {
                func_invoker::call(fx, args, checked_args);
            }
        };

        template<class _FuncT, int _Index, int _Count>
        struct _check_args_for_null
        {
            template<class _RawArgsT, class _CheckedArgsT>
            static void call(_FuncT &fx, _RawArgsT &args, const _checked_args<_CheckedArgsT>& checked_args)
            {
                typedef typename _get_args<_RawArgsT, _Index>::arg arg_type;
                _check_args_for_null_impl
                <
                    _FuncT, _Index, _Count,
                    is_null_pointer<typename remove_reference<arg_type>::type>::value == bool(true)
                    && intern::_has_feature<intern::_stdex_has_native_nullptr>::value == bool(false)
                >::call(fx, args, checked_args);
            }
        };


        template<class _FuncT, int _Index, int _Count>
        struct _func_invoker_impl
        {
            typedef _check_args_for_null<_FuncT, _Index, _Count> null_checker;

            template<class _RawArgsT, class _CheckedArgsT>
            static void call(_FuncT &fx, _RawArgsT &args, const _checked_args<_CheckedArgsT>& checked_args)
            {
                null_checker::call(fx, args, checked_args);
            }
        };

        template<class _FuncT>
        struct _func_invoker_impl<_FuncT, 0, 0>
        {
            static void call(_FuncT &fx)
            {
                struct _functor
                {
                    void operator()(_FuncT &fx) {fx();}
                }; 
                _functor _f;
                _f(fx);
            }
        };

        template<class _FuncT>
        struct _func_invoker_impl<_FuncT, 2, 2>
        {
            template<class _ArgT0, class _ArgT1, class _ResArgsT>
            static void func(_FuncT &fx, _arg<_ArgT0, 0>&, _arg<_ArgT1, 1>&, _ResArgsT &res)
            {
                struct _functor: _ResArgsT
                {
                    typedef _ResArgsT base_type;
                    typedef _ArgT0 arg0_type;
                    typedef _ArgT1 arg1_type;
                    _functor(const base_type &other) : base_type(other) {}
                    void operator()(_FuncT &fx) {
                        using ::stdex::detail::_arg;
                        fx(_arg<arg0_type, 0>::value, _arg<arg1_type, 1>::value);
                    }
                };
                
                _functor _f = res;
                _f(fx);
            }
            
            template<class _RawArgsT, class _CheckedArgsT>
            static void call(_FuncT &fx, _RawArgsT &args, const _checked_args<_CheckedArgsT>&)
            {
                func(fx, _get_arg<0>(args), _get_arg<1>(args), args);
            }
        };

        template<class _FuncT, int _Index, int _Count>
        struct _func_invoker
        {
            template<class _RawArgsT>
            static void call(_FuncT &fx, _RawArgsT &args)
            {
                _func_invoker_impl<_FuncT, _Index, _Count>::call(fx, args, _checked_args<_RawArgsT>());
            }
        };
    } // namespace detail

    namespace detail{

        template<class _ArgT>
        struct _arg_is_void:
            bool_constant<
                is_same<_ArgT, void_type>::value == bool(true) ||
                is_void<_ArgT>::value == bool(true)
            >
        { };

        template<class _ArgsT, class _ArgT, int _Index, bool>
        struct _make_args_impl;

        template<class _ArgsT, class _ArgT, class _NextArgT, int _Index>
        struct _add;

        template<class _ArgsT>
        struct _make_args_impl1;

        template<class _ArgsT, class _ArgT, int _Index>
        struct _make_args_impl1<_args<_ArgsT, _ArgT, _Index>/**/>
        {
            template<class _NextArgT>
            struct add
            {
                typedef _add<_ArgsT, _ArgT, _NextArgT, _Index> type;
            };
        };

        template<class _ArgsT, class _ArgT, int _Index, bool VVV>
        struct _make_args_impl:
            _make_args_impl1<_ArgsT>
        {
            typedef _make_args_impl<_ArgsT, _ArgT, _Index, VVV> type;
            typedef _ArgsT args;
        };

        template<class _ArgsT, class _ArgT, int _Index>
        struct _make_args_impl<_ArgsT, _ArgT, _Index, false>:
            _make_args_impl1<_args<_ArgsT, _ArgT, _Index>/**/>
        { 
            typedef _make_args_impl<_ArgsT, _ArgT, _Index, false> type;
            typedef _args<_ArgsT, _ArgT, _Index> args;
        };

        template<class _ArgsT, class _ArgT, class _NextArgT, int _Index>
        struct _add :
            _make_args_impl<_args<_ArgsT, _ArgT, _Index>, _NextArgT, _Index + 1, _arg_is_void<_NextArgT>::value == bool(true)>
        { };

        template<class _ArgT>
        struct _make_args:
            _make_args_impl<void, _ArgT, 0, _arg_is_void<_ArgT>::value>
        { };

        namespace functional_std {
            // since there is no move-semantic
            template<class _Tp>
            _Tp& move(_Tp &ref)
            {
                using namespace std;
                using namespace stdex;

                return ref;
            }

            template<class _Tp>
            typename remove_reference<_Tp>::type& forward(typename remove_reference<_Tp>::type &ref)
            {
                using namespace std;
                using namespace stdex;

                return ref;
            }
        }

        template<
            class _R, 
            class _Arg0T  = void_type, 
            class _Arg1T  = void_type, 
            class _Arg2T  = void_type,
            class _Arg3T  = void_type,
            class _Arg4T  = void_type,
            class _Arg5T  = void_type,
            class _Arg6T  = void_type,
            class _Arg7T  = void_type,
            class _Arg8T  = void_type,
            class _Arg9T  = void_type,
            class _Arg10T = void_type,
            class _Arg11T = void_type,
            class _Arg12T = void_type,
            class _Arg13T = void_type,
            class _Arg14T = void_type,
            class _Arg15T = void_type,
            class _Arg16T = void_type,
            class _Arg17T = void_type,
            class _Arg18T = void_type,
            class _Arg19T = void_type,
            class _Arg20T = void_type,
            class _Arg21T = void_type,
            class _Arg22T = void_type,
            class _Arg23T = void_type // up to 24 args
        >
        class function
        {
            typedef
                typename _make_args< _Arg0T >::type::
                template add       < _Arg1T >::type::
                template add       < _Arg2T >::type::
                template add       < _Arg3T >::type::
                template add       < _Arg4T >::type::
                template add       < _Arg5T >::type::
                template add       < _Arg6T >::type::
                template add       < _Arg7T >::type::
                template add       < _Arg8T >::type::
                template add       < _Arg9T >::type::
                template add       < _Arg10T>::type::
                template add       < _Arg11T>::type::
                template add       < _Arg12T>::type::
                template add       < _Arg13T>::type::
                template add       < _Arg14T>::type::
                template add       < _Arg15T>::type::
                template add       < _Arg16T>::type::
                template add       < _Arg17T>::type::
                template add       < _Arg18T>::type::
                template add       < _Arg19T>::type::
                template add       < _Arg20T>::type::
                template add       < _Arg21T>::type::
                template add       < _Arg22T>::type::
                template add       < _Arg23T>::type::
            args args_type;

            struct func_base {
                virtual func_base* _copy() const = 0;
                virtual func_base* _move() _STDEX_NOEXCEPT_FUNCTION = 0;
                virtual _R _co_call(function::args_type&) = 0;
                virtual void _delete_this() _STDEX_NOEXCEPT_FUNCTION = 0;

                func_base() {}
                virtual ~func_base() {}
            private:
                func_base(const func_base&) _STDEX_DELETED_FUNCTION;
                func_base& operator=(const func_base&) _STDEX_DELETED_FUNCTION;
            };

        public:
            typedef _R return_type;

            function() _STDEX_NOEXCEPT_FUNCTION : _fx(nullptr)  {}
            function(stdex::nullptr_t) _STDEX_NOEXCEPT_FUNCTION : _fx(nullptr)  {}

            function(const function& other) :
                _fx(nullptr)
            { 
                if (!other._fx)
                    return;
                _fx = other._fx->_copy();
            }
            //function(function&& other) _STDEX_NOEXCEPT_FUNCTION;


            template<class _FuncT>
            function(_FuncT func)
            {
                struct _functor :
                    func_base
                {
                    typedef _FuncT func_type;
                    typedef func_base function_func_base;
                    typedef args_type function_args_type;
                    typedef _functor type;

                    _functor(func_type func) :
                        _func(stdex::detail::functional_std::move(func)) {}

                    virtual function_func_base* _copy() const { return (new type(_func)); }
                    virtual function_func_base* _move() _STDEX_NOEXCEPT_FUNCTION { return (new type(stdex::detail::functional_std::move(_func))); }
                    virtual _R _co_call(function_args_type &args)
                    {
                        typedef 
                        stdex::detail::_func_invoker<func_type, 0, function_args_type::count> 
                        func_invoker_with_null_checks;
                        typedef 
                        stdex::detail::_func_invoker<func_type, function_args_type::count, function_args_type::count> 
                        func_invoker_without_null_checks;

                        func_invoker_without_null_checks::call(_func, args);
                    }
                    virtual void _delete_this() _STDEX_NOEXCEPT_FUNCTION { delete this; }
                    // dtor non-virtual due to _delete_this()

                    func_type _func;
                };

                delete _fx;
                _fx = new _functor(stdex::detail::functional_std::move(func));
            }

            _R operator()(
                _Arg0T  arg0  = void_type(),
                _Arg1T  arg1  = void_type(),
                _Arg2T  arg2  = void_type(),
                _Arg3T  arg3  = void_type(),
                _Arg4T  arg4  = void_type(),
                _Arg5T  arg5  = void_type(),
                _Arg6T  arg6  = void_type(),
                _Arg7T  arg7  = void_type(),
                _Arg8T  arg8  = void_type(),
                _Arg9T  arg9  = void_type(),
                _Arg10T arg10 = void_type(),
                _Arg11T arg11 = void_type(),
                _Arg12T arg12 = void_type(),
                _Arg13T arg13 = void_type(),
                _Arg14T arg14 = void_type(),
                _Arg15T arg15 = void_type(),
                _Arg16T arg16 = void_type(),
                _Arg17T arg17 = void_type(),
                _Arg18T arg18 = void_type(),
                _Arg19T arg19 = void_type(),
                _Arg20T arg20 = void_type(),
                _Arg21T arg21 = void_type(),
                _Arg22T arg22 = void_type(),
                _Arg23T arg23 = void_type() ) const
            {
                typedef _args<void,    _Arg0T, 0> args_x1;               

                using stdex::detail::functional_std::forward;
                using stdex::detail::functional_std::move;

                args_type args = 
                    args_x1(forward<_Arg0T>(arg0))
                    .make(forward<_Arg1T >(arg1 ))
                    .make(forward<_Arg2T >(arg2 ))
                    .make(forward<_Arg3T >(arg3 ))
                    .make(forward<_Arg4T >(arg4 ))
                    .make(forward<_Arg5T >(arg5 ))
                    .make(forward<_Arg6T >(arg6 ))
                    .make(forward<_Arg7T >(arg7 ))
                    .make(forward<_Arg8T >(arg8 ))
                    .make(forward<_Arg9T >(arg9 ))
                    .make(forward<_Arg10T>(arg10))
                    .make(forward<_Arg11T>(arg11))
                    .make(forward<_Arg12T>(arg12))
                    .make(forward<_Arg13T>(arg13))
                    .make(forward<_Arg14T>(arg14))
                    .make(forward<_Arg15T>(arg15))
                    .make(forward<_Arg16T>(arg16))
                    .make(forward<_Arg17T>(arg17))
                    .make(forward<_Arg18T>(arg18))
                    .make(forward<_Arg19T>(arg19))
                    .make(forward<_Arg20T>(arg20))
                    .make(forward<_Arg21T>(arg21))
                    .make(forward<_Arg22T>(arg22))
                    .make(forward<_Arg23T>(arg23))
                    ;
                return _fx->_co_call(move(args));
            }

        private:


            func_base* _fx;
        };

    } // namespace detail
    

    using detail::function;

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
                    {    // fold in another byte
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
                    {    // fold in another byte
                    _val ^= (_SizeT)_first[_next];
                    _val *= _FNV_prime;
                    }
                return (_val);
            }
        };

        template<class _KeyT, class>
        struct _bitwise_hash_impl
        {    // hash functor for plain old data
            typedef _KeyT argument_type;
            typedef std::size_t result_type;

            std::size_t operator()(const _KeyT& _keyval) const
            {    // hash _keyval to std::size_t value by pseudorandomizing transform
                return (_bitwise_hash_seq<std::size_t, sizeof(std::size_t)>::call((const unsigned char *)&_keyval, sizeof (_KeyT)));
            }
        };
        
        template<class _KeyT>
        struct _bitwise_hash_impl<float, _KeyT>
        {
            typedef _KeyT argument_type;
            typedef std::size_t result_type;

            std::size_t operator()(const argument_type& _keyval) const
            {    // hash _keyval to std::size_t value by pseudorandomizing transform
                struct labdas{
                    static std::size_t hash_seq(const argument_type &keyval)
                    {
                        return (_bitwise_hash_seq<std::size_t, sizeof(std::size_t)>::call((const unsigned char *)&keyval, sizeof (argument_type)));
                    }
                };

                return (labdas::hash_seq(
                    _keyval == 0 ? 0 : _keyval));    // map -0 to 0
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
    {    // hash functor for enums
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
    {    // hash functor for bool
    };

    template<>
    struct hash<char>
        : public detail::_hash_impl<char>::type
    {    // hash functor for char
    };

    template<>
    struct hash<signed char>
        : public detail::_hash_impl<signed char>::type
    {    // hash functor for signed char
    };

    template<>
    struct hash<unsigned char>
        : public detail::_hash_impl<unsigned char>::type
    {    // hash functor for unsigned char
    };

    template<>
    struct hash<wchar_t>
        : public detail::_hash_impl<wchar_t>::type
    {    // hash functor for wchar_t
    };

    template<>
    struct hash<short>
        : public detail::_hash_impl<short>::type
    {    // hash functor for short
    };

    template<>
    struct hash<unsigned short>
        : public detail::_hash_impl<unsigned short>::type
    {    // hash functor for unsigned short
    };

    template<>
    struct hash<int>
        : public detail::_hash_impl<int>::type
    {    // hash functor for int
    };

    template<>
    struct hash<unsigned int>
        : public detail::_hash_impl<unsigned int>::type
    {    // hash functor for unsigned int
    };

    template<>
    struct hash<long>
        : public detail::_hash_impl<long>::type
    {    // hash functor for long
    };

    template<>
    struct hash<unsigned long>
        : public detail::_hash_impl<unsigned long>::type
    {    // hash functor for unsigned long
    };

    template<>
    struct hash<long long>
        : public detail::_hash_impl<long long>::type
    {    // hash functor for long long
    };

    template<>
    struct hash<unsigned long long>
        : public detail::_hash_impl<unsigned long long>::type
    {    // hash functor for unsigned long long
    };

    template<>
    struct hash<float>
        : public detail::_hash_impl<float>::type
    {    // hash functor for float
    };

    template<>
    struct hash<double>
        : public detail::_hash_impl<double>::type
    {    // hash functor for double
    };

    template<>
    struct hash<long double>
        : public detail::_hash_impl<long double>::type
    {    // hash functor for long double
    };

    template<class _KeyT>
    struct hash<_KeyT *>
        : public detail::_hash_impl<_KeyT *>::type
    {    // hash functor for _KeyT *
    };

    // standard hash overloads for std headers

    template<std::size_t _Bits>
    struct hash<std::bitset<_Bits>/**/>;
    //{    // hash functor for bitset<_Bits>
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
    //{    // hash functor for vector<bool, _AllocatorT>
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
    {    // functor for operator&
        typedef _Tp first_argument_type;
        typedef _Tp second_argument_type;
        typedef _Tp result_type;

        _Tp operator()(const _Tp& _left, const _Tp& _right) const
        {    // apply operator& to operands
            return (_left & _right);
        }
    };

    template<class _Tp>
    struct bit_or
    {    // functor for operator|
        typedef _Tp first_argument_type;
        typedef _Tp second_argument_type;
        typedef _Tp result_type;

        _Tp operator()(const _Tp& _left, const _Tp& _right) const
        {    // apply operator| to operands
            return (_left | _right);
        }
    };

    template<class _Tp>
    struct bit_xor
    {    // functor for operator^
        typedef _Tp first_argument_type;
        typedef _Tp second_argument_type;
        typedef _Tp result_type;

        _Tp operator()(const _Tp& _left, const _Tp& _right) const
        {    // apply operator^ to operands
            return (_left ^ _right);
        }
    };

    template<class _Tp>
    struct bit_not
    {    // functor for unary operator~
        typedef _Tp argument_type;
        typedef _Tp result_type;

        _Tp operator()(const _Tp& _left) const
        {    // apply operator~ to operand
            return (~_left);
        }
    };
    
 } // namespace stdex
 
 #undef _STDEX_DELETED_FUNCTION
 #undef _STDEX_NOEXCEPT_FUNCTION
 
 #endif // _STDEX_FUNCTIONAL_H
 
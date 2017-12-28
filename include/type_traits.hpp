#ifndef _STDEX_TYPE_TRAITS_H
#define _STDEX_TYPE_TRAITS_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// stdex includes
/*none*/

// POSIX includes
/*none*/

// std includes
#include <cstddef>

namespace stdex
{
	template<class _Tp, _Tp Val>
	struct integral_constant
	{	// convenient template for integral constant types
		static const _Tp value = Val;

		typedef const _Tp value_type;
		typedef integral_constant<_Tp, Val> type;

		operator value_type() const
		{	// return stored value
			return (value);
		}

		value_type operator()() const
		{	// return stored value
			return (value);
		}
	};

	typedef integral_constant<bool, true> true_type;
	typedef integral_constant<bool, false> false_type;

	template<bool Val>
	struct bool_constant :
		public integral_constant<bool, Val>
	{};

	// Primary template.
	/// Define a member typedef @c type to one of two argument types.
	template<bool _Cond, class _Iftrue, class _Iffalse>
	struct conditional
	{
		typedef _Iftrue type;
	};

	// Partial specialization for false.
	template<class _Iftrue, class _Iffalse>
	struct conditional<false, _Iftrue, _Iffalse>
	{
		typedef _Iffalse type;
	};

	namespace detail
	{
		struct void_type {};

		template<class _B1 = void_type, class _B2 = void_type, class _B3 = void_type, class _B4 = void_type>
		struct _or_;


		template<>
		struct _or_<void_type, void_type, void_type, void_type>;

		template<class _B1>
		struct _or_<_B1, void_type, void_type, void_type>
			: public _B1
		{ };

		template<class _B1, class _B2>
		struct _or_<_B1, _B2, void_type, void_type>
			: public conditional<_B1::value, _B1, _B2>::type
		{ };

		template<class _B1, class _B2, class _B3>
		struct _or_<_B1, _B2, _B3, void_type>
			: public conditional<_B1::value, _B1, _or_<_B2, _B3> >::type
		{ };

		template<class _B1 = void_type, class _B2 = void_type, class _B3 = void_type, class _B4 = void_type>
		struct _and_;


		template<>
		struct _and_<void_type, void_type, void_type, void_type>;

		template<class _B1>
		struct _and_<_B1, void_type, void_type, void_type>
			: public _B1
		{ };

		template<class _B1, class _B2>
		struct _and_<_B1, _B2, void_type, void_type>
			: public conditional<_B1::value, _B2, _B1>::type
		{ };

		template<class _B1, class _B2, class _B3>
		struct _and_<_B1, _B2, _B3, void_type>
			: public conditional<_B1::value, _and_<_B2, _B3>, _B1>::type
		{ };

		template<class _Pp>
		struct _not_
		{ 
			static const bool value = !bool(_Pp::value);
		};
	}

	template<bool Val>
	struct _cat_base: 
		integral_constant<bool, Val>
	{	// base class for type predicates
	};

	template<class _Tp>
	struct remove_const
	{	// remove top level const qualifier
		typedef _Tp type;
	};

	template<class _Tp>
	struct remove_const<const _Tp>
	{	// remove top level const qualifier
		typedef _Tp type;
	};

	template<class _Tp>
	struct remove_const<const volatile _Tp>
	{	// remove top level const qualifier
		typedef volatile _Tp type;
	};

	// TEMPLATE CLASS remove_volatile
	template<class _Tp>
	struct remove_volatile
	{	// remove top level volatile qualifier
		typedef _Tp type;
	};

	template<class _Tp>
	struct remove_volatile<volatile _Tp>
	{	// remove top level volatile qualifier
		typedef _Tp type;
	};

	// TEMPLATE CLASS remove_cv
	template<class _Tp>
	struct remove_cv
	{	// remove top level const and volatile qualifiers
		typedef typename remove_const<typename remove_volatile<_Tp>::type>::type
			type;
	};

	namespace detail
	{
		template <class> struct is_floating_point : public false_type {};

		template<> struct is_floating_point<float> : public true_type {};
		template<> struct is_floating_point<double> : public true_type {};
		template<> struct is_floating_point<long double> : public true_type {};

		template <class> struct is_integral : public false_type {};

		template<> struct is_integral<bool> : public true_type {};
		template<> struct is_integral<char> : public true_type {};
		template<> struct is_integral<wchar_t> : public true_type {};

#ifdef STDEX_FORCE_CPP11_TYPES_SUPPORT
#include "types_ex.h"
		template<> struct is_integral<char16_t> : public true_type {};
		template<> struct is_integral<char32_t> : public true_type {};
		template<> struct is_integral<int64_t> : public true_type {};
		template<> struct is_integral<uint64_t> : public true_type {};
#endif

		template<> struct is_integral<unsigned char> : public true_type {};
		template<> struct is_integral<unsigned short int> : public true_type {};
		template<> struct is_integral<unsigned int> : public true_type {};
		template<> struct is_integral<unsigned long int> : public true_type {};

		template<> struct is_integral<signed char> : public true_type {};
		template<> struct is_integral<short int> : public true_type {};
		template<> struct is_integral<int> : public true_type {};
		template<> struct is_integral<long int> : public true_type {};
	}


	template <class _Tp> struct is_floating_point : public detail::is_floating_point<typename remove_cv<_Tp>::type> {};

	template <class _Tp>
	struct is_integral :
		public true_type
	{
	};

	namespace detail
	{
		template<bool>
		struct _sign_unsign_chooser
		{
		};

		template<typename _Tp>
		struct _signed_comparer
		{
			static const bool value = _Tp(-1) < _Tp(0);
		};

		template<typename _Tp>
		struct UnsignedComparer
		{
			static const bool value = _Tp(0) < _Tp(-1);
		};

		template<>
		struct _sign_unsign_chooser<true>//integral
		{
			template<class _Tp>
			struct _signed :
				public _cat_base<_signed_comparer<typename remove_cv<_Tp>::type>::value>
			{
			};

			template<class _Tp>
			struct _unsigned :
				public _cat_base<UnsignedComparer<typename remove_cv<_Tp>::type>::value>
			{
			};
		};

		template<>
		struct _sign_unsign_chooser<false>//floating point
		{
			template<class _Tp>
			struct _signed :
				public is_floating_point<_Tp>
			{
			};

			template<class _Tp>
			struct _unsigned :
				public false_type
			{
			};
		};
	}

	template <bool, class _Tp = void>
	struct enable_if
	{};

	template <class _Tp>
	struct enable_if<true, _Tp> {
		typedef _Tp type;
	};

	template<class, class>
	struct is_same :
		public false_type
	{
	};

	template<class _Tp>
	struct is_same<_Tp, _Tp> :
		public true_type//specialization
	{
	};

	template <class _Tp>
	struct is_const :
		public false_type
	{
	};

	template <class _Tp>
	struct is_const<const _Tp> :
		public true_type
	{
	};

	template <class _Tp>
	struct is_const<const volatile _Tp> :
		public true_type
	{
	};

	template< class _Tp >
	struct remove_reference
	{
		typedef _Tp type;
	};

	template< class _Tp >
	struct remove_reference<_Tp&>
	{
		typedef _Tp type;
	};

	template<class T>
	struct is_signed
	{	// determine whether T is a signed type

		static const bool value = detail::_sign_unsign_chooser<is_integral<T>::value>::template _signed<T>::value;
	};

	template<class T>
	struct is_unsigned
	{	// determine whether T is an unsigned type
		static const bool value = detail::_sign_unsign_chooser<is_integral<T>::value>::template _unsigned<T>::value;
	};

	namespace detail
	{
		template<class>
		struct _is_void_helper
			: public false_type { };

		template<>
		struct _is_void_helper<void>
			: public true_type { };

	}
	/// is_void
	template<class _Tp>
	struct is_void: 
		public detail::_is_void_helper<typename remove_cv<_Tp>::type>::type
	{ };

	/// is_array
	template<class>
	struct is_array: 
		public false_type { };

	template<class _Tp, std::size_t _Size>
	struct is_array<_Tp[_Size]>: 
		public true_type { };

	/*template<class _Tp>
	struct is_array<_Tp[]>:
		public true_type { }; */

	namespace detail
	{
		template<class>
		struct _is_pointer_helper: 
			public false_type { };

		template<class _Tp>
		struct _is_pointer_helper<_Tp*>: 
			public true_type { };
	}

	/// is_pointer
	template<class _Tp>
	struct is_pointer: 
		public detail::_is_pointer_helper<typename remove_cv<_Tp>::type>::type
	{ };

	/// is_lvalue_reference
	template<class>
	struct is_lvalue_reference: 
		public false_type { };

	template<class _Tp>
	struct is_lvalue_reference<_Tp&>: 
		public true_type { };

	namespace detail
	{
		template <class R>
		struct _is_function_ptr_helper : false_type {};
		template <class R >
		struct _is_function_ptr_helper<R(*)()> : true_type {};
		template <class R >
		struct _is_function_ptr_helper<R(*)(...)> : true_type {};
		template <class R, class T0>
		struct _is_function_ptr_helper<R(*)(T0)> : true_type {};
		template <class R, class T0>
		struct _is_function_ptr_helper<R(*)(T0 ...)> : true_type {};
		template <class R, class T0, class T1>
		struct _is_function_ptr_helper<R(*)(T0, T1)> : true_type {};
		template <class R, class T0, class T1>
		struct _is_function_ptr_helper<R(*)(T0, T1 ...)> : true_type {};
		template <class R, class T0, class T1, class T2>
		struct _is_function_ptr_helper<R(*)(T0, T1, T2)> : true_type {};
		template <class R, class T0, class T1, class T2>
		struct _is_function_ptr_helper<R(*)(T0, T1, T2 ...)> : true_type {};
		template <class R, class T0, class T1, class T2, class T3>
		struct _is_function_ptr_helper<R(*)(T0, T1, T2, T3)> : true_type {};
		template <class R, class T0, class T1, class T2, class T3>
		struct _is_function_ptr_helper<R(*)(T0, T1, T2, T3 ...)> : true_type {};
		template <class R, class T0, class T1, class T2, class T3, class T4>
		struct _is_function_ptr_helper<R(*)(T0, T1, T2, T3, T4)> : true_type {};
		template <class R, class T0, class T1, class T2, class T3, class T4>
		struct _is_function_ptr_helper<R(*)(T0, T1, T2, T3, T4 ...)> : true_type {};
		template <class R, class T0, class T1, class T2, class T3, class T4, class T5>
		struct _is_function_ptr_helper<R(*)(T0, T1, T2, T3, T4, T5)> : true_type {};
		template <class R, class T0, class T1, class T2, class T3, class T4, class T5>
		struct _is_function_ptr_helper<R(*)(T0, T1, T2, T3, T4, T5 ...)> : true_type {};
		template <class R, class T0, class T1, class T2, class T3, class T4, class T5, class T6>
		struct _is_function_ptr_helper<R(*)(T0, T1, T2, T3, T4, T5, T6)> : true_type {};
		template <class R, class T0, class T1, class T2, class T3, class T4, class T5, class T6>
		struct _is_function_ptr_helper<R(*)(T0, T1, T2, T3, T4, T5, T6 ...)> : true_type {};
		template <class R, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7>
		struct _is_function_ptr_helper<R(*)(T0, T1, T2, T3, T4, T5, T6, T7)> : true_type {};
		template <class R, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7>
		struct _is_function_ptr_helper<R(*)(T0, T1, T2, T3, T4, T5, T6, T7 ...)> : true_type {};
		template <class R, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>
		struct _is_function_ptr_helper<R(*)(T0, T1, T2, T3, T4, T5, T6, T7, T8)> : true_type {};
		template <class R, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>
		struct _is_function_ptr_helper<R(*)(T0, T1, T2, T3, T4, T5, T6, T7, T8 ...)> : true_type {};
		template <class R, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9>
		struct _is_function_ptr_helper<R(*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9)> : true_type {};
		template <class R, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9>
		struct _is_function_ptr_helper<R(*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9 ...)> : true_type {};
		template <class R, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10>
		struct _is_function_ptr_helper<R(*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10)> : true_type {};
		template <class R, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10>
		struct _is_function_ptr_helper<R(*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10 ...)> : true_type {};
		template <class R, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11>
		struct _is_function_ptr_helper<R(*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11)> : true_type {};
		template <class R, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11>
		struct _is_function_ptr_helper<R(*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11 ...)> : true_type {};
		template <class R, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12>
		struct _is_function_ptr_helper<R(*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12)> : true_type {};
		template <class R, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12>
		struct _is_function_ptr_helper<R(*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12 ...)> : true_type {};
		template <class R, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13>
		struct _is_function_ptr_helper<R(*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13)> : true_type {};
		template <class R, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13>
		struct _is_function_ptr_helper<R(*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13 ...)> : true_type {};
		template <class R, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13, class T14>
		struct _is_function_ptr_helper<R(*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14)> : true_type {};
		template <class R, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13, class T14>
		struct _is_function_ptr_helper<R(*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14 ...)> : true_type {};
		template <class R, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13, class T14, class T15>
		struct _is_function_ptr_helper<R(*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15)> : true_type {};
		template <class R, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13, class T14, class T15>
		struct _is_function_ptr_helper<R(*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15 ...)> : true_type {};
		template <class R, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13, class T14, class T15, class T16>
		struct _is_function_ptr_helper<R(*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16)> : true_type {};
		template <class R, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13, class T14, class T15, class T16>
		struct _is_function_ptr_helper<R(*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16 ...)> : true_type {};
		template <class R, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13, class T14, class T15, class T16, class T17>
		struct _is_function_ptr_helper<R(*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17)> : true_type {};
		template <class R, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13, class T14, class T15, class T16, class T17>
		struct _is_function_ptr_helper<R(*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17 ...)> : true_type {};
		template <class R, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13, class T14, class T15, class T16, class T17, class T18>
		struct _is_function_ptr_helper<R(*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18)> : true_type {};
		template <class R, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13, class T14, class T15, class T16, class T17, class T18>
		struct _is_function_ptr_helper<R(*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18 ...)> : true_type {};
		template <class R, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13, class T14, class T15, class T16, class T17, class T18, class T19>
		struct _is_function_ptr_helper<R(*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19)> : true_type {};
		template <class R, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13, class T14, class T15, class T16, class T17, class T18, class T19>
		struct _is_function_ptr_helper<R(*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19 ...)> : true_type {};
		template <class R, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13, class T14, class T15, class T16, class T17, class T18, class T19, class T20>
		struct _is_function_ptr_helper<R(*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20)> : true_type {};
		template <class R, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13, class T14, class T15, class T16, class T17, class T18, class T19, class T20>
		struct _is_function_ptr_helper<R(*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20 ...)> : true_type {};
		template <class R, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13, class T14, class T15, class T16, class T17, class T18, class T19, class T20, class T21>
		struct _is_function_ptr_helper<R(*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21)> : true_type {};
		template <class R, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13, class T14, class T15, class T16, class T17, class T18, class T19, class T20, class T21>
		struct _is_function_ptr_helper<R(*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21 ...)> : true_type {};
		template <class R, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13, class T14, class T15, class T16, class T17, class T18, class T19, class T20, class T21, class T22>
		struct _is_function_ptr_helper<R(*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22)> : true_type {};
		template <class R, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13, class T14, class T15, class T16, class T17, class T18, class T19, class T20, class T21, class T22>
		struct _is_function_ptr_helper<R(*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22 ...)> : true_type {};
		template <class R, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13, class T14, class T15, class T16, class T17, class T18, class T19, class T20, class T21, class T22, class T23>
		struct _is_function_ptr_helper<R(*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23)> : true_type {};
		template <class R, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13, class T14, class T15, class T16, class T17, class T18, class T19, class T20, class T21, class T22, class T23>
		struct _is_function_ptr_helper<R(*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23 ...)> : true_type {};
		template <class R, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13, class T14, class T15, class T16, class T17, class T18, class T19, class T20, class T21, class T22, class T23, class T24>
		struct _is_function_ptr_helper<R(*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24)> : true_type {};
		template <class R, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13, class T14, class T15, class T16, class T17, class T18, class T19, class T20, class T21, class T22, class T23, class T24>
		struct _is_function_ptr_helper<R(*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24 ...)> : true_type {};
	}

	/// is_function
	template<class _Tp>
	struct is_function
	{
		static const bool value = detail::_is_function_ptr_helper<_Tp>::value;
	};

	namespace detail
	{
		template<class>
		struct _is_member_object_pointer_helper: 
			public false_type { };

		template<class _Tp, class _Cp>
		struct _is_member_object_pointer_helper<_Tp _Cp::*>
		{
			static const bool value = !is_function<_Tp>::value;
		};

		template<class>
		struct _is_member_function_pointer_helper: 
			public false_type { };

		template<class _Tp, class _Cp>
		struct _is_member_function_pointer_helper<_Tp _Cp::*> 
		{ 
			static const bool value = is_function<_Tp>::value;
		};
	}

	/// is_member_object_pointer
	template<class _Tp>
	struct is_member_object_pointer: 
		public detail::_is_member_object_pointer_helper<typename remove_cv<_Tp>::type>::type
	{ };

	/// is_member_function_pointer
	template<class _Tp>
	struct is_member_function_pointer: 
		public detail::_is_member_function_pointer_helper<typename remove_cv<_Tp>::type>::type
	{ };

	/// is_reference
	template<class _Tp>
	struct is_reference: 
		public detail::_or_<is_lvalue_reference<_Tp>/*, is_rvalue_reference<_Tp> */>::type
	{};
	
	/// is_arithmetic
	template<class _Tp>
	struct is_arithmetic: 
		public detail::_or_<is_integral<_Tp>, is_floating_point<_Tp> >::type
	{ };

	/// is_object
	template<class _Tp>
	struct is_object: 
		public detail::_not_< detail::_or_< is_function<_Tp>, is_reference<_Tp>, is_void<_Tp> > >::type
	{};

} // namespace stdex

#endif // _STDEX_TYPE_TRAITS_H

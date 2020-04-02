#ifndef _STDEX_MOVE_H
#define _STDEX_MOVE_H

// stdex includes
#include "./type_traits.hpp" // stdex::is_class

// POSIX includes
/*none*/

// std includes
#include <cstddef> // std::ptrdiff_t, std::size_t, NULL

#ifdef _MSC_VER
#pragma warning (push)
	#pragma warning (disable : 4512) // assignment operator could not be generated
#endif

#ifdef _STDEX_NATIVE_CPP11_SUPPORT

#define _STDEX_DELETED_FUNCTION =delete
#define _STDEX_NOEXCEPT_FUNCTION noexcept 

#else

#define _STDEX_DELETED_FUNCTION 
#define _STDEX_NOEXCEPT_FUNCTION throw()

#endif

namespace stdex
{
	namespace detail
	{
		template<class _Tp, _Tp _Value>
		class deleted_implicit_copy_constructor_impl
		{
			const _Tp dummy:1;
		public:
			deleted_implicit_copy_constructor_impl(const _Tp &):
				dummy(_Value) { (void)(dummy); }
			deleted_implicit_copy_constructor_impl &operator=(const _Tp &)
			{ return *this; }
		};

		typedef stdex::detail::deleted_implicit_copy_constructor_impl<char, 0> deleted_implicit_copy_constructor;
		
		class delete_implicit_copy_constructor:
			deleted_implicit_copy_constructor
		{
		public:
			delete_implicit_copy_constructor(const bool &):
				deleted_implicit_copy_constructor(1)
			{ }
		};
	} // namespace detail

	namespace move_detail
	{
		template<class _Tp, bool>
		struct rvalue_reference_base_impl;

		template<class _Tp>
		struct rvalue_reference_base_impl<_Tp, true>
		{
			typedef typename stdex::remove_reference<typename stdex::remove_cv<_Tp>::type>::type type;
		};

		template<class _Tp>
		struct rvalue_reference_base_impl<_Tp, false>
		{
			typedef typename stdex::remove_reference<typename stdex::remove_cv<_Tp>::type>::type value_type;

			struct type {
				mutable value_type value;

				operator value_type&() const
				{
					return value;
				}
			};
		};
		template<class _Tp>
		struct rvalue_reference_base
		{
			typedef 
			typename 
			rvalue_reference_base_impl<
				_Tp, 
				stdex::is_class<typename stdex::remove_cv<_Tp>::type>::value
			>::type type;
		};
	} // namespace move_detail

    template<class _Tp>
    class rvalue_reference:
		public move_detail::rvalue_reference_base<_Tp>::type
    { 
	public:
		typedef typename move_detail::rvalue_reference_base<_Tp>::type base_type;
	};

	namespace move_detail
	{
		template<class _Tp>
		struct rvalue_reference_base<rvalue_reference<_Tp>/**/>
		{
			typedef typename rvalue_reference<_Tp>::base_type type;
		};
	} // namespace move_detail

    template<class _Tp>
    class rvalue_reference <const _Tp>:
		public move_detail::rvalue_reference_base<_Tp>::type
    { 
	public:
		typedef typename move_detail::rvalue_reference_base<_Tp>::type base_type;
	};

	namespace move_detail
	{
		template<class _Tp>
		struct _is_rvalue_reference:
			false_type
		{ };

		template<class _Tp>
		struct _is_rvalue_reference<rvalue_reference<_Tp>/**/>:
			true_type
		{ };

	} // namespace move_detail

	template<class _Tp>
	rvalue_reference<typename stdex::remove_cv<_Tp>::type>& move(_Tp& value) _STDEX_NOEXCEPT_FUNCTION
	{
		typedef rvalue_reference<typename stdex::remove_cv<_Tp>::type> type;
		return reinterpret_cast<type&>(value);
	}

	template<class _Tp>
	const rvalue_reference<const _Tp>& move(const rvalue_reference<const _Tp>& value) _STDEX_NOEXCEPT_FUNCTION
	{
		return value;
	}

	template<class _Tp>
	rvalue_reference<_Tp>& move(rvalue_reference<_Tp>& value) _STDEX_NOEXCEPT_FUNCTION
	{
		return value;
	}

	template<class _Tp>
	const rvalue_reference<const typename stdex::remove_cv<_Tp>::type>& move(const _Tp& value, ...) _STDEX_NOEXCEPT_FUNCTION
	{
		typedef const rvalue_reference<const typename stdex::remove_cv<_Tp>::type> type;
		return reinterpret_cast<type&>(value);
	}


	template<class _Tp>
	_Tp& forward(_Tp& value) _STDEX_NOEXCEPT_FUNCTION
	{
		return value;
	}

	template<class _Tp>
	const rvalue_reference<const _Tp>& forward(const rvalue_reference<const _Tp>& value) _STDEX_NOEXCEPT_FUNCTION
	{
		return value;
	}

	template<class _Tp>
	rvalue_reference<_Tp>& forward(rvalue_reference<_Tp>& value) _STDEX_NOEXCEPT_FUNCTION
	{
		return value;
	}

	template<class _Tp>
	const _Tp& forward(const _Tp& value, ...) _STDEX_NOEXCEPT_FUNCTION
	{
		return value;
	}

}

#define STDEX_NOT_COPYABLE \
	stdex::detail::delete_implicit_copy_constructor _stdex_icc_deleter;

#define STDEX_DELETE_ICC() _stdex_icc_deleter(true)

#define STDEX_RV_REF(Type) stdex::rvalue_reference< Type >&
#define STDEX_RV_REF_CONST(Type) const stdex::rvalue_reference< Type const>&

#undef _STDEX_DELETED_FUNCTION
#undef _STDEX_NOEXCEPT_FUNCTION

#ifdef _MSC_VER
	#pragma warning (pop)
#endif

#endif // _STDEX_MOVE_H

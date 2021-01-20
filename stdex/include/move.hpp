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
			delete_implicit_copy_constructor(delete_implicit_copy_constructor&):
				deleted_implicit_copy_constructor(1) {}
			delete_implicit_copy_constructor(delete_implicit_copy_constructor&, char dummy = 0) :
				deleted_implicit_copy_constructor(dummy) {} // ambiguous copy constructor
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
				//value_type value;
				operator value_type&() const
				{
					return *reinterpret_cast<value_type*>(const_cast<type*>(this));
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

		template<>
		struct rvalue_reference_base<bool>
		{
			typedef 
			rvalue_reference_base_impl<
				bool, 
				false
			>::type type;
		};
	} // namespace move_detail

#define STDEX_RV_REF(Type) stdex::rvalue_reference< Type, false >&

#define STDEX_RV_REF_CONST(Type) const stdex::rvalue_reference< const Type , true >&

#define STDEX_FWD_REF(Type) Type&

	template<class _Tp>
	class rvalue_reference_moved;

    template<class _Tp, bool _IsConst = false>
    class rvalue_reference:
		public rvalue_reference <const _Tp, true>
    { 
		typedef rvalue_reference <const _Tp, true> base_type;
		typedef _Tp value_type;

		rvalue_reference();
		rvalue_reference(rvalue_reference const&);
		//~rvalue_reference() throw();
		void operator=(rvalue_reference const&);
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
    class rvalue_reference <const _Tp, true>:
		public move_detail::rvalue_reference_base<_Tp>::type
    { 
		typedef typename move_detail::rvalue_reference_base<_Tp>::type base_type;
		typedef const _Tp value_type;

		rvalue_reference();
		//~rvalue_reference() throw();
		rvalue_reference(rvalue_reference const&);
		void operator=(rvalue_reference const&);
	};

	template<class _Tp>
	class rvalue_reference_moved:
		public rvalue_reference<_Tp, stdex::is_const<_Tp>::value>
	{
		rvalue_reference_moved();
		//~rvalue_reference() throw();
		rvalue_reference_moved(rvalue_reference_moved const&);
		void operator=(rvalue_reference_moved const&);

		typedef rvalue_reference<_Tp, stdex::is_const<_Tp>::value> base_type;

	public:
		template<class _ChildT>
		operator rvalue_reference<_ChildT, stdex::is_const<_Tp>::value> & () {
			_ChildT& lvalue_ref = *this;
			return reinterpret_cast<rvalue_reference<_ChildT, stdex::is_const<_Tp>::value>&>(lvalue_ref);
		}
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

		template<class _Tp>
		struct _is_rvalue_reference<rvalue_reference_moved<_Tp>/**/> :
			true_type
		{ };

		template<class _Tp>
		struct _do_const_cast:
			false_type
		{ };

		template<class _Tp, bool _IsConst>
		struct _do_const_cast<rvalue_reference<_Tp, _IsConst>/**/>:
			bool_constant<_IsConst == bool(false)>
		{ };

		template<class _Tp>
		struct _do_const_cast<rvalue_reference_moved<_Tp>/**/> :
			stdex::detail::_not_<stdex::is_const<_Tp>/**/>
		{ };

		template<class _Tp>
		struct _move_const
		{
			typedef STDEX_RV_REF_CONST(typename stdex::remove_cv<_Tp>::type) type;
			static type call(const _Tp &value) {return reinterpret_cast<type>(value);}
		};

		template<class _Tp>
		struct _move_const<rvalue_reference<_Tp>/**/>
		{
			typedef typename stdex::remove_const<_Tp>::type value_type;
			typedef STDEX_RV_REF_CONST(_Tp) type1;
			static type1 call(STDEX_RV_REF_CONST(value_type) value) {return reinterpret_cast<type1>(value);}

			typedef STDEX_RV_REF(_Tp) type2;
			static type2 call(const STDEX_RV_REF(value_type) value) {return const_cast<type2>(value);}
		};

		//template<class _Tp>
		//struct _move_const<rvalue_reference_moved<_Tp>/**/>:
		//	_move_const<rvalue_reference<_Tp>/**/>
		//{ };

	} // namespace move_detail


	//template<class _Tp>
	//rvalue_reference<_Tp>& move(const rvalue_reference_moved<_Tp>& value) _STDEX_NOEXCEPT_FUNCTION
	//{
	//	typedef rvalue_reference<_Tp> type;
	//	return reinterpret_cast<type&>(value);
	//}

	template<class _Tp>
	STDEX_RV_REF(typename stdex::remove_cv<_Tp>::type) move(_Tp& value) _STDEX_NOEXCEPT_FUNCTION
	{
		typedef STDEX_RV_REF(typename stdex::remove_cv<_Tp>::type) type;
		return reinterpret_cast<type>(value);
	}

	template<class _Tp>
	rvalue_reference<_Tp, false>& move(const rvalue_reference<_Tp, false>& value) _STDEX_NOEXCEPT_FUNCTION
	{
		return const_cast<rvalue_reference<_Tp>&>(value);
	}

	//template<class _Tp>
	//rvalue_reference<_Tp, false>& move(rvalue_reference<_Tp, false>& value) _STDEX_NOEXCEPT_FUNCTION
	//{
	//	return value;
	//}

	//template<class _Tp>
	//const rvalue_reference<const _Tp, true>& move(const rvalue_reference<const _Tp, true>& value) _STDEX_NOEXCEPT_FUNCTION
	//{
	//	return value;
	//}

	template<class _Tp>
	rvalue_reference_moved<_Tp>& move(rvalue_reference<_Tp, false> &value) _STDEX_NOEXCEPT_FUNCTION
	{
		return reinterpret_cast<rvalue_reference_moved<_Tp>&>(value);
	}

	template<class _Tp>
	const rvalue_reference_moved<_Tp>& move(const rvalue_reference<_Tp, true>& value) _STDEX_NOEXCEPT_FUNCTION
	{
		return reinterpret_cast<const rvalue_reference_moved<_Tp>&>(value);
	}

	template<class _Tp>
	rvalue_reference_moved<_Tp>& move(rvalue_reference_moved<_Tp>& value) _STDEX_NOEXCEPT_FUNCTION
	{
		return value;
	}

	template<class _Tp>
	typename
	stdex::conditional<
		move_detail::_do_const_cast<_Tp>::value == bool(true),
		const _Tp&,
		STDEX_RV_REF_CONST(typename stdex::remove_cv<_Tp>::type)
	>::type move(const _Tp& value) _STDEX_NOEXCEPT_FUNCTION
	{
		return
			move_detail::_move_const<_Tp>::call(value);
	}

	template<class _Tp>
	STDEX_FWD_REF(_Tp) forward(_Tp& value) _STDEX_NOEXCEPT_FUNCTION
	{
		return value;
	}

	template<class _Tp>
	typename
	stdex::conditional<
		move_detail::_do_const_cast<_Tp>::value == bool(true),
		_Tp&,
		const _Tp&
	>::type forward(const _Tp& value) _STDEX_NOEXCEPT_FUNCTION
	{
		typedef
		typename
		stdex::conditional<
			move_detail::_do_const_cast<_Tp>::value == bool(true),
			_Tp,
			const _Tp
		>::type type;
		return const_cast<type&>(value);
	}

	namespace move_detail
	{
		template<class _Tp>
		struct _peek_conversion_operator_helper:
			intern::_has_bug<intern::_stdex_enum_can_have_member_pointer_bug>
		{ };

		template<class _Tp>
		struct _peek_conversion_operator
		{
			typedef 
			typename
			conditional<
				_peek_conversion_operator_helper<_Tp>::value,
				struct _disabled&,
				_Tp
			>::type type;
		};
	} // namespace move_detail
}

#define STDEX_NOT_COPYABLE \
	stdex::detail::delete_implicit_copy_constructor _stdex_implicit_copy_constructor_deleter;

#define STDEX_MOVABLE(Type) \
	friend class stdex::rvalue_reference< Type, false >; \
	friend class stdex::rvalue_reference< const Type, true >;\
	public: \
	template<bool _IsConst> \
	inline operator stdex::rvalue_reference<typename stdex::move_detail::_peek_conversion_operator< Type >::type, _IsConst>& () \
	{ \
		typedef stdex::rvalue_reference<typename stdex::move_detail::_peek_conversion_operator< Type >::type, false> result_type; \
		Type &lvalue_ref = *this; \
		return *reinterpret_cast<result_type*>( \
				&const_cast<char&>( \
					reinterpret_cast<const volatile char&>(lvalue_ref))); \
	} \
	private:

#ifdef __BORLANDC__
    //#undef STDEX_MOVABLE
    //#define STDEX_MOVABLE(Nothing)
#endif

#define STDEX_MOVABLE_BUT_NOT_COPYABLE(Type) \
	STDEX_NOT_COPYABLE \
	STDEX_MOVABLE(Type)

#define STDEX_DELETE_ICC() _stdex_implicit_copy_constructor_deleter(true)

#undef _STDEX_DELETED_FUNCTION
#undef _STDEX_NOEXCEPT_FUNCTION

#ifdef _MSC_VER
	#pragma warning (pop)
#endif

#endif // _STDEX_MOVE_H

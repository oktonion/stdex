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

namespace stdex
{
	namespace detail
	{
		template<class T, T val>
		class deleted_implicit_copy_constructor_impl
		{
			const T dummy:1;
		public:
			deleted_implicit_copy_constructor_impl(const T &):
				dummy(val) { (void)(dummy); }
			deleted_implicit_copy_constructor_impl &operator=(deleted_implicit_copy_constructor_impl &other)
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
		template<class _Tp, bool Value>
		struct rvalue_reference_base_impl
		{
			typedef typename stdex::remove_const<_Tp>::type type;
		};
		template<class _Tp>
		struct rvalue_reference_base_impl<_Tp, false>
		{
			struct type { template<class T> type(T) {}};
		};
		template<class _Tp>
		struct rvalue_reference_base
		{
			typedef 
			typename 
			rvalue_reference_base_impl<
				_Tp, 
				stdex::is_class<typename stdex::remove_const<_Tp>::type>::value
			>::type type;
		};
	} // namespace move_detail

    template<class _Tp>
    class rvalue_reference:
		public move_detail::rvalue_reference_base<_Tp>::type
    {
	protected:
		typedef typename move_detail::rvalue_reference_base<_Tp>::type base_type;
		typedef typename stdex::remove_const<_Tp>::type value_type;
		value_type& _ref;

        explicit rvalue_reference(value_type &ref)
            : base_type(ref)
			, _ref((ref)) 
		{ }
		
    public:
        rvalue_reference(const rvalue_reference<_Tp> &other)
			: base_type(other)
			, _ref(other._ref) 
		{ } 

		operator value_type&() const {return _ref;}

		static rvalue_reference<value_type> move(value_type &value) 
		{
			return rvalue_reference<value_type>(value);
		}

		static rvalue_reference<value_type> move(const rvalue_reference<value_type>& value)
		{
			return rvalue_reference<value_type>(value._ref);
		}
    };

    template<class _Tp>
    class rvalue_reference <const _Tp>:
		public move_detail::rvalue_reference_base<_Tp>::type
    {
	protected:
		typedef typename move_detail::rvalue_reference_base<_Tp>::type base_type;
		typedef const _Tp value_type;
		value_type& _ref;

        explicit rvalue_reference(value_type &ref)
            : base_type(ref)
			, _ref((ref)) 
		{ }

    public:
        rvalue_reference(const rvalue_reference<_Tp> &other)
			: base_type(other)
			, _ref(other._ref) 
		{ }

		operator value_type&() const {return _ref;}

		static rvalue_reference<value_type> move(value_type &value) 
		{
			return rvalue_reference<value_type>(value);
		}

		static rvalue_reference<value_type> move(const rvalue_reference<value_type>& value)
		{
			return rvalue_reference<value_type>(value._ref);
		}
    };

	template<class _Tp>
	rvalue_reference<_Tp> move(_Tp& value)
	{
		return rvalue_reference<_Tp>::move(value);
	}

	template<class _Tp>
	rvalue_reference<const _Tp> move(const _Tp& value, ...)
	{
		return rvalue_reference<const _Tp>::move(value);
	}

	template<class _Tp>
	rvalue_reference<_Tp> move(const rvalue_reference<_Tp>& value)
	{
		return rvalue_reference<_Tp>::move(value);
	}
}

#define STDEX_NOT_COPYABLE \
	stdex::detail::delete_implicit_copy_constructor _stdex_icc_deleter;

#define STDEX_DELETE_ICC() _stdex_icc_deleter(true)

#define STDEX_RV_REF(Type) stdex::rvalue_reference<Type> 
#define STDEX_RV_REF_CONST(Type) stdex::rvalue_reference<Type const> 

#ifdef _MSC_VER
	#pragma warning (pop)
#endif

#endif // _STDEX_MOVE_H

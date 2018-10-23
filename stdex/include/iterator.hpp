#ifndef _STDEX_ITERATOR_H
#define _STDEX_ITERATOR_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// stdex includes

// POSIX includes

// std includes
#include <iterator>
#include <cstddef> //std::size_t

#ifdef _STDEX_NATIVE_CPP11_SUPPORT

#define DELETED_FUNCTION =delete
#define NOEXCEPT_FUNCTION noexcept

#else

#define DELETED_FUNCTION 
#define NOEXCEPT_FUNCTION throw()

#endif

namespace stdex
{
	template<class ContainerType>
	inline
	typename ContainerType::iterator begin(ContainerType &value)
	{	// get beginning of sequence
		return (value.begin());
	}

	template<class ContainerType>
	inline
	typename ContainerType::iterator begin(const ContainerType &value)
	{	// get beginning of sequence
		return (value.begin());
	}

	template<class ContainerType>
	inline
	typename ContainerType::iterator end(ContainerType &value)
	{	// get end of sequence
		return (value.end());
	}

	template<class ContainerType>
	inline
	typename ContainerType::iterator end(const ContainerType &value)
	{	// get end of sequence
		return (value.end());
	}

	template<class T, std::size_t Size>
	inline
	T *begin(T(&value)[Size]) NOEXCEPT_FUNCTION
	{	// get beginning of array
		return (value);
	}

	template<class T, std::size_t Size>
	inline
	T *end(T(&value)[Size]) NOEXCEPT_FUNCTION
	{	// get end of array
		return (value + Size);
	}

	namespace detail
	{
		template <bool, class _Tp>
		struct _iterator_enable_if
		{ };

		template <class _Tp>
		struct _iterator_enable_if<true, _Tp>
		{
			typedef _Tp type;
		};

		typedef char _iterator_yes_type;
		struct _iterator_no_type
		{
			char padding[8];
		};

		_iterator_yes_type _input_iterator_cat_tester(std::input_iterator_tag*);
		_iterator_no_type _input_iterator_cat_tester(...);
		_iterator_yes_type _forward_iterator_cat_tester(std::forward_iterator_tag*);
		_iterator_no_type _forward_iterator_cat_tester(...);
		_iterator_yes_type _bidirectional_iterator_cat_tester(std::bidirectional_iterator_tag*);
		_iterator_no_type _bidirectional_iterator_cat_tester(...);
		_iterator_yes_type _random_access_iterator_cat_tester(std::random_access_iterator_tag*);
		_iterator_no_type _random_access_iterator_cat_tester(...);
		
		template<class _ItCategory, class>
		struct _iterator_cat_is;

		template<class _ItCategory>
		struct _iterator_cat_is<_ItCategory, std::input_iterator_tag>
		{ 
			static const bool value = sizeof(_input_iterator_cat_tester((_ItCategory*)(0))) == sizeof(_iterator_yes_type);
		};

		template<class _ItCategory>
		struct _iterator_cat_is<_ItCategory, std::forward_iterator_tag>
		{ 
			static const bool value = sizeof(_forward_iterator_cat_tester((_ItCategory*)(0))) == sizeof(_iterator_yes_type);
		};

		template<class _ItCategory>
		struct _iterator_cat_is<_ItCategory, std::bidirectional_iterator_tag>
		{ 
			static const bool value = sizeof(_bidirectional_iterator_cat_tester((_ItCategory*)(0))) == sizeof(_iterator_yes_type);
		};

		template<class _ItCategory>
		struct _iterator_cat_is<_ItCategory, std::random_access_iterator_tag>
		{ 
			static const bool value = sizeof(_random_access_iterator_cat_tester((_ItCategory*)(0))) == sizeof(_iterator_yes_type);
		};
	}
	template<class ForwardIt>
	inline
	typename detail::_iterator_enable_if<
		detail::_iterator_cat_is<
			typename ForwardIt::iterator_category, 
			std::forward_iterator_tag
		>::value == bool(true), 
		ForwardIt
	>::type next(ForwardIt it,
	typename std::iterator_traits<ForwardIt>::difference_type n = 1)
	{
		std::advance(it, n);
		return it;
	}

	template<class BidirIt>
	inline
	typename detail::_iterator_enable_if<
		detail::_iterator_cat_is<
			typename BidirIt::iterator_category, 
			std::bidirectional_iterator_tag
		>::value == bool(true), 
		BidirIt
	>::type prev(BidirIt it,
	typename std::iterator_traits<BidirIt>::difference_type n = 1)
	{
		std::advance(it, -n);
		return it;
	}
}

#endif // _STDEX_ITERATOR_H
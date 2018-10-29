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
	// Iterator primitives 

	using std::iterator_traits; // provides uniform interface to the properties of an iterator
								// (class template)
	using std::input_iterator_tag;
	using std::output_iterator_tag;
	using std::forward_iterator_tag;
	using std::bidirectional_iterator_tag;
	using std::random_access_iterator_tag;

	using std::iterator; // base class to ease the definition of required types for simple iterators
						 // (class template)

	//Iterator adaptors

	using std::reverse_iterator; // iterator adaptor for reverse-order traversal
								 // (class template)
	//<TODO>: using std::make_reverse_iterator; // (C++14)
									  // creates a std::reverse_iterator of type inferred from the argument
									  // (function template)
	//using std::move_iterator; // (C++11)
							  // iterator adaptor which dereferences to an rvalue reference
							  // (class template)
	//using std::make_move_iterator; 	// (C++11)
									// creates a std::move_iterator of type inferred from the argument
									// (function template)
	using std::back_insert_iterator; // iterator adaptor for insertion at the end of a container
									 // (class template)
	using std::back_inserter; // creates a std::back_insert_iterator of type inferred from the argument
							  // (function template)
	using std::front_insert_iterator; // iterator adaptor for insertion at the front of a container
									  // (class template)
	using std::front_inserter; // creates a std::front_insert_iterator of type inferred from the argument
							   // (function template)
	using std::insert_iterator; 	// iterator adaptor for insertion into a container
									// (class template)
	using std::inserter; // creates a std::insert_iterator of type inferred from the argument
						 // (function template)

	// Stream iterators

	using std::istream_iterator; // input iterator that reads from std::basic_istream
								 // (class template)
	using std::ostream_iterator; // output iterator that writes to std::basic_ostream
								 // (class template)
	using std::istreambuf_iterator; // input iterator that reads from std::basic_streambuf
									// (class template)
	using std::ostreambuf_iterator; // output iterator that writes to std::basic_streambuf
									// (class template)

	// Iterator operations

	using std::advance; // advances an iterator by given distance
						// (function)
	using std::distance; // returns the distance between two iterators
						 // (function)

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

		template<class ForwardIt>
		struct _iterator_cat_is_forward:
			_iterator_enable_if<
				_iterator_cat_is<
					typename std::iterator_traits<ForwardIt>::iterator_category,
					std::forward_iterator_tag
					>::value == bool(true),
				ForwardIt
			>
		{};

		template<class BidirIt>
		struct _iterator_cat_is_bi:
			_iterator_enable_if<
				_iterator_cat_is<
					typename std::iterator_traits<BidirIt>::iterator_category,
					std::bidirectional_iterator_tag
					>::value == bool(true),
				BidirIt
			>
		{};
	}

	template<class ForwardIt>
	inline
	typename 
		detail::_iterator_cat_is_forward<ForwardIt>::
	type next(ForwardIt it,
		typename std::iterator_traits<ForwardIt>::difference_type n = 1) // increment an iterator 
	{
		std::advance(it, n);
		return it;
	}

	template<class BidirIt>
	inline
	typename 
		detail::_iterator_cat_is_bi<BidirIt>::
	type prev(BidirIt it,
	typename std::iterator_traits<BidirIt>::difference_type n = 1) // decrement an iterator 
	{
		std::advance(it, -n);
		return it;
	}

	// Range access 

	// begin (C++11)
	// returns an iterator to the beginning of a container or array 
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

	template<class T, std::size_t Size>
	inline
	T *begin(T(&value)[Size]) NOEXCEPT_FUNCTION
	{	// get beginning of array
		return (value);
	}

	// end (C++11)
	// returns an iterator to the end of a container or array 
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
	T *end(T(&value)[Size]) NOEXCEPT_FUNCTION
	{	// get end of array
		return (value + Size);
	}

	// Container access (C++ 17)
}

#endif // _STDEX_ITERATOR_H
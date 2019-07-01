#ifndef _STDEX_ITERATOR_H
#define _STDEX_ITERATOR_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// stdex includes

// POSIX includes

// std includes
#include <iterator>
#include <cstddef> //cstddef::size_t

#ifdef _STDEX_NATIVE_CPP11_SUPPORT

#define _STDEX_DELETED_FUNCTION =delete
#define _STDEX_NOEXCEPT_FUNCTION noexcept

#else

#define _STDEX_DELETED_FUNCTION 
#define _STDEX_NOEXCEPT_FUNCTION throw()

#endif

namespace stdex
{
	namespace std_cpp11
	{
#ifndef STDEX_DO_NOT_ADD_CPP11_STD // define to exclude std implementations
		using namespace std;
#endif
	}

	namespace cstddef
	{
		typedef std::size_t size_t;
	}

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
		{
		private:
			struct _iterator_enable_if_dummy;
		public:
			typedef _iterator_enable_if_dummy(&type)[1];
		};

		template <class _Tp>
		struct _iterator_enable_if<true, _Tp>
		{
			typedef _Tp type;
		};

		template<bool, class _ItType>
		struct _iterator_traits_enable_if
		{
			typedef _iterator_enable_if<false, void>::type type;
			typedef _iterator_enable_if<false, void>::type iterator_category;
			typedef _iterator_enable_if<false, void>::type value_type;
			typedef _iterator_enable_if<false, void>::type difference_type;
			typedef _iterator_enable_if<false, void>::type pointer;
			typedef _iterator_enable_if<false, void>::type reference;
		};

		template<class _ItType>
		struct _iterator_traits_enable_if<true, _ItType>:
			std::iterator_traits<_ItType>,
			_iterator_enable_if<true, _ItType>
		{ };

		typedef char _iterator_yes_type;
		struct _iterator_no_type
		{
			char padding[8];
		};

		_iterator_yes_type _input_iterator_cat_tester(std::input_iterator_tag*);
		_iterator_no_type _input_iterator_cat_tester(...);
		_iterator_yes_type _output_iterator_cat_tester(std::output_iterator_tag*);
		_iterator_no_type _output_iterator_cat_tester(...);
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
		struct _iterator_cat_is<_ItCategory, std::output_iterator_tag>
		{ 
			static const bool value = sizeof(_output_iterator_cat_tester((_ItCategory*)(0))) == sizeof(_iterator_yes_type);
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

		template<class _ItCategory, class _CheckCategory>
		struct _iterator_cat_is_valid:
			_iterator_cat_is<_ItCategory, _CheckCategory>
		{ };

		template<class _ItCategory>
		struct _iterator_cat_is_valid<_ItCategory, std::output_iterator_tag>
		{ 
			static const bool value = 
				_iterator_cat_is<_ItCategory, std::forward_iterator_tag>::value ||
				_iterator_cat_is<_ItCategory, std::output_iterator_tag>::value;
		};

		template<class _InputIt>
		struct _if_iterator_cat_is_input:
			_iterator_traits_enable_if<
				_iterator_cat_is<
					typename std::iterator_traits<_InputIt>::iterator_category,
					std::input_iterator_tag
					>::value == bool(true),
				_InputIt
			>
		{};
		
		template<class _OutputIt>
		struct _if_iterator_is_valid_output:
			_iterator_traits_enable_if<
				_iterator_cat_is_valid<
					typename std::iterator_traits<_OutputIt>::iterator_category,
					std::output_iterator_tag
					>::value == bool(true),
				_OutputIt
			>
		{};

		template<class _ForwardIt>
		struct _if_iterator_cat_is_forward:
			_iterator_traits_enable_if<
				_iterator_cat_is<
					typename std::iterator_traits<_ForwardIt>::iterator_category,
					std::forward_iterator_tag
					>::value == bool(true),
				_ForwardIt
			>
		{};

		template<class _BidirIt>
		struct _if_iterator_cat_is_bi:
			_iterator_traits_enable_if<
				_iterator_cat_is<
					typename std::iterator_traits<_BidirIt>::iterator_category,
					std::bidirectional_iterator_tag
					>::value == bool(true),
				_BidirIt
			>
		{};

		template<class _RandomIt>
		struct _if_iterator_cat_is_rand_access:
			_if_iterator_cat_is_bi<_RandomIt>
		{};
	} // namespace detail

	namespace std_cpp11
	{
		namespace impl
		{
			// next (C++11)
			// increment an iterator 
			template<class _ForwardIt>
			inline
			_ForwardIt next(_ForwardIt _it,
				typename detail::_if_iterator_cat_is_forward<_ForwardIt>::difference_type _n) 
			{
				std::advance(_it, _n);
				return _it;
			}

			// next (C++11)
			// increment an iterator by one
			template<class _ForwardIt>
			inline
			_ForwardIt next(_ForwardIt _it) 
			{
				return impl::next(_it, 1);
			}

			// prev (C++11)
			// decrement an iterator 
			template<class _BidirIt>
			inline
			_BidirIt prev(_BidirIt _it,
				typename detail::_if_iterator_cat_is_bi<_BidirIt>::difference_type _n) 
			{
				std::advance(_it, -_n);
				return _it;
			}
			
			// prev (C++11)
			// decrement an iterator by 1
			template<class _BidirIt>
			inline
			_BidirIt prev(_BidirIt _it) 
			{
				return impl::prev(_it, 1);
			}
		} // namespace impl

		using impl::next; // to avoid ambiguity with std::next function
		using impl::prev; // to avoid ambiguity with std::prev function
	} // namespace std_cpp11

	// next (C++11)
	// increment an iterator 
	using std_cpp11::next;

	// prev (C++11)
	// decrement an iterator
	using std_cpp11::prev;

	// Range access 

	namespace std_cpp11
	{
		namespace impl
		{
			// begin (C++11)
			// returns an iterator to the beginning of a container or array 
			template<class _ContainerType>
			inline
			typename _ContainerType::iterator begin(_ContainerType &value)
			{	// get beginning of sequence
				return (value.begin());
			}
	
			// begin (C++11)
			// returns an iterator to the beginning of a container or array 
			template<class _ContainerType>
			inline
			typename _ContainerType::iterator begin(const _ContainerType &value)
			{	// get beginning of sequence
				return (value.begin());
			}

			// end (C++11)
			// returns an iterator to the end of a container or array 
			template<class _ContainerType>
			inline
			typename _ContainerType::iterator end(_ContainerType &value)
			{	// get end of sequence
				return (value.end());
			}

			// end (C++11)
			// returns an iterator to the end of a container or array 
			template<class _ContainerType>
			inline
			typename _ContainerType::iterator end(const _ContainerType &value)
			{	// get end of sequence
				return (value.end());
			}
		} // namespace impl

		using impl::begin;// to avoid ambiguity with std::begin function
		using impl::end;  // to avoid ambiguity with std::end function
	} // namespace std_cpp11

	// begin (C++11)
	// returns an iterator to the beginning of a container or array
	using std_cpp11::begin;

	// begin (C++11)
	// returns an iterator to the beginning of a container or array 
	template<class _Tp, cstddef::size_t Size>
	inline
	_Tp *begin(_Tp(&value)[Size]) _STDEX_NOEXCEPT_FUNCTION
	{	// get beginning of array
		return (value);
	}

	// begin (C++11)
	// returns an iterator to the beginning of a container or array 
	template<class _Tp, cstddef::size_t Size>
	inline
	const _Tp *begin(const _Tp(&value)[Size]) _STDEX_NOEXCEPT_FUNCTION
	{	// get beginning of array
		return (value);
	}

	// end (C++11)
	// returns an iterator to the end of a container or array 
	using std_cpp11::end;

	// end (C++11)
	// returns an iterator to the end of a container or array 
	template<class _Tp, cstddef::size_t Size>
	inline
	_Tp *end(_Tp(&value)[Size]) _STDEX_NOEXCEPT_FUNCTION
	{	// get end of array
		return (value + Size);
	}

	// end (C++11)
	// returns an iterator to the end of a container or array 
	template<class _Tp, cstddef::size_t Size>
	inline
	const _Tp *end(const _Tp(&value)[Size]) _STDEX_NOEXCEPT_FUNCTION
	{	// get end of array
		return (value + Size);
	}

	// Container access (C++ 17)
} // namespace stdex

#undef _STDEX_DELETED_FUNCTION
#undef _STDEX_NOEXCEPT_FUNCTION

#endif // _STDEX_ITERATOR_H

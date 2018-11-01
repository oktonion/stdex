#ifndef _STDEX_ALGORITHM_H
#define _STDEX_ALGORITHM_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// stdex includes
#include "./iterator.hpp"

// POSIX includes

// std includes
#include <algorithm>
#include <cstddef> // std::size_t
#include <cassert> // assert

#ifdef _STDEX_NATIVE_CPP11_SUPPORT

#define DELETED_FUNCTION =delete
#define NOEXCEPT_FUNCTION noexcept

#else

#define DELETED_FUNCTION 
#define NOEXCEPT_FUNCTION throw()

#endif

namespace stdex
{
	// Non-modifying sequence operations

	// all_of (C++11)  
	// checks if a predicate is true for all of the elements in a range     
	template<class _InputIt, class _UnaryPredicate>
	inline
	bool all_of(_InputIt first, 
		typename detail::_iterator_cat_is_input<_InputIt>::type last, _UnaryPredicate p)
	{
		for (; first != last; ++first) {
			if (!p(*first)) {
				return false;
			}
		}
		return true;
	}

	// any_of (C++11)
	// checks if a predicate is true for any of the elements in a range
	template<class _InputIt, class _UnaryPredicate>
	inline
	bool any_of(_InputIt first, 
		typename detail::_iterator_cat_is_input<_InputIt>::type last, _UnaryPredicate p)
	{
		return std::find_if(first, last, p) != last;
	}

	// none_of (C++11)
	// checks if a predicate is true for none of the elements in a range
	template<class _InputIt, class _UnaryPredicate>
	inline
	bool none_of(_InputIt first, 
		typename detail::_iterator_cat_is_input<_InputIt>::type last, _UnaryPredicate p)
	{
		for (; first != last; ++first) {
			if (p(*first)) return false;
		}
		return true;
	}

	using std::for_each; // applies a function to a range of elements
						 // (function template)
	//<TODO>: using std::for_each_n; // (C++17) applies a function object to the first n elements of a sequence
					  // (function template)
	using std::count; // returns the number of elements
	using std::count_if; // returns the number of elements satisfying specific criteria
						 // (function template)
	using std::mismatch; // finds the first position where two ranges differ
						 // (function template)
	using std::equal; // determines if two sets of elements are the same
					  // (function template)

	// finds the first element satisfying specific criteria                  
	using std::find;
	using std::find_if; 
	// find_if_not (C++11)
	template<class _InputIt, class _UnaryPredicate>
	inline 
	_InputIt find_if_not(_InputIt first, 
		typename detail::_iterator_cat_is_input<_InputIt>::type last, _UnaryPredicate p)
	{
		for (; first != last; ++first) {
			if (!p(*first)) {
				return first;
			}
		}
		return last;
	}

	using std::find_end; // finds the last sequence of elements in a certain range
						 // (function template)
	using std::find_first_of; // searches for any one of a set of elements
							  // (function template)
	using std::adjacent_find; // finds the first two adjacent items that are equal (or satisfy a given predicate)
							  // (function template)
	using std::search; // searches for a range of elements
					   // (function template)
	using std::search_n; // searches a range for a number of consecutive copies of an element
						 // (function template)

	// Modifying sequence operations 

	// copies a range of elements to a new location
	using std::copy;
	// copy_if (C++11)
	template<class _InputIt, class _OutputIt, class _UnaryPredicate>
	inline
	typename 
		detail::_iterator_cat_is_output<_OutputIt>::
	type  copy_if(_InputIt first, 
		typename detail::_iterator_cat_is_input<_InputIt>::type last, _OutputIt d_first, _UnaryPredicate p)
	{
		while (first != last) {
			if (p(*first))
				*d_first++ = *first;
			first++;
		}
		return d_first;
	}

	namespace detail
	{
		template<class _InputIt, class _OutputIt>
		struct _copy_n_args_check:
			_iterator_enable_if<
				_iterator_cat_is<
					typename std::iterator_traits<_InputIt>::iterator_category,
					std::input_iterator_tag
					>::value == bool(true) &&
				_iterator_cat_is<
					typename std::iterator_traits<_OutputIt>::iterator_category,
					std::output_iterator_tag
					>::value == bool(true),
				_OutputIt
			>
		{

		};

		template<class _InputIt, class _OutputT>
		struct _copy_n_input_it_check :
			_iterator_enable_if<
				_iterator_cat_is<
					typename std::iterator_traits<_InputIt>::iterator_category,
					std::input_iterator_tag
					>::value == bool(true),
				_OutputT*
			>
		{

		};
	}

	// copy_n (C++11)
	// copies a number of elements to a new location
	template<class _InputIt, class _Diff, class _OutputIt>
	inline
	typename 
		detail::_copy_n_args_check<_InputIt, _OutputIt>::
	type copy_n(_InputIt first, _Diff count, _OutputIt result)
	{
		if (count > 0) {
			*result++ = *first;
			for (_Diff i = 1; i < count; ++i) {
				*result++ = *++first;
			}
		}
		return result;
	}

	template<class _InputT, std::size_t _InputSize, class _Diff, class _OutputIt> 
	inline
	typename 
		detail::_iterator_cat_is_output<_OutputIt>::
	type copy_n(_InputT(&first_arr)[_InputSize], _Diff count, _OutputIt result)
	{
		assert(count > _InputSize);

        _InputT *first = first_arr;

		if (count > 0) {
			*result++ = *first;
			for (_Diff i = 1; i < count; ++i) {
				*result++ = *++first;
			}
		}
		return result;
	}

	template<class _InputIt, class _Diff, class _OutputT, std::size_t _OutputSize>
	inline
	typename 
		detail::_copy_n_input_it_check<_InputIt, _OutputT>::
	type copy_n(_InputIt first, _Diff count, _OutputT(&result_arr)[_OutputSize])
	{
		assert(count > _OutputSize);

        _OutputT *result = result_arr;

		if (count > 0) {
			*result++ = *first;
			for (_Diff i = 1; i < count; ++i) {
				*result++ = *++first;
			}
		}
		return result;
	}

	template<class _InputT, std::size_t _InputSize, class _Diff, class _OutputT, std::size_t _OutputSize>
	inline
	_OutputT* copy_n(_InputT(&first_arr)[_InputSize], _Diff count, _OutputT(&result_arr)[_OutputSize])
	{
		assert(count > _OutputSize);
		assert(count > _InputSize);

        _InputT *first = first_arr;
        _OutputT *result = result_arr;

		if (count > 0) {
			*result++ = *first;
			for (_Diff i = 1; i < count; ++i) {
				*result++ = *++first;
			}
		}
		return result;
	}
}

#endif // _STDEX_ALGORITHM_H

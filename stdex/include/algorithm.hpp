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
#include <cstdlib> // std::rand
#include <map> // std::pair
#include <functional> // std::less

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

	// Non-modifying sequence operations

	// all_of (C++11)  
	// checks if a predicate is true for all of the elements in a range     
	template<class _InputIt, class _UnaryPredicate>
	inline
	bool all_of(_InputIt first, 
		typename detail::_if_iterator_cat_is_input<_InputIt>::type last, _UnaryPredicate p)
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
		typename detail::_if_iterator_cat_is_input<_InputIt>::type last, _UnaryPredicate p)
	{
		return std::find_if(first, last, p) != last;
	}

	// none_of (C++11)
	// checks if a predicate is true for none of the elements in a range
	template<class _InputIt, class _UnaryPredicate>
	inline
	bool none_of(_InputIt first, 
		typename detail::_if_iterator_cat_is_input<_InputIt>::type last, _UnaryPredicate p)
	{
		for (; first != last; ++first) {
			if (p(*first)) return false;
		}
		return true;
	}

	// applies a function to a range of elements
	// (function template)
	using std::for_each; 
						 
	//<TODO>: using std::for_each_n; // (C++17) applies a function object to the first n elements of a sequence
					  // (function template)

	// returns the number of elements					  
	using std::count; 
	
	// returns the number of elements satisfying specific criteria
	// (function template)
	using std::count_if; 

	// finds the first position where two ranges differ						 
	// (function template)
	using std::mismatch; 

	// determines if two sets of elements are the same						 
	// (function template)
	using std::equal; 
					  

	// finds the first element satisfying specific criteria                  
	using std::find;

	// finds the first element satisfying specific criteria   
	using std::find_if; 

	// find_if_not (C++11)
	template<class _InputIt, class _UnaryPredicate>
	inline 
	_InputIt find_if_not(_InputIt first, 
		typename detail::_if_iterator_cat_is_input<_InputIt>::type last, _UnaryPredicate p)
	{
		for (; first != last; ++first) {
			if (!p(*first)) {
				return first;
			}
		}
		return last;
	}

	// finds the last sequence of elements in a certain range
	// (function template)
	using std::find_end; 

	// searches for any one of a set of elements
	// (function template)						 
	using std::find_first_of; 

	// finds the first two adjacent items that are equal (or satisfy a given predicate)
	// (function template)							  
	using std::adjacent_find; 

	// searches for a range of elements
	// (function template)							  
	using std::search; 

	// searches a range for a number of consecutive copies of an element
	// (function template)					   
	using std::search_n; 
						 
	// Modifying sequence operations 

	// copies a range of elements to a new location
	using std::copy;

	namespace detail
	{
		template<class _InputIt, class _OutputIt>
		struct _copy_if_args_check:
			_iterator_enable_if<
				_iterator_cat_is<
					typename std::iterator_traits<_InputIt>::iterator_category,
					std::input_iterator_tag
					>::value == bool(true) &&
				_iterator_cat_is_valid<
					typename std::iterator_traits<_OutputIt>::iterator_category,
					std::output_iterator_tag
					>::value == bool(true),
				_InputIt
			>
		{ };

		template<class _InputIt, class _OutputIt>
		struct _copy_if_args_check<_InputIt, const _OutputIt>:
			_iterator_enable_if<false, void>
		{};
	}

	// copy_if (C++11)
	template<class _InputIt, class _OutputIt, class _UnaryPredicate>
	inline
	_OutputIt  copy_if(
		_InputIt first, 
		typename detail::_copy_if_args_check<_InputIt, _OutputIt>::type last,
		_OutputIt d_first, 
		_UnaryPredicate p)
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
		namespace algorithm_detail
		{
			_iterator_no_type copy_n(...); // dummy

			_iterator_no_type copy_n_check(_iterator_no_type);
			_iterator_yes_type copy_n_check(...);

			struct dummy_input_iterator:
				public std::iterator<std::input_iterator_tag, int>
			{
				reference operator*();
				dummy_input_iterator& operator++ ();
				dummy_input_iterator operator++ (int);
			};

			struct dummy_output_iterator :
				public std::iterator<std::output_iterator_tag, int>
			{
				reference operator*();
				dummy_output_iterator& operator++ ();
				dummy_output_iterator operator++ (int);
			};

			struct _has_buggy_copy_n
			{
				static int A[20];
				static const int B[20];
				static const bool value = sizeof(copy_n_check(copy_n(dummy_input_iterator(), sizeof(A) / sizeof(int), dummy_output_iterator()))) == sizeof(_iterator_yes_type);
			};

			struct _has_buggy_copy_n1
			{
				static int A[20];
				static const int B[20];
				static const bool value = sizeof(copy_n_check(copy_n(B, sizeof(A) / sizeof(int), dummy_output_iterator()))) == sizeof(_iterator_yes_type);
			};

			struct _has_buggy_copy_n2
			{
				static int A[20];
				static const int B[20];
				static const bool value = sizeof(copy_n_check(copy_n(dummy_input_iterator(), sizeof(A) / sizeof(int), A))) == sizeof(_iterator_yes_type);
			};

			struct _has_buggy_copy_n3
			{
				static int A[20];
				static const int B[20];
				static const bool value = sizeof(copy_n_check(copy_n(B, sizeof(A) / sizeof(int), A))) == sizeof(_iterator_yes_type);
			};
		}

		template<class _InputIt, class _OutputIt>
		struct _copy_n_args_check:
			_iterator_enable_if<
				_iterator_cat_is<
					typename std::iterator_traits<_InputIt>::iterator_category,
					std::input_iterator_tag
					>::value == bool(true) &&
				_iterator_cat_is_valid<
					typename std::iterator_traits<_OutputIt>::iterator_category,
					std::output_iterator_tag
					>::value == bool(true) &&
				algorithm_detail::_has_buggy_copy_n::value == bool(false),
				cstddef::size_t
			>
		{ };

		template<class _InputIt, class _OutputT>
		struct _copy_n_input_it_check :
			_iterator_enable_if<
				_iterator_cat_is<
					typename std::iterator_traits<_InputIt>::iterator_category,
					std::input_iterator_tag
					>::value == bool(true) &&
				algorithm_detail::_has_buggy_copy_n2::value == bool(false),
				cstddef::size_t
			>
		{ };

		template<class _InputIt, class _OutputT>
		struct _copy_n_input_it_check<_InputIt, const _OutputT> :
			_iterator_enable_if<false, void>
		{ };

		template<class _InputIt, class _OutputT>
		struct _copy_n_input_it_check1 :
			_iterator_enable_if<
				_iterator_cat_is<
					typename std::iterator_traits<_InputIt>::iterator_category,
					std::input_iterator_tag
					>::value == bool(true) &&
				algorithm_detail::_has_buggy_copy_n3::value == bool(false),
				cstddef::size_t
			>
		{ };

		template<class _InputIt, class _OutputT>
		struct _copy_n_input_it_check1<_InputIt, const _OutputT> :
			_iterator_enable_if<false, void>
		{ };

		template<class _OutputIt>
		struct _copy_n_output_it_check :
			_iterator_enable_if<
				_iterator_cat_is<
					typename std::iterator_traits<_OutputIt>::iterator_category,
					std::output_iterator_tag
					>::value == bool(true) &&
				algorithm_detail::_has_buggy_copy_n1::value == bool(false),
				cstddef::size_t
			>
		{ };
	}

	namespace std_cpp11
	{
		namespace impl
		{
			// copy_n (C++11)
			// copies a number of elements to a new location
			template<class _InputIt, class _OutputIt>
			inline
			_OutputIt copy_n(_InputIt first,
				 typename detail::_copy_n_args_check<_InputIt, _OutputIt>::type count, _OutputIt result)
			{
				if (count > 0) {
					*result++ = *first;
					for (cstddef::size_t i = 1; i < count; ++i) {
						*result++ = *++first;
					}
				}
				return result;
			}

			
		}
		using namespace impl;	
		
	}

	// copy_n (C++11)
	// copies a number of elements to a new location
	using std_cpp11::copy_n;

	// copy_n (C++11)
	// copies a number of elements to a new location
	template<class _InputT, cstddef::size_t _InputSize, class _OutputIt> 
	inline
	_OutputIt copy_n(_InputT(&first_arr)[_InputSize],
			typename detail::_copy_n_output_it_check<_OutputIt>::type count, _OutputIt result)
	{
		assert(count <= _InputSize);

		_InputT *first = first_arr;

		if (count > 0) {
			*result++ = *first;
			for (cstddef::size_t i = 1; i < count; ++i) {
				*result++ = *++first;
			}
		}
		return result;
	}

	// copy_n (C++11)
	// copies a number of elements to a new location
	template<class _InputIt, class _OutputT, cstddef::size_t _OutputSize>
	inline
	_OutputT* copy_n(_InputIt first, 
			typename detail::_copy_n_input_it_check<_InputIt, _OutputT>::type count, _OutputT(&result_arr)[_OutputSize])
	{
		assert(count <= _OutputSize);

		_OutputT *result = result_arr;

		if (count > 0) {
			*result++ = *first;
			for (cstddef::size_t i = 1; i < count; ++i) {
				*result++ = *++first;
			}
		}
		return result;
	}

	// copy_n (C++11)
	// copies a number of elements to a new location
	template<
		class _InputT, cstddef::size_t _InputSize, 
		class _OutputT, cstddef::size_t _OutputSize
		>
	inline
	_OutputT* copy_n(_InputT(&first_arr)[_InputSize], 
			cstddef::size_t count, _OutputT(&result_arr)[_OutputSize])
	{
		assert(count <= _OutputSize);
		assert(count <= _InputSize);

		_InputT *first = first_arr;
		_OutputT *result = result_arr;

		if (count > 0) {
			*result++ = *first;
			for (cstddef::size_t i = 1; i < count; ++i) {
				*result++ = *++first;
			}
		}
		return result;
	}

	// copies a range of elements in backwards order
	// (function template)
	using std::copy_backward;

	// (C++11)
	// moves a range of elements to a new location
	// (function template)
	// <TODO>: move()

	// (C++11)
	// moves a range of elements to a new location in backwards order
	// (function template)
	// <TODO>: move_backward()

	// copy-assigns the given value to every element in a range
	// (function template)
	using std::fill;

	// copy-assigns the given value to N elements in a range
	// (function template)
	using std::fill_n;

	// applies a function to a range of elements
	// (function template)
	using std::transform;

	// assigns the results of successive function calls to every element in a range
	// (function template)
	using std::generate;

	// assigns the results of successive function calls to N elements in a range
	// (function template)
	using std::generate_n;

	// removes elements satisfying specific criteria
	// (function template)
	using std::remove;

	// removes elements satisfying specific criteria
	// (function template)
	using std::remove_if;

	// copies a range of elements omitting those that satisfy specific criteria
	// (function template)
	using std::remove_copy;

	// copies a range of elements omitting those that satisfy specific criteria
	// (function template)
	using std::remove_copy_if;

	// replaces all values satisfying specific criteria with another value
	// (function template)
	using std::replace;

	// replaces all values satisfying specific criteria with another value
	// (function template)
	using std::replace_if;

	// copies a range, replacing elements satisfying specific criteria with another value
	// (function template)
	using std::replace_copy;

	// copies a range, replacing elements satisfying specific criteria with another value
	// (function template)
	using std::replace_copy_if;

	// swaps the values of two objects
	// (function template)
	using std::swap;

	// swaps two ranges of elements
	// (function template)
	using std::swap_ranges;

	// swaps the elements pointed to by two iterators
	// (function template)
	using std::iter_swap;

	// reverses the order of elements in a range
	// (function template)
	using std::reverse;

	// creates a copy of a range that is reversed
	// (function template)
	using std::reverse_copy;

	// rotates the order of elements in a range
	// (function template)
	using std::rotate;

	// copies and rotate a range of elements
	// (function template)
	using std::rotate_copy;

	namespace std_cpp11
	{
		namespace impl
		{
			template<class _RandomIt>
			inline
			void random_shuffle(_RandomIt first, 
				typename detail::_if_iterator_cat_is_rand_access<_RandomIt>::type last)
			{
				typename std::iterator_traits<_RandomIt>::difference_type i, n;
				n = last - first;
				for (i = n-1; i > 0; --i) {
					swap(first[i], first[std::rand() % (i+1)]);
					// rand() % (i+1) isn't actually correct, because the generated number
					// is not uniformly distributed for most values of i. A correct implementation
					// will need to essentially reimplement C++11 std::uniform_int_distribution,
					// which is not implemented (yet).
				}
			}

			// randomly re-orders elements in a range with user random number generator func
			// (function template)
			template<class _RandomIt, class _RandomFunc>
			inline
			void random_shuffle(_RandomIt first,
				typename detail::_if_iterator_cat_is_rand_access<_RandomIt>::type last, _RandomFunc &r)
			{
				typename std::iterator_traits<_RandomIt>::difference_type i, n;
				n = last - first;
				for (i = n-1; i > 0; --i) {
					swap(first[i], first[r(i+1)]);
				}
			}
		}
		using namespace impl;
		
	}

	// randomly re-orders elements in a range
	// (function template)
	using std_cpp11::random_shuffle;

	// (C++11)
	// randomly re-orders elements in a range
	// (function template)
	// <TODO>: shuffle()

	// removes consecutive duplicate elements in a range
	// (function template)
	using std::unique;

	// creates a copy of some range of elements that contains no consecutive duplicates
	// (function template)
	using std::unique_copy;

	// Partitioning operations

	namespace std_cpp11
	{
		namespace impl
		{
			template<class _InputIt, class _UnaryPredicate>
			inline
			bool is_partitioned(_InputIt first, 
				typename detail::_if_iterator_cat_is_input<_InputIt>::type last, _UnaryPredicate p)
			{
				for (; first != last; ++first)
					if (!p(*first))
						break;
				for (; first != last; ++first)
					if (p(*first))
						return false;
				return true;
			}
		}
		using namespace impl;
		
	}
	// (C++11)
	// determines if the range is partitioned by the given predicate
	// (function template)
	using std_cpp11::is_partitioned;

	// divides a range of elements into two groups
	// (function template)
	using std::partition;

	namespace detail
	{
		template<class _InputIt, class _OutputIt1, class _OutputIt2>
		struct _partition_copy_args_check:
			_iterator_enable_if<
				_iterator_cat_is<
					typename std::iterator_traits<_InputIt>::iterator_category,
					std::input_iterator_tag
					>::value == bool(true) &&
				_iterator_cat_is_valid<
					typename std::iterator_traits<_OutputIt1>::iterator_category,
					std::output_iterator_tag
					>::value == bool(true) &&
				_iterator_cat_is_valid<
					typename std::iterator_traits<_OutputIt2>::iterator_category,
					std::output_iterator_tag
					>::value == bool(true),
				_InputIt
			>
		{};

		template<class _InputIt, class _OutputIt1, class _OutputIt2>
		struct _partition_copy_args_check<_InputIt, const _OutputIt1, _OutputIt2>:
			_iterator_traits_enable_if<false, void>
		{};

		template<class _InputIt, class _OutputIt1, class _OutputIt2>
		struct _partition_copy_args_check<_InputIt, _OutputIt1, const _OutputIt2> :
			_iterator_traits_enable_if<false, void>
		{};

		template<class _InputIt, class _OutputIt1, class _OutputIt2>
		struct _partition_copy_args_check<_InputIt, const _OutputIt1, const _OutputIt2> :
			_iterator_traits_enable_if<false, void>
		{};
	}

	namespace std_cpp11
	{
		namespace impl
		{
			template<class _InputIt, class _OutputIt1, class _OutputIt2, class _UnaryPredicate>
			inline
			std::pair<_OutputIt1, _OutputIt2>
				partition_copy(
					_InputIt first, 
					typename detail::_partition_copy_args_check<_InputIt, _OutputIt1, _OutputIt2>::type last,
					_OutputIt1 d_first_true, 
					_OutputIt2 d_first_false,
				_UnaryPredicate p)
			{
				while (first != last) {
					if (p(*first)) {
						*d_first_true = *first;
						++d_first_true;
					} else {
						*d_first_false = *first;
						++d_first_false;
					}
					++first;
				}
				return std::pair<_OutputIt1, _OutputIt2>(d_first_true, d_first_false);
			}
		}
		using namespace impl;
		
	}

	// (C++11)
	// copies a range dividing the elements into two groups
	// (function template)
	using std_cpp11::partition_copy;

	// divides elements into two groups while preserving their relative order
	// (function template)
	using std::stable_partition;
	
	// locates the partition point of a partitioned range
	// (function template)
	// <TODO>: partition_point()


	// Sorting operations

	namespace std_cpp11
	{
		namespace impl
		{
			// (C++11)
			// finds the largest sorted subrange
			// (function template)
			template<class _ForwardIt>
			inline
			_ForwardIt is_sorted_until(_ForwardIt first, 
				typename detail::_if_iterator_cat_is_forward<_ForwardIt>::type last)
			{
				if (first != last) {
					_ForwardIt next = first;
					while (++next != last) {
						if (*next < *first)
							return next;
						first = next;
					}
				}
				return last;
			}

			// (C++11)
			// finds the largest sorted subrange given binary comparison function comp
			// (function template)
			template <class _ForwardIt, class _Compare>
			inline
			_ForwardIt is_sorted_until(_ForwardIt first, 
				typename detail::_if_iterator_cat_is_forward<_ForwardIt>::type last, _Compare comp) 
			{
				if (first != last) {
					_ForwardIt next = first;
					while (++next != last) {
						if (true == comp(*next, *first))
							return next;
						first = next;
					}
				}
				return last;
			}

			// (C++11)
			// checks whether a range is sorted into ascending order
			// (function template)
			template<class _ForwardIt>
			inline
			bool is_sorted(_ForwardIt first, 
				typename detail::_if_iterator_cat_is_forward<_ForwardIt>::type last)
			{
				return impl::is_sorted_until(first, last) == last;
			}

			// (C++11)
			// checks with given binary comparison function comp whether a range is sorted into ascending order
			// (function template)
			template<class _ForwardIt, class _Compare>
			inline
			bool is_sorted(_ForwardIt first, 
				typename detail::_if_iterator_cat_is_forward<_ForwardIt>::type last, _Compare comp)
			{
				return impl::is_sorted_until(first, last, comp) == last;
			}
		}
		using namespace impl;
		
	}

	// (C++11)
	// finds the largest sorted subrange
	// (function template)
	using std_cpp11::is_sorted_until;

	// (C++11)
	// checks whether a range is sorted into ascending order
	// (function template)
	using std_cpp11::is_sorted;

	// sorts a range into ascending order
	// (function template)
	using std::sort;

	// sorts the first N elements of a range
	// (function template)
	using std::partial_sort;

	// copies and partially sorts a range of elements
	// (function template)
	using std::partial_sort_copy;

	// sorts a range of elements while preserving order between equal elements
	// (function template)
	using std::stable_sort;

	// partially sorts the given range making sure that it is partitioned by the given element
	// (function template)	
	using std::nth_element;

	// Binary search operations (on sorted ranges)

	// returns an iterator to the first element not less than the given value
	// (function template)
	using std::lower_bound;

	// returns an iterator to the first element greater than a certain value
	// (function template)
	using std::upper_bound;

	// determines if an element exists in a certain range
	// (function template)
	using std::binary_search;

	// returns range of elements matching a specific key
	// (function template)
	using std::equal_range;

	// Set operations (on sorted ranges)

	// merges two sorted ranges
	// (function template)
	using std::merge;

	// merges two ordered ranges in-place
	// (function template)
	using std::inplace_merge;

	// returns true if one set is a subset of another
	// (function template)
	using std::includes;

	// computes the difference between two sets
	// (function template)
	using std::set_difference;

	// computes the intersection of two sets
	// (function template)
	using std::set_intersection;

	// computes the symmetric difference between two sets
	// (function template)
	using std::set_symmetric_difference;

	// computes the union of two sets
	// (function template)
	using std::set_union;

	// Heap operations

	// (C++11)
	// checks if the given range is a max heap
	// (function template)
	// <TODO>: is_heap()

	// (C++11)
	// finds the largest subrange that is a max heap
	// (function template)
	// <TODO>: is_heap_until()

	// creates a max heap out of a range of elements
	// (function template)
	using std::make_heap;

	// adds an element to a max heap
	// (function template)
	using std::push_heap;

	// removes the largest element from a max heap
	// (function template)
	using std::pop_heap;

	// turns a max heap into a range of elements sorted in ascending order
	// (function template)
	using std::sort_heap;

	// Minimum/maximum operations

	// (C++17)
	// clamps a value between a pair of boundary values
	// (function template)
	// <TODO>: clamp()

	// returns the greater of the given values
	// (function template)
	using std::max;

	// returns the largest element in a range
	// (function template)
	using std::max_element;

	// returns the smaller of the given values
	// (function template)
	using std::min;

	// returns the smallest element in a range
	// (function template)
	using std::min_element;

	namespace std_cpp11
	{
		namespace impl
		{
			// (C++11)
			// returns the smaller and larger of two elements
			// (function template)
			template<class _Tp> 
			inline
			std::pair<const _Tp&, const _Tp&> minmax( const _Tp& a, const _Tp& b )
			{
				return (b < a) ? std::pair<const _Tp&, const _Tp&>(b, a)
							: std::pair<const _Tp&, const _Tp&>(a, b);
			}

			// (C++11)
			// returns the smaller and larger of two elements using the given comparison function comp
			// (function template)
			template<class _Tp, class _Compare> 
			inline
			std::pair<const _Tp&, const _Tp&> minmax( const _Tp& a, const _Tp& b, _Compare comp )
			{
				return comp(b, a) ? std::pair<const _Tp&, const _Tp&>(b, a)
								: std::pair<const _Tp&, const _Tp&>(a, b);
			}

			// (C++11)
			// returns the smallest and the largest elements in a range 
			// using the given binary comparison function comp
			template<class _ForwardIt, class _Compare>
			inline
			std::pair<_ForwardIt, _ForwardIt> 
				minmax_element(_ForwardIt first, 
					typename detail::_if_iterator_cat_is_forward<_ForwardIt>::type last, _Compare comp)
			{
				std::pair<_ForwardIt, _ForwardIt> result(first, first);
	
				if (first == last) return result;
				if (++first == last) return result;
	
				if (comp(*first, *result.first)) {
					result.first = first;
				} else {
					result.second = first;
				}
				while (++first != last) {
					_ForwardIt i = first;
					if (++first == last) {
						if (comp(*i, *result.first)) result.first = i;
						else if (!(comp(*i, *result.second))) result.second = i;
						break;
					} else {
						if (comp(*first, *i)) {
							if (comp(*first, *result.first)) result.first = first;
							if (!(comp(*i, *result.second))) result.second = i;
						} else {
							if (comp(*i, *result.first)) result.first = i;
							if (!(comp(*first, *result.second))) result.second = first;
						}
					}
				}
				return result;
			}

			// (C++11)
			// returns the smallest and the largest elements in a range
			// (function template)
			template<class _ForwardIt>
			inline
			std::pair<_ForwardIt, _ForwardIt> 
				minmax_element(_ForwardIt first, 
					typename detail::_if_iterator_cat_is_forward<_ForwardIt>::type last)
			{
				typedef typename std::iterator_traits<_ForwardIt>::value_type value_type;
				return impl::minmax_element(first, last, std::less<value_type>());
			}
		}
		using namespace impl;
		
	}

	// (C++11)
	// returns the smaller and larger of two elements
	// (function template)
	using std_cpp11::minmax;

	// (C++11)
	// returns the smallest and the largest elements in a range
	using std_cpp11::minmax_element;

	// returns true if one range is lexicographically less than another
	// (function template)
	using std::lexicographical_compare;

	namespace detail
	{
		template<class _ForwardIt1, class _ForwardIt2>
		struct _if_iterators_cat_are_forward:
			_iterator_traits_enable_if<
				_iterator_cat_is<
					typename std::iterator_traits<_ForwardIt1>::iterator_category,
					std::forward_iterator_tag
					>::value == bool(true) &&
				_iterator_cat_is<
					typename std::iterator_traits<_ForwardIt2>::iterator_category,
					std::forward_iterator_tag
					>::value == bool(true),
				_ForwardIt1
			>
		{};
	}

	namespace std_cpp11
	{
		namespace impl
		{
			// (C++11)
			// determines if a sequence is a permutation of another sequence
			// (function template)
			template<class _ForwardIt1, class _ForwardIt2>
			inline
			bool is_permutation(_ForwardIt1 first, 
				typename detail::_if_iterators_cat_are_forward<_ForwardIt1, _ForwardIt2>::type last, _ForwardIt2 d_first)
			{
				// skip common prefix
				std::pair<_ForwardIt1, _ForwardIt2> tie = std::mismatch(first, last, d_first);
				first = tie.first;
				d_first = tie.second;
				// iterate over the rest, counting how many times each element
				// from [first, last) appears in [d_first, d_last)
				if (first != last) {
					_ForwardIt2 d_last = d_first;
					std::advance(d_last, std::distance(first, last));
					for (_ForwardIt1 i = first; i != last; ++i) {
							if (i != std::find(first, i, *i)) continue; // already counted this *i
							typename iterator_traits<_ForwardIt2>::difference_type m = std::count(d_first, d_last, *i);
							if (m==0 || std::count(i, last, *i) != m) {
								return false;
							}
						}
					}
				return true;
			}
		}
		using namespace impl;
		
	}

	// (C++11)
	// determines if a sequence is a permutation of another sequence
	// (function template)
	using std_cpp11::is_permutation;

	// generates the next greater lexicographic permutation of a range of elements
	// (function template)
	using std::next_permutation;

	// generates the next smaller lexicographic permutation of a range of elements
	// (function template)
	using std::prev_permutation;
}

#endif // _STDEX_ALGORITHM_H

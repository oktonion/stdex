#ifndef _STDEX_CORE_H
#define _STDEX_CORE_H

#ifndef __has_feature
	#define __has_feature(x) 0 // Compatibility with non-clang compilers.
#endif

//#define STDEX_FORCE_CPP11_TYPES_SUPPORT //uncomment to force support of char16_t and char32_t in C++03

// Any compiler claiming C++11 supports, Visual C++ 2015 and Clang version supporting constexpr
#if ((__cplusplus >= 201103L) || (_MSC_VER >= 1900) || (__has_feature(cxx_constexpr))) // C++ 11 implementation

	#define _STDEX_NATIVE_CPP11_SUPPORT
	#define _STDEX_NATIVE_CPP11_TYPES_SUPPORT

#endif

#ifndef _STDEX_NATIVE_CPP11_SUPPORT

	#if (!defined(_MSC_VER) || _MSC_VER < 1800)

		#define _STDEX_IMPLEMENTS_NULLPTR_SUPPORT

	#else

		#define _STDEX_NATIVE_NULLPTR_SUPPORT

	#endif

#endif

#if _MSC_VER // Visual C++ fallback

	#define _STDEX_NATIVE_MICROSOFT_COMPILER_EXTENSIONS_SUPPORT

	#if (__cplusplus >= 199711L)
		#define _STDEX_NATIVE_CPP_98_SUPPORT
	#endif

#endif

// C++ 98 trick:
#if ((__cplusplus >= 199711L) && ((defined(__INTEL_COMPILER) || defined(__clang__) || (defined(__GNUC__) && ((__GNUC__ > 4) || (__GNUC__ == 4 && __GNUC_MINOR__ >= 4))))))

	#ifndef _STDEX_NATIVE_CPP_98_SUPPORT
		#define _STDEX_NATIVE_CPP_98_SUPPORT
	#endif

#endif


#define forever for(;;)



#ifdef _STDEX_NATIVE_CPP11_SUPPORT

	#include <cstddef>
	namespace stdex
	{
		namespace detail
		{
			template <typename T, size_t N>
			constexpr size_t my_countof(T const (&)[N]) noexcept
			{
				return N;
			}
		} // namespace detail
	}
	#define countof(arr) stdex::detail::my_countof(arr)
	#define STATIC_ASSERT(expression, message) static_assert(expression, #message)

#else //no C++11 support

	// nullptr and nullptr_t implementation
	#ifndef _STDEX_NATIVE_NULLPTR_SUPPORT
		#include "nullptr.h"
	#endif // _STDEX_NATIVE_NULLPTR_SUPPORT

	namespace stdex
	{
		namespace detail {

			template <bool>
			struct StaticAssertion;

			template <>
			struct StaticAssertion<true>
			{
			}; // StaticAssertion<true>

			template<int i>
			struct StaticAssertionTest
			{
			}; // StaticAssertionTest<int>
		}
	}

	#define CONCATENATE(arg1, arg2)   CONCATENATE1(arg1, arg2)
	#define CONCATENATE1(arg1, arg2)  CONCATENATE2(arg1, arg2)
	#define CONCATENATE2(arg1, arg2)  arg1##arg2

	#define STATIC_ASSERT(expression, message)\
	  struct CONCATENATE(__static_assertion_at_line_, __LINE__)\
	  {\
		stdex::detail::StaticAssertion<static_cast<bool>((expression))> CONCATENATE(CONCATENATE(CONCATENATE(STATIC_ASSERTION_FAILED_AT_LINE_, __LINE__), _WITH__), message);\
	  };\
	  typedef stdex::detail::StaticAssertionTest<sizeof(CONCATENATE(__static_assertion_at_line_, __LINE__))> CONCATENATE(__static_assertion_test_at_line_, __LINE__)

	#define static_assert(expression, message) STATIC_ASSERT(expression, ERROR_MESSAGE_STRING)


	#ifdef _STDEX_NATIVE_MICROSOFT_COMPILER_EXTENSIONS_SUPPORT // Visual C++ fallback
		#include <stdlib.h>
		#define countof(arr) _countof(arr)

	#elif defined(_STDEX_NATIVE_CPP_98_SUPPORT)// C++ 98 trick
		#include <cstddef>
		template <typename T, std::size_t N>
		char(&COUNTOF_REQUIRES_ARRAY_ARGUMENT(T(&)[N]))[N];
			
		#define countof(x) sizeof(COUNTOF_REQUIRES_ARRAY_ARGUMENT(x))
	#else
		#define countof(arr) sizeof(arr) / sizeof(arr[0])
	#endif
#endif

#endif // _STDEX_CORE_H

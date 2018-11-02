#ifndef _STDEX_CORE_H
#define _STDEX_CORE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#ifndef __has_feature         // Optional of course.
  #define __has_feature(x) 0  // Compatibility with non-clang compilers.
#endif
#ifndef __has_extension
  #define __has_extension __has_feature // Compatibility with pre-3.0 compilers.
#endif

//#define STDEX_FORCE_CPP11_TYPES_SUPPORT //uncomment to force support of char16_t and char32_t in C++03

// Any compiler claiming C++11 supports, Visual C++ 2015 and Clang version supporting constexpr
#if ((__cplusplus >= 201103L) || (_MSC_VER >= 1900) || (__has_feature(cxx_constexpr) || (__has_extension(cxx_constexpr)))) // C++ 11 implementation

	#define _STDEX_NATIVE_CPP11_SUPPORT
	#define _STDEX_NATIVE_CPP11_TYPES_SUPPORT

#endif

#if !defined(_STDEX_NATIVE_CPP11_TYPES_SUPPORT)

	#if ((__cplusplus > 199711L) || defined(__CODEGEARC__) || defined(__GXX_EXPERIMENTAL_CXX0X__) || defined(__cpp_unicode_characters) || __has_feature(cxx_unicode_literals))
		#define _STDEX_NATIVE_CPP11_TYPES_SUPPORT
	#endif

	#if (defined(__apple_build_version__) && (__clang_major__ >= 3))
		#ifndef _STDEX_NATIVE_CPP11_TYPES_SUPPORT
			#define _STDEX_NATIVE_CPP11_TYPES_SUPPORT
		#endif
	#endif

#endif

#if ((!defined(_MSC_VER) || _MSC_VER < 1600) && !defined(_STDEX_NATIVE_CPP11_SUPPORT))

	#if (__has_feature(cxx_nullptr) || __has_extension(cxx_nullptr))
		#define _STDEX_NATIVE_NULLPTR_SUPPORT
	#else
		#if !defined(nullptr)
			#define _STDEX_IMPLEMENTS_NULLPTR_SUPPORT
		#else
			#define STRINGIZE_HELPER(x) #x
			#define STRINGIZE(x) STRINGIZE_HELPER(x)
			#define WARNING(desc) message(__FILE__ "(" STRINGIZE(__LINE__) ") : warning: " desc)

			#pragma WARNING("stdex library - macro 'nullptr' was previously defined by user; ignoring stdex macro definition")

			#undef STRINGIZE_HELPER
			#undef STRINGIZE
			#undef WARNING
		#endif
	#endif

	#if (__has_feature(cxx_static_assert) || __has_extension(cxx_static_assert))
		#define _STDEX_NATIVE_STATIC_ASSERT_SUPPORT
	#else
		#if !defined(static_assert)
			#define _STDEX_IMPLEMENTS_STATIC_ASSERT_SUPPORT
		#else
			#define STRINGIZE_HELPER(x) #x
			#define STRINGIZE(x) STRINGIZE_HELPER(x)
			#define WARNING(desc) message(__FILE__ "(" STRINGIZE(__LINE__) ") : warning: " desc)

			#pragma WARNING("stdex library - macro 'static_assert' was previously defined by user; ignoring stdex macro definition")

			#undef STRINGIZE_HELPER
			#undef STRINGIZE
			#undef WARNING
		#endif
	#endif

#else

	#define _STDEX_NATIVE_NULLPTR_SUPPORT
	#define _STDEX_NATIVE_STATIC_ASSERT_SUPPORT

#endif

#if (_MSC_VER >= 1600)

	#ifndef _STDEX_NATIVE_CPP11_TYPES_SUPPORT
		#define _STDEX_NATIVE_CPP11_TYPES_SUPPORT
	#endif

#endif



#if _MSC_VER // Visual C++ fallback

	#define _STDEX_NATIVE_MICROSOFT_COMPILER_EXTENSIONS_SUPPORT
	#define _STDEX_CDECL __cdecl

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

#if !defined(forever)
	#define forever for(;;)
#else
	#define STRINGIZE_HELPER(x) #x
	#define STRINGIZE(x) STRINGIZE_HELPER(x)
	#define WARNING(desc) message(__FILE__ "(" STRINGIZE(__LINE__) ") : warning: " desc)

	#pragma WARNING("stdex library - macro 'forever' was previously defined by user; ignoring stdex macro definition")

	#undef STRINGIZE_HELPER
	#undef STRINGIZE
	#undef WARNING
#endif

#undef stdex_assert

#if defined(assert)
#ifndef NDEBUG
	#include <iostream>
	#define stdex_assert(condition, message) \
	do { \
		if (! (condition)) { \
			std::cerr << "Assertion `" #condition "` failed in " << __FILE__ \
					  << " line " << __LINE__ << ": " << message << std::endl; \
			std::terminate(); \
		} \
	} while (false)
#else
	#define stdex_assert(condition, message) ((void)0)
#endif
#endif

#ifdef _STDEX_NATIVE_CPP11_SUPPORT

	#include <cstddef>
	namespace stdex
	{
		namespace detail
		{
			template <class T, std::size_t N>
			constexpr std::size_t _my_countof(T const (&)[N]) noexcept
			{
				return N;
			}
		} // namespace detail
	}
	#define countof(arr) stdex::detail::_my_countof(arr)
	#define STATIC_ASSERT(expression, message) static_assert((expression), #message)

	#ifdef _STDEX_NATIVE_NULLPTR_SUPPORT
		namespace stdex
		{
			typedef std::nullptr_t nullptr_t;
		}
	#endif

#else //no C++11 support

	// nullptr and nullptr_t implementation
	#ifdef _STDEX_IMPLEMENTS_NULLPTR_SUPPORT
		#include "nullptr.h"
	#else
	namespace stdex
	{
		typedef std::nullptr_t nullptr_t;
	}
	#endif // _STDEX_IMPLEMENTS_NULLPTR_SUPPORT

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

#ifdef _STDEX_IMPLEMENTS_STATIC_ASSERT_SUPPORT
	#ifdef static_assert
		#undef static_assert
	#endif
	#define static_assert(expression, message) STATIC_ASSERT(expression, ERROR_MESSAGE_STRING)
#endif


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

#ifdef _STDEX_NATIVE_CPP11_SUPPORT

#define stdex_noexcept noexcept

#else

#define stdex_noexcept throw()

#endif

#endif // _STDEX_CORE_H

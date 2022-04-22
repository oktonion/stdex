#ifdef __cplusplus

#ifndef _STDEX_CORE_H
#define _STDEX_CORE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <cerrno>


#ifndef __has_feature         // Optional of course.
    #define _STDEX_HAS_FEATURE_BUILTIN(xxx) 0  // Compatibility with non-clang compilers.
#else
    #define _STDEX_HAS_FEATURE_BUILTIN(xxx) __has_feature(xxx)
#endif
#ifndef __has_extension
    #define _STDEX_HAS_EXTENSION_BUILTIN(xxx) _STDEX_HAS_FEATURE_BUILTIN(xxx) // Compatibility with pre-3.0 compilers.
#else
    #define _STDEX_HAS_EXTENSION_BUILTIN(xxx) __has_extension(xxx)
#endif

//#define STDEX_FORCE_CPP11_TYPES_SUPPORT //uncomment to force support of char16_t and char32_t in C++03

// Any compiler claiming C++11 supports, Visual C++ 2015 and Clang version supporting constexpr
#if ((__cplusplus >= 201103L) || (_MSC_VER >= 1900) || (_STDEX_HAS_FEATURE_BUILTIN(cxx_constexpr) || (_STDEX_HAS_EXTENSION_BUILTIN(cxx_constexpr)))) // C++ 11 implementation

    #define _STDEX_NATIVE_CPP11_SUPPORT
    #define _STDEX_NATIVE_CPP11_TYPES_SUPPORT
    #define _STDEX_NATIVE_CPP_98_SUPPORT

#endif

// LWG 310     C++98     it is unspecified whether errno is a macro or an identifier with external linkage     errno must be macro
#ifdef errno
    #define _STDEX_NATIVE_CPP_98_SUPPORT
#endif

#if !defined(_STDEX_NATIVE_CPP11_TYPES_SUPPORT)

    #if ((__cplusplus > 199711L) || defined(__CODEGEARC__) || defined(__GXX_EXPERIMENTAL_CXX0X__) || defined(__cpp_unicode_characters) || _STDEX_HAS_FEATURE_BUILTIN(cxx_unicode_literals))
        #define _STDEX_NATIVE_CPP11_TYPES_SUPPORT
    #endif

    #if (defined(__apple_build_version__) && (__clang_major__ >= 3))
        #ifndef _STDEX_NATIVE_CPP11_TYPES_SUPPORT
            #define _STDEX_NATIVE_CPP11_TYPES_SUPPORT
        #endif
    #endif

#endif

#if (_STDEX_HAS_FEATURE_BUILTIN(cxx_nullptr) || _STDEX_HAS_EXTENSION_BUILTIN(cxx_nullptr))
    #define _STDEX_NATIVE_NULLPTR_SUPPORT
#endif

#if ((!defined(_MSC_VER) || _MSC_VER < 1600) && !defined(_STDEX_NATIVE_CPP11_SUPPORT))

    
    #if !defined(_STDEX_NATIVE_NULLPTR_SUPPORT)
        #if !defined(nullptr)
            #define _STDEX_IMPLEMENTS_NULLPTR_SUPPORT
        #else
            #define STRINGIZE_HELPER(xxx) #xxx
            #define STRINGIZE(xxx) STRINGIZE_HELPER(xxx)
            #define WARNING(desc) message(__FILE__ "(" STRINGIZE(__LINE__) ") : warning: " desc)

            #pragma WARNING("stdex library - macro 'nullptr' was previously defined by user; ignoring stdex macro definition")

            #undef STRINGIZE_HELPER
            #undef STRINGIZE
            #undef WARNING
        #endif
    #endif

    #if (_STDEX_HAS_FEATURE_BUILTIN(cxx_static_assert) || _STDEX_HAS_EXTENSION_BUILTIN(cxx_static_assert))
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

#if !defined(_STDEX_NATIVE_NULLPTR_SUPPORT)
    #define _STDEX_NATIVE_NULLPTR_SUPPORT
#endif
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
    #define _STDEX_STDCALL __stdcall
    #define _STDEX_FASTCALL __fastcall

    #if (__cplusplus >= 199711L)
        #define _STDEX_NATIVE_CPP_98_SUPPORT
    #endif

#endif

// C++ 98 trick:
#if (__cplusplus >= 199711L)
    #if (defined(__INTEL_COMPILER) ||  defined(__clang__))
        #ifndef _STDEX_NATIVE_CPP_98_SUPPORT
            #define _STDEX_NATIVE_CPP_98_SUPPORT
        #endif
    #endif
#endif

#if defined(__GNUC__)
    #if (__GNUC__ > 4) || (__GNUC__ == 4 && __GNUC_MINOR__ >= 4)
        #ifndef _STDEX_NATIVE_CPP_98_SUPPORT
            #define _STDEX_NATIVE_CPP_98_SUPPORT  
        #endif
    #endif
    #if !__x86_64__ && !__ppc64__
        #define _STDEX_CDECL __attribute__((cdecl))
        #define _STDEX_STDCALL __attribute__((stdcall))
        #define _STDEX_FASTCALL __attribute__((fastcall))
    #endif
#endif

#if !defined(forever)
    #define forever for(;;)
#else
    #define STRINGIZE_HELPER(xxx) #xxx
    #define STRINGIZE(xxx) STRINGIZE_HELPER(xxx)
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
            template <class _Tp, std::size_t _Count>
            constexpr std::size_t _my_countof(_Tp const (&)[_Count]) noexcept
            {
                return _Count;
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
    #else
        #include "nullptr.h"
    #endif

#else //no C++11 support

    // nullptr and nullptr_t implementation
    #if defined(_STDEX_IMPLEMENTS_NULLPTR_SUPPORT) && !defined(_STDEX_NATIVE_NULLPTR_SUPPORT)
        #include "nullptr.h"
    #else
    namespace stdex
    {
        typedef std::nullptr_t nullptr_t;
    }
    #endif // _STDEX_IMPLEMENTS_NULLPTR_SUPPORT && !_STDEX_NATIVE_NULLPTR_SUPPORT

    namespace stdex
    {
        namespace detail {

            template <bool>
            struct StaticAssertion;

            template <>
            struct StaticAssertion<true>
            {
            }; // StaticAssertion<true>

            template<int>
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
        stdex::detail::StaticAssertion<bool((expression))> CONCATENATE(CONCATENATE(CONCATENATE(STATIC_ASSERTION_FAILED_AT_LINE_, __LINE__), _WITH__), message);\
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
        template <class _Tp, std::size_t _Count>
        char(&COUNTOF_REQUIRES_ARRAY_ARGUMENT(_Tp(&)[_Count]))[_Count];
            
        #define countof(xxx) sizeof(COUNTOF_REQUIRES_ARRAY_ARGUMENT(xxx))
    #else
        #define countof(arr) sizeof(arr) / sizeof(arr[0])
    #endif
#endif

#ifdef _STDEX_NATIVE_CPP11_SUPPORT

#define stdex_noexcept noexcept

#else

#define stdex_noexcept throw()

#endif

//#ifndef _STDEX_NATIVE_CPP11_TYPES_SUPPORT
    //#ifndef __CHAR16_TYPE__
        #define stdex_char16_t stdex::uint_least16_t
    //#endif
    //#ifndef __CHAR32_TYPE__
        #define stdex_char32_t stdex::uint_least32_t
    //#endif
//#endif

#undef _STDEX_HAS_FEATURE_BUILTIN
#undef _STDEX_HAS_EXTENSION_BUILTIN

#endif // _STDEX_CORE_H

#endif // __cplusplus

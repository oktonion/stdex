#ifndef _STDEX_FUNCTIONAL_H
#define _STDEX_FUNCTIONAL_H
 
 #if _MSC_VER > 1000
 #pragma once
 #endif // _MSC_VER > 1000
 
 // stdex includes
 
 // POSIX includes
 
 // std includes
 #include <functional>
 
 #ifdef _STDEX_NATIVE_CPP11_SUPPORT
 
 #define _STDEX_DELETED_FUNCTION =delete
 #define _STDEX_NOEXCEPT_FUNCTION noexcept
 
 #else
 
 #define _STDEX_DELETED_FUNCTION 
 #define _STDEX_NOEXCEPT_FUNCTION throw()
 
 #endif
 
 namespace stdex
 {
     namespace functional_cpp11
     {
 #ifndef STDEX_DO_NOT_ADD_CPP11_STD // define to exclude std implementations
         using namespace std;
 #endif
     }
 
    
 } // namespace stdex
 
 #undef _STDEX_DELETED_FUNCTION
 #undef _STDEX_NOEXCEPT_FUNCTION
 
 #endif // _STDEX_FUNCTIONAL_H

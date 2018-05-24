#ifndef _STDEX_STDINT_H
#define _STDEX_STDINT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./core.h"

#if defined(_STDEX_NATIVE_CPP11_TYPES_SUPPORT) || defined(INT64_MAX)
	#include <stdint.h>
#else
	#if (defined(_MSC_VER) || defined(__BORLANDC__))
		typedef __int16 int16_t;
		typedef unsigned __int16 uint16_t;
		typedef __int32 int32_t;
		typedef unsigned __int32 uint32_t;
		typedef __int64 int64_t;
		typedef unsigned __int64 uint64_t;
	#else
		#include <stdint.h>
	#endif

	typedef int16_t char16_t;
	typedef uint16_t uchar16_t;
	typedef int32_t char32_t;
	typedef uint32_t uchar32_t;

#endif

#endif // _STDEX_STDINT_H

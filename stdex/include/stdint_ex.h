#ifndef _STDEX_STDINT_H
#define _STDEX_STDINT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// stdex includes
#include "./core.h"

// POSIX includes
/*none*/

// std includes
#include <climits>

#if defined(_STDEX_NATIVE_CPP11_TYPES_SUPPORT)
	#include <stdint.h>
#else
	#if (defined(_MSC_VER) || defined(__BORLANDC__))
		#if !defined(INT16_MIN) && !defined(INT16_MAX)
		typedef __int16 int16_t;
		#endif

		#if !defined(UINT16_MIN) && !defined(UINT16_MAX)
		typedef unsigned __int16 uint16_t;
		#endif

		#if !defined(INT32_MIN) && !defined(INT32_MAX)
		typedef __int32 int32_t;
		#endif

		#if !defined(UINT32_MIN) && !defined(UINT32_MAX)
		typedef unsigned __int32 uint32_t;
		#endif

		#if !defined(INT64_MIN) && !defined(INT64_MAX)
		typedef __int64 int64_t;
		#endif

		#if !defined(UINT64_MIN) && !defined(UINT64_MAX)
		typedef unsigned __int64 uint64_t;
		#endif
	#else
		#include <stdint.h>
	#endif

	typedef int16_t char16_t;
	typedef uint16_t uchar16_t;
	typedef int32_t char32_t;
	typedef uint32_t uchar32_t;

#endif

#endif // _STDEX_STDINT_H

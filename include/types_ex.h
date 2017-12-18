#ifndef STDEX_TYPES_H
#define STDEX_TYPES_H

#if __cplusplus >= 201103L || _MSC_VER >= 1900

#else
	#if defined(_MSC_VER) || defined(__BORLANDC__)
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

typedef unsigned char byte;

#endif

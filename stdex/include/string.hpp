#ifndef _STDEX_STRING_H
#define _STDEX_STRING_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./core.h"
#include "./type_traits.hpp"
#include "./sstream.hpp"
#include "./stdint_ex.h"

#include <cstring>
#include <cctype>
#include <cstdlib>
#include <cstdio>

#include <iostream>
#include <string>
#include <stdint.h>



namespace stdex
{
	using std::basic_string;
	using std::char_traits;

	using std::string;
	using std::wstring;

	namespace detail
	{
		template<bool>
		struct minus;

		template<>
		struct minus<true>
		{
			template<typename T>
			inline static void apply(T &val)
			{
				val = -val;
			}
		};

		template<>
		struct minus<false>
		{
			template<typename T>
			inline static void apply(T &val)
			{
			}
		};
	}

	template <class _T>
	inline _T stot(const char *s, int base = 10)
	{
		_T num = 0;
		bool negative = false;
		static const char digits[] = "0123456789abcdefghijklmnopqrstuvwxyz";
		
		while(isspace(*s)) s++;
			
		if(*s == '-') {negative = true; s++;}
		else if(*s == '+') s++;
		
		if(*s == '0')
			{
				s++;
				if(*s == 'x' || *s == 'X')
					{
						if(base == 0) base = 16;
						else if(base != 16) return 0;
						s++;
					}
				else if(isdigit(*s))
					{
						if(base == 0) base = 8;
					}
				else if(*s == 0) return 0;
			}
		else if(*s == 0) return 0;
		else if(base == 0) base = 10;
		
		for(int digit; *s; s++)
			{
				const char *where = strchr(digits, tolower(*s));
				
				if(where == 0) break;
				digit = where - digits;
				if(digit >= base) break;
				
				num = num * base + digit;
			}

		if(negative) stdex::detail::minus<stdex::is_signed<_T>::value>::apply(num);
		
		return num;
	}
	
	template <class _T>
	inline _T stot(const string &s, size_t *idx = 0, int base = 10)
	{
		return stot<_T>(s.c_str() + (idx ? *idx : 0), base);
	}
	
	inline int stoi(const string &s, size_t *idx = 0, int base = 10)
	{
		return stot<int>(s.c_str() + (idx ? *idx : 0), base);
	}

	inline long stol(const string &s, size_t *idx = 0, int base = 10)
	{
		return stot<long>(s.c_str() + (idx ? *idx : 0), base);
	}

	inline unsigned long stoul(const string &s, size_t *idx = 0, int base = 10)
	{
		return stot<unsigned long>(s.c_str() + (idx ? *idx : 0), base);
	}
	
	inline float stof(const string &s, size_t *idx = 0)
	{
		return stot<float>(s.c_str() + (idx ? *idx : 0));
	}

	inline double stod(const string &s, size_t *idx = 0)
	{
		return strtod(s.c_str() + (idx ? *idx : 0), nullptr);
	}

	inline double stold(const string &s, size_t *idx = 0)
	{
		return stot<long double>(s.c_str() + (idx ? *idx : 0));
	}

	inline int64_t stoll(const string &s, size_t *idx = 0, int base = 10)
	{
		return stot<int64_t>(s.c_str() + (idx ? *idx : 0), base);
	}

	inline uint64_t stoull(const string &s, size_t *idx = 0, int base = 10)
	{
		return stot<uint64_t>(s.c_str() + (idx ? *idx : 0), base);
	}
	
	inline string to_string(int value, int radix = 10)
	{
		using namespace std;
		char buf[21];
		itoa(value, buf, radix);
		
		return string(buf);
	}

	inline string to_string(unsigned int value)
	{
		using namespace std;
		char buf[30];
		sprintf(buf, "%ul", value);

		return string(buf);
	}
	
	inline string to_string(bool value)
	{
		if(value)
			return string("true");
		return string("false");
	}

	inline string to_string(float value)
	{
		using namespace std;
		char buf[256];
		sprintf(buf, "%f", value);

		return string(buf);
	}

	inline string to_string(double value)
	{
		using namespace std;
		char buf[512];
		sprintf(buf, "%f", value);
		
		return string(buf);
	}

	inline string to_string(const char *x)
	{
		return x;
	}

	inline string to_string(const std::string &x)
	{
		return x;
	}

	template<class _T>
	inline string to_string(const _T &t)
	{
		stringstream ss;
		ss << t;
		return ss.str();
	}
	
}

#endif // _STDEX_STRING_H

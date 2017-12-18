#ifndef STDEX_BASIC_STRING_H
#define STDEX_BASIC_STRING_H

#include "code_extensions.h"

#include <cstring>
#include <cctype>
#include <cstdlib>
#include <cstdio>

#include <iostream>
#include <string>
#include <sstream>

namespace std
{

	namespace details
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

	template <typename T>
	inline T stot(const char *s, int base = 10)
	{
		T num = 0;
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

		if(negative) details::minus<std::is_signed<T>::value>::apply(num);
		
		return num;
	}
	
	template <typename T>
	inline T stot(const std::string &s, int base = 10)
	{
		return stot<T>(s.c_str(), base);
	}
	
#ifndef STDEX_HAS_CPP11_SUPPORT
	inline int stoi(const std::string &s, int base = 10)
	{
		return stot<int>(s.c_str(), base);
	}

	inline long stol(const std::string &s, int base = 10)
	{
		return stot<long>(s.c_str(), base);
	}

	inline unsigned long stoul(const std::string &s, int base = 10)
	{
		return stot<unsigned long>(s.c_str(), base);
	}
	
	inline float stof(const std::string &s)
	{
		return stot<float>(s.c_str());
	}

	inline double stod(const std::string &s)
	{
		return strtod(s.c_str(), nullptr);
	}

	inline double stold(const std::string &s)
	{
		return stot<long double>(s.c_str());
	}

#ifdef STDEX_FORCE_CPP11_TYPES_SUPPORT
	#include "types_ex.h"
	
	inline int64_t stoll(const std::string &s, int base = 10)
	{
		return stot<int64_t>(s.c_str(), base);
	}

	inline uint64_t stoull(const std::string &s, int base = 10)
	{
		return stot<uint64_t>(s.c_str(), base);
	}
#endif
	
	
	inline std::string to_string(int value, int radix = 10)
	{
		static char buf[21];
		itoa(value, buf, radix);
		
		return buf;
	}

	inline std::string to_string(unsigned int value)
	{
		static char buf[30];
		sprintf(buf, "%ul", value);

		return buf;
	}
	
	inline std::string to_string(bool value)
	{
		if(value)
			return "true";
		return "false";
	}

	inline std::string to_string(float value)
	{
		static char buf[256];
		sprintf(buf, "%f", value);

		return buf;
	}

	inline std::string to_string(double value)
	{
		static char buf[512];
		sprintf(buf, "%f", value);
		
		return buf;
	}

	inline std::string to_string(const char *x)
	{
		return x;
	}

	inline std::string to_string(const std::string &x)
	{
		return x;
	}
	
#endif

	template<typename T> 
	inline std::string to_string(const T &t)
	{
		std::stringstream ss;
		ss << t;
		return ss.str();
	}
	
}

#endif

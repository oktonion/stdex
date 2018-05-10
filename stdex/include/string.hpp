#ifndef _STDEX_STRING_H
#define _STDEX_STRING_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// stdex includes
#include "./core.h"
#include "./type_traits.hpp"
#include "./sstream.hpp"
#include "./stdint_ex.h"

// POSIX includes
/*none*/

// std includes
#include <cstring>
#include <cctype>
#include <cstdlib>
#include <cstdio>

#include <iostream>
#include <string>
#include <stdint.h>
#include <climits>
#include <stdexcept>
#include <cmath>



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

		template <class _T>
		inline _T _cs_to_integral(const char *s, const char *&num_s_end, int base = 10)
		{
			using namespace std;
			_T num = 0;
			bool negative = false;
			static const char digits [] = "0123456789abcdefghijklmnopqrstuvwxyz";

			while (isspace(*s)) s++;

			if (*s == '-') { negative = true; s++; }
			else if (*s == '+') { s++; }

			if (*s == '0')
			{
				s++;

				if (*s == 'x' || *s == 'X')
				{
					if (base == 0) base = 16;
					else if (base != 16)
						return 0;
					s++;
				}
				else if (isdigit(*s))
				{
					if (base == 0) base = 8;
				}
				else if (*s == 0)
					return 0;
			}
			else if (*s == 0) return 0;
			else if (base == 0) base = 10;

			for (int digit; *s; s++)
			{
				const char *where = strchr(digits, tolower(*s));

				if (where == 0) break;
				digit = where - digits;
				if (digit >= base) break;

				num = num * base + digit;
			}

			if (negative) stdex::detail::minus<stdex::is_signed<_T>::value>::apply(num);

			num_s_end = s;

			return num;
		}

		template <class _T>
		inline double _cs_to_floating_point(const char *str, const char *&num_s_end)
		{
			using namespace std;

			char *endptr = 0;
			double _value = strtod(str, &endptr);

#ifdef HUGE_VAL
			if (_value == HUGE_VAL || _value == -HUGE_VAL)
#else
			if (errno == ERANGE)
#endif
				num_s_end = 0;
			else if (_value > std::numeric_limits<_T>::max())
				num_s_end = 0;
			else
				num_s_end = endptr;

			return _value;
		}
	}


	
	template <class _T>
	inline _T stot(const string &s, size_t *idx = 0, int base = 10)
	{
		const char *_eptr = s.c_str(), *_ptr = _eptr;
		_T _value = stot<_T>(_ptr, _eptr, base);

		if (_ptr == _eptr)
			throw(std::invalid_argument("invalid stdex::stot argument"));
		if(!_eptr)
			throw(std::out_of_range("stdex::stot argument out of range"));

		if (idx != 0)
			*idx = (size_t) (_eptr - _ptr);

		return (_value);
	}
	
	inline int stoi(const string &s, size_t *idx = 0, int base = 10)
	{
		const char *_eptr = s.c_str(), *_ptr = _eptr;
		int _value = detail::_cs_to_integral<int>(_ptr, _eptr, base);

		if (_ptr == _eptr)
			throw(std::invalid_argument("invalid stdex::stoi argument"));
		if (!_eptr)
			throw(std::out_of_range("stdex::stoi argument out of range"));

		if (idx != 0)
			*idx = (size_t) (_eptr - _ptr);

		return (_value);
	}

	inline long stol(const string &s, size_t *idx = 0, int base = 10)
	{
		const char *_eptr = s.c_str(), *_ptr = _eptr;
		long _value = detail::_cs_to_integral<long>(_ptr, _eptr, base);

		if (_ptr == _eptr)
			throw(std::invalid_argument("invalid stdex::stol argument"));
		if (!_eptr)
			throw(std::out_of_range("stdex::stol argument out of range"));

		if (idx != 0)
			*idx = (size_t) (_eptr - _ptr);

		return (_value);
	}

	inline unsigned long stoul(const string &s, size_t *idx = 0, int base = 10)
	{
		const char *_eptr = s.c_str(), *_ptr = _eptr;
		unsigned long _value = detail::_cs_to_integral<unsigned long>(_ptr, _eptr, base);

		if (_ptr == _eptr)
			throw(std::invalid_argument("invalid stdex::stoul argument"));
		if(!_eptr)
			throw(std::out_of_range("stdex::stoul argument out of range"));

		if (idx != 0)
			*idx = (size_t) (_eptr - _ptr);

		return (_value);
	}
	
	inline float stof(const string &s, size_t *idx = 0)
	{
		const char *_eptr = s.c_str(), *_ptr = _eptr;
		
		float _value = detail::_cs_to_floating_point<float>(_ptr, _eptr);

		if (_ptr == _eptr)
			throw(std::invalid_argument("invalid stdex::stof argument"));
		if(!_eptr)
			throw(std::out_of_range("stdex::stof argument out of range"));

		if (idx != 0)
			*idx = (size_t) (_eptr - _ptr);

		return (_value);
	}

	inline double stod(const string &s, size_t *idx = 0)
	{
		const char *_eptr = s.c_str(), *_ptr = _eptr;
		
		double _value = detail::_cs_to_floating_point<double>(_ptr, _eptr);

		if (_ptr == _eptr)
			throw(std::invalid_argument("invalid stdex::stod argument"));
		if(!_eptr)
			throw(std::out_of_range("stdex::stod argument out of range"));

		if (idx != 0)
			*idx = (size_t) (_eptr - _ptr);

		return (_value);
	}

	inline long double stold(const string &s, size_t *idx = 0)
	{
		const char *_eptr = s.c_str(), *_ptr = _eptr;
		
		long double _value = detail::_cs_to_floating_point<long double>(_ptr, _eptr);

		if (_ptr == _eptr)
			throw(std::invalid_argument("invalid stdex::stold argument"));
		if(!_eptr)
			throw(std::out_of_range("stdex::stold argument out of range"));

		if (idx != 0)
			*idx = (size_t) (_eptr - _ptr);

		return (_value);
	}

#ifdef LLONG_MAX
	inline int64_t stoll(const string &s, size_t *idx = 0, int base = 10)
	{
		const char *_eptr = s.c_str(), *_ptr = _eptr;
		
		int64_t _value = detail::_cs_to_integral<int64_t>(_ptr, _eptr, base);

		if (_ptr == _eptr)
			throw(std::invalid_argument("invalid stdex::stoll argument"));
		if(!_eptr)
			throw(std::out_of_range("stdex::stoll argument out of range"));

		if (idx != 0)
			*idx = (size_t) (_eptr - _ptr);

		return (_value);
	}

	inline uint64_t stoull(const string &s, size_t *idx = 0, int base = 10)
	{
		const char *_eptr = s.c_str(), *_ptr = _eptr;
		
		uint64_t _value = detail::_cs_to_integral<uint64_t>(_ptr, _eptr, base);

		if (_ptr == _eptr)
			throw(std::invalid_argument("invalid stdex::stoull argument"));
		if(!_eptr)
			throw(std::out_of_range("stdex::stoull argument out of range"));

		if (idx != 0)
			*idx = (size_t) (_eptr - _ptr);

		return (_value);
	}
#endif

	template<class _T>
	inline string to_string(const _T &t)
	{
		stringstream ss;
		ss << t;
		return ss.str();
	}
	
	template<>
	inline string to_string<int>(const int &value)
	{
		using namespace std;
		char buf[30];
		sprintf(buf, "%d", value);
		
		return string(buf);
	}

	template<>
	inline string to_string<unsigned int>(const unsigned int &value)
	{
		using namespace std;
		char buf[30];
		sprintf(buf, "%u", value);

		return string(buf);
	}
	
	template<>
	inline string to_string<bool>(const bool &value)
	{
		if(value)
			return string("true");
		return string("false");
	}

	template<>
	inline string to_string<float>(const float &value)
	{
		using namespace std;
		char buf[256];
		sprintf(buf, "%f", value);

		return string(buf);
	}

	template<>
	inline string to_string<double>(const double &value)
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

	template<>
	inline string to_string<std::string>(const std::string &x)
	{
		return x;
	}

	template<>
	inline 
		enable_if<is_same<int, long>::value == (false), string>::type 
	to_string<long>(const long &value)
	{
		using namespace std;
		char buf[512];
		sprintf(buf, "%ld", value);

		return string(buf);
	}

	template<>
	inline
		enable_if<is_same<unsigned int, unsigned long>::value == (false), string>::type
	to_string<unsigned long>(const unsigned long &value)
	{
		using namespace std;
		char buf[512];
		sprintf(buf, "%lu", value);

		return string(buf);
	}
	
	template<>
	inline
		enable_if<is_same<long double, double>::value == (false), string>::type
	to_string<long double>(const long double &value)
	{
		using namespace std;
		char buf[512];
		sprintf(buf, "%Lf", value);

		return string(buf);
	}

#ifdef LLONG_MAX

	template<>
	inline
		enable_if<is_same<long long, long>::value == (false), string>::type
	to_string<long long>(const long long &value)
	{
		using namespace std;
		char buf[512];
		sprintf(buf, "%lld", value);

		return string(buf);
	}

	template<>
	inline
		enable_if<is_same<unsigned long long, unsigned long>::value == (false), string>::type
	to_string<unsigned long long>(const unsigned long long &value)
	{
		using namespace std;
		char buf[512];
		sprintf(buf, "%llu", value);

		return string(buf);
	}
#endif

}

#endif // _STDEX_STRING_H

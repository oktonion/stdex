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
#include <cfloat>
#include <stdexcept>
#include <cmath>
#include <errno.h>
#include <limits>



namespace stdex
{
	using std::basic_string;
	using std::char_traits;

	using std::string;
	using std::wstring;

	namespace detail
	{
		template<bool _IsSigned>
		struct _str_to_integral_chooser_impl
		{
			typedef long int type;
			static long int call(const char* str, char** endptr, int base)
			{
				using namespace std;

				return strtol(str, endptr, base);
			}
		};

		template<>
		struct _str_to_integral_chooser_impl<false>
		{
			typedef unsigned long int type;
			static unsigned long int call(const char* str, char** endptr, int base)
			{
				using namespace std;

				return strtoul(str, endptr, base);
			}
		};

		template<class _T>
		struct _str_to_integral_chooser
		{
			typedef _str_to_integral_chooser_impl<is_signed<_T>::value> impl;
		};

#ifdef LLONG_MAX
		template<bool _IsSigned>
		struct _str_to_integral_chooser_impl_ll
		{
			typedef long long int type;
			static long int call(const char* str, char** endptr, int base)
			{
				using namespace std;

				return strtoll(str, endptr, base);
			}
		};

		template<>
		struct _str_to_integral_chooser_impl_ll<false>
		{
			typedef unsigned long long int type;
			static unsigned long int call(const char* str, char** endptr, int base)
			{
				using namespace std;

				return strtoull(str, endptr, base);
			}
		};

		template<class _T>
		struct _str_to_integral_chooser_ll
		{
			typedef _str_to_integral_chooser_impl_ll<is_signed<_T>::value> impl;
		};
#endif


		template <class _T>
		inline _T _cs_to_integral(const char *s, const char *&num_s_end, int base = 10)
		{
			typedef typename _str_to_integral_chooser<_T>::impl _str_to_integral;

			int last_errno = errno;
			errno = 0;
			char *endptr = 0;
			typename _str_to_integral::type _value = _str_to_integral::call(s, &endptr, base);

#ifdef LONG_MAX 
#ifdef LONG_MIN 
			if ((_value == LONG_MAX || _value == LONG_MIN) && errno == ERANGE)
#else
			if ((_value == LONG_MAX || _value == -LONG_MAX) && errno == ERANGE)
#endif
#else
#ifdef LONG_MIN
			if ((_value == -LONG_MIN || _value == LONG_MIN) && errno == ERANGE)
#else
			if (errno == ERANGE)
#endif
#endif
				num_s_end = 0;
			else if (_value > std::numeric_limits<_T>::max() || _value < std::numeric_limits<_T>::min())
				num_s_end = 0;
			else
				num_s_end = endptr;

			if (errno != last_errno)
				errno = last_errno;

			return _value;
		}

		template <>
		inline int _cs_to_integral<int>(const char *s, const char *&num_s_end, int base)
		{
			typedef _str_to_integral_chooser<int>::impl _str_to_integral;

			int last_errno = errno;
			errno = 0;
			char *endptr = 0;
			_str_to_integral::type _value = _str_to_integral::call(s, &endptr, base);

#ifdef LONG_MAX 
#ifdef LONG_MIN 
			if ((_value == LONG_MAX || _value == LONG_MIN) && errno == ERANGE)
#else
			if ((_value == LONG_MAX || _value == -LONG_MAX) && errno == ERANGE)
#endif
#else
#ifdef LONG_MIN
			if ((_value == -LONG_MIN || _value == LONG_MIN) && errno == ERANGE)
#endif
#endif
				num_s_end = 0;
			else if (_value > std::numeric_limits<int>::max() || _value < std::numeric_limits<int>::min())
				num_s_end = 0;
			else
				num_s_end = endptr;

			if (errno != last_errno)
				errno = last_errno;

			return _value;
		}

#ifdef LLONG_MAX
		template <class _T>
		inline _T _cs_to_integral_ll(const char *s, const char *&num_s_end, int base = 10)
		{
			typedef typename _str_to_integral_chooser_ll<_T>::impl _str_to_integral;

			int last_errno = errno;
			errno = 0;
			char *endptr = 0;
			typename _str_to_integral::type _value = _str_to_integral::call(s, &endptr, base);

#ifdef LLONG_MAX 
#ifdef LLONG_MIN 
			if ((_value == LLONG_MAX || _value == LLONG_MIN) && errno == ERANGE)
#else
			if ((_value == LLONG_MAX || _value == -LLONG_MAX) && errno == ERANGE)
#endif
#else
#ifdef LLONG_MIN
			if ((_value == -LLONG_MIN || _value == LLONG_MIN) && errno == ERANGE)
#else
			if (errno == ERANGE)
#endif
#endif
				num_s_end = 0;
			else if (_value > std::numeric_limits<_T>::max() || _value < std::numeric_limits<_T>::min())
				num_s_end = 0;
			else
				num_s_end = endptr;

			if (errno != last_errno)
				errno = last_errno;

			return _value;
		}
#endif

		template <class _T>
		inline long double _cs_to_floating_point(const char *str, const char *&num_s_end)
		{
			using namespace std;

			int last_errno = errno;
			errno = 0;
			char *endptr = 0;
			double _value = strtod(str, &endptr);

#ifdef HUGE_VAL
			if ((_value == HUGE_VAL || _value == -HUGE_VAL) && errno == ERANGE)
#else
			if (errno == ERANGE)
#endif
				num_s_end = 0;
			else if (_value > std::numeric_limits<_T>::max() || _value < -std::numeric_limits<_T>::max())
				num_s_end = 0;
			else
				num_s_end = endptr;

			if (errno != last_errno)
				errno = last_errno;

			return _value;
		}

#ifdef HUGE_VALL
		template <>
		inline long double _cs_to_floating_point<long double>(const char *str, const char *&num_s_end)
		{
			using namespace std;

			int last_errno = errno;
			errno = 0;
			char *endptr = 0;
			long double _value = strtold(str, &endptr);

			if ((_value == HUGE_VALL || _value == -HUGE_VALL) && errno == ERANGE)
				num_s_end = 0;
			else
				num_s_end = endptr;

			if (errno != last_errno)
				errno = last_errno;

			return _value;
		}
#endif
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
		
		double _value = static_cast<double>(detail::_cs_to_floating_point<double>(_ptr, _eptr));

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
		
		typedef conditional<is_same<long double, double>::value, double, long double>::type type;

		long double _value = detail::_cs_to_floating_point<type>(_ptr, _eptr);

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
		
		int64_t _value = detail::_cs_to_integral_ll<int64_t>(_ptr, _eptr, base);

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
		
		uint64_t _value = detail::_cs_to_integral_ll<uint64_t>(_ptr, _eptr, base);

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
		char buf[1024];
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
		char buf[1024];
		sprintf(buf, "%lld", value);

		return string(buf);
	}

	template<>
	inline
		enable_if<is_same<unsigned long long, unsigned long>::value == (false), string>::type
	to_string<unsigned long long>(const unsigned long long &value)
	{
		using namespace std;
		char buf[2048];
		sprintf(buf, "%llu", value);

		return string(buf);
	}
#endif

}

#endif // _STDEX_STRING_H

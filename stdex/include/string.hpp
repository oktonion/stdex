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

		template<class _T, unsigned long N>
		struct _type_cs_len
		{
			enum
			{
				value = _type_cs_len<_T, N / (unsigned long)(10)>::value + 1
			};
		};

		template<class _T >
		struct _type_cs_len< _T, 0>
		{
			enum
			{
				value = 1
			};
		};

		template<class _T, bool _HasQuietNaN, bool _HasSignalingNaN>
		struct _not_a_number_impl
		{
			static _T NaN() { return std::numeric_limits<_T>::quiet_NaN();}
		};

		template<class _T>
		struct _not_a_number_impl<_T, false, true>
		{
			static _T NaN() { return std::numeric_limits<_T>::signaling_NaN(); }
		};

		template<class _T>
		struct _not_a_number_impl<_T, false, false>
		{
			static _T NaN() { typedef _T type; return type(); }
		};

		template<class _T>
		struct _not_a_number
		{
			typedef _not_a_number_impl<_T, std::numeric_limits<_T>::has_quiet_NaN, std::numeric_limits<_T>::has_signaling_NaN> impl;
		};

		template<class _T, bool _HasInfinity>
		struct _infinity_impl
		{
			static _T inf() { return std::numeric_limits<_T>::infinity(); }
		};

		template<class _T>
		struct _infinity_impl<_T, false>
		{
			static _T inf() { return std::numeric_limits<_T>::max(); }
		};

		template<class _T>
		struct _infinity
		{
			typedef _infinity_impl<_T, std::numeric_limits<_T>::has_infinity> impl;
		};

#if defined(LLONG_MAX) || defined(LLONG_MIN) 

#ifndef _STDEX_NATIVE_CPP11_SUPPORT
		long long
			_cs_to_signed_ll(const char *nptr, char **endptr,  int base)
		{
			using namespace std;
			 const char *s = nptr;
			 unsigned long long acc;
			 int c;
			 unsigned long long cutoff;
			 int neg = 0, any, cutlim;

			/*
			* Skip white space and pick up leading +/- sign if any.
			* If base is 0, allow 0x for hex and 0 for octal, else
			* assume decimal; if base is already 16, allow 0x.
			*/
			do {
				c = *s++;
			} while (isspace(c));
			if (c == '-') {
				neg = 1;
				c = *s++;
			}
			else if (c == '+')
				c = *s++;
			if ((base == 0 || base == 16) &&
				c == '0' && (*s == 'x' || *s == 'X')) {
				c = s[1];
				s += 2;
				base = 16;
			}
			if (base == 0)
				base = c == '0' ? 8 : 10;

			/*
			* Compute the cutoff value between legal numbers and illegal
			* numbers.  That is the largest legal value, divided by the
			* base.  An input number that is greater than this value, if
			* followed by a legal input character, is too big.  One that
			* is equal to this value may be valid or not; the limit
			* between valid and invalid numbers is then based on the last
			* digit.  For instance, if the range for longs is
			* [-2147483648..2147483647] and the input base is 10,
			* cutoff will be set to 214748364 and cutlim to either
			* 7 (neg==0) or 8 (neg==1), meaning that if we have accumulated
			* a value > 214748364, or equal but the next digit is > 7 (or 8),
			* the number is too big, and we will return a range error.
			*
			* Set any if any `digits' consumed; make it negative to indicate
			* overflow.
			*/
			cutoff = neg ? -(unsigned long long) LLONG_MIN : LLONG_MAX;
			cutlim = cutoff % (unsigned long long) base;
			cutoff /= (unsigned long long) base;
			for (acc = 0, any = 0;; c = *s++) {
				if (isdigit(c))
					c -= '0';
				else if (isalpha(c))
					c -= isupper(c) ? 'A' - 10 : 'a' - 10;
				else
					break;
				if (c >= base)
					break;
				if (any < 0 || acc > cutoff || (acc == cutoff && c > cutlim))
					any = -1;
				else {
					any = 1;
					acc *= base;
					acc += c;
				}
			}
			if (any < 0) {
				acc = neg ? LLONG_MIN : LLONG_MAX;
				errno = ERANGE;
			}
			else if (neg)
				acc = -acc;
			if (endptr != 0)
				*endptr = (char *) (any ? s - 1 : nptr);
			return (acc);
		}

		unsigned long long
			_cs_to_unsigned_ll(const char *nptr, char **endptr,  int base)
		{
			 const char *s = nptr;
			 unsigned long long acc;
			 int c;
			 unsigned long long cutoff;
			 int neg = 0, any, cutlim;

			/*
			* See strtol for comments as to the logic used.
			*/
			do {
				c = *s++;
			} while (isspace(c));
			if (c == '-') {
				neg = 1;
				c = *s++;
			}
			else if (c == '+')
				c = *s++;
			if ((base == 0 || base == 16) &&
				c == '0' && (*s == 'x' || *s == 'X')) {
				c = s[1];
				s += 2;
				base = 16;
			}
			if (base == 0)
				base = c == '0' ? 8 : 10;
			cutoff = (unsigned long long) ULLONG_MAX / (unsigned long long) base;
			cutlim = (unsigned long long) ULLONG_MAX % (unsigned long long) base;
			for (acc = 0, any = 0;; c = *s++) {
				if (isdigit(c))
					c -= '0';
				else if (isalpha(c))
					c -= isupper(c) ? 'A' - 10 : 'a' - 10;
				else
					break;
				if (c >= base)
					break;
				if (any < 0 || acc > cutoff || (acc == cutoff && c > cutlim))
					any = -1;
				else {
					any = 1;
					acc *= base;
					acc += c;
				}
			}
			if (any < 0) {
				acc = ULLONG_MAX;
				errno = ERANGE;
			}
			else if (neg)
				acc = -acc;
			if (endptr != 0)
				*endptr = (char *) (any ? s - 1 : nptr);
			return (acc);
		}
#else
		long long
			_cs_to_signed_ll(const char *nptr, char **endptr,  int base)
		{
			using namespace std;

			return strtoll(nptr, endptr, base);
		}

		unsigned long long
			_cs_to_unsigned_ll(const char *nptr, char **endptr,  int base)
		{
			using namespace std;

			return strtoull(nptr, endptr, base);
		}
#endif


		template<bool _IsSigned>
		struct _str_to_integral_chooser_impl_ll
		{
			typedef long long int type;
			static long long int call(const char* str, char** endptr, int base)
			{
				using namespace std;

				return _cs_to_signed_ll(str, endptr, base);
			}
		};

		template<>
		struct _str_to_integral_chooser_impl_ll<false>
		{
			typedef unsigned long long int type;
			static unsigned long long int call(const char* str, char** endptr, int base)
			{
				using namespace std;

				return _cs_to_unsigned_ll(str, endptr, base);
			}
		};

		template<class _T>
		struct _str_to_integral_chooser_ll
		{
			typedef _str_to_integral_chooser_impl_ll<is_signed<_T>::value> impl;
		};

		template<class _T, unsigned long long N>
		struct _type_cs_len_ll
		{
			enum
			{
				value = _type_cs_len_ll<_T, N / (unsigned long long)(10)>::value + 1
			};
		};

		template<class _T >
		struct _type_cs_len_ll< _T, 0>
		{
			enum
			{
				value = 2
			};
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

#if defined(LLONG_MAX) || defined(LLONG_MIN)
		template <class _T>
		inline _T _cs_to_integral_ll(const char *s, const char *&num_s_end, int base)
		{
			typedef typename _str_to_integral_chooser_ll<_T>::impl _str_to_integral;

			int last_errno = errno;
			errno = 0;
			char *endptr = 0;
			typename _str_to_integral::type _value = _str_to_integral::call(s, &endptr, base);

			if (errno == ERANGE)
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
#else
		long double
			_a_to_floating_point(const char *str)
		{
			using namespace std;

			long double value;

			if (sscanf(str, "%Lf", &value) == EOF)
			{

			  long double fp_integer_part = 0.0L, fp_fractional_part = 0.0L;
			  size_t i, length = strlen(str);

			  i = 0; // Left to right
			  while (str[i] != '.') {
				  fp_integer_part = fp_integer_part * 10.0L + (str[i] - '0');
				  i++;
			  }

			
			  i = length - 1; // Right to left

			  while (!isdigit(str[i]) && str[i] != '.')
				 i--;

			  while (str[i] != '.') {
				  fp_fractional_part = (fp_fractional_part + (str[i] - '0')) / 10.0L;
				  i--;
			  }

			  value = fp_integer_part + fp_fractional_part;
			}

			if(value > numeric_limits<long double>::max() || value < numeric_limits<long double>::min())
			{
			  errno = ERANGE;
			  value = 0.0;
			}

			return value;
		}

		long double
			_cs_to_long_double(const char *str, char const **ptr)
		{
			using namespace std;

			const char *p;

			if (!ptr)
				return _a_to_floating_point(str);

			p = str;

			while (isspace(*p))
				++p;

			if (*p == '+' || *p == '-')
				++p;

			typedef _not_a_number<long double>::impl not_a_number_impl;
			typedef _infinity<long double>::impl infinity_impl;

			/* INF or INFINITY.  */
			if ((p[0] == 'i' || p[0] == 'I')
				&& (p[1] == 'n' || p[1] == 'N')
				&& (p[2] == 'f' || p[2] == 'F'))
			{
				if ((p[3] == 'i' || p[3] == 'I')
					&& (p[4] == 'n' || p[4] == 'N')
					&& (p[5] == 'i' || p[5] == 'I')
					&& (p[6] == 't' || p[6] == 'T')
					&& (p[7] == 'y' || p[7] == 'Y'))
				{
					*ptr = p + 8;
					return infinity_impl::inf();
				}
				else
				{
					*ptr = p + 3;
					return infinity_impl::inf();
				}
			}

			/* NAN or NAN(foo).  */
			if ((p[0] == 'n' || p[0] == 'N')
				&& (p[1] == 'a' || p[1] == 'A')
				&& (p[2] == 'n' || p[2] == 'N'))
			{
				p += 3;
				if (*p == '(')
				{
					++p;
					while (*p != '\0' && *p != ')')
						++p;
					if (*p == ')')
						++p;
				}
				*ptr = p;
				return not_a_number_impl::NaN();
			}

			/* digits, with 0 or 1 periods in it.  */
			if (isdigit(*p) || *p == '.')
			{
				int got_dot = 0;
				while (isdigit(*p) || (!got_dot && *p == '.'))
				{
					if (*p == '.')
						got_dot = 1;
					++p;
				}

				/* Exponent.  */
				if (*p == 'e' || *p == 'E')
				{
					int i;
					i = 1;
					if (p[i] == '+' || p[i] == '-')
						++i;
					if (isdigit(p[i]))
					{
						while (isdigit(p[i]))
							++i;
						*ptr = p + i;
						if (std::numeric_limits<long double>::max_exponent10 < i)
						{
							errno = ERANGE;
							return std::numeric_limits<long double>::max();
						}
						return _a_to_floating_point(str);
					}
				}
				*ptr = p;
				return _a_to_floating_point(str);
			}
			/* Didn't find any digits.  Doesn't look like a number.  */
			*ptr = str;
			return not_a_number_impl::NaN();
		}

		template <>
		inline long double _cs_to_floating_point<long double>(const char *str, const char *&num_s_end)
		{
			using namespace std;

			int last_errno = errno;
			errno = 0;
			long double _value = _cs_to_long_double(str, &num_s_end);

			if (errno == ERANGE)
				num_s_end = 0;

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
		
		typedef conditional<sizeof(long double) == sizeof(double), double, long double>::type type;

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
#ifdef INT_MAX
		char buf[detail::_type_cs_len<int, INT_MAX>::value];
#else
		char buf[32];
#endif
		sprintf(buf, "%d", value);
		
		return string(buf);
	}

	template<>
	inline string to_string<unsigned int>(const unsigned int &value)
	{
		using namespace std;
#ifdef UINT_MAX
		char buf[detail::_type_cs_len<unsigned int, UINT_MAX>::value];
#else
		char buf[32];
#endif
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
#if defined(FLT_MANT_DIG) && defined(FLT_MIN_EXP)
		char buf[3 + FLT_MANT_DIG - FLT_MIN_EXP];
#else
		char buf[256];
#endif
		sprintf(buf, "%f", value);

		return string(buf);
	}

	template<>
	inline string to_string<double>(const double &value)
	{
		using namespace std;
#if defined(DBL_MANT_DIG) && defined(DBL_MIN_EXP)
		char buf[3 + DBL_MANT_DIG - DBL_MIN_EXP];
#else
		char buf[2048]; // strange assumption, I know
#endif
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
#ifdef LONG_MAX
		char buf[detail::_type_cs_len<long, LONG_MAX>::value];
#else
#ifdef ULONG_MAX
		char buf[detail::_type_cs_len<long, (ULONG_MAX / 2)>::value];
#else
		char buf[256]; // strange assumption, I know
#endif
#endif
		sprintf(buf, "%ld", value);

		return string(buf);
	}

	template<>
	inline
		enable_if<is_same<unsigned int, unsigned long>::value == (false), string>::type
	to_string<unsigned long>(const unsigned long &value)
	{
		using namespace std;
#ifdef ULONG_MAX
		char buf[detail::_type_cs_len<unsigned long, ULONG_MAX>::value];
#else
#ifdef LONG_MAX
		char buf[detail::_type_cs_len<unsigned long, (LONG_MAX * 2 + 1)>::value];
#else
		char buf[512]; // strange assumption, I know
#endif
#endif
		sprintf(buf, "%lu", value);

		return string(buf);
	}
	
	template<>
	inline
		enable_if<is_same<long double, double>::value == (false), string>::type
	to_string<long double>(const long double &value)
	{
		using namespace std;
#if defined(LDBL_MANT_DIG) && defined(LDBL_MIN_EXP)
		char buf[3 + LDBL_MANT_DIG - LDBL_MIN_EXP];
#else
		char buf[4096]; // strange assumption, I know
#endif
		sprintf(buf, "%Lf", value);

		string result(buf);

		// some compilers ignore 'f' flag of spintf and print large values with scientific notation, as if 'e' flag was passed
		// so we are removing substrings like 'e-10' and trying to enforce the precision by slow and not so precise conversion:

		size_t e_pos = result.rfind('e'); 
		if (e_pos != string::npos)
		{
#ifdef LDBL_MAX_10_EXP
			char str_integer_part_reverse[LDBL_MAX_10_EXP + 3];
#else
			char str_integer_part_reverse[sizeof(buf)];
#endif
			long double fp_integer_part, fp_fractional_part;
			size_t symbols_converted_n = 0;
			fp_fractional_part = modf(value, &fp_integer_part);
			while (fp_integer_part > 0.0L)
			{
				str_integer_part_reverse[symbols_converted_n++] = '0' + (int) fmod(fp_integer_part, 10.0L);
				fp_integer_part = floor(fp_integer_part / 10.0L);
			}

			for (size_t i = 0; i < symbols_converted_n; i++)
				buf[i] = str_integer_part_reverse[symbols_converted_n - i - 1];

			buf[symbols_converted_n++] = '.';

			if (fp_fractional_part > 0.0L)
			{
				while (fp_fractional_part > 0.0L)
				{
					fp_fractional_part *= 10.0L;
					fp_fractional_part = modf(fp_fractional_part, &fp_integer_part);
					buf[symbols_converted_n++] = '0' + (int) fp_integer_part;
				}
			}
			else
			   buf[symbols_converted_n++] = '0';

			buf[symbols_converted_n] = 0; // EOS

			result = buf;
		}

		return result;
	}

#ifdef LLONG_MAX

	template<>
	inline
		enable_if<is_same<long long, long>::value == (false), string>::type
	to_string<long long>(const long long &value)
	{
		using namespace std;
#ifdef LLONG_MAX
		char buf[detail::_type_cs_len_ll<long long, LLONG_MAX>::value];
#else
#ifdef ULLONG_MAX
		char buf[detail::_type_cs_len_ll<long long, (ULONG_MAX / 2)>::value];
#else
		char buf[1024]; // strange assumption, I know
#endif
#endif
		sprintf(buf, "%lld", value);

		return string(buf);
	}

	template<>
	inline
		enable_if<is_same<unsigned long long, unsigned long>::value == (false), string>::type
	to_string<unsigned long long>(const unsigned long long &value)
	{
		using namespace std;
#ifdef ULLONG_MAX
		char buf[detail::_type_cs_len_ll<unsigned long long, ULLONG_MAX>::value];
#else
#ifdef LLONG_MAX
		char buf[detail::_type_cs_len_ll<unsigned long long, (LLONG_MAX * 2 + 1)>::value];
#else
		char buf[1024]; // strange assumption, I know
#endif
#endif
		sprintf(buf, "%llu", value);

		return string(buf);
	}
#endif

}

#endif // _STDEX_STRING_H

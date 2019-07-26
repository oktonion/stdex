#include "../stdex/include/cstdint.hpp"

#include "../stdex/include/core.h"

#include <iostream>

#define DYNAMIC_VERIFY(cond) if(!(cond)) {std::cout << "check condition \'" << #cond << "\' failed at line " << __LINE__ << std::endl; return __LINE__;}
#define RUN_TEST(test) {std::cout << #test << std::endl; int line = test(); if(line != 0) {std::cout << "failed at line " << line << std::endl; return line;}}

void test01()
{
  typedef stdex::int8_t          my_int8_t;
  typedef stdex::int16_t         my_int16_t;
  typedef stdex::int32_t         my_int32_t;
  typedef stdex::int64_t         my_int64_t;
  typedef stdex::int_fast8_t     my_int_fast8_t;
  typedef stdex::int_fast16_t    my_int_fast16_t;
  typedef stdex::int_fast32_t    my_int_fast32_t;
  typedef stdex::int_fast64_t    my_int_fast64_t;	
  typedef stdex::int_least8_t    my_int_least8_t;
  typedef stdex::int_least16_t   my_int_least16_t;
  typedef stdex::int_least32_t   my_int_least32_t;
  typedef stdex::int_least64_t   my_int_least64_t;
  typedef stdex::intmax_t        my_intmax_t;
  typedef stdex::intptr_t        my_intptr_t;
  typedef stdex::uint8_t         my_uint8_t;
  typedef stdex::uint16_t        my_uint16_t;
  typedef stdex::uint32_t        my_uint32_t;
  typedef stdex::uint64_t        my_uint64_t;
  typedef stdex::uint_fast8_t    my_uint_fast8_t;
  typedef stdex::uint_fast16_t   my_uint_fast16_t;
  typedef stdex::uint_fast32_t   my_uint_fast32_t;
  typedef stdex::uint_fast64_t   my_uint_fast64_t;	
  typedef stdex::uint_least8_t   my_uint_least8_t;
  typedef stdex::uint_least16_t  my_uint_least16_t;
  typedef stdex::uint_least32_t  my_uint_least32_t;
  typedef stdex::uint_least64_t  my_uint_least64_t;
  typedef stdex::uintmax_t       my_uintmax_t;
  typedef stdex::uintptr_t       my_uintptr_t;
}

void test02()
{
	stdex::int8_t i8 = STDEX_INT8_C(0);
	(void) i8;
	stdex::uint8_t ui8 = STDEX_UINT8_C(0);
	(void) ui8;
	stdex::int16_t i16 = STDEX_INT16_C(0);
	(void) i16;
	stdex::uint16_t ui16 = STDEX_UINT16_C(0);
	(void) ui16;
	stdex::int32_t i32 = STDEX_INT32_C(0);
	(void) i32;
	stdex::uint32_t ui32 = STDEX_UINT32_C(0);
	(void) ui32;
#ifdef LLONG_MAX
	stdex::int64_t i64 = 0;
	(void) i64;
#endif
#ifdef ULLONG_MAX
	stdex::uint64_t ui64 = 0;
	(void) ui64;
#endif
	stdex::int_least8_t i8least = STDEX_INT8_C(0);
	(void) i8least;
	stdex::uint_least8_t ui8least = STDEX_UINT8_C(0);
	(void) ui8least;
	stdex::int_least16_t i16least = STDEX_INT16_C(0);
	(void) i16least;
	stdex::uint_least16_t ui16least = STDEX_UINT16_C(0);
	(void) ui16least;
	stdex::int_least32_t i32least = STDEX_INT32_C(0);
	(void) i32least;
	stdex::uint_least32_t ui32least = STDEX_UINT32_C(0);
	(void) ui32least;
#ifdef LLONG_MAX
	stdex::int_least64_t i64least = 0;
	(void) i64least;
#endif
#ifdef ULLONG_MAX
	stdex::uint_least64_t ui64least = 0;
	(void) ui64least;
#endif
	stdex::int_fast8_t i8fast = STDEX_INT8_C(0);
	(void) i8fast;
	stdex::uint_fast8_t ui8fast = STDEX_UINT8_C(0);
	(void) ui8fast;
	stdex::int_fast16_t i16fast = STDEX_INT16_C(0);
	(void) i16fast;
	stdex::uint_fast16_t ui16fast = STDEX_UINT16_C(0);
	(void) ui16fast;
	stdex::int_fast32_t i32fast = STDEX_INT32_C(0);
	(void) i32fast;
	stdex::uint_fast32_t ui32fast = STDEX_UINT32_C(0);
	(void) ui32fast;
#ifdef ULLONG_MAX
	stdex::int_fast64_t i64fast = 0;
	(void) i64fast;
#endif
#ifdef ULLONG_MAX
	stdex::uint_fast64_t ui64fast = 0;
	(void) ui64fast;
#endif
	stdex::intmax_t im = 0;
	(void) im;
	stdex::uintmax_t uim = 0;
	(void) uim;
}

template<class T, stdex::intmax_t Value>
struct signed_integral_constant_checker
{
	static const T value = Value;
};

template<class T, stdex::uintmax_t Value>
struct unsigned_integral_constant_checker
{
	static const T value = Value;
};

#define SIGNED_INTEGRAL_CONSTANT_CHECK(bitsn, cvalue)\
	char int_of_##bitsn##_bits_check_fail[((signed_integral_constant_checker<stdex::int##bitsn##_t, STDEX_INT##bitsn##_C(cvalue)>::value == STDEX_INT##bitsn##_C(cvalue))) ? 1 : -1]; \
	char int_least_of_##bitsn##_bits_check_fail[((signed_integral_constant_checker<stdex::int_least##bitsn##_t, STDEX_INT##bitsn##_C(cvalue)>::value == STDEX_INT##bitsn##_C(cvalue))) ? 1 : -1]; \
	char int_fast_of_##bitsn##_bits_check_fail[((signed_integral_constant_checker<stdex::int_fast##bitsn##_t, STDEX_INT##bitsn##_C(cvalue)>::value == STDEX_INT##bitsn##_C(cvalue))) ? 1 : -1];
#define UNSIGNED_INTEGRAL_CONSTANT_CHECK(bitsn, cvalue)\
	char uint_of_##bitsn##_bits_check_fail[((unsigned_integral_constant_checker<stdex::uint##bitsn##_t, STDEX_UINT##bitsn##_C(cvalue)>::value == STDEX_UINT##bitsn##_C(cvalue))) ? 1 : -1]; \
	char uint_least_of_##bitsn##_bits_check_fail[((unsigned_integral_constant_checker<stdex::uint_least##bitsn##_t, STDEX_UINT##bitsn##_C(cvalue)>::value == STDEX_UINT##bitsn##_C(cvalue))) ? 1 : -1]; \
	char uint_fast_of_##bitsn##_bits_check_fail[((unsigned_integral_constant_checker<stdex::uint_fast##bitsn##_t, STDEX_UINT##bitsn##_C(cvalue)>::value == STDEX_UINT##bitsn##_C(cvalue))) ? 1 : -1];

#define INTEGRAL_CONSTANT_CHECK(bitsn, value_signed, value_unsigned)\
	SIGNED_INTEGRAL_CONSTANT_CHECK(bitsn, value_signed) \
	UNSIGNED_INTEGRAL_CONSTANT_CHECK(bitsn, value_unsigned)

void test03()
{
	INTEGRAL_CONSTANT_CHECK(8, -127, 255);
	INTEGRAL_CONSTANT_CHECK(16, -32767, 65535);
	INTEGRAL_CONSTANT_CHECK(32, -2147483647, 4294967295);
	// no 64bit checks
}

template <class T1, class T2>
int integral_constant_type_check(T1, T2)
{
	//
	// the types T1 and T2 may not be exactly
	// the same type, but they should be the
	// same size and signedness. We could use
	// numeric_limits to verify this, but
	// numeric_limits implementations currently
	// vary too much, or are incomplete or missing.
	//
	T1 t1 = static_cast<T1>(-1);  // cast suppresses warnings
	T2 t2 = static_cast<T2>(-1);  // ditto

	STATIC_ASSERT(sizeof(T1) == sizeof(T2), integral_sizes_should_be_the_same);
	DYNAMIC_VERIFY(t1 == t2);


	// native headers are permitted to promote small
	// unsigned types to type int:
	if (sizeof(T1) >= sizeof(int))
	{
		if (t1 > 0)
		{
			DYNAMIC_VERIFY(t2 > 0);
		}
		else
		{
			DYNAMIC_VERIFY(!(t2 > 0));
		}
	}
	else if (t1 < 0)
	{
		DYNAMIC_VERIFY(!(t2 > 0));
	}

	if (t1 > 0)
	{
		DYNAMIC_VERIFY(t2 > 0);
	}
	else
	{
		DYNAMIC_VERIFY(!(t2 > 0));
	}
	return 0;
}

int test04()
{
	DYNAMIC_VERIFY((integral_constant_type_check(stdex::int8_t(0), STDEX_INT8_C(0)) == 0));
	DYNAMIC_VERIFY((integral_constant_type_check(stdex::uint8_t(0), STDEX_UINT8_C(0)) == 0));
	DYNAMIC_VERIFY((integral_constant_type_check(stdex::int16_t(0), STDEX_INT16_C(0)) == 0));
	DYNAMIC_VERIFY((integral_constant_type_check(stdex::uint16_t(0), STDEX_UINT16_C(0)) == 0));
	DYNAMIC_VERIFY((integral_constant_type_check(stdex::int32_t(0), STDEX_INT32_C(0)) == 0));
	DYNAMIC_VERIFY((integral_constant_type_check(stdex::uint32_t(0), STDEX_UINT32_C(0)) == 0));

#ifdef LLONG_MAX
	DYNAMIC_VERIFY((integral_constant_type_check(stdex::int64_t(0), STDEX_INT64_C(0)) == 0));
#endif
#ifdef ULLONG_MAX
	DYNAMIC_VERIFY((integral_constant_type_check(stdex::uint64_t(0), STDEX_UINT64_C(0)) == 0));
#endif

	return 0;
}



int main(void)
{
	test01();
	test02();
	test03();

	RUN_TEST(test04);
	return 0;
}

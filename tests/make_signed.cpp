#include "../stdex/include/core.h"
#include "../stdex/include/type_traits.hpp"

int main(void)
{
    using namespace stdex;
    
    // make_signed
    STATIC_ASSERT((is_same<make_signed<const int>::type, signed const int>::value == (true)), make_signed_t_const_int);
    STATIC_ASSERT((is_same<make_signed<unsigned>::type, signed>::value == (true)), make_signed_t_unsigned);
    STATIC_ASSERT((is_same<make_signed<char>::type, signed char>::value == (true)), make_signed_t_char);
    // Positive tests.
    {
        typedef make_signed<const int>::type  	test2_type;
        STATIC_ASSERT((is_same<test2_type, const int>::value == (true)), make_signed_const_int);

        typedef make_signed<const unsigned int>::type  	test21c_type;
        STATIC_ASSERT((is_same<test21c_type, const signed int>::value == (true)), make_signed_const_unsigned_int);

        typedef make_signed<volatile unsigned int>::type  	test21v_type;
        STATIC_ASSERT((is_same<test21v_type, volatile signed int>::value == (true)), make_signed_volatile_unsigned_int);

        typedef make_signed<const volatile unsigned int>::type  	test21cv_type;
        STATIC_ASSERT((is_same<test21cv_type, const volatile signed int>::value == (true)), make_signed_const_volatile_unsigned_int);

        typedef make_signed<const char>::type  	test22_type;
        STATIC_ASSERT((is_same<test22_type, const signed char>::value == (true)), make_signed_const_char);

        typedef make_signed<volatile wchar_t>::type  	test23_type;
        STATIC_ASSERT(is_signed<test23_type>::value && is_volatile<test23_type>::value && sizeof(test23_type) == sizeof(volatile wchar_t), make_signed_volatile_wchar_t);

        typedef make_signed<const int>::type  	test2_type;
        STATIC_ASSERT((is_same<test2_type, const int>::value == (true)), check);

        typedef make_signed<const unsigned int>::type  	test21c_type;
        STATIC_ASSERT((is_same<test21c_type, const signed int>::value == (true)), check);

        typedef make_signed<volatile unsigned int>::type  	test21v_type;
        STATIC_ASSERT((is_same<test21v_type, volatile signed int>::value == (true)), check);

        typedef make_signed<const volatile unsigned int>::type  	test21cv_type;
        STATIC_ASSERT((is_same<test21cv_type, const volatile signed int>::value == (true)), check);

        typedef make_signed<const char>::type  	test22_type;
        STATIC_ASSERT((is_same<test22_type, const signed char>::value == (true)), check);

        typedef make_signed<volatile wchar_t>::type  	test23_type;
        STATIC_ASSERT((is_signed<test23_type>::value && is_volatile<test23_type>::value && sizeof(test23_type) == sizeof(volatile wchar_t)), check);

        typedef make_signed<wchar_t>::type wchar_signed;
        typedef make_unsigned<wchar_t>::type wchar_unsigned;
        STATIC_ASSERT((!is_same<wchar_signed, wchar_unsigned>::value == (true)), make_signed_t_wchar_t);
        STATIC_ASSERT((is_signed<make_signed<stdex_char16_t>::type>::value == (true)), make_signed_t_stdex_char16_t);
        STATIC_ASSERT((is_signed<make_signed<stdex_char32_t>::type>::value == (true)), make_signed_t_stdex_char32_t);
    }

#if (CHECK_FOR_COMPILE_ERROR_TESTS == 1)
    // Negative  tests.
    {
        typedef make_signed<bool>::type     	test1_type;

        typedef make_signed<int[4]>::type     test3_type;

        typedef void (fn_type)();
        typedef make_signed<fn_type>::type  	test4_type;

        typedef make_signed<float>::type test5_type;
    }
#endif
    return 0;
}

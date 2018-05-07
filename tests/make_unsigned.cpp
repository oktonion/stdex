#include "../stdex/include/core.h"
#include "../stdex/include/type_traits.hpp"

int main(void)
{
    using namespace stdex;
    
    // make_unsigned
    // Positive tests.
    {
        typedef make_unsigned<const unsigned int>::type  	test2_type;
        STATIC_ASSERT((is_same<test2_type, const unsigned int>::value == (true)), check);

        typedef make_unsigned<const signed int>::type  	test21c_type;
        STATIC_ASSERT((is_same<test21c_type, const unsigned int>::value == (true)), check);

        typedef make_unsigned<volatile signed int>::type  	test21v_type;
        STATIC_ASSERT((is_same<test21v_type, volatile unsigned int>::value == (true)), check);

        typedef make_unsigned<const volatile signed int>::type  	test21cv_type;
        STATIC_ASSERT((is_same<test21cv_type, const volatile unsigned int>::value == (true)), check);

        typedef make_unsigned<const char>::type  	test22_type;
        STATIC_ASSERT((is_same<test22_type, const unsigned char>::value == (true)), check);

        typedef make_unsigned<volatile wchar_t>::type  	test23_type;
        STATIC_ASSERT((is_unsigned<test23_type>::value == (true) && is_volatile<test23_type>::value == (true) && sizeof(test23_type) == sizeof(volatile wchar_t)), check);

        typedef make_unsigned<const unsigned int>::type  	test2_type;
        STATIC_ASSERT((is_same<test2_type, const unsigned int>::value == (true)), check);

        typedef make_unsigned<const signed int>::type  	test21c_type;
        STATIC_ASSERT((is_same<test21c_type, const unsigned int>::value == (true)), check);

        typedef make_unsigned<volatile signed int>::type  	test21v_type;
        STATIC_ASSERT((is_same<test21v_type, volatile unsigned int>::value == (true)), check);

        typedef make_unsigned<const volatile signed int>::type  	test21cv_type;
        STATIC_ASSERT((is_same<test21cv_type, const volatile unsigned int>::value == (true)), check);

        typedef make_unsigned<const char>::type  	test22_type;
        STATIC_ASSERT((is_same<test22_type, const unsigned char>::value == (true)), check);

        typedef make_unsigned<volatile wchar_t>::type  	test23_type;
        STATIC_ASSERT((is_unsigned<test23_type>::value == (true) && is_volatile<test23_type>::value == (true) && sizeof(test23_type) == sizeof(volatile wchar_t)), check);

    }

#if (CHECK_FOR_COMPILE_ERROR_TESTS == 1)
    // Negative  tests.
    {
        typedef make_unsigned<bool>::type     	test1_type;

        typedef make_unsigned<int[4]>::type     test3_type;

        typedef void (fn_type)();
        typedef make_unsigned<fn_type>::type  	test4_type;

        typedef make_unsigned<float>::type test5_type;
    }
#endif
    return 0;
}

#include "../stdex/include/core.h"
#include "../stdex/include/type_traits.hpp"

struct ClassType {};

int main(void)
{
    using namespace stdex;
    
    // is_unsigned
    STATIC_ASSERT(is_unsigned<void>::value == false, check);

    STATIC_ASSERT(is_unsigned<char>::value == (char(-1) > char(0)), check);
    STATIC_ASSERT(is_unsigned<signed char>::value == false, check);
    STATIC_ASSERT(is_unsigned<unsigned char>::value == true, check);
    STATIC_ASSERT(is_unsigned<wchar_t>::value == (wchar_t(-1) > wchar_t(0)), check);
    STATIC_ASSERT(is_unsigned<short>::value == false, check);
    STATIC_ASSERT(is_unsigned<unsigned short>::value == true, check);
    STATIC_ASSERT(is_unsigned<int>::value == false, check);
    STATIC_ASSERT(is_unsigned<unsigned int>::value == true, check);
    STATIC_ASSERT(is_unsigned<long>::value == false, check);
    STATIC_ASSERT(is_unsigned<unsigned long>::value == true, check);

    STATIC_ASSERT(is_unsigned<float>::value == false, check);
    STATIC_ASSERT(is_unsigned<double>::value == false, check);
    STATIC_ASSERT(is_unsigned<long double>::value == false, check);
    return 0;
}
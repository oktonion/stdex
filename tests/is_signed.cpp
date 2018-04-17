#include "../stdex/include/core.h"
#include "../stdex/include/type_traits.hpp"

struct ClassType {};

int main(void)
{
    using namespace stdex;
    
    // is_signed
    STATIC_ASSERT(is_signed<void>::value == false, check);

    STATIC_ASSERT(is_signed<char>::value == (char(-1) < char(0)), check);
    STATIC_ASSERT(is_signed<signed char>::value == true, check);
    STATIC_ASSERT(is_signed<unsigned char>::value == false, check);
    STATIC_ASSERT(is_signed<wchar_t>::value == (wchar_t(-1) < wchar_t(0)), check);
    STATIC_ASSERT(is_signed<short>::value == true, check);
    STATIC_ASSERT(is_signed<unsigned short>::value == false, check);
    STATIC_ASSERT(is_signed<int>::value == true, check);
    STATIC_ASSERT(is_signed<unsigned int>::value == false, check);
    STATIC_ASSERT(is_signed<long>::value == true, check);
    STATIC_ASSERT(is_signed<unsigned long>::value == false, check);

    STATIC_ASSERT(is_signed<float>::value == true, check);
    STATIC_ASSERT(is_signed<double>::value == true, check);
    STATIC_ASSERT(is_signed<long double>::value == true, check);
    return 0;
}
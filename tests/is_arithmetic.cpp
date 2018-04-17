#include "../stdex/include/core.h"
#include "../stdex/include/type_traits.hpp"

struct ClassType {};

int main(void)
{
    using namespace stdex;
    
    // is_arithmetic
    STATIC_ASSERT(is_arithmetic<void>::value == (false), can_not_be_arithmetic);
                                
    STATIC_ASSERT(is_arithmetic<char>::value == (true), should_be_arithmetic);
    STATIC_ASSERT(is_arithmetic<signed char>::value == (true), should_be_arithmetic);
    STATIC_ASSERT(is_arithmetic<unsigned char>::value == (true), should_be_arithmetic);
    STATIC_ASSERT(is_arithmetic<wchar_t>::value == (true), should_be_arithmetic);
    STATIC_ASSERT(is_arithmetic<short>::value == (true), should_be_arithmetic);
    STATIC_ASSERT(is_arithmetic<unsigned short>::value == (true), should_be_arithmetic);
    STATIC_ASSERT(is_arithmetic<int>::value == (true), should_be_arithmetic);
    STATIC_ASSERT(is_arithmetic<unsigned int>::value == (true), should_be_arithmetic);
    STATIC_ASSERT(is_arithmetic<long>::value == (true), should_be_arithmetic);
    STATIC_ASSERT(is_arithmetic<unsigned long>::value == (true), should_be_arithmetic);
    STATIC_ASSERT(is_arithmetic<long long>::value == (true), should_be_arithmetic);
    STATIC_ASSERT(is_arithmetic<unsigned long long>::value == (true), should_be_arithmetic);
    STATIC_ASSERT(is_arithmetic<float>::value == (true), should_be_arithmetic);
    STATIC_ASSERT(is_arithmetic<double>::value == (true), should_be_arithmetic);
    STATIC_ASSERT(is_arithmetic<long double>::value == (true), should_be_arithmetic);

    // Sanity check.
    STATIC_ASSERT(is_arithmetic<ClassType>::value == (false), can_not_be_arithmetic);
    return 0;
}

#include "../stdex/include/core.h"
#include "../stdex/include/type_traits.hpp"

struct ClassType {};

int main(void)
{
    using namespace stdex;
    
    STATIC_ASSERT(is_integral<void>::value == (false), can_not_be_integral);
								 
    STATIC_ASSERT(is_integral<char>::value == (true), should_be_integral);
    STATIC_ASSERT(is_integral<signed char>::value == (true), should_be_integral);
    STATIC_ASSERT(is_integral<unsigned char>::value == (true), should_be_integral);
    STATIC_ASSERT(is_integral<wchar_t>::value == (true), should_be_integral);
    STATIC_ASSERT(is_integral<char16_t>::value == (true), should_be_integral);
    STATIC_ASSERT(is_integral<char32_t>::value == (true), should_be_integral);
    STATIC_ASSERT(is_integral<short>::value == (true), should_be_integral);
    STATIC_ASSERT(is_integral<unsigned short>::value == (true), should_be_integral);
    STATIC_ASSERT(is_integral<int>::value == (true), should_be_integral);
    STATIC_ASSERT(is_integral<unsigned int>::value == (true), should_be_integral);
    STATIC_ASSERT(is_integral<long>::value == (true), should_be_integral);
    STATIC_ASSERT(is_integral<unsigned long>::value == (true), should_be_integral);
                                
    STATIC_ASSERT(is_integral<float>::value == (false), should_be_integral);
    STATIC_ASSERT(is_integral<double>::value == (false), should_be_integral);
    STATIC_ASSERT(is_integral<long double>::value == (false), should_be_integral);

    // Sanity check.
    STATIC_ASSERT(is_integral<ClassType>::value == (false), can_not_be_integral);

    return 0;
}
#include "../stdex/include/core.h"
#include "../stdex/include/type_traits.hpp"

struct ClassType {};

int main(void)
{
    using namespace stdex;
    
    // is_compound
    STATIC_ASSERT(is_compound<void>::value == false, should_not_be_compound);
    STATIC_ASSERT(is_compound<char>::value == false, should_not_be_compound);
    STATIC_ASSERT(is_compound<signed char>::value == false, should_not_be_compound);
    STATIC_ASSERT(is_compound<unsigned char>::value == false, should_not_be_compound);
    STATIC_ASSERT(is_compound<wchar_t>::value == false, should_not_be_compound);
    STATIC_ASSERT(is_compound<short>::value == false, should_not_be_compound);
    STATIC_ASSERT(is_compound<unsigned short>::value == false, should_not_be_compound);
    STATIC_ASSERT(is_compound<int>::value == false, should_not_be_compound);
    STATIC_ASSERT(is_compound<unsigned int>::value == false, should_not_be_compound);
    STATIC_ASSERT(is_compound<long>::value == false, should_not_be_compound);
    STATIC_ASSERT(is_compound<unsigned long>::value == false, should_not_be_compound);
    STATIC_ASSERT(is_compound<long long>::value == false, should_not_be_compound);
    STATIC_ASSERT(is_compound<unsigned long long>::value == false, should_not_be_compound);
    STATIC_ASSERT(is_compound<float>::value == false, should_not_be_compound);
    STATIC_ASSERT(is_compound<double>::value == false, should_not_be_compound);
    STATIC_ASSERT(is_compound<long double>::value == false, should_not_be_compound);

    // libstdc++/56609
    STATIC_ASSERT(is_compound<stdex::nullptr_t>::value == false, should_not_be_compound);

    // Sanity check.
    STATIC_ASSERT(is_compound<ClassType>::value == true, should_be_compound);
    return 0;
}

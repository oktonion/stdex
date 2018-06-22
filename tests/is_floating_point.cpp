#include "../stdex/include/core.h"
#include "../stdex/include/type_traits.hpp"

struct ClassType
{
};

int main(void)
{
    using namespace stdex;

    // is_floating_point
    STATIC_ASSERT(is_floating_point<void>::value == (false), can_not_be_floating_point);
    STATIC_ASSERT(is_floating_point<char>::value == (false), can_not_be_floating_point);
    STATIC_ASSERT(is_floating_point<signed char>::value == (false), can_not_be_floating_point);
    STATIC_ASSERT(is_floating_point<unsigned char>::value == (false), can_not_be_floating_point);
    STATIC_ASSERT(is_floating_point<wchar_t>::value == (false), can_not_be_floating_point);
    STATIC_ASSERT(is_floating_point<short>::value == (false), can_not_be_floating_point);
    STATIC_ASSERT(is_floating_point<unsigned short>::value == (false), can_not_be_floating_point);
    STATIC_ASSERT(is_floating_point<int>::value == (false), can_not_be_floating_point);
    STATIC_ASSERT(is_floating_point<unsigned int>::value == (false), can_not_be_floating_point);
    STATIC_ASSERT(is_floating_point<long>::value == (false), can_not_be_floating_point);
    STATIC_ASSERT(is_floating_point<unsigned long>::value == (false), can_not_be_floating_point);

    STATIC_ASSERT(is_floating_point<float>::value == (true), should_be_floating_point);
    STATIC_ASSERT(is_floating_point<double>::value == (true), should_be_floating_point);
    STATIC_ASSERT(is_floating_point<long double>::value == (true), should_be_floating_point);

    // Sanity check.
    STATIC_ASSERT(is_floating_point<ClassType>::value == (false), can_not_be_floating_point);

    return 0;
}

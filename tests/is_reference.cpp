#include "../stdex/include/core.h"
#include "../stdex/include/type_traits.hpp"

struct ClassType {};

int main(void)
{
    using namespace stdex;
    
    // is_reference
    STATIC_ASSERT(is_reference<int&>::value == (true), should_be_reference);
    STATIC_ASSERT(is_reference<ClassType&>::value == (true), should_be_reference);
    STATIC_ASSERT(is_reference<int(&)(int)>::value == (true), should_be_reference);
    STATIC_ASSERT(is_reference<const int&>::value == (true), should_be_reference);
    STATIC_ASSERT(is_reference<const volatile ClassType&>::value == (true), should_be_reference);
    STATIC_ASSERT(is_reference<volatile int(&)(int)>::value == (true), should_be_reference);

    // Sanity check.
    STATIC_ASSERT(is_reference<ClassType>::value == (false), can_not_be_reference);
    return 0;
}

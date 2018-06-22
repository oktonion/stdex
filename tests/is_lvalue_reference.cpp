#include "../stdex/include/core.h"
#include "../stdex/include/type_traits.hpp"

struct ClassType
{
};

int main(void)
{
    using namespace stdex;

    // is_lvalue_reference
    STATIC_ASSERT(is_lvalue_reference<int &>::value == (true), should_be_lvalue_reference);
    STATIC_ASSERT(is_lvalue_reference<ClassType &>::value == (true), should_be_lvalue_reference);
    STATIC_ASSERT(is_lvalue_reference<int (&)(int)>::value == (true), should_be_lvalue_reference);

    // Sanity check.

    STATIC_ASSERT(is_lvalue_reference<ClassType>::value == (false), can_not_be_lvalue_reference);
    return 0;
}

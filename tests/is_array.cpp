#include "../stdex/include/core.h"
#include "../stdex/include/type_traits.hpp"

struct ClassType
{
};

int main(void)
{
    using namespace stdex;

    // is_array
    STATIC_ASSERT(is_array<int[2]>::value == (true), should_be_array);
    STATIC_ASSERT(is_array<int[2][3]>::value == (true), should_be_array);
    STATIC_ASSERT(is_array<float * [2]>::value == (true), should_be_array);
    STATIC_ASSERT(is_array<float * [2][3]>::value == (true), should_be_array);
    STATIC_ASSERT(is_array<ClassType[2]>::value == (true), should_be_array);
    STATIC_ASSERT(is_array<ClassType[2][3]>::value == (true), should_be_array);

    // Sanity check.
    STATIC_ASSERT(is_array<ClassType>::value == (false), can_not_be_array);

    return 0;
}

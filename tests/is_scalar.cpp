#include "../stdex/include/core.h"
#include "../stdex/include/type_traits.hpp"

struct ClassType {};
enum EnumType {};

int main(void)
{
    using namespace stdex;
    
    // is_scalar
    STATIC_ASSERT(is_scalar<int>::value == (true), should_be_scalar);
    STATIC_ASSERT(is_scalar<float>::value == (true), should_be_scalar);
    STATIC_ASSERT(is_scalar<EnumType>::value == (true), should_be_scalar); // implement is_enum!
    STATIC_ASSERT(is_scalar<int*>::value == (true), should_be_scalar);
    STATIC_ASSERT(is_scalar<int(*)(int)>::value == (true), should_be_scalar);
    STATIC_ASSERT(is_scalar<stdex::nullptr_t>::value == (true), should_be_scalar);

    {
        typedef int(ClassType::*memobjt);
        typedef int (ClassType::*memfunct) (int);

        STATIC_ASSERT(is_scalar<memobjt>::value == (true), should_be_scalar);
        STATIC_ASSERT(is_scalar<memfunct>::value == (true), should_be_scalar);
    }

    // Sanity check.
    STATIC_ASSERT(is_scalar<ClassType>::value == (false), can_not_be_scalar);
    return 0;
}

#include "../stdex/include/core.h"
#include "../stdex/include/type_traits.hpp"

struct ClassType {};

int main(void)
{
    using namespace stdex;
    
    // is_pointer
    STATIC_ASSERT(is_pointer<int*>::value == (true), should_be_pointer);
    STATIC_ASSERT(is_pointer<ClassType*>::value == (true), should_be_pointer);
    STATIC_ASSERT(is_pointer<int(*)(int)>::value == (true), should_be_pointer);
                            
    // Sanity check.			
    STATIC_ASSERT(is_pointer<ClassType>::value == (false), can_not_be_pointer);
    return 0;
}
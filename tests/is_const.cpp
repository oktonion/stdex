#include "../stdex/include/core.h"
#include "../stdex/include/type_traits.hpp"

struct ClassType {};

int main(void)
{
    using namespace stdex;
    
    // is_const
    // Positive tests.
    STATIC_ASSERT(is_const<const int>::value == (true), should_be_const);
    STATIC_ASSERT(is_const<const volatile int>::value == (true), should_be_const);
    STATIC_ASSERT(is_const<cClassType>::value == (true), should_be_const);
    STATIC_ASSERT(is_const<cvClassType>::value == (true), should_be_const);
                            
    // Negative tests.	  
    STATIC_ASSERT(is_const<int>::value == (false), can_not_be_const);
    STATIC_ASSERT(is_const<volatile int>::value == (false), can_not_be_const);
    STATIC_ASSERT(is_const<ClassType>::value == (false), can_not_be_const);
    STATIC_ASSERT(is_const<vClassType>::value == (false), can_not_be_const);
    return 0;
}
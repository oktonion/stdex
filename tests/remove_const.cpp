#include "../stdex/include/core.h"
#include "../stdex/include/type_traits.hpp"

struct ClassType {};

int main(void)
{
    using namespace stdex;
    
    // remove_const
    STATIC_ASSERT((is_same<remove_const<const volatile int>::type, volatile int>::value == (true)), should_be_same);
    STATIC_ASSERT((is_same<remove_const<const int*>::type, const int*>::value == (true)), should_be_same);
    STATIC_ASSERT((is_same<remove_const<const volatile ClassType>::type, volatile ClassType>::value == (true)), should_be_same);
    STATIC_ASSERT((is_same<remove_const<const ClassType*>::type, const ClassType*>::value == (true)), should_be_same);
    return 0;
}

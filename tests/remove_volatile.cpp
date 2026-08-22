#include "../stdex/include/core.h"
#include "../stdex/include/type_traits.hpp"

struct ClassType {};

int main(void)
{
    using namespace stdex;
    
    // remove_volatile
    STATIC_ASSERT((is_same<remove_volatile<const volatile int>::type, const int>::value == (true)), should_be_same);
    STATIC_ASSERT((is_same<remove_volatile<volatile int*>::type, volatile int*>::value == (true)), should_be_same);
    STATIC_ASSERT((is_same<remove_volatile<const volatile ClassType>::type, const ClassType>::value == (true)), should_be_same);
    STATIC_ASSERT((is_same<remove_volatile<volatile ClassType*>::type, volatile ClassType*>::value == (true)), should_be_same);
    return 0;
}

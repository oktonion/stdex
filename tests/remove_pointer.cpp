#include "../stdex/include/core.h"
#include "../stdex/include/type_traits.hpp"

struct ClassType {};

int main(void)
{
    using namespace stdex;
    
    // remove_pointer
    STATIC_ASSERT((is_same<remove_pointer<int*>::type, int>::value == (true)), should_be_same);
    STATIC_ASSERT((is_same<remove_pointer<int>::type, int>::value == (true)), should_be_same);
    STATIC_ASSERT((is_same<remove_pointer<const int*>::type, const int>::value == (true)), should_be_same);
    STATIC_ASSERT((is_same<remove_pointer<volatile int*>::type, volatile int>::value == (true)), should_be_same);
    STATIC_ASSERT((is_same<remove_pointer<const volatile int*>::type, const volatile int>::value == (true)), should_be_same);
    STATIC_ASSERT((is_same<remove_pointer<int**>::type, int*>::value == (true)), should_be_same);
    STATIC_ASSERT((is_same<remove_pointer<ClassType*>::type, ClassType>::value == (true)), should_be_same);
    STATIC_ASSERT((is_same<remove_pointer<ClassType>::type, ClassType>::value == (true)), should_be_same);
    return 0;
}

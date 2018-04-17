#include "../stdex/include/core.h"
#include "../stdex/include/type_traits.hpp"

struct ClassType {};

int main(void)
{
    using namespace stdex;
    
    // remove_reference
    STATIC_ASSERT((is_same<remove_reference<int&>::type, int>::value == (true)), should_be_same);
    STATIC_ASSERT((is_same<remove_reference<int>::type, int>::value == (true)), should_be_same);
    STATIC_ASSERT((is_same<remove_reference<const int&>::type, const int>::value == (true)), should_be_same);
    STATIC_ASSERT((is_same<remove_reference<int*&>::type, int*>::value == (true)), should_be_same);
    STATIC_ASSERT((is_same<remove_reference<ClassType&>::type, ClassType>::value == (true)), should_be_same);
    STATIC_ASSERT((is_same<remove_reference<ClassType>::type, ClassType>::value == (true)), should_be_same);
    STATIC_ASSERT((is_same<remove_reference<int>::type, int>::value == (true)), should_be_same);
    STATIC_ASSERT((is_same<remove_reference<ClassType>::type, ClassType>::value == (true)), should_be_same);
    return 0;
}
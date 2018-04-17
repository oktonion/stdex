#include "../stdex/include/core.h"
#include "../stdex/include/type_traits.hpp"

struct ClassType {};

int main(void)
{
    using namespace stdex;
    
    //remove_all_extents
    STATIC_ASSERT((is_same<remove_all_extents<int>::type, int>::value == (true)), shoud_be_same);
    STATIC_ASSERT((is_same<remove_all_extents<int[2]>::type, int>::value == (true)), shoud_be_same);
    STATIC_ASSERT((is_same<remove_all_extents<int[2][3]>::type, int>::value == (true)), shoud_be_same);
    STATIC_ASSERT((is_same<remove_all_extents<const int[2][3]>::type, const int>::value == (true)), shoud_be_same);
    STATIC_ASSERT((is_same<remove_all_extents<ClassType>::type, ClassType>::value == (true)), shoud_be_same);
    STATIC_ASSERT((is_same<remove_all_extents<ClassType[2]>::type, ClassType>::value == (true)), shoud_be_same);
    STATIC_ASSERT((is_same<remove_all_extents<ClassType[2][3]>::type, ClassType>::value == (true)), shoud_be_same);
    STATIC_ASSERT((is_same<remove_all_extents<const ClassType[2][3]>::type, const ClassType>::value == (true)), shoud_be_same);
    return 0;
}
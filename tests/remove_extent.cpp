#include "../stdex/include/core.h"
#include "../stdex/include/type_traits.hpp"

struct ClassType {};

int main(void)
{
    using namespace stdex;
    
    // remove_extent
    STATIC_ASSERT((is_same<remove_extent<int>::type, int>::value == (true)), shoud_be_same);
    STATIC_ASSERT((is_same<remove_extent<const int>::type, const int>::value == (true)), shoud_be_same);
    STATIC_ASSERT((is_same<remove_extent<int[2]>::type, int>::value == (true)), shoud_be_same);
    STATIC_ASSERT((is_same<remove_extent<int[2][3]>::type, int[3]>::value == (true)), shoud_be_same);
    STATIC_ASSERT((is_same<remove_extent<const int[2]>::type, const int>::value == (true)), shoud_be_same);
    STATIC_ASSERT((is_same<remove_extent<ClassType>::type, ClassType>::value == (true)), shoud_be_same);
    STATIC_ASSERT((is_same<remove_extent<ClassType[2]>::type, ClassType>::value == (true)), shoud_be_same);
    STATIC_ASSERT((is_same<remove_extent<ClassType[2][3]>::type, ClassType[3]>::value == (true)), shoud_be_same);
    STATIC_ASSERT((is_same<remove_extent<const ClassType[2]>::type, const ClassType>::value == (true)), shoud_be_same);
    return 0;
}

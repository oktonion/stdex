#include "../stdex/include/core.h"
#include "../stdex/include/type_traits.hpp"

struct ClassType {};

int main(void)
{
    using namespace stdex;
    
    // extent
    STATIC_ASSERT((extent<int, 0>::value == (0)), extent_check);
    STATIC_ASSERT((extent<int[2], 0>::value == (2)), extent_check);
    STATIC_ASSERT((extent<int[2][4], 0>::value == (2)), extent_check);
    STATIC_ASSERT((extent<int [][4], 0>::value == (0)), extent_check);
    STATIC_ASSERT((extent<int, 1>::value == 0), extent_check);
    STATIC_ASSERT((extent<int[2], 1>::value == 0), extent_check);
    STATIC_ASSERT((extent<int[2][4], 1>::value == 4), extent_check);
    //STATIC_ASSERT((extent<int [][4], 1>::value == 4), extent_check);
    STATIC_ASSERT((extent<int[10][4][6][8][12][2], 4>::value == 12), extent_check);
    STATIC_ASSERT((extent<ClassType, 0>::value == (0)), extent_check);
    STATIC_ASSERT((extent<ClassType[2], 0>::value == (2)), extent_check);
    STATIC_ASSERT((extent<ClassType[2][4], 0>::value == (2)), extent_check);
    STATIC_ASSERT((extent<ClassType [][4], 0>::value == (0)), extent_check);
    STATIC_ASSERT((extent<ClassType, 1>::value == 0), extent_check);
    STATIC_ASSERT((extent<ClassType[2], 1>::value == 0), extent_check);
    STATIC_ASSERT((extent<ClassType[2][4], 1>::value == 4), extent_check);
    //STATIC_ASSERT((extent<ClassType [][4], 1>::value == 4), extent_check);
    STATIC_ASSERT((extent<ClassType[10][4][6][8][12][2], 4>::value == 12), extent_check);
    return 0;
}

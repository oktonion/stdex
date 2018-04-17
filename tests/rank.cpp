#include "../stdex/include/core.h"
#include "../stdex/include/type_traits.hpp"

struct ClassType {};

int main(void)
{
    using namespace stdex;
    
    // rank
    STATIC_ASSERT(rank<int>::value == (0), rank_check);
    STATIC_ASSERT(rank<int[2]>::value == (1), rank_check);
    //STATIC_ASSERT(rank<int [][4]>::value == (2), rank_check);
    STATIC_ASSERT(rank<int[2][2][4][4][6][6]>::value == (6), rank_check);
    STATIC_ASSERT(rank<ClassType>::value == (0), rank_check);
    STATIC_ASSERT(rank<ClassType[2]>::value == (1), rank_check);
    //STATIC_ASSERT(rank<ClassType [][4]>::value == (2), rank_check);
    STATIC_ASSERT(rank<ClassType[2][2][4][4][6][6]>::value == (6), rank_check);
    return 0;
}
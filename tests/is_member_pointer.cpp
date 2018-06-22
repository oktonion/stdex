#include "../stdex/include/core.h"
#include "../stdex/include/type_traits.hpp"

struct ClassType
{
};

int main(void)
{
    using namespace stdex;

    // is_member_pointer
    {
        typedef int(ClassType::*member1_t);
        typedef const int(ClassType::*member2_t);
        typedef ClassType(ClassType::*member3_t);
        typedef int (ClassType::*member4_t)(int);
        typedef int (ClassType::*member5_t)(int) const;
        typedef int (ClassType::*member6_t)(float, ...);
        typedef ClassType (ClassType::*member7_t)(ClassType);
        typedef float (ClassType::*member8_t)(int, float, int[], int &);

        STATIC_ASSERT(is_member_pointer<member1_t>::value == (true), should_be_member_pointer);
        STATIC_ASSERT(is_member_pointer<member2_t>::value == (true), should_be_member_pointer);
        STATIC_ASSERT(is_member_pointer<member3_t>::value == (true), should_be_member_pointer);

        STATIC_ASSERT(is_member_pointer<member4_t>::value == (true), should_be_member_pointer);
        STATIC_ASSERT(is_member_pointer<member5_t>::value == (true), should_be_member_pointer);
        STATIC_ASSERT(is_member_pointer<member6_t>::value == (true), should_be_member_pointer);
        STATIC_ASSERT(is_member_pointer<member7_t>::value == (true), should_be_member_pointer);
        STATIC_ASSERT(is_member_pointer<member8_t>::value == (true), should_be_member_pointer);
    }

    // Sanity check.
    STATIC_ASSERT(is_member_pointer<ClassType>::value == (false), can_not_be_member_pointer);
    return 0;
}

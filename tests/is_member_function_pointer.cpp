#include "../stdex/include/core.h"
#include "../stdex/include/type_traits.hpp"

struct ClassType
{
};

int main(void)
{
    using namespace stdex;

    // Positive tests.
    {
        typedef int (ClassType::*mfunc1)();
        typedef int (ClassType::*mfunc2)(int) const;
        typedef int (ClassType::*mfunc3)(float, ...);
        typedef ClassType (ClassType::*mfunc4)(ClassType);
        typedef float (ClassType::*mfunc5)(int, float, int[], int &);

        STATIC_ASSERT(is_member_function_pointer<mfunc1>::value == (true), should_be_a_member_function_pointer);
        STATIC_ASSERT(is_member_function_pointer<mfunc2>::value == (true), should_be_a_member_function_pointer);
        STATIC_ASSERT(is_member_function_pointer<mfunc3>::value == (true), should_be_a_member_function_pointer);
        STATIC_ASSERT(is_member_function_pointer<mfunc4>::value == (true), should_be_a_member_function_pointer);
        STATIC_ASSERT(is_member_function_pointer<mfunc5>::value == (true), should_be_a_member_function_pointer);
    }

    // Negative tests.
    {
        typedef int(ClassType::*mobj1);
        typedef const int(ClassType::*mobj2);
        typedef ClassType(ClassType::*mobj3);
        typedef float &ref_type;
        typedef int simple_type;
        typedef long *ptr_type;

        STATIC_ASSERT(is_member_function_pointer<mobj1>::value == (false), can_not_be_a_member_function_pointer);
        STATIC_ASSERT(is_member_function_pointer<mobj2>::value == (false), can_not_be_a_member_function_pointer);
        STATIC_ASSERT(is_member_function_pointer<mobj3>::value == (false), can_not_be_a_member_function_pointer);
        STATIC_ASSERT(is_member_function_pointer<ref_type>::value == (false), can_not_be_a_member_function_pointer);
        STATIC_ASSERT(is_member_function_pointer<simple_type>::value == (false), can_not_be_a_member_function_pointer);
        STATIC_ASSERT(is_member_function_pointer<ptr_type>::value == (false), can_not_be_a_member_function_pointer);
    }

    // Sanity check.
    STATIC_ASSERT(is_member_function_pointer<ClassType>::value == (false), can_not_be_a_member_function_pointer);
    return 0;
}

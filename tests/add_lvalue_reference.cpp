#include "../stdex/include/core.h"
#include "../stdex/include/type_traits.hpp"

struct ClassType
{
};

int main(void)
{
    using namespace stdex;

    // add_lvalue_reference
    STATIC_ASSERT((is_same<add_lvalue_reference<int>::type, int &>::value == (true)), should_be_same);
    STATIC_ASSERT((is_same<add_lvalue_reference<int &>::type, int &>::value == (true)), should_be_same);
    STATIC_ASSERT((is_same<add_lvalue_reference<const int &>::type, const int &>::value == (true)), should_be_same);
    STATIC_ASSERT((is_same<add_lvalue_reference<const int>::type, const int &>::value == (true)), should_be_same);
    STATIC_ASSERT((is_same<add_lvalue_reference<int *>::type, int *&>::value == (true)), should_be_same);
    STATIC_ASSERT((is_same<add_lvalue_reference<ClassType &>::type, ClassType &>::value == (true)), should_be_same);
    STATIC_ASSERT((is_same<add_lvalue_reference<ClassType>::type, ClassType &>::value == (true)), should_be_same);
    STATIC_ASSERT((is_same<add_lvalue_reference<void>::type, void>::value == (true)), should_be_same);
    STATIC_ASSERT((is_same<add_lvalue_reference<const void>::type, const void>::value == (true)), should_be_same);
    return 0;
}

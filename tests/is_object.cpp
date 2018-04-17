#include "../stdex/include/core.h"
#include "../stdex/include/type_traits.hpp"

struct ClassType {};
typedef void foo0_t();
typedef void foo1_t(int);
typedef float foo2_t(int&, double);
typedef void foo3_t(int&, bool, int, int);
typedef int& foo4_t(int, bool, int*, int [], int, int, int, int, int);
typedef void foo5_t(int, bool, int*, int [], ...);

int main(void)
{
    using namespace stdex;
    
    // is_object
    STATIC_ASSERT(is_object<foo0_t>::value == (false), can_not_be_object);
    STATIC_ASSERT(is_object<foo1_t>::value == (false), can_not_be_object);
    STATIC_ASSERT(is_object<foo2_t>::value == (false), can_not_be_object);
    STATIC_ASSERT(is_object<foo3_t>::value == (false), can_not_be_object);
    STATIC_ASSERT(is_object<foo4_t>::value == (false), can_not_be_object);
    STATIC_ASSERT(is_object<foo5_t>::value == (false), can_not_be_object);
    STATIC_ASSERT(is_object<int&>::value == (false), can_not_be_object);
    STATIC_ASSERT(is_object<ClassType&>::value == (false), can_not_be_object);
    STATIC_ASSERT(is_object<void>::value == (false), can_not_be_object);
    STATIC_ASSERT(is_object<const void>::value == (false), can_not_be_object);

    // Sanity check.
    STATIC_ASSERT(is_object<ClassType>::value == (true), should_be_object);

    return 0;
}
#include "../stdex/include/core.h"
#include "../stdex/include/type_traits.hpp"

struct ClassType {};

typedef void foo1_t(int);
typedef float foo2_t(int&, double);

int main(void)
{
    using namespace stdex;
    
    // add_const
    STATIC_ASSERT((is_same<add_const<int>::type, const int>::value == (true)), should_be_same);
    STATIC_ASSERT((is_same<add_const<volatile int>::type, const volatile int>::value == (true)), should_be_same);
    STATIC_ASSERT((is_same<add_const<int*>::type, int* const>::value == (true)), should_be_same);
    STATIC_ASSERT((is_same<add_const<int&>::type, int&>::value == (true)), should_be_same);
    STATIC_ASSERT((is_same<add_const<foo1_t>::type, foo1_t>::value == (true)), should_be_same);
    STATIC_ASSERT((is_same<add_const<const int>::type, const int>::value == (true)), should_be_same);
    STATIC_ASSERT((is_same<add_const<ClassType>::type, const ClassType>::value == (true)), should_be_same);
    STATIC_ASSERT((is_same<add_const<volatile ClassType>::type, const volatile ClassType>::value == (true)), should_be_same);
    STATIC_ASSERT((is_same<add_const<ClassType*>::type, ClassType* const>::value == (true)), should_be_same);
    STATIC_ASSERT((is_same<add_const<ClassType&>::type, ClassType&>::value == (true)), should_be_same);
    STATIC_ASSERT((is_same<add_const<foo2_t>::type, foo2_t>::value == (true)), should_be_same);
    STATIC_ASSERT((is_same<add_const<const ClassType>::type, const ClassType>::value == (true)), should_be_same);
    return 0;
}

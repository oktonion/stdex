#include "../stdex/include/core.h"
#include "../stdex/include/type_traits.hpp"

struct ClassType {};

typedef void foo1_t(int);
typedef float foo2_t(int&, double);

int main(void)
{
    using namespace stdex;
    
    // add_cv
    STATIC_ASSERT((is_same<add_cv<int>::type, const volatile int>::value == (true)), should_be_same);
    STATIC_ASSERT((is_same<add_cv<const int>::type, const volatile int>::value == (true)), should_be_same);
    STATIC_ASSERT((is_same<add_cv<int*>::type, int* const volatile>::value == (true)), should_be_same);
    STATIC_ASSERT((is_same<add_cv<int&>::type, int&>::value == (true)), should_be_same);
    STATIC_ASSERT((is_same<add_cv<foo1_t>::type, foo1_t>::value == (true)), should_be_same);
    STATIC_ASSERT((is_same<add_cv<const volatile int>::type, const volatile int>::value == (true)), should_be_same);
    STATIC_ASSERT((is_same<add_cv<ClassType>::type, const volatile ClassType>::value == (true)), should_be_same);
    STATIC_ASSERT((is_same<add_cv<volatile ClassType>::type, const volatile ClassType>::value == (true)), should_be_same);
    STATIC_ASSERT((is_same<add_cv<ClassType*>::type, ClassType* const volatile>::value == (true)), should_be_same);
    STATIC_ASSERT((is_same<add_cv<ClassType&>::type, ClassType&>::value == (true)), should_be_same);
    STATIC_ASSERT((is_same<add_cv<foo2_t>::type, foo2_t>::value == (true)), should_be_same);
    STATIC_ASSERT((is_same<add_cv<const volatile ClassType>::type, const volatile ClassType>::value == (true)), should_be_same);
    return 0;
}
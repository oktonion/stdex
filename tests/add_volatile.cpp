#include "../stdex/include/core.h"
#include "../stdex/include/type_traits.hpp"

struct ClassType {};

typedef void foo1_t(int);
typedef float foo2_t(int&, double);

int main(void)
{
    using namespace stdex;
    
    // add_volatile
    STATIC_ASSERT((is_same<add_volatile<int>::type, volatile int>::value == (true)), should_be_same);
    STATIC_ASSERT((is_same<add_volatile<const int>::type, const volatile int>::value == (true)), should_be_same);
    STATIC_ASSERT((is_same<add_volatile<int*>::type, int* volatile>::value == (true)), should_be_same);
    STATIC_ASSERT((is_same<add_volatile<int&>::type, int&>::value == (true)), should_be_same);
    STATIC_ASSERT((is_same<add_volatile<foo1_t>::type, foo1_t>::value == (true)), should_be_same);
    STATIC_ASSERT((is_same<add_volatile<volatile int>::type, volatile int>::value == (true)), should_be_same);
    STATIC_ASSERT((is_same<add_volatile<ClassType>::type, volatile ClassType>::value == (true)), should_be_same);
    STATIC_ASSERT((is_same<add_volatile<const ClassType>::type, const volatile ClassType>::value == (true)), should_be_same);
    STATIC_ASSERT((is_same<add_volatile<ClassType*>::type, ClassType* volatile>::value == (true)), should_be_same);
    STATIC_ASSERT((is_same<add_volatile<ClassType&>::type, ClassType&>::value == (true)), should_be_same);
    STATIC_ASSERT((is_same<add_volatile<foo2_t>::type, foo2_t>::value == (true)), should_be_same);
    STATIC_ASSERT((is_same<add_volatile<volatile ClassType>::type, volatile ClassType>::value == (true)), should_be_same);
    return 0;
}

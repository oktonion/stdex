#include "../stdex/include/core.h"
#include "../stdex/include/type_traits.hpp"

struct ClassType {};
typedef void foo0_t();
typedef void foo1_t(int);

int main(void)
{
    using namespace stdex;
    
    // add_pointer
    STATIC_ASSERT((is_same<add_pointer<int>::type, int*>::value == (true)), should_be_same);
    STATIC_ASSERT((is_same<add_pointer<int*>::type, int**>::value == (true)), should_be_same);
    STATIC_ASSERT((is_same<add_pointer<const int>::type, const int*>::value == (true)), should_be_same);
    STATIC_ASSERT((is_same<add_pointer<const volatile int>::type, const volatile int*>::value == (true)), should_be_same);
    STATIC_ASSERT((is_same<add_pointer<int&>::type, int*>::value == (true)), should_be_same);
    STATIC_ASSERT((is_same<add_pointer<ClassType*>::type, ClassType**>::value == (true)), should_be_same);
    STATIC_ASSERT((is_same<add_pointer<ClassType>::type, ClassType*>::value == (true)), should_be_same);
    STATIC_ASSERT((is_same<add_pointer<foo0_t>::type, foo0_t*>::value == (true)), should_be_same);
    STATIC_ASSERT((is_same<add_pointer<foo1_t>::type, foo1_t*>::value == (true)), should_be_same);
    return 0;
}

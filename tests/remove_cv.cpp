#include "../stdex/include/core.h"
#include "../stdex/include/type_traits.hpp"

struct ClassType
{
};

int main(void)
{
    using namespace stdex;

    // remove_cv
    STATIC_ASSERT((is_same<remove_cv<const volatile int>::type, int>::value == (true)), should_be_same);
    STATIC_ASSERT((is_same<remove_cv<const volatile int *>::type, const volatile int *>::value == (true)), should_be_same);
    STATIC_ASSERT((is_same<remove_cv<const volatile ClassType>::type, ClassType>::value == (true)), should_be_same);
    STATIC_ASSERT((is_same<remove_cv<const volatile ClassType *>::type, const volatile ClassType *>::value == (true)), should_be_same);
    return 0;
}

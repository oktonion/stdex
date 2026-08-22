#include "../stdex/include/core.h"
#include "../stdex/include/type_traits.hpp"

struct ClassType {};

int main(void)
{
    using namespace stdex;
    
    // is_same
    // Positive tests.
    STATIC_ASSERT((is_same<int, int>::value == (true)), should_be_same);
    STATIC_ASSERT((is_same<const int, const int>::value == (true)), should_be_same);
    STATIC_ASSERT((is_same<int&, int&>::value == (true)), should_be_same);
    STATIC_ASSERT((is_same<ClassType, ClassType>::value == (true)), should_be_same);

    // Negative tests.
    STATIC_ASSERT((is_same<void, int>::value == (false)), can_not_be_same);
    STATIC_ASSERT((is_same<int, const int>::value == (false)), can_not_be_same);
    STATIC_ASSERT((is_same<int, int&>::value == (false)), can_not_be_same);
    STATIC_ASSERT((is_same<int, ClassType>::value == (false)), can_not_be_same);
    return 0;
}

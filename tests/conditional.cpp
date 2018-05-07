#include "../stdex/include/core.h"
#include "../stdex/include/type_traits.hpp"

int main(void)
{
    using namespace stdex;
    
    // conditional
    // Positive tests.
    {
        typedef conditional<true, char, int>::type     test1_type;
        STATIC_ASSERT((is_same<test1_type, char>::value == (true)), shoud_be_same);

        typedef conditional<false, char, int>::type     test2_type;
        STATIC_ASSERT((is_same<test2_type, int>::value == (true)), shoud_be_same);
    }
    return 0;
}

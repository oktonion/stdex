#include "../stdex/include/core.h"
#include "../stdex/include/type_traits.hpp"

int main(void)
{
    using namespace stdex;
    
    // make_unsigned
    // Negative  tests.
    {
        typedef make_unsigned<bool>::type     	test1_type;
    }

    return 0;
}

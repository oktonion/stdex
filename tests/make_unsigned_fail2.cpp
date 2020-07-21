#include "../stdex/include/core.h"
#include "../stdex/include/type_traits.hpp"

int main(void)
{
    using namespace stdex;
    
    // make_unsigned
    // Negative  tests.
    {

        typedef make_unsigned<int[4]>::type     test3_type;

    }
    
    return 0;
}

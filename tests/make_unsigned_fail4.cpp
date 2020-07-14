#include "../stdex/include/core.h"
#include "../stdex/include/type_traits.hpp"

int main(void)
{
    using namespace stdex;
    
    // make_unsigned
    // Negative  tests.
    {

        typedef make_unsigned<float>::type test5_type;
    }
    
    return 0;
}

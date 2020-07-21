#include "../stdex/include/core.h"
#include "../stdex/include/type_traits.hpp"

int main(void)
{
    using namespace stdex;
    
    // make_unsigned
    // Negative  tests.
    {

        typedef void (fn_type)();
        typedef make_unsigned<fn_type>::type  	test4_type;

    }
    
    return 0;
}

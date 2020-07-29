#include "../stdex/include/core.h"
#include "../stdex/include/type_traits.hpp"

int main(void)
{
    using namespace stdex;

    // Negative  tests.
    {
        typedef void (fn_type)();
        typedef make_signed<fn_type>::type  	test4_type;
    }
    return 0;
}

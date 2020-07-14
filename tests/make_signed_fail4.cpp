#include "../stdex/include/core.h"
#include "../stdex/include/type_traits.hpp"

int main(void)
{
    using namespace stdex;

    // Negative  tests.
    {
        typedef make_signed<float>::type test5_type;

    }
    return 0;
}

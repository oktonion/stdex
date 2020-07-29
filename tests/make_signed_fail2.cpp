#include "../stdex/include/core.h"
#include "../stdex/include/type_traits.hpp"

int main(void)
{
    using namespace stdex;

    // Negative  tests.
    {
        typedef make_signed<int[4]>::type     test3_type;
    }
    return 0;
}

#include "../stdex/include/core.h"
#include "../stdex/include/type_traits.hpp"

int main(void)
{
    using namespace stdex;
    
    // enable_if
    // Positive tests.
    {
        typedef enable_if<true, int>::type test1_type;
    }
    return 0;
}
#include "../stdex/include/core.h"
#include "../stdex/include/ratio.hpp"
#include "../stdex/include/cstdint.hpp"

#define VERIFY(cond) STATIC_ASSERT((cond), check)

int main(void)
{
    using namespace stdex;
        {
            // Negative tests:
            volatile ratio<STDEX_INTMAX_MIN, 1> r3; //??
        }

    return 0;
}

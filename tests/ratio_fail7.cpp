#include "../stdex/include/core.h"
#include "../stdex/include/ratio.hpp"
#include "../stdex/include/cstdint.hpp"

#define VERIFY(cond) STATIC_ASSERT((cond), check)

int main(void)
{
    using namespace stdex;
        {
            // Negative tests:
            volatile ratio_multiply<ratio<-STDEX_INTMAX_MAX, 2>, ratio<3, 2> >::type r7; // error
        }

    return 0;
}

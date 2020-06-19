#include "../stdex/include/core.h"
#include "../stdex/include/ratio.hpp"
#include "../stdex/include/cstdint.hpp"

#define VERIFY(cond) STATIC_ASSERT((cond), check)

static const stdex::intmax_t M = STDEX_INTMAX_MAX;

int main(void)
{
    using namespace stdex;
		{
			// Negative tests:
			volatile ratio_multiply<ratio<-M, 2>, ratio<3, 2> >::type r7; // error
		}

    return 0;
}

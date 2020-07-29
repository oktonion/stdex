#include "../stdex/include/core.h"
#include "../stdex/include/ratio.hpp"
#include "../stdex/include/cstdint.hpp"

#define VERIFY(cond) STATIC_ASSERT((cond), check)

int main(void)
{
    using namespace stdex;
		{
			// Negative tests:
			volatile ratio_add<ratio<STDEX_INTMAX_MAX, 1>, ratio<1> >::type r6; // error
		}

    return 0;
}

#include "../stdex/include/core.h"
#include "../stdex/include/ratio.hpp"
#include "../stdex/include/cstdint.hpp"

#define VERIFY(cond) STATIC_ASSERT((cond), check)

int main(void)
{
    using namespace stdex;
		{
			// Negative tests:
			volatile ratio<1, 0> r5; // error
		}

    return 0;
}

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
			volatile ratio<M, M> r1; //??
			volatile ratio<-M, M> r2; //??
			volatile ratio<M, 1> r3; //??
			volatile ratio<1, M> r4; //??
			volatile ratio<1, 0> r5; // error

			volatile ratio_add<ratio<M, 1>, ratio<1> >::type r6; // error
			volatile ratio_multiply<ratio<-M, 2>, ratio<3, 2> >::type r1; // error
			volatile ratio_multiply<ratio<M>, ratio<M> >::type r2; // error
		}

    return 0;
}

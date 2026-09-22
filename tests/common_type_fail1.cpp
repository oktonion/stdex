#include "../stdex/include/core.h"
#include "../stdex/include/type_traits.hpp"


struct A { };
struct B : private A { };


int main(void)
{
    using namespace stdex;
	typedef common_type<A, B>::type common_type_A_B; // ct-error: no 'type' member in 'struct stdex::common_type'
    return 0;
}

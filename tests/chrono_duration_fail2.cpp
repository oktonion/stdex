#include "../stdex/include/core.h"
#include "../stdex/include/chrono.hpp"

#include <iostream>
#define DYNAMIC_VERIFY(cond) if(!(cond)) {std::cout << "check condition \'" << #cond << "\' failed at line " << __LINE__ << std::endl; return -1;}



int main(void)
{
    using namespace stdex;
    using namespace stdex::chrono;

    duration<int, stdex::micro> d2(8);
    duration<int, stdex::milli> d2_copy(d2); // { dg-error "no matching|no type" }

    return 0;
}

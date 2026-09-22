#include "../stdex/include/core.h"
#include "../stdex/include/chrono.hpp"

#include <iostream>
#define DYNAMIC_VERIFY(cond) if(!(cond)) {std::cout << "check condition \'" << #cond << "\' failed at line " << __LINE__ << std::endl; return -1;}



int main(void)
{
    using namespace stdex;
    using namespace stdex::chrono;

    {
        duration<double> d(3.5);
        duration<int> i = d;
    }

    return 0;
}

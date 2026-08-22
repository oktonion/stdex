#include "../stdex/include/core.h"
#include "../stdex/include/chrono.hpp"

#include <iostream>
#define DYNAMIC_VERIFY(cond) if(!(cond)) {std::cout << "check condition \'" << #cond << "\' failed at line " << __LINE__ << std::endl; return -1;}



int main(void)
{
    // Check if period is positive
    typedef int rep_type;
    typedef stdex::ratio<-1> period_type;
    typedef stdex::chrono::duration<rep_type, period_type> test_type;
    test_type d;  // { dg-error "required from here" }

    return 0;
}

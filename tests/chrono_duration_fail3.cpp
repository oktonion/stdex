#include "../stdex/include/core.h"
#include "../stdex/include/chrono.hpp"

#include <iostream>
#define DYNAMIC_VERIFY(cond) if(!(cond)) {std::cout << "check condition \'" << #cond << "\' failed at line " << __LINE__ << std::endl; return -1;}



int main(void)
{
    // Check if rep is a duration type
    typedef stdex::chrono::duration<int> rep_type;
    typedef stdex::chrono::duration<rep_type> test_type;
    test_type d; // { dg-error "required from here" }

    return 0;
}
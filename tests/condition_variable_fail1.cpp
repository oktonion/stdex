#include "../stdex/include/core.h"
#include "../stdex/include/condition_variable.hpp"
#include "../stdex/include/mutex.hpp"
#include "../stdex/include/system_error.hpp"
#include "../stdex/include/thread.hpp"

#include <iostream>
#define DYNAMIC_VERIFY(cond) if(!(cond)) {std::cout << "check condition \'" << #cond << "\' failed at line " << __LINE__ << std::endl; return __LINE__;}
#define RUN_TEST(test) {std::cout << #test << std::endl; int line = test(); if(line != 0) {return line;}}

int main(void)
{
    using namespace stdex;
    
    // condition_variable
    {
        condition_variable c1;
        condition_variable c2;
        c1 = c2; // dg-error "deleted"
    }

    return 0;
}

#include "../stdex/include/core.h"
#include "../stdex/include/thread.hpp"
#include "../stdex/include/system_error.hpp"

#include <cstdlib>
#include <iostream>
#include <ctime>

#if defined(_STDEX_NATIVE_CPP11_SUPPORT) || defined(__MACH__)
#include <thread>
#include <system_error>
#endif

#define DYNAMIC_VERIFY(cond) if(!(cond)) {std::cout << "check condition \'" << #cond << "\' failed at line " << __LINE__ << std::endl; return __LINE__;}
#define RUN_TEST(test) {std::cout << #test << std::endl; int line = test(); if(line != 0) {std::cout << "failed at line " << line << std::endl; return line;}}

int main(void)
{
    using namespace stdex;
    
        {
            typedef thread test_type;
            test_type t1;
            test_type t2;
            t1 = t2;
        }

    return 0;
}

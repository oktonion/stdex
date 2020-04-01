#include "../stdex/include/core.h"
#include "../stdex/include/system_error.hpp"
#include "../stdex/include/mutex.hpp"
#include "../stdex/include/thread.hpp"

#include <iostream>
#include <string>

#define VERIFY(cond) STATIC_ASSERT((cond), check)
#define DYNAMIC_VERIFY(cond) if(!(cond)) {std::cout << "check condition \'" << #cond << "\' failed at line " << __LINE__ << std::endl; return -1;}
#define RUN_TEST(test) {std::cout << #test << std::endl; int line = test(); if(line != 0) {std::cout << "failed at line " << line << std::endl; return line;}}


int test1()
{
    stdex::mutex m;
    stdex::unique_lock<stdex::mutex> 
        lock1(m),
        lock2(stdex::move(lock1));
    
    DYNAMIC_VERIFY(lock2.mutex()->native_handle == m.native_handle);
    DYNAMIC_VERIFY(!lock1.mutex());
    
    return 0;
}

int main()
{
    RUN_TEST(test1);

    return 0;
}


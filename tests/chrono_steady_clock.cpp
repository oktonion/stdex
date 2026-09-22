#include "../stdex/include/core.h"
#include "../stdex/include/chrono.hpp"

#include <ctime>

#include <iostream>
#define DYNAMIC_VERIFY(cond) if(!(cond)) {std::cout << "check condition \'" << #cond << "\' failed at line " << __LINE__ << std::endl; return __LINE__;}
#define RUN_TEST(test) {std::cout << #test << std::endl; int line = test(); if(line != 0) {std::cout << "failed at line " << line << std::endl; return line;}}

int consistency_test(void)
{
    typedef stdex::chrono::steady_clock C;
    STATIC_ASSERT((stdex::is_same<C::rep, C::duration::rep>::value), should_be_same);
    STATIC_ASSERT((stdex::is_same<C::period, C::duration::period>::value), should_be_same);
    STATIC_ASSERT((stdex::is_same<C::duration, C::time_point::duration>::value), should_be_same);
    DYNAMIC_VERIFY(C::is_steady);
    return 0;
}

int now_test()
{
    typedef stdex::chrono::steady_clock C;
    C::time_point t1 = C::now();
    C::time_point t2 = C::now();
    DYNAMIC_VERIFY(t2 >= t1);
    // make sure t2 didn't wrap around
    DYNAMIC_VERIFY(t2 > stdex::chrono::time_point<C>());

    return 0;
}

int main(void)
{
    RUN_TEST(consistency_test);
    RUN_TEST(now_test);
    return 0;
}
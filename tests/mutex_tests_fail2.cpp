#include "../stdex/include/mutex.hpp"

#include <iostream>
#include <string>

#define VERIFY(cond) STATIC_ASSERT((cond), check)
#define DYNAMIC_VERIFY(cond) if(!(cond)) {std::cout << "check condition \'" << #cond << "\' failed at line " << __LINE__ << std::endl; return -1;}
#define RUN_TEST(test) {std::cout << #test << std::endl; int line = test(); if(line != 0) {std::cout << "failed at line " << line << std::endl; return line;}}

namespace mutex_tests
{
    typedef stdex::mutex mutex_type;
    
    int test1()
    {
        using namespace stdex;

        {
            // assign
            mutex_type m1;
            mutex_type m2(m1);        // { dg-error "deleted" }
        }

        return 0;
    }
}


int main(void)
{
    using namespace stdex;
    using namespace mutex_tests;
    
    RUN_TEST(test1);

    return 0;
}

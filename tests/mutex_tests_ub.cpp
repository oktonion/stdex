#include "../stdex/include/core.h"
#include "../stdex/include/system_error.hpp"
#include "../stdex/include/mutex.hpp"
#include "../stdex/include/thread.hpp"

#include <iostream>
#include <string>

#define VERIFY(cond) STATIC_ASSERT((cond), check)
#define DYNAMIC_VERIFY(cond) if(!(cond)) {std::cout << "check condition \'" << #cond << "\' failed at line " << __LINE__ << std::endl; return -1;}
#define RUN_TEST(test) {std::cout << #test << std::endl; int line = test(); if(line != 0) {std::cout << "failed at line " << line << std::endl; return line;}}
#define DYNAMIC_VERIFY_FAIL {std::cout << "check condition " << "failed at line " << __LINE__ << std::endl; return -1;}

namespace mutex_tests
{
    typedef stdex::mutex mutex_type;
    
    int unlock_test_ub()
    {
        using namespace stdex;
        
        try
        {
            // Unlock mutex that hasn't been locked.
            mutex_type mmm;
            mmm.unlock();
        }
        catch (const system_error&)
        {
            // POSIX == EPERM
            return 0; // may throw - not specified by standard
        }
        catch (...)
        {
            DYNAMIC_VERIFY_FAIL;
        }

        return 0; // may not - not specified by standard
    }
}


int main(void)
{
    using namespace stdex;
    using namespace mutex_tests;

    RUN_TEST(unlock_test_ub);

    return 0;
}
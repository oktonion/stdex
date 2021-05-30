#include "../stdex/include/core.h"
#include "../stdex/include/chrono.hpp"

#include <ctime>

#include <iostream>
#define DYNAMIC_VERIFY(cond) if(!(cond)) {std::cout << "check condition \'" << #cond << "\' failed at line " << __LINE__ << std::endl; return -1;}

int test1(void)
{
    using namespace stdex;
    using namespace stdex::chrono;

    typedef         
    stdex::detail::_safe_multiply<
        hours::period::num, stdex::ratio<1>::num 
    > check1;

    // system_clock
    {
        system_clock::time_point t1 = system_clock::now();
        std::time_t t1_tt = std::time(0);
        bool is_steady = system_clock::is_steady;
        (void)(&is_steady); // suppress unused warning
        stdex::time_t t2 = system_clock::to_time_t(t1);
        double sec_diff = stdex::difftime(t2, t1_tt);
        system_clock::time_point t3 = system_clock::from_time_t(t2);
        t3 = t3; // suppress unused warning
        stdex::time_t t4 = system_clock::to_time_t(t3);
        DYNAMIC_VERIFY(t4 == t2);

        microseconds mcs = duration_cast<microseconds>(t3 - t1);
        mcs = mcs; // suppress unused warning

        mcs = duration_cast<microseconds>(system_clock::now() - t1);
        mcs = mcs; // suppress unused warning

        {
            stdex::time_t 
                t1 = std::time(0),
                t2 = system_clock::to_time_t(system_clock::now());

            double sec_diff = stdex::difftime(t2, t1);
            int diff = int(sec_diff);

            std::cout << "delta: " << diff << std::endl;
            DYNAMIC_VERIFY((diff == 0 || diff == 1)); // we could afford 1 sec shift in time measurement
        }

        {
            stdex::time_t
                t1 = 0,
                t2 = system_clock::to_time_t(system_clock::time_point(0));

            double sec_diff = stdex::difftime(t2, t1);
            int diff = int(sec_diff);

            std::cout << "delta: " << diff << std::endl;
            DYNAMIC_VERIFY((diff == 0 || diff == 1)); // we could afford 1 sec shift in time measurement
        }
    }
    return 0;
}

static int res = test1();

int main(void)
{
    if(0 == res)
        return test1();
    return res;
}
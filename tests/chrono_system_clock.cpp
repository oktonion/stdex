#include "../stdex/include/core.h"
#include "../stdex/include/chrono.hpp"

#include <ctime>

#include <iostream>
#define DYNAMIC_VERIFY(cond) if(!(cond)) {std::cout << "check condition \'" << #cond << "\' failed at line " << __LINE__ << std::endl; return __LINE__;}
#define RUN_TEST(test) {std::cout << #test << std::endl; int line = test(); if(line != 0) {std::cout << "failed at line " << line << std::endl; return line;}}

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
        DYNAMIC_VERIFY(-1 != t2);
        DYNAMIC_VERIFY(-1 != t1_tt);
        double sec_diff = stdex::difftime(t2, t1_tt);
        system_clock::time_point t3 = system_clock::from_time_t(t2);
        t3 = t3; // suppress unused warning
        stdex::time_t t4 = system_clock::to_time_t(t3);
        std::cout << "t4 == " << t4 << ", t2 == " << t2 << " with diff " << sec_diff << std::endl;
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

int test2()
{
    using namespace stdex;
    using namespace stdex::chrono;
    system_clock::time_point t1 = system_clock::now();

    system_clock::time_point t2;

    for (std::size_t i = 0; i < std::size_t(std::size_t(-1) / std::size_t(2)); ++i)
    {
        t2 = system_clock::now();
        milliseconds diff = duration_cast<milliseconds>(t2 - t1);
        if (diff.count() > 1) break;
    }
    system_clock::duration diff1 = t1 - t2;
    system_clock::duration diff2 = t2 - t1;

    DYNAMIC_VERIFY((diff1.count() < 0 && diff2.count() > 0) || (diff2.count() < 0 && diff1.count() > 0));

    if (diff1.count() < 0)
    {
        DYNAMIC_VERIFY(duration_cast<milliseconds>(diff1).count() < 0);
        DYNAMIC_VERIFY(duration_cast<milliseconds>(diff2).count() > 0);
        DYNAMIC_VERIFY(duration_cast<milliseconds>(diff1).count() == (-duration_cast<milliseconds>(diff2).count()));
        DYNAMIC_VERIFY(diff1.count() == (-diff2.count()));
    }
    else
    {
        std::cout << "there was an OS time shift" << std::endl;
        DYNAMIC_VERIFY(duration_cast<milliseconds>(diff2).count() < 0);
        DYNAMIC_VERIFY(duration_cast<milliseconds>(diff1).count() > 0);
        DYNAMIC_VERIFY(duration_cast<milliseconds>(diff2).count() == (-duration_cast<milliseconds>(diff1).count()));
        DYNAMIC_VERIFY(diff2.count() == (-diff1.count()));
    }

    return 0;
}

int main(void)
{
    if(0 == res)
        RUN_TEST(test1)
    else
    {
        std::cout << "static test failed at line " << res << std::endl;
        return -1;
    }

    RUN_TEST(test2);
    return 0;
}
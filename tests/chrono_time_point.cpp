#include "../stdex/include/core.h"
#include "../stdex/include/chrono.hpp"

#include <iostream>
#define DYNAMIC_VERIFY(cond) if(!(cond)) {std::cout << "check condition \'" << #cond << "\' failed at line " << __LINE__ << std::endl; return -1;}

int main(void)
{
    using namespace stdex;
    using namespace stdex::chrono;

    // time_point
    {
        time_point<system_clock> t1;
        DYNAMIC_VERIFY(t1.time_since_epoch() == system_clock::duration::zero());

        time_point<steady_clock> t2;
        DYNAMIC_VERIFY(t2.time_since_epoch() == steady_clock::duration::zero());

        time_point<high_resolution_clock> t3;
        DYNAMIC_VERIFY(t3.time_since_epoch() == high_resolution_clock::duration::zero());
    }
    
    {
        time_point<system_clock> t1, t2;
        t1 += seconds(1);
        DYNAMIC_VERIFY(t2.time_since_epoch() + seconds(1) == t1.time_since_epoch());

        t1 -= seconds(1);
        DYNAMIC_VERIFY(t2.time_since_epoch() == t1.time_since_epoch());
    }

    {
        time_point<system_clock> t1;
        time_point<system_clock> t2(t1 + seconds(1));
        DYNAMIC_VERIFY(t2.time_since_epoch() == t1.time_since_epoch() + seconds(1));

        time_point<system_clock> t3(seconds(1) + t1);
        DYNAMIC_VERIFY(t3.time_since_epoch() == t1.time_since_epoch() + seconds(1));

        time_point<system_clock> t4(seconds(1));
        time_point<system_clock> t5(seconds(2));

        time_point<system_clock> t6(t5 - seconds(1));
        DYNAMIC_VERIFY(t6.time_since_epoch() == t4.time_since_epoch());

        time_point<system_clock> t7(t5 - t4);
        DYNAMIC_VERIFY(t7.time_since_epoch() == t4.time_since_epoch());
    }

    {
        time_point<system_clock> t1(seconds(1));
        time_point<system_clock> t2(seconds(1));
        time_point<system_clock> t3(seconds(2));

        DYNAMIC_VERIFY(t1 == t2);
        DYNAMIC_VERIFY(t1 != t3);
        DYNAMIC_VERIFY(t1 < t3);
        DYNAMIC_VERIFY(t1 <= t3);
        DYNAMIC_VERIFY(t1 <= t2);
        DYNAMIC_VERIFY(t3 > t1);
        DYNAMIC_VERIFY(t3 >= t1);
        DYNAMIC_VERIFY(t2 >= t1);
    }
    return 0;
}

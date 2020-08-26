#include "../stdex/include/core.h"
#include "../stdex/include/chrono.hpp"

#include <ctime>

#include <iostream>
#define DYNAMIC_VERIFY(cond) if(!(cond)) {std::cout << "check condition \'" << #cond << "\' failed at line " << __LINE__ << std::endl; return -1;}

int main(void)
{
    using namespace stdex;
    using namespace stdex::chrono;

    // system_clock
    {
        system_clock::time_point t1 = system_clock::now();
        bool is_steady = system_clock::is_steady;
        (void)(&is_steady); // suppress unused warning
        stdex::time_t t2 = system_clock::to_time_t(t1);
        system_clock::time_point t3 = system_clock::from_time_t(t2);
        t3 = t3; // suppress unused warning

        microseconds mcs = duration_cast<microseconds>(t3 - t1);
        mcs = mcs; // suppress unused warning

        mcs = duration_cast<microseconds>(system_clock::now() - t1);
        mcs = mcs; // suppress unused warning

        {
            stdex::time_t 
            t1 = std::time(0),
            t2 = system_clock::to_time_t(system_clock::now());

            DYNAMIC_VERIFY(t2 - t1 == 0);
        }
    }
    return 0;
}

#include "../stdex/include/core.h"
#include "../stdex/include/chrono.hpp"

int main(void)
{
    using namespace stdex;
    using namespace stdex::chrono;

    // system_clock
    {
        system_clock::time_point t1 = system_clock::now();
        bool is_steady = system_clock::is_steady;
        (void)(&is_steady); // suppress unused warning
        ::time_t t2 = system_clock::to_time_t(t1);
        system_clock::time_point t3 = system_clock::from_time_t(t2);
        t3 = t3; // suppress unused warning

        microseconds mcs = duration_cast<microseconds>(t3 - t1);
        mcs = mcs; // suppress unused warning

        mcs = duration_cast<microseconds>(system_clock::now() - t1);
        mcs = mcs; // suppress unused warning
    }
    return 0;
}

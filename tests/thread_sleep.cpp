#include "../stdex/include/core.h"
#include "../stdex/include/chrono.hpp"
#include "../stdex/include/thread.hpp"

#include <iostream>
#include <string>
#define DYNAMIC_VERIFY(cond) if(!(cond)) {std::cout << "check condition \'" << #cond << "\' failed at line " << __LINE__ << std::endl; return -1;}

int main(void)
{
    using namespace stdex;
    using namespace stdex::chrono;

    steady_clock::time_point begin_tp = steady_clock::now();
    this_thread::sleep_for(milliseconds(-1));
    steady_clock::time_point end_tp = steady_clock::now();

    DYNAMIC_VERIFY(duration_cast<milliseconds>(end_tp - begin_tp).count() < 1);

    begin_tp = steady_clock::now();
    this_thread::sleep_for(seconds(-1));
    end_tp = steady_clock::now();

    DYNAMIC_VERIFY(duration_cast<milliseconds>(end_tp - begin_tp).count() < 1);

    begin_tp = steady_clock::now();
    this_thread::sleep_for(seconds(1));
    end_tp = steady_clock::now();

    DYNAMIC_VERIFY(duration_cast<seconds>(end_tp - begin_tp).count() >= 1);
    return 0;
}

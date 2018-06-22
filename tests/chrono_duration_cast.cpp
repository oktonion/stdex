#include "../stdex/include/chrono.hpp"
#include "../stdex/include/core.h"

int main(void)
{
    using namespace stdex;
    using namespace stdex::chrono;

    // duration_cast
    {
        minutes mn1(6000);
        hours hr1 = duration_cast<hours>(mn1);
        hr1 = hr1;
    }
    return 0;
}

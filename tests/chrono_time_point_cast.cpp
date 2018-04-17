#include "../stdex/include/core.h"
#include "../stdex/include/chrono.hpp"

int main(void)
{
    using namespace stdex;
    using namespace stdex::chrono;

    // time_point_cast
    {
        typedef time_point<system_clock, hours> 	to_type;
        typedef time_point<system_clock, minutes> 	from_type;

        minutes m(6000);
        hours h(19);
        to_type tpm(h); // time_point object with minutes
        from_type tph(m); // time_point object with hours

        to_type res(time_point_cast<hours>(tpm));
    }
    return 0;
}
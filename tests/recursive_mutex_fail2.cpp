#include "../stdex/include/mutex.hpp"


int main(void)
{
    // assign
    typedef stdex::recursive_mutex mutex_type;
    mutex_type m1;
    mutex_type m2(m1);        // { dg-error "deleted" }

    return 0;
}

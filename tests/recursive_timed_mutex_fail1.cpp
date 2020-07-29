#include "../stdex/include/mutex.hpp"


int main(void)
{
    // assign
    typedef stdex::recursive_timed_mutex mutex_type;
    mutex_type m1;
    mutex_type m2;
    m1 = m2;			// { dg-error "deleted" }

    return 0;
}
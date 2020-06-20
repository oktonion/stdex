#include "../stdex/include/mutex.hpp"

struct clok
{
  // no clok::rep or clok::period defined
  typedef stdex::chrono::milliseconds duration;
  typedef stdex::chrono::time_point<clok> time_point;
  static const bool is_steady = false;
  static time_point now();
};


int main(void)
{
    {
        stdex::timed_mutex m;
        m.try_lock_until(clok::now()); // { dg-error "here" }
    }

    return 0;
}
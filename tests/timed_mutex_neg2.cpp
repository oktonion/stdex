#include "../stdex/include/mutex.hpp"

struct cloc
{
  typedef stdex::chrono::milliseconds duration;
  typedef duration::rep rep;
  typedef duration::period period;
  // cloc::time_point::duration should be the same as cloc::duration:
  typedef stdex::chrono::time_point<cloc, stdex::chrono::seconds> time_point;
  static const bool is_steady = false;
  static time_point now();
};

int main(void)
{
    {
        stdex::timed_mutex m;
        m.try_lock_until(cloc::now()); // { dg-error "here" }
    }

    return 0;
}
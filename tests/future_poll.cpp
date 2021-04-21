#include "../stdex/include/core.h"
#include "../stdex/include/future.hpp"
#include "../stdex/include/chrono.hpp"

#include <iostream>
#include <string>

#define VERIFY(cond) STATIC_ASSERT((cond), check)
#define DYNAMIC_VERIFY(cond) if(!(cond)) {std::cout << "check condition \'" << #cond << "\' failed at line " << __LINE__ << std::endl; return -1;}
#define RUN_TEST(test) {std::cout << #test << std::endl; int line = test(); if(line != 0) {std::cout << "failed at line " << line << std::endl; return line;}}
#define DYNAMIC_VERIFY_FAIL {std::cout << "check condition " << "failed at line " << __LINE__ << std::endl; return -1;}
#define DYNAMIC_VERIFY_ABORT(cond) if(!(cond)) {std::cout << "check condition \'" << #cond << "\' failed at line " << __LINE__ << std::endl; std::abort();}

int iterations = 200;

using namespace stdex;

template<typename Duration>
double
print(const char* desc, Duration dur)
{
  chrono::nanoseconds::rep ns = chrono::duration_cast<chrono::nanoseconds>(dur).count();
  double d = double(ns) / iterations;
  std::cout << desc << ": " << ns << "ns for " << iterations
    << " calls, avg " << d << "ns per call\n";
  return d;
}

int main()
{
  promise<int> p;
  future<int> f = 
      detail::future_detail::move(&p.get_future());

 start_over:
  chrono::high_resolution_clock::time_point start = chrono::high_resolution_clock::now();
  for(int i = 0; i < iterations; i++)
    f.wait_for(chrono::seconds(0));
  chrono::high_resolution_clock::time_point stop = chrono::high_resolution_clock::now();

  /* We've run too few iterations for the clock resolution.
     Attempt to calibrate it.  */
  if (start == stop)
    {
      /* After set_value, wait_for is faster, so use that for the
     calibration to avoid zero at low clock resultions.  */
      promise<int> pc;
      future<int> fc = 
          detail::future_detail::move(&pc.get_future());
      pc.set_value(1);

      /* Loop until the clock advances, so that start is right after a
     time increment.  */
      do
    start = chrono::high_resolution_clock::now();
      while (start == stop);
      int i = 0;
      /* Now until the clock advances again, so that stop is right
     after another time increment.  */
      do
    {
      fc.wait_for(chrono::seconds(0));
      stop = chrono::high_resolution_clock::now();
      i++;
    }
      while (start == stop);
      /* Go for some 10 cycles, but if we're already past that and
     still get into the calibration loop, double the iteration
     count and try again.  */
      if (iterations < i * 10)
    iterations = i * 10;
      else
    iterations *= 2;
      goto start_over;
    }

  double wait_for_0 = print("wait_for(0s)", stop - start);

  start = chrono::high_resolution_clock::now();
  for(int i = 0; i < iterations; i++)
    f.wait_until(chrono::system_clock::time_point::min());
  stop = chrono::high_resolution_clock::now();
  double wait_until_sys_min 
    = print("wait_until(system_clock minimum)", stop - start);

  start = chrono::high_resolution_clock::now();
  for(int i = 0; i < iterations; i++)
    f.wait_until(chrono::steady_clock::time_point::min());
  stop = chrono::high_resolution_clock::now();
  double wait_until_steady_min 
    = print("wait_until(steady_clock minimum)", stop - start);

  start = chrono::high_resolution_clock::now();
  for(int i = 0; i < iterations; i++)
    f.wait_until(chrono::system_clock::time_point());
  stop = chrono::high_resolution_clock::now();
  double wait_until_sys_epoch 
    = print("wait_until(system_clock epoch)", stop - start);

  start = chrono::high_resolution_clock::now();
  for(int i = 0; i < iterations; i++)
    f.wait_until(chrono::steady_clock::time_point());
  stop = chrono::high_resolution_clock::now();
  double wait_until_steady_epoch 
    = print("wait_until(steady_clock epoch", stop - start);

  (void)wait_until_steady_epoch;
  (void)wait_until_sys_epoch;

  p.set_value(1);

  start = chrono::high_resolution_clock::now();
  for(int i = 0; i < iterations; i++)
    f.wait_for(chrono::seconds(0));
  stop = chrono::high_resolution_clock::now();
  double ready = print("wait_for when ready", stop - start);

  // Polling before ready with wait_for(0s) should be almost as fast as
  // after the result is ready.
  DYNAMIC_VERIFY( wait_for_0 < (ready * 30) );

  // Polling before ready using wait_until(min) should not be terribly slow.
  DYNAMIC_VERIFY( wait_until_sys_min < (ready * 100) );
  DYNAMIC_VERIFY( wait_until_steady_min < (ready * 100) );

#if 0
  // Polling before ready using wait_until(epoch) should not be terribly slow.
  DYNAMIC_VERIFY( wait_until_sys_epoch < (ready * 100) );
  DYNAMIC_VERIFY( wait_until_steady_epoch < (ready * 100) );
#endif
    return 0;
}

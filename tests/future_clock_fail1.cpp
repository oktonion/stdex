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

struct clock
{
  typedef stdex::chrono::milliseconds duration;
  typedef duration::rep rep;
  typedef duration::period period;
  // clock::time_point::duration should be the same as clock::duration:
  typedef stdex::chrono::time_point<clock, stdex::chrono::seconds> time_point;
  static const bool is_steady = false;
  static time_point now();
};

int main()
{
  stdex::promise<void> p;
  stdex::future<void> f;
  p.get_future().swap(f);
  f.wait_until(clock::now()); // { dg-error "here" }

  return 0;
}

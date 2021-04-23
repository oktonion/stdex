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

int test01()
{
  stdex::future<int> f0;
  DYNAMIC_VERIFY( !f0.valid() );

  stdex::promise<int> p1;
  stdex::future<int> f1(p1.get_future());

  DYNAMIC_VERIFY( f1.valid() );

  p1.set_value(1);

  DYNAMIC_VERIFY( f1.valid() );

  f1.swap(f0);

  DYNAMIC_VERIFY( !f1.valid() );
  DYNAMIC_VERIFY( !f0.valid() );

  return 0;
}

int main()
{
  RUN_TEST(test01);
  
  return 0;
}

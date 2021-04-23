#include "../stdex/include/core.h"
#include "../stdex/include/future.hpp"
#include "../stdex/include/chrono.hpp"

#include <iostream>
#include <string>
#include <future>

#define VERIFY(cond) STATIC_ASSERT((cond), check)
#define DYNAMIC_VERIFY(cond) if(!(cond)) {std::cout << "check condition \'" << #cond << "\' failed at line " << __LINE__ << std::endl; return -1;}
#define RUN_TEST(test) {std::cout << #test << std::endl; int line = test(); if(line != 0) {std::cout << "failed at line " << line << std::endl; return line;}}
#define DYNAMIC_VERIFY_FAIL {std::cout << "check condition " << "failed at line " << __LINE__ << std::endl; return -1;}
#define DYNAMIC_VERIFY_ABORT(cond) if(!(cond)) {std::cout << "check condition \'" << #cond << "\' failed at line " << __LINE__ << std::endl; std::abort();}

int value = 99;

int test01()
{
  stdex::promise<int> p1;
  stdex::future<int> f1;

  DYNAMIC_VERIFY(f1.valid() == false);
  p1.get_future().swap(f1);

  DYNAMIC_VERIFY(f1.valid() == true);

  stdex::shared_future<int> f2 = f1.share();

  p1.set_value(value);
  DYNAMIC_VERIFY( f2.get() == value );

  return 0;
}

int test02()
{
  stdex::promise<int&> p1;
  stdex::future<int&> f1;

  DYNAMIC_VERIFY(f1.valid() == false);
  p1.get_future().swap(f1);

  DYNAMIC_VERIFY(f1.valid() == true);

  stdex::shared_future<int&> f2 = f1.share();

  p1.set_value(value);
  DYNAMIC_VERIFY( &f2.get() == &value );

  return 0;
}

int test03()
{
  using stdex::detail::future_detail::move;
  
  stdex::promise<void> p1;
  stdex::future<void> f1;

  DYNAMIC_VERIFY(f1.valid() == false);
  p1.get_future().swap(f1);

  DYNAMIC_VERIFY(f1.valid() == true);

  stdex::shared_future<void> f2 = f1.share();

  p1.set_value();
  f2.get();

  return 0;
}

int main()
{
  RUN_TEST(test01);
  RUN_TEST(test02);
  RUN_TEST(test03);
  
  return 0;
}

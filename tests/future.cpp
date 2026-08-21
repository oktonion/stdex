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
  stdex::promise<int> p;
  stdex::future<int> f;
  p.get_future().swap(f);
  p.set_value(0);
  f.get();
  try
  {
    f.get();
    DYNAMIC_VERIFY_FAIL
  }
  catch (const stdex::future_error& e)
  {
    DYNAMIC_VERIFY( e.code() == stdex::future_errc::no_state );
  }
  return 0;
}

int test02()
{
  stdex::promise<int&> p;
  stdex::future<int&> f;
  p.get_future().swap(f);
  int i = 0;
  p.set_value(i);
  f.get();
  try
  {
    f.get();
    DYNAMIC_VERIFY_FAIL
  }
  catch (const stdex::future_error& e)
  {
    DYNAMIC_VERIFY( e.code() == stdex::future_errc::no_state );
  }
  return 0;
}

int test03()
{
  stdex::promise<void> p;
  stdex::future<void> f;
  p.get_future().swap(f);
  p.set_value();
  f.get();
  try
  {
    f.get();
    DYNAMIC_VERIFY_FAIL
  }
  catch (const stdex::future_error& e)
  {
    DYNAMIC_VERIFY( e.code() == stdex::future_errc::no_state );
  }
  return 0;
}


int main()
{
  try
  {
    RUN_TEST(test01);
    RUN_TEST(test02);
    RUN_TEST(test03);
  }
  catch (const std::exception& e)
  {
      std::cout << "unexpected exception " << e.what() << std::endl;
      return -1;
  }
  catch (...)
  {
      std::cout << "unexpected exception " << std::endl;
      return -2;
  }
  return 0;
}

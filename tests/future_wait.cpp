#include "../stdex/include/core.h"
#include "../stdex/include/future.hpp"
#include "../stdex/include/chrono.hpp"
#include "../stdex/include/thread.hpp"

#include <iostream>
#include <string>

#define VERIFY(cond) STATIC_ASSERT((cond), check)
#define DYNAMIC_VERIFY(cond) if(!(cond)) {std::cout << "check condition \'" << #cond << "\' failed at line " << __LINE__ << std::endl; return -1;}
#define RUN_TEST(test) {std::cout << #test << std::endl; int line = test(); if(line != 0) {std::cout << "failed at line " << line << std::endl; return line;}}
#define DYNAMIC_VERIFY_FAIL {std::cout << "check condition " << "failed at line " << __LINE__ << std::endl; return -1;}
#define DYNAMIC_VERIFY_ABORT(cond) if(!(cond)) {std::cout << "check condition \'" << #cond << "\' failed at line " << __LINE__ << std::endl; std::abort();}

void fut_wait(stdex::future<void>* f)
{
  f->wait();
}

int test01()
{
  stdex::promise<void> p1;
  stdex::future<void> f1;

  p1.get_future().swap(f1);

  stdex::thread t1(fut_wait, &f1);

  p1.set_value();
  t1.join();

  return 0;
}

int test02()
{
  stdex::promise<int> p1;
  stdex::future<int> f1;

  p1.get_future().swap(f1);

  stdex::chrono::milliseconds delay(100);

  DYNAMIC_VERIFY( f1.wait_for(delay) == stdex::future_status::timeout );

  p1.set_value(1);

  stdex::chrono::system_clock::time_point before = stdex::chrono::system_clock::now();
  DYNAMIC_VERIFY( f1.wait_for(delay) == stdex::future_status::ready );
  DYNAMIC_VERIFY( stdex::chrono::system_clock::now() < (before + delay) );

  return 0;
}

stdex::chrono::system_clock::time_point make_time(int i)
{
  return stdex::chrono::system_clock::now() + stdex::chrono::milliseconds(i);
}

int test03()
{
  stdex::promise<int> p1;
  stdex::future<int> f1;

  p1.get_future().swap(f1);

  stdex::chrono::system_clock::time_point when = make_time(10);
  DYNAMIC_VERIFY( f1.wait_until(when) == stdex::future_status::timeout );
  DYNAMIC_VERIFY( stdex::chrono::system_clock::now() >= when );

  p1.set_value(1);

  when = make_time(100);
  DYNAMIC_VERIFY( f1.wait_until(when) == stdex::future_status::ready );
  DYNAMIC_VERIFY( stdex::chrono::system_clock::now() < when );

  return 0;
}

int main()
{
  RUN_TEST(test01);
  RUN_TEST(test02);
  RUN_TEST(test03);
  
  return 0;
}

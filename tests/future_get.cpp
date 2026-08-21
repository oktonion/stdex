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

int value = 99;

int test01()
{
  stdex::promise<int> p1;
  stdex::future<int> f1;

  p1.get_future().swap(f1);

  p1.set_value(value);
  DYNAMIC_VERIFY( f1.get() == value );
  DYNAMIC_VERIFY( !f1.valid() );

  return 0;
}

int test02()
{
  stdex::promise<int&> p1;
  stdex::future<int&> f1;

  p1.get_future().swap(f1);

  p1.set_value(value);
  DYNAMIC_VERIFY( &f1.get() == &value );
  DYNAMIC_VERIFY( !f1.valid() );

  return 0;
}

int test03()
{
  stdex::promise<void> p1;
  stdex::future<void> f1;

  p1.get_future().swap(f1);

  p1.set_value();
  f1.get();
  DYNAMIC_VERIFY( !f1.valid() );

  return 0;
}

struct my_exception:
    public std::exception
{
    int value() const { return ::value; }

    char const* what() const stdex_noexcept
    {
        return "Unknown exception";
    }
};

int test04()
{
  stdex::promise<int> p1;
  stdex::future<int> f1;

  p1.get_future().swap(f1);

  p1.set_exception(stdex::make_exception_ptr(my_exception()));
  try
  {
    (void) f1.get();
    DYNAMIC_VERIFY( false );
  }
  catch (const my_exception& e)
  {
    DYNAMIC_VERIFY( e.value() == value );
  }
  DYNAMIC_VERIFY( !f1.valid() );

  return 0;
}

int test05()
{
  stdex::promise<int&> p1;
  stdex::future<int&> f1;

  p1.get_future().swap(f1);

  p1.set_exception(stdex::make_exception_ptr(my_exception()));
  try
  {
    (void) f1.get();
    DYNAMIC_VERIFY( false );
  }
  catch (const my_exception& e)
  {
    DYNAMIC_VERIFY( e.value() == value );
  }
  DYNAMIC_VERIFY( !f1.valid() );

  return 0;
}

int test06()
{
  stdex::promise<void> p1;
  stdex::future<void> f1;

  p1.get_future().swap(f1);

  p1.set_exception(stdex::make_exception_ptr(my_exception()));
  try
  {
    f1.get();
    DYNAMIC_VERIFY( false );
  }
  catch (const my_exception& e)
  {
      DYNAMIC_VERIFY(e.value() == value);
  }
  DYNAMIC_VERIFY( !f1.valid() );

  return 0;
}

int main()
{
    try
    {
        RUN_TEST(test01);
        RUN_TEST(test02);
        RUN_TEST(test03);
        RUN_TEST(test04);
        RUN_TEST(test05);
        RUN_TEST(test06);
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

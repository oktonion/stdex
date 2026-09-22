#include "../stdex/include/core.h"
#include "../stdex/include/system_error.hpp"
#include "../stdex/include/string.hpp"

#include <iostream>

#define VERIFY(cond) STATIC_ASSERT((cond), check)
#define DYNAMIC_VERIFY(cond) if(!(cond)) {std::cout << "check condition \'" << #cond << "\' failed at line " << __LINE__ << std::endl; return -1;}
#define RUN_TEST(test) {std::cout << #test << std::endl; int line = test(); if(line != 0) {std::cout << "failed at line " << line << std::endl; return line;}}

struct test_category : public stdex::error_category
{
    test_category() stdex_noexcept {}
    virtual const char*
    name() const stdex_noexcept
    {
        const char* s = "stdex_test::test_category";
        return s;
    }

    virtual std::string
    message(int) const
    { return std::string("message to be determined"); }
    };

    struct test_derived_category : public test_category
    {
    virtual const char*
    name() const stdex_noexcept
    {
        const char* s = "stdex_test::test_derived_category";
        return s;
    }
};

std::string get_name(const stdex::error_category& ec)
{
  return ec.name();
}

int test1()
{
  test_category c1;
  test_derived_category c2;

  DYNAMIC_VERIFY(get_name(c1) == c1.name());
  DYNAMIC_VERIFY(get_name(c2) == c2.name());

  return 0;
}

int test2()
{
  test_category c1;
  test_derived_category c2;
  DYNAMIC_VERIFY( c1 == c1 );
  DYNAMIC_VERIFY( !(c1 == c2) );

  return 0;
}

int test3()
{
  test_category c1;
  DYNAMIC_VERIFY( !(c1 < c1) );
  test_derived_category c2;
  DYNAMIC_VERIFY( (c1 < c2) || (c2 < c1) );

  return 0;
}

int test4()
{
  test_category c1;
  test_derived_category c2;
  DYNAMIC_VERIFY( !(c1 != c1) );
  DYNAMIC_VERIFY( c1 != c2 );

  return 0;
}

int test5()
{
  stdex::error_category &c1 = *(new test_category());

  DYNAMIC_VERIFY( !(c1 != c1) );
  DYNAMIC_VERIFY( c1 == c1 );

  delete &c1;

  return 0;
}

int main(void)
{
    using namespace stdex;

    RUN_TEST(test1);
    RUN_TEST(test2);
    RUN_TEST(test3);
    RUN_TEST(test4);
    RUN_TEST(test5);

    return 0;
}

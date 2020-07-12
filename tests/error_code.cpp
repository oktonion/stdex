#include "../stdex/include/core.h"
#include "../stdex/include/system_error.hpp"
#include "../stdex/include/string.hpp"

#include <iostream>

#define VERIFY(cond) STATIC_ASSERT((cond), check)
#define DYNAMIC_VERIFY(cond) if(!(cond)) {std::cout << "check condition \'" << #cond << "\' failed at line " << __LINE__ << std::endl; return -1;}
#define RUN_TEST(test) {std::cout << #test << std::endl; int line = test(); if(line != 0) {std::cout << "failed at line " << line << std::endl; return line;}}

namespace test
{
  using stdex::system_error;
  using stdex::error_code;
  using stdex::error_category;
  using stdex::system_category;
}

struct test_category : public stdex::error_category
{
    test_category() stdex_noexcept {}
    virtual const char*
    name() const stdex_noexcept
    {
        const char* s = "stdex_test::test_category";
        return s;
    }

    virtual stdex::string
    message(int) const
    { return stdex::string("message to be determined"); }
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

enum my_errc { my_err = 0 };

class my_error_category_impl
: public stdex::error_category
{
public:
  const char* name() const stdex_noexcept { return ""; }
  stdex::string message(int) const { return ""; }
} my_error_category_instance;

stdex::error_code
make_error_code(my_errc e)
{
  return stdex::error_code(static_cast<int>(e),
			 my_error_category_instance);
}

namespace stdex
{
  template<>
    struct is_error_code_enum<my_errc>
    : public true_type { };
}

int test1()
{
  // 1
  stdex::error_code e1;
  DYNAMIC_VERIFY( e1.value() == 0 );
  DYNAMIC_VERIFY( e1.category() == stdex::system_category() );

  // 2
  const test_category cat;
  stdex::error_code e2(e1.value(), cat);
  DYNAMIC_VERIFY( e2.value() == e1.value() );
  DYNAMIC_VERIFY( e2.category() == cat );

  // 3
  stdex::error_code e3(stdex::make_error_code(stdex::errc::operation_not_supported));
  DYNAMIC_VERIFY( e3.value() == int(stdex::errc::operation_not_supported) );
  DYNAMIC_VERIFY( e3.category() == stdex::generic_category() );

  return 0;
}

int test2()
{
  stdex::error_code ec1(my_err);
  DYNAMIC_VERIFY( ec1 == make_error_code(my_err) );

  return 0;
}

int test3()
{
  stdex::error_code ec2;
  ec2 = my_err;
  DYNAMIC_VERIFY( ec2 == make_error_code(my_err) );

  return 0;
}

int test4()
{
  // 1
  stdex::error_code e1;
  if (static_cast<bool>(e1))
    {
      DYNAMIC_VERIFY( false );
    }

  // 2
  stdex::error_code e2(stdex::make_error_code(stdex::errc::operation_not_supported));
  if (static_cast<bool>(e2))
    {
      return 0;
    }

  DYNAMIC_VERIFY( false );
}

int test5()
{
  stdex::error_code e1;
  stdex::error_code e2(stdex::make_error_code(stdex::errc::operation_not_supported));

  DYNAMIC_VERIFY( e1 == e1 );
  DYNAMIC_VERIFY( !(e1 == e2) );

  const test_category cat;
  stdex::error_code e3(e2.value(), cat);
  DYNAMIC_VERIFY( !(e2 == e3) );

  return 0;
}

int test6()
{
  stdex::error_code e1;
  stdex::error_code e2(stdex::make_error_code(stdex::errc::operation_not_supported));

  if(stdex::errc::operation_not_supported < 0)
  {
    stdex::error_code tmp = e1;
    e1 = e2;
    e2 = tmp;
  }

  DYNAMIC_VERIFY( !(e1 < e1) );
  DYNAMIC_VERIFY( !(e2 < e2) );

  DYNAMIC_VERIFY( (e1.value() < e2.value()) );
  DYNAMIC_VERIFY( !(e2.value() < e1.value()) );
  DYNAMIC_VERIFY( (e1 < e2) );
  DYNAMIC_VERIFY( !(e2 < e1) );
  DYNAMIC_VERIFY( (e1 < e2) == (e1.value() < e2.value()) );

  const test_category cat;
  stdex::error_code e3(e2.value(), cat);
  DYNAMIC_VERIFY( !(e3 < e3) );
  DYNAMIC_VERIFY( (e2 < e3) == (e2.category() < e3.category()) );

  return 0;
}

int test7()
{
  stdex::error_code e1;
  stdex::error_code e2(stdex::make_error_code(stdex::errc::operation_not_supported));

  DYNAMIC_VERIFY( !(e1 != e1) );
  DYNAMIC_VERIFY( e1 != e2 );

  const test_category cat;
  stdex::error_code e3(e2.value(), cat);
  DYNAMIC_VERIFY( e2 != e3 );

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
    RUN_TEST(test6);
    RUN_TEST(test7);

    return 0;
}
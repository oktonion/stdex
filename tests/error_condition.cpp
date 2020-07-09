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

enum my_errc { my_err = 0 };

class my_error_category_impl
: public stdex::error_category
{
public:
  const char* name() const stdex_noexcept { return ""; }
  stdex::string message(int) const { return ""; }
} my_error_category_instance;

stdex::error_condition
make_error_condition(my_errc e)
{
  return stdex::error_condition(static_cast<int>(e),
			      my_error_category_instance);
}

namespace stdex
{
  template<>
    struct is_error_condition_enum<my_errc>
    : public true_type { };
}

int test1()
{
  // 1
  stdex::error_condition e1;
  DYNAMIC_VERIFY( e1.value() == 0 );
  DYNAMIC_VERIFY( e1.category() == stdex::generic_category() );

  // 2
  const test_category cat;
  stdex::error_condition e2(e1.value(), cat);
  DYNAMIC_VERIFY( e2.value() == e1.value() );
  DYNAMIC_VERIFY( e2.category() == cat );

  // 3
  stdex::error_condition e3(stdex::errc::operation_not_supported);
  DYNAMIC_VERIFY( e3.value() == int(stdex::errc::operation_not_supported) );
  DYNAMIC_VERIFY( e3.category() == stdex::generic_category() );

  return 0;
}

// libstdc++/39881
int test2()
{
  stdex::error_condition ec1(my_err);
  DYNAMIC_VERIFY( ec1 == make_error_condition(my_err) );
  return 0;
}

// libstdc++/39881
int test3()
{
  stdex::error_condition ec2;
  ec2 = my_err;
  DYNAMIC_VERIFY( ec2 == make_error_condition(my_err) );
  return 0;
}

int test4()
{
    // 1
    stdex::error_condition e1;
    if (static_cast<bool>(e1))
    {
        DYNAMIC_VERIFY( false );
    }

    // 2
    stdex::error_condition e2(stdex::errc::operation_not_supported);
    if (static_cast<bool>(e2))
    {
        DYNAMIC_VERIFY( true );
    }

    return 0;
}


int test5()
{
  stdex::error_condition e1;
  stdex::error_condition e2(stdex::errc::operation_not_supported);

  DYNAMIC_VERIFY( e1 == e1 );
  DYNAMIC_VERIFY( !(e1 == e2) );

  const test_category cat;
  stdex::error_condition e3(e2.value(), cat);
  DYNAMIC_VERIFY( !(e2 == e3) );

  return 0;
}

int test6()
{
  stdex::error_condition e1;
  stdex::error_condition e2(stdex::errc::operation_not_supported);

  DYNAMIC_VERIFY( !(e1 < e1) );
  DYNAMIC_VERIFY( !(e2 < e2) );

  DYNAMIC_VERIFY( (e1 < e2) == (e1.value() < e2.value()) );

  const test_category cat;
  stdex::error_condition e3(e2.value(), cat);
  DYNAMIC_VERIFY( !(e3 < e3) );
  DYNAMIC_VERIFY( (e2 < e3) == (e2.category() < e3.category()) );
  return 0;
}

int test7()
{
  stdex::error_condition e1;
  stdex::error_condition e2(stdex::errc::operation_not_supported);

  DYNAMIC_VERIFY( !(e1 != e1) );
  DYNAMIC_VERIFY( e1 != e2 );

  const test_category cat;
  stdex::error_condition e3(e2.value(), cat);
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
#include "../stdex/include/core.h"
#include "../stdex/include/future.hpp"
#include "../stdex/include/chrono.hpp"

#include <iostream>
#include <string>
#include <cstring>
#include <memory>

#define VERIFY(cond) STATIC_ASSERT((cond), check)
#define DYNAMIC_VERIFY(cond) if(!(cond)) {std::cout << "check condition \'" << #cond << "\' failed at line " << __LINE__ << std::endl; return -1;}
#define RUN_TEST(test) {std::cout << #test << std::endl; int line = test(); if(line != 0) {std::cout << "failed at line " << line << std::endl; return line;}}
#define DYNAMIC_VERIFY_FAIL {std::cout << "check condition " << "failed at line " << __LINE__ << std::endl; return -1;}
#define DYNAMIC_VERIFY_ABORT(cond) if(!(cond)) {std::cout << "check condition \'" << #cond << "\' failed at line " << __LINE__ << std::endl; std::abort();}

int default_error_condition_test()
{
    const stdex::error_category& e_cat = stdex::future_category();
    stdex::error_condition e_cond = e_cat.default_error_condition(static_cast<int>(stdex::errc::not_a_directory));
    DYNAMIC_VERIFY(e_cond.category() == e_cat);
    DYNAMIC_VERIFY(e_cond.value() == static_cast<int>(stdex::errc::not_a_directory));

  return 0;
}

int equivalent_error_code_int_test()
{
    const stdex::error_category& e_cat = stdex::future_category();
    DYNAMIC_VERIFY(e_cat.equivalent(stdex::error_code(5, e_cat), 5));
    DYNAMIC_VERIFY(!e_cat.equivalent(stdex::error_code(5, e_cat), 6));
  return 0;
}

int equivalent_int_error_condition_test()
{
    const stdex::error_category& e_cat = stdex::future_category();
    stdex::error_condition e_cond = e_cat.default_error_condition(5);
    DYNAMIC_VERIFY(e_cat.equivalent(5, e_cond));
    DYNAMIC_VERIFY(!e_cat.equivalent(6, e_cond));
  return 0;
}

int future_category_test()
{
    const stdex::error_category& ec = stdex::future_category();
    DYNAMIC_VERIFY(std::strcmp(ec.name(), "future") == 0);
  return 0;
}

int make_error_code_test()
{
      stdex::error_code ec = make_error_code(stdex::future_errc::broken_promise);
      DYNAMIC_VERIFY(ec.value() == static_cast<int>(stdex::future_errc::broken_promise));
      DYNAMIC_VERIFY(ec.category() == stdex::future_category());
  return 0;
}

int make_error_condition_test()
{
      const stdex::error_condition ec1 =
        stdex::make_error_condition(stdex::future_errc::future_already_retrieved);
      DYNAMIC_VERIFY(ec1.value() ==
                static_cast<int>(stdex::future_errc::future_already_retrieved));
      DYNAMIC_VERIFY(ec1.category() == stdex::future_category());
  return 0;
}

int main()
{
  try
  {
    RUN_TEST(default_error_condition_test);
    RUN_TEST(equivalent_error_code_int_test);
    RUN_TEST(equivalent_int_error_condition_test);
    RUN_TEST(future_category_test);
    RUN_TEST(make_error_code_test);
    RUN_TEST(make_error_condition_test);
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

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

int main(void)
{
  test_category c1;
  test_category c2(c1); // { dg-error "deleted" }


    return 0;
}

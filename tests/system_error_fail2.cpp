#include "../stdex/include/core.h"
#include "../stdex/include/system_error.hpp"

#include <iostream>
#include <string>

#define VERIFY(cond) STATIC_ASSERT((cond), check)
#define DYNAMIC_VERIFY(cond) if(!(cond)) {std::cout << "check condition \'" << #cond << "\' failed at line " << __LINE__ << std::endl; return -1;}
#define RUN_TEST(test) {std::cout << #test << std::endl; int line = test(); if(line != 0) {std::cout << "failed at line " << line << std::endl; return line;}}
#define DYNAMIC_VERIFY_FAIL {std::cout << "check condition " << "failed at line " << __LINE__ << std::endl; return -1;}
using std::size_t;

struct ClassType {};
enum my_errc { my_err = 0 };
enum my_not_errc { my_not_err = 0 };

class my_error_category_impl
    : public stdex::error_category
{
public:
    const char* name() const stdex_noexcept { return ""; }
    std::string message(int) const { return ""; }
} my_error_category_instance;

namespace stdex
{
    stdex::error_code
        make_error_code(my_errc e)
    {
        return stdex::error_code(static_cast<int>(e),
            my_error_category_instance);
    }
}

using stdex::make_error_code;

namespace stdex
{
    stdex::error_condition
        make_error_condition(my_errc e)
    {
        return stdex::error_condition(static_cast<int>(e),
            my_error_category_instance);
    }
}

using stdex::make_error_condition;

namespace stdex
{
    template<>
    struct is_error_code_enum<my_errc>
        : public true_type {};
}

namespace stdex
{
    template<>
    struct is_error_condition_enum<my_errc>
        : public true_type { };
}

class fuzzy_logic :
    public stdex::system_error
{
public:
    fuzzy_logic() : stdex::system_error(stdex::error_code(), "whoa") { }
};


int test4()
{
    using namespace stdex;

    {
        error_condition e;
        int i = e; // error "cannot convert"
    }

    return 0;
}

int main(void)
{
    using namespace stdex;
    
    RUN_TEST(test4);

    return 0;
}

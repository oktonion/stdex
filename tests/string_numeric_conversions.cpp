#include "../stdex/include/core.h"
#include "../stdex/include/string.hpp"

#include <cstdlib>
#include <iostream>
#include <cerrno>

#define DYNAMIC_VERIFY(cond) if(!(cond)) {std::cout << "check condition \'" << #cond << "\' failed at line " << __LINE__ << std::endl; return __LINE__;}
#define RUN_TEST(test) {std::cout << #test << std::endl; int line = test(); if(line != 0) {std::cout << "failed at line " << line << std::endl; return line;}}

namespace string_tests
{
    int test1()
    {
        using namespace stdex;

        const string one(to_string(-2));
        DYNAMIC_VERIFY( one == "-2" );

        const string two(to_string(10u));
        DYNAMIC_VERIFY( two == "10" );

        const string three(to_string(2l));
        DYNAMIC_VERIFY( three == "2" );

        const string four(to_string(3000ul));
        DYNAMIC_VERIFY( four == "3000" );

#ifdef LLONG_MAX
        const string five(to_string(7ll));
        DYNAMIC_VERIFY( five == "7" );

        const string six(to_string(400ull));
        DYNAMIC_VERIFY( six == "400" );
#endif

        const string seven(to_string(-1.0F));
        DYNAMIC_VERIFY( seven == "-1.000000" );

        const string eight(to_string(2.0));
        DYNAMIC_VERIFY( eight == "2.000000" );

        const string nine(to_string(-4.0L));
        DYNAMIC_VERIFY( nine == "-4.000000" );

        return 0;
    }

    int test2()
    {
        errno = ERANGE;
        stdex::stoi("42");
        DYNAMIC_VERIFY( errno == ERANGE ); // errno should not be altered by successful call

        return 0;
    }
}

int main(void)
{
    using namespace stdex;
    using namespace string_tests;
    
    RUN_TEST(test1);
    RUN_TEST(test2);

    return 0;
}

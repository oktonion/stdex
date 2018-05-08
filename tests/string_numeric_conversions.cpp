#include "../stdex/include/core.h"
#include "../stdex/include/string.hpp"

#include <cstdlib>
#include <iostream>
#include <cerrno>
#include <limits>

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

    int test3()
    {
        using namespace stdex;
        bool test = false;

        try
        {
            string one;
            stdex::stod(one);
        }
        catch (std::invalid_argument)
        {
            test = true;
        }
        catch (...)
        {
        }
        DYNAMIC_VERIFY(test);

        test = false;
        try
        {
            string one("a");
            stdex::stod(one);
        }
        catch (std::invalid_argument)
        {
            test = true;
        }
        catch (...)
        {
        }
        DYNAMIC_VERIFY(test);

        double d1 = 0.0;
        std::size_t idx1 = 0;
        try
        {
            string one("2.0a");
            d1 = stdex::stod(one, &idx1);
        }
        catch (...)
        {
            test = false;
        }
        DYNAMIC_VERIFY(test);
        DYNAMIC_VERIFY(d1 == 2.0);
        DYNAMIC_VERIFY(idx1 == 3);

        test = false;
        try
        {
            string one("1e");
            one.append(2 * std::numeric_limits<double>::max_exponent10, '9');
            d1 = stdex::stod(one);
        }
        catch (std::out_of_range)
        {
            test = true;
        }
        catch (...)
        {
        }
        DYNAMIC_VERIFY(test);
        DYNAMIC_VERIFY(d1 == 2.0);

        try
        {
            long double ld0 = std::numeric_limits<double>::max() / 100.0;
            string one(to_string(ld0));
            stdex::stod(one);
        }
        catch (...)
        {
            test = false;
        }
        DYNAMIC_VERIFY(test);

        if ((std::numeric_limits<long double>::max() / 10000.0L)
                > std::numeric_limits<double>::max())
        {
            test = false;
            d1 = -1.0;
            try
            {
                long double ld1 = std::numeric_limits<double>::max();
                ld1 *= 100.0;
                string one(to_string(ld1));
                d1 = stdex::stod(one);
            }
            catch (std::out_of_range)
            {
                test = true;
            }
            catch (...)
            {
            }
            DYNAMIC_VERIFY(test);
            DYNAMIC_VERIFY(d1 == -1.0);
        }

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

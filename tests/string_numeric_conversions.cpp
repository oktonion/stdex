#include "../stdex/include/core.h"
#include "../stdex/include/string.hpp"

#include <cstdlib>
#include <iostream>
#include <cerrno>
#include <limits>

#define DYNAMIC_VERIFY(cond) if(!(cond)) {std::cout << "check condition \'" << #cond << "\' failed at line " << __LINE__ << std::endl; return __LINE__;}
#define RUN_TEST(test) {std::cout << #test << std::endl; int line = test(); if(line != 0) {std::cout << "failed at line " << line << std::endl; return line;}}
using std::size_t;

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

    int stod_test()
    {
        using namespace stdex;
        bool test = false;
        
        try
        {
            string one;
            stdex::stod(one);
        }
        catch (const std::invalid_argument &)
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
        catch (const std::invalid_argument &)
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
        catch (const std::out_of_range &)
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
            catch (const std::out_of_range&)
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

    int stof_test()
    {
        using namespace stdex;

        bool test = false;

        try
        {
            string one;
            stdex::stof(one);
        }
        catch (const std::invalid_argument &)
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
            stdex::stof(one);
        }
        catch (const std::invalid_argument &)
        {
            test = true;
        }
        catch (...)
        {
        }
        DYNAMIC_VERIFY(test);

        float f1 = 0.0f;
        std::size_t idx1 = 0;
        try
        {
            string one("2.0a");
            f1 = stdex::stof(one, &idx1);
        }
        catch (...)
        {
            test = false;
        }
        DYNAMIC_VERIFY(test);
        DYNAMIC_VERIFY(f1 == 2.0f);
        DYNAMIC_VERIFY(idx1 == 3);

        test = false;
        try
        {
            string one("1e");
            one.append(2 * std::numeric_limits<float>::max_exponent10, '9');
            f1 = stdex::stof(one);
        }
        catch (const std::out_of_range &)
        {
            test = true;
        }
        catch (...)
        {
        }
        DYNAMIC_VERIFY(test);
        DYNAMIC_VERIFY(f1 == 2.0f);

        try
        {
            long double ld0 = std::numeric_limits<float>::max() / 100.0;
            string one(to_string(ld0));
            stdex::stof(one);
        }
        catch (...)
        {
            test = false;
        }
        DYNAMIC_VERIFY(test);

        if ((std::numeric_limits<long double>::max() / 10000.0L)> std::numeric_limits<float>::max())
        {
            test = false;
            f1 = -1.0f;
            try
            {
                long double ld1 = std::numeric_limits<float>::max();
                ld1 *= 100.0;
                string one(to_string(ld1));
                f1 = stdex::stof(one);
            }
            catch (const std::out_of_range &)
            {
                test = true;
            }
            catch (...)
            {
            }
            DYNAMIC_VERIFY(test);
            DYNAMIC_VERIFY(f1 == -1.0f);
        }

        return 0;
    }

    int stoi_test()
    {
        using namespace stdex;
        bool test = false;

        try
        {
            string one;
            stdex::stoi(one);
        }
        catch (const std::invalid_argument &)
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
            stdex::stoi(one);
        }
        catch (const std::invalid_argument &)
        {
            test = true;
        }
        catch (...)
        {
        }
        DYNAMIC_VERIFY(test);

        int i1 = 0;
        try
        {
            string one("a");
            i1 = stdex::stoi(one, 0, 16);
        }
        catch (...)
        {
            test = false;
        }
        DYNAMIC_VERIFY(test);
        DYNAMIC_VERIFY(i1 == 10);

        std::size_t idx1 = 0;
        try
        {
            string one("78");
            i1 = stdex::stoi(one, &idx1, 8);
        }
        catch (...)
        {
            test = false;
        }
        DYNAMIC_VERIFY(test);
        DYNAMIC_VERIFY(i1 == 7);
        DYNAMIC_VERIFY(idx1 == 1);

        try
        {
            string one("10112");
            i1 = stdex::stoi(one, &idx1, 2);
        }
        catch (...)
        {
            test = false;
        }
        DYNAMIC_VERIFY(test);
        DYNAMIC_VERIFY(i1 == 11);
        DYNAMIC_VERIFY(idx1 == 4);

        try
        {
            string one("0XE");
            i1 = stdex::stoi(one, &idx1, 0);
        }
        catch (...)
        {
            test = false;
        }
        DYNAMIC_VERIFY(test);
        DYNAMIC_VERIFY(i1 == 14);
        DYNAMIC_VERIFY(idx1 == 3);

        test = false;
        try
        {
            string one(1000, '9');
            i1 = stdex::stoi(one);
        }
        catch (const std::out_of_range &)
        {
            test = true;
        }
        catch (const std::exception &e)
        {
            std::cout << e.what() << std::endl;
        }
        catch (...)
        {
        }
        DYNAMIC_VERIFY(test);
        DYNAMIC_VERIFY(i1 == 14);

#ifdef LLONG_MAX
        try
        {
            i1 = std::numeric_limits<int>::max();
            string one(to_string((long long) i1));
            i1 = stdex::stoi(one);
        }
        catch (...)
        {
            test = false;
        }
        DYNAMIC_VERIFY(test);
        DYNAMIC_VERIFY(i1 == std::numeric_limits<int>::max());

        try
        {
            i1 = std::numeric_limits<int>::min();
            string one(to_string((long long) i1));
            i1 = stdex::stoi(one);
        }
        catch (...)
        {
            test = false;
        }
        DYNAMIC_VERIFY(test);
        DYNAMIC_VERIFY(i1 == std::numeric_limits<int>::min());

        test = false;
        i1 = 1;
        try
        {
            long long ll0 = std::numeric_limits<int>::max();
            ++ll0;
            string one(to_string(ll0));
            i1 = stdex::stoi(one);
        }
        catch (const std::out_of_range &)
        {
            test = true;
        }
        catch (...)
        {
        }
        DYNAMIC_VERIFY(test);
        DYNAMIC_VERIFY(i1 == 1);

        test = false;
        try
        {
            long long ll1 = std::numeric_limits<int>::min();
            --ll1;
            string one(to_string(ll1));
            i1 = stdex::stoi(one);
        }
        catch (const std::out_of_range &)
        {
            test = true;
        }
        catch (...)
        {
        }
        DYNAMIC_VERIFY(test);
        DYNAMIC_VERIFY(i1 == 1);

#endif
        return 0;
    }

    int stol_test()
    {
        using namespace stdex;
        bool test = false;

        try
        {
            string one;
            stdex::stol(one);
        }
        catch (const std::invalid_argument &)
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
            stdex::stol(one);
        }
        catch (const std::invalid_argument &)
        {
            test = true;
        }
        catch (...)
        {
        }
        DYNAMIC_VERIFY(test);

        long l1 = 0;
        try
        {
            string one("a");
            l1 = stdex::stol(one, 0, 16);
        }
        catch (...)
        {
            test = false;
        }
        DYNAMIC_VERIFY(test);
        DYNAMIC_VERIFY(l1 == 10);

        std::size_t idx1 = 0;
        try
        {
            string one("78");
            l1 = stdex::stol(one, &idx1, 8);
        }
        catch (...)
        {
            test = false;
        }
        DYNAMIC_VERIFY(test);
        DYNAMIC_VERIFY(l1 == 7);
        DYNAMIC_VERIFY(idx1 == 1);

        try
        {
            string one("10112");
            l1 = stdex::stol(one, &idx1, 2);
        }
        catch (...)
        {
            test = false;
        }
        DYNAMIC_VERIFY(test);
        DYNAMIC_VERIFY(l1 == 11);
        DYNAMIC_VERIFY(idx1 == 4);

        try
        {
            string one("0XE");
            l1 = stdex::stol(one, &idx1, 0);
        }
        catch (...)
        {
            test = false;
        }
        DYNAMIC_VERIFY(test);
        DYNAMIC_VERIFY(l1 == 14);
        DYNAMIC_VERIFY(idx1 == 3);

        test = false;
        try
        {
            string one(1000, '9');
            l1 = stdex::stol(one);
        }
        catch (const std::out_of_range &)
        {
            test = true;
        }
        catch (...)
        {
        }
        DYNAMIC_VERIFY(test);
        DYNAMIC_VERIFY(l1 == 14);

#ifdef LLONG_MAX
        try
        {
            l1 = std::numeric_limits<long>::max();
            string one(to_string((long long) l1));
            l1 = stdex::stol(one);
        }
        catch (...)
        {
            test = false;
        }
        DYNAMIC_VERIFY(test);
        DYNAMIC_VERIFY(l1 == std::numeric_limits<long>::max());

        try
        {
            l1 = std::numeric_limits<long>::min();
            string one(to_string((long long) l1));
            l1 = stdex::stol(one);
        }
        catch (...)
        {
            test = false;
        }
        DYNAMIC_VERIFY(test);
        DYNAMIC_VERIFY(l1 == std::numeric_limits<long>::min());
#endif
        return 0;
    }  

    int stold_test()
    {
        using namespace stdex;
        bool test = false;

        

        try
        {
            string one;
            stdex::stold(one);
        }
        catch (const std::invalid_argument &)
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
            stdex::stold(one);
        }
        catch (const std::invalid_argument &)
        {
            test = true;
        }
        catch (...)
        {
        }
        DYNAMIC_VERIFY(test);

        
        long double ld1 = 0.0L;
        std::size_t idx1 = 0;
        try
        {
            string one("2.0a");
            ld1 = stdex::stold(one, &idx1);
        }
        catch (...)
        {
            test = false;
        }
        DYNAMIC_VERIFY(test);
        DYNAMIC_VERIFY(ld1 == 2.0L);
        DYNAMIC_VERIFY(idx1 == 3);

       
        test = false;
        try
        {
            string one("1e");
            one.append(2 * std::numeric_limits<long double>::max_exponent10, '9');
            ld1 = stdex::stold(one);
        }
        catch (const std::out_of_range &)
        {
            test = true;
        }
        catch (...)
        {
        }
        DYNAMIC_VERIFY(test);
        DYNAMIC_VERIFY(ld1 == 2.0L);

        try
        {
            long double ld0 = std::numeric_limits<long double>::max() / 100.0L;
            string one(to_string(ld0));
            stdex::stold(one);
        }
        catch (...)
        {
            test = false;
        }
        DYNAMIC_VERIFY(test);

        return 0;
    }  

#ifdef LLONG_MAX
    int stoll_test()
    {
        using namespace stdex;
        bool test = false;

        try
        {
            string one;
            stdex::stoll(one);
        }
        catch (const std::invalid_argument &)
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
            stdex::stoll(one);
        }
        catch (const std::invalid_argument &)
        {
            test = true;
        }
        catch (...)
        {
        }
        DYNAMIC_VERIFY(test);

        long long ll1 = 0;
        try
        {
            string one("a");
            ll1 = stdex::stoll(one, 0, 16);
        }
        catch (...)
        {
            test = false;
        }
        DYNAMIC_VERIFY(test);
        DYNAMIC_VERIFY(ll1 == 10);

        size_t idx1 = 0;
        try
        {
            string one("78");
            ll1 = stdex::stoll(one, &idx1, 8);
        }
        catch (...)
        {
            test = false;
        }
        DYNAMIC_VERIFY(test);
        DYNAMIC_VERIFY(ll1 == 7);
        DYNAMIC_VERIFY(idx1 == 1);

        try
        {
            string one("10112");
            ll1 = stdex::stoll(one, &idx1, 2);
        }
        catch (...)
        {
            test = false;
        }
        DYNAMIC_VERIFY(test);
        DYNAMIC_VERIFY(ll1 == 11);
        DYNAMIC_VERIFY(idx1 == 4);

        try
        {
            string one("0XE");
            ll1 = stdex::stoll(one, &idx1, 0);
        }
        catch (...)
        {
            test = false;
        }
        DYNAMIC_VERIFY(test);
        DYNAMIC_VERIFY(ll1 == 14);
        DYNAMIC_VERIFY(idx1 == 3);

        test = false;
        try
        {
            string one(1000, '9');
            ll1 = stdex::stoll(one);
        }
        catch (const std::out_of_range &)
        {
            test = true;
        }
        catch (...)
        {
        }
        DYNAMIC_VERIFY(test);
        DYNAMIC_VERIFY(ll1 == 14);

        try
        {
            ll1 = std::numeric_limits<long long>::max();
            string one(stdex::to_string(ll1));
            ll1 = stdex::stoll(one);
        }
        catch (...)
        {
            test = false;
        }
        DYNAMIC_VERIFY(test);
        DYNAMIC_VERIFY(ll1 == std::numeric_limits<long long>::max());

        try
        {
            ll1 = std::numeric_limits<long long>::min();
            string one(to_string(ll1));
            ll1 = stdex::stoll(one);
        }
        catch (...)
        {
            test = false;
        }
        DYNAMIC_VERIFY(test);
        DYNAMIC_VERIFY(ll1 == std::numeric_limits<long long>::min());

        return 0;
    }
#endif

    int stoul_test()
    // stoul
    {
        bool test = false;
        using namespace stdex;

        try
        {
            string one;
            stdex::stoul(one);
        }
        catch (const std::invalid_argument &)
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
            stdex::stoul(one);
        }
        catch (const std::invalid_argument &)
        {
            test = true;
        }
        catch (...)
        {
        }
        DYNAMIC_VERIFY(test);

        unsigned long ul1 = 0;
        try
        {
            string one("a");
            ul1 = stdex::stoul(one, 0, 16);
        }
        catch (...)
        {
            test = false;
        }
        DYNAMIC_VERIFY(test);
        DYNAMIC_VERIFY(ul1 == 10);

        size_t idx1 = 0;
        try
        {
            string one("78");
            ul1 = stdex::stoul(one, &idx1, 8);
        }
        catch (...)
        {
            test = false;
        }
        DYNAMIC_VERIFY(test);
        DYNAMIC_VERIFY(ul1 == 7);
        DYNAMIC_VERIFY(idx1 == 1);

        try
        {
            string one("10112");
            ul1 = stdex::stoul(one, &idx1, 2);
        }
        catch (...)
        {
            test = false;
        }
        DYNAMIC_VERIFY(test);
        DYNAMIC_VERIFY(ul1 == 11);
        DYNAMIC_VERIFY(idx1 == 4);

        try
        {
            string one("0XE");
            ul1 = stdex::stoul(one, &idx1, 0);
        }
        catch (...)
        {
            test = false;
        }
        DYNAMIC_VERIFY(test);
        DYNAMIC_VERIFY(ul1 == 14);
        DYNAMIC_VERIFY(idx1 == 3);

        test = false;
        try
        {
            string one(1000, '9');
            ul1 = stdex::stoul(one);
        }
        catch (const std::out_of_range &)
        {
            test = true;
        }
        catch (...)
        {
        }
        DYNAMIC_VERIFY(test);
        DYNAMIC_VERIFY(ul1 == 14);

        test = true;
        try
        {
            string one(stdex::to_string((std::numeric_limits<unsigned long>::max)()));
            std::cout << "DEBUG:" << one << std::endl;
            ul1 = stdex::stoul(one);
        }
        catch (const std::out_of_range &)
        {
            test = false;
        }
        catch (...)
        {
            test = false;
        }
        DYNAMIC_VERIFY(test);
        DYNAMIC_VERIFY(ul1 == std::numeric_limits<unsigned long>::max());
#ifdef LLONG_MAX
        try
        {
            ul1 = std::numeric_limits<unsigned long>::max();
            string one(stdex::to_string((unsigned long long)ul1));
            ul1 = stdex::stoul(one);
        }
        catch (...)
        {
            test = false;
        }
        DYNAMIC_VERIFY(test);
        DYNAMIC_VERIFY(ul1 == std::numeric_limits<unsigned long>::max());
#endif
        return 0;
    }

#ifdef LLONG_MAX
	int stoull_test()
    {
        bool test = false;
        using namespace stdex;

        try
        {
            string one;
            stdex::stoull(one);
        }
        catch (const std::invalid_argument &)
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
            stdex::stoull(one);
        }
        catch (const std::invalid_argument &)
        {
            test = true;
        }
        catch (...)
        {
        }
        DYNAMIC_VERIFY(test);

        unsigned long long ull1 = 0;
        try
        {
            string one("a");
            ull1 = stdex::stoull(one, 0, 16);
        }
        catch (...)
        {
            test = false;
        }
        DYNAMIC_VERIFY(test);
        DYNAMIC_VERIFY(ull1 == 10);

        size_t idx1 = 0;
        try
        {
            string one("78");
            ull1 = stdex::stoull(one, &idx1, 8);
        }
        catch (...)
        {
            test = false;
        }
        DYNAMIC_VERIFY(test);
        DYNAMIC_VERIFY(ull1 == 7);
        DYNAMIC_VERIFY(idx1 == 1);

        try
        {
            string one("10112");
            ull1 = stdex::stoull(one, &idx1, 2);
        }
        catch (...)
        {
            test = false;
        }
        DYNAMIC_VERIFY(test);
        DYNAMIC_VERIFY(ull1 == 11);
        DYNAMIC_VERIFY(idx1 == 4);

        try
        {
            string one("0XE");
            ull1 = stdex::stoull(one, &idx1, 0);
        }
        catch (...)
        {
            test = false;
        }
        DYNAMIC_VERIFY(test);
        DYNAMIC_VERIFY(ull1 == 14);
        DYNAMIC_VERIFY(idx1 == 3);

        test = false;
        try
        {
            string one(1000, '9');
            ull1 = stdex::stoull(one);
        }
        catch (const std::out_of_range &)
        {
            test = true;
        }
        catch (...)
        {
        }
        DYNAMIC_VERIFY(test);
        DYNAMIC_VERIFY(ull1 == 14);

        try
        {
            ull1 = std::numeric_limits<unsigned long long>::max();
            string one(to_string(ull1));
            ull1 = stdex::stoull(one);
        }
        catch (...)
        {
            test = false;
        }
        DYNAMIC_VERIFY(test);
        DYNAMIC_VERIFY(ull1 == std::numeric_limits<unsigned long long>::max());

        return 0;
    }
#endif

    int to_string_test()
    {
        using namespace stdex;

#ifdef LLONG_MAX
        typedef long long test_type;
        typedef unsigned long long utest_type;
#else
        typedef long test_type;
        typedef unsigned long utest_type;
#endif
        test_type ll1 = -2;
        string one(to_string(ll1));
        DYNAMIC_VERIFY(one == "-2");

        test_type ll2 = 10;
        string two(to_string(ll2));
        DYNAMIC_VERIFY(two == "10");
        utest_type ull1 = 2;
        string three(to_string(ull1));
        DYNAMIC_VERIFY(three == "2");

        utest_type ull2 = 3000;
        string four(to_string(ull2));
        DYNAMIC_VERIFY(four == "3000");

        long double ld1 = 2.0L;
        string five(to_string(ld1));
        DYNAMIC_VERIFY(five == "2.000000");

        long double ld2 = -4.0L;
        string six(to_string(ld2));
        DYNAMIC_VERIFY(six == "-4.000000");

        string str = to_string("testtesttest");
        DYNAMIC_VERIFY(str == "testtesttest");

        char test_arr[]= {'t', 'e', 's', 't', '\0'};
        string str_arr = to_string(test_arr);
        DYNAMIC_VERIFY(str_arr == test_arr);

        return 0;
    }
}

int main(void)
{
    using namespace stdex;
    using namespace string_tests;

    RUN_TEST(test1);
    RUN_TEST(test2);
    RUN_TEST(stod_test);
    RUN_TEST(stof_test);
    RUN_TEST(stoi_test);
    RUN_TEST(stol_test);
    RUN_TEST(stold_test);
    #ifdef LLONG_MAX
    RUN_TEST(stoll_test);
    RUN_TEST(stoull_test);
    #endif
    RUN_TEST(stoul_test);
    RUN_TEST(to_string_test);

    return 0;
}

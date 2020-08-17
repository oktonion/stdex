#include "../stdex/include/core.h"
#include "../stdex/include/thread.hpp"
#include "../stdex/include/system_error.hpp"

#include <cstdlib>
#include <iostream>
#include <ctime>

#if defined(_STDEX_NATIVE_CPP11_SUPPORT) || defined(__MACH__)
#include <thread>
#include <system_error>
#endif

#define DYNAMIC_VERIFY(cond) if(!(cond)) {std::cout << "check condition \'" << #cond << "\' failed at line " << __LINE__ << std::endl; return __LINE__;}
#define RUN_TEST(test) {std::cout << #test << std::endl; int line = test(); if(line != 0) {std::cout << "failed at line " << line << std::endl; return line;}}
#define DYNAMIC_VERIFY_FAIL {std::cout << "check condition " << "failed at line " << __LINE__ << std::endl; return -1;}
using std::size_t;


int test14()
{
    #if defined(_STDEX_NATIVE_CPP11_SUPPORT) || defined(__MACH__)
    
    std::intmax_t std_dur, std_desired_dur;

    {   
        using namespace std;
        using namespace std::chrono;
        typedef std::intmax_t intmax_type;

        steady_clock::time_point start = steady_clock::now();

        stdex::this_thread::sleep_for(stdex::chrono::milliseconds(25000));

        steady_clock::duration dur = 
            steady_clock::now() - start;

        intmax_type desired_dur = duration_cast<milliseconds>(dur).count();

        start = steady_clock::now();

        for(std::size_t i = 0; i < 100; ++i)
        {
            stdex::this_thread::sleep_for(stdex::chrono::milliseconds(250));
        }
        dur = 
            steady_clock::now() - start;
        std::cout << "std::duration is " << duration_cast<milliseconds>(dur).count() << " ms, desired is " << desired_dur << " ms" << std::endl;
        DYNAMIC_VERIFY(desired_dur >= intmax_type(25000));
        DYNAMIC_VERIFY(duration_cast<milliseconds>(dur).count() >= intmax_type(25000));
        //DYNAMIC_VERIFY(duration_cast<milliseconds>(dur).count() < desired_dur + intmax_type(2000)); // 2 sec is bullshit but better than nothing

        std_dur = duration_cast<milliseconds>(dur).count();
        std_desired_dur = desired_dur;
    }
    #endif
    
    {   
        using namespace stdex;
        using namespace stdex::chrono;
        typedef stdex::intmax_t intmax_type;

        steady_clock::time_point start = steady_clock::now();

        this_thread::sleep_for(milliseconds(25000));

        steady_clock::duration dur = 
            steady_clock::now() - start;

        intmax_type desired_dur = duration_cast<milliseconds>(dur).count();

        start = steady_clock::now();

        for(std::size_t i = 0; i < 100; ++i)
        {
            this_thread::sleep_for(milliseconds(250));
        }
        dur = 
            steady_clock::now() - start;
        std::cout << "duration is " << duration_cast<milliseconds>(dur).count() << " ms, desired is " << desired_dur << " ms" << std::endl;

        #if defined(_STDEX_NATIVE_CPP11_SUPPORT) || defined(__MACH__)
        std::cout << "std::duration is " << std_dur << " ms, stdex::duration is " << duration_cast<milliseconds>(dur).count() << " ms" << std::endl;
        std::cout << "std::desired is " << std_desired_dur << " ms, stdex::desired is " << desired_dur << " ms" << std::endl;
        #endif

        DYNAMIC_VERIFY(desired_dur >= intmax_type(25000));
        DYNAMIC_VERIFY(duration_cast<milliseconds>(dur).count() >= intmax_type(25000));
        DYNAMIC_VERIFY(duration_cast<milliseconds>(dur).count() < desired_dur + intmax_type(2500)); // 2.5 sec is bullshit but better than nothing
    }

    return 0;
}

int main(void)
{
    using namespace stdex;

    stdex::ratio<1l, 60l>::den;

    //int a[sizeof(stdex::intmax_t) < sizeof(long long) ? -1 : 1];
    std::cout <<
        stdex::detail::cstdint_detail::_max_sized_integer_step<>::value << std::endl;

    for (size_t i = 0; i < 50; ++i)
    {
        std::cout << "step " << i << std::endl;

    }

    for (size_t i = 0; i < 5; ++i)
    {
        DYNAMIC_VERIFY(thread::hardware_concurrency() >= 1);
        RUN_TEST(test14);
    }

    return 0;
}

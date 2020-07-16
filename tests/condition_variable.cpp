#include "../stdex/include/core.h"
#include "../stdex/include/condition_variable.hpp"
#include "../stdex/include/mutex.hpp"
#include "../stdex/include/system_error.hpp"
#include "../stdex/include/thread.hpp"

#include <iostream>
#define DYNAMIC_VERIFY(cond) if(!(cond)) {std::cout << "check condition \'" << #cond << "\' failed at line " << __LINE__ << std::endl; return __LINE__;}
#define RUN_TEST(test) {std::cout << #test << std::endl; int line = test(); if(line != 0) {return line;}}
#define DYNAMIC_VERIFY_FAIL {std::cout << "check condition " << "failed at line " << __LINE__ << std::endl; return -1;}

namespace cond_var_tests
{
	int counter = 0;

	struct Inc
	{
		Inc() { ++counter; }
		~Inc() { ++counter; }
	};

	stdex::mutex mx;
	stdex::condition_variable cv;

	bool val = false;

	bool func_val() { return cond_var_tests::val; }

	void func()
	{
		stdex::this_thread::sleep_for(stdex::chrono::milliseconds(10000));
		stdex::unique_lock<stdex::mutex> lock(cond_var_tests::mx);
		stdex::notify_all_at_thread_exit(cv, lock);

		Inc inc;
	}

	bool condition_func()
	{
		return cond_var_tests::counter == 2;
	}


	struct FPClock : stdex::chrono::system_clock
	{
		typedef double rep;
		typedef stdex::ratio<1> period;
		typedef stdex::chrono::duration<rep, period> duration;
		typedef stdex::chrono::time_point<FPClock> time_point;

		static time_point now()
		{
			return time_point(duration(stdex::chrono::system_clock::now().time_since_epoch()));
		}
	};
}

bool false_predicate()
{
	return false;
}

int test1()
{
    using namespace stdex;

    try
    {
        condition_variable c1;
    }
    catch (const system_error&)
    {
        DYNAMIC_VERIFY_FAIL ;
    }
    catch (...)
    {
        DYNAMIC_VERIFY_FAIL ;
    }

    return 0;
}

int test2()
{
    using namespace stdex;

    try
    {
        chrono::microseconds ms(500);
        condition_variable c1;
        mutex m;
        unique_lock<mutex> l(m);

        chrono::system_clock::time_point then = chrono::system_clock::now();
        bool result = c1.wait_for(l, ms, &false_predicate);
        DYNAMIC_VERIFY(result == false);
        DYNAMIC_VERIFY((chrono::system_clock::now() - then) >= ms);
        DYNAMIC_VERIFY(l.owns_lock());
    }
    catch (const system_error&)
    {
        DYNAMIC_VERIFY_FAIL ;
    }
    catch (...)
    {
        DYNAMIC_VERIFY_FAIL ;
    }

    return 0;
}

int test3()
{
    using namespace stdex;

    try
    {
        chrono::microseconds ms(500);
        condition_variable c1;
        mutex m;
        unique_lock<mutex> l(m);

        chrono::steady_clock::time_point then = chrono::steady_clock::now();
        bool result = c1.wait_for(l, ms, &false_predicate);
        const chrono::steady_clock::duration t = chrono::steady_clock::now() - then;
        DYNAMIC_VERIFY(result == false);
        std::cout << stdex::chrono::duration_cast<chrono::microseconds>(t).count() << " >= " << ms.count() << std::endl;
        DYNAMIC_VERIFY((chrono::steady_clock::now() - then) >= ms);
        DYNAMIC_VERIFY(l.owns_lock());
    }
    catch (const system_error&)
    {
        DYNAMIC_VERIFY_FAIL ;
    }
    catch (...)
    {
        DYNAMIC_VERIFY_FAIL ;
    }

    return 0;
}

int test4()
{
    using namespace stdex;

    unique_lock<mutex> lock( cond_var_tests::mx );
    thread t( cond_var_tests::func );
    cond_var_tests::cv.wait(lock, cond_var_tests::condition_func);
    t.join();

    return 0;
}

int test5()
{
    using namespace stdex;

    mutex mx;
    unique_lock<mutex> l(mx);
    condition_variable cv;
    cv.wait_until(l, cond_var_tests::FPClock::now());
    
    return 0;
}

int test6()
{
    using namespace stdex;

    mutex mx;
    condition_variable cv;

    for (int i = 0; i < 3; ++i)
    {
        unique_lock<mutex> l(mx);
        chrono::system_clock::time_point start = chrono::system_clock::now();
        cv.wait_for(l, chrono::duration<float>(1), cond_var_tests::func_val);
        chrono::system_clock::time_point t = chrono::system_clock::now();
        std::cout << stdex::chrono::duration_cast<chrono::milliseconds>((t - start)).count() << " >= " << 1000 << std::endl;
        DYNAMIC_VERIFY( (t - start) >= chrono::duration<float>(1) );
    }

    return 0;
}

int main(void)
{
    using namespace stdex;
    
    // condition_variable

    RUN_TEST(test1);
    RUN_TEST(test2);
    RUN_TEST(test3);
    RUN_TEST(test4);
    RUN_TEST(test5);
    RUN_TEST(test6);

    return 0;
}

#include "../stdex/include/core.h"
#include "../stdex/include/thread.hpp"
#include "../stdex/include/system_error.hpp"

#include <cstdlib>
#include <iostream>
#include <ctime>

#define DYNAMIC_VERIFY(cond) if(!(cond)) {std::cout << "check condition \'" << #cond << "\' failed at line " << __LINE__ << std::endl; return __LINE__;}
#define RUN_TEST(test) {std::cout << #test << std::endl; int line = test(); if(line != 0) {std::cout << "failed at line " << line << std::endl; return line;}}
using std::size_t;

namespace thread_tests_std
{
	template<class T>
	class reference_wrapper
	{
	public:
		reference_wrapper(T &ref_) :
			_ptr(&ref_)
		{ }

		reference_wrapper(const reference_wrapper &other): 
			_ptr(other._ptr)
		{ }

		reference_wrapper& operator=(const reference_wrapper &other)
		{
			_ptr = other._ptr;
			return (*this);
		}

		operator T&() const
		{
			return *_ptr;
		}

	private:
		T *_ptr;
	};

	template<class T>
	reference_wrapper<T> ref(T &ref_)
	{
		return reference_wrapper<T>(ref_);
	}

	template<class T>
	reference_wrapper<T> ref(reference_wrapper<T> &ref_)
	{
		return ref_;
	}
}

size_t active_thread_left = 0;

void
free_function(stdex::thread::id& id)
{
	id = stdex::this_thread::get_id();
	std::cout << "[id]=" << id << std::endl;
	static int i = 0;

	++i;

	if (i % 4 == 0)
	{
		std::srand( (unsigned int)(std::time(nullptr)) );

		stdex::this_thread::sleep_for(stdex::chrono::milliseconds(1 + (std::rand() % (330 - 1 + 1))));
	}
	active_thread_left--;
}

struct copyable
{
	copyable() {}
	~copyable() {}
	copyable(const copyable&)
	{
		++copy_count;
	}

	void operator()(stdex::thread::id& id) const
	{
		free_function(id);
	}

	static int copy_count;
};

int copyable::copy_count = 0;

bool f_was_called = false;

void f()
{
	f_was_called = true;
	active_thread_left--;
}

struct ClassType {};

int thread_func1(int) { return 0; }
int thread_func2(int, float) { return 2; }
int thread_func3(int, double, float) { return 2; }
double thread_func4(int, int, int, float) { return 2.0; }
void thread_func5(int, int, int, float, int) { return; }
void thread_func7(int, int, int, float, int, float*, void*) { return; }
void thread_func8(float*, float*, void*, ClassType*) { return; }

int thread_func_nullptr_check_ret = 0;

struct functor
{
	functor() {}
	~functor() {}

	int operator()(float *arg1, double *arg2, void(*arg3)(int, float, int*), float(functor::*arg4)(int, void*), ClassType *arg5)
	{
        if (arg1 == nullptr)
        {
            thread_func_nullptr_check_ret = __LINE__;
            std::cout << "fail: arg1 == nullptr" << std::endl;
            return 0;
        }
        if (arg2 != nullptr)
        {
            thread_func_nullptr_check_ret = __LINE__;
            std::cout << "fail: arg2(" << arg2 << ") != nullptr" << std::endl;
            return 0;
        }
        if (nullptr != arg3)
        {
            thread_func_nullptr_check_ret = __LINE__;
            std::cout << "fail: nullptr != arg3(" << arg3 << ")" << std::endl;
            return 0;
        }
        if (nullptr != arg4)
        {
            thread_func_nullptr_check_ret = __LINE__;
            std::cout << "fail: nullptr != arg4(" << arg4 << ")" << std::endl;
            return 0;
        }
        if (nullptr == arg5)
        {
            thread_func_nullptr_check_ret = __LINE__;
            std::cout << "fail: nullptr == arg5" << std::endl;
            return 0;
        }

        thread_func_nullptr_check_ret = 0;
		return 0;
	}
};



void thread_func_nullptr_check(float *arg1, float *arg2, void *arg3, ClassType *arg4) 
{ 
    if (arg1 != nullptr)
    {
        thread_func_nullptr_check_ret = __LINE__;
        std::cout << "fail: arg1(" << arg1 << ") != nullptr" << std::endl;
        return;
    }
    if (arg2 == nullptr)
    {
        thread_func_nullptr_check_ret = __LINE__;
        std::cout << "fail: arg2 == nullptr" << std::endl;
        return;
    }
    if (nullptr != arg3)
    {
        thread_func_nullptr_check_ret = __LINE__;
        std::cout << "fail: nullptr != arg3(" << arg3 << ")" << std::endl;
        return;
    }
    if (nullptr == arg4)
    {
        thread_func_nullptr_check_ret = __LINE__;
        std::cout << "fail: nullptr == arg4" << std::endl;
        return;
    }

    thread_func_nullptr_check_ret = 0;
}

int total = 0;

// Functor has internal state.
struct moveable
{
	int i;

	moveable() {};
	~moveable() {};
	//moveable(const moveable& c) = delete;
	//moveable& operator=(const moveable&) = delete;

	moveable(int j) : i(j) { }
	//moveable(moveable&& m) : i(m.i) { }

	void operator()() const { total += i; active_thread_left--;}
};

int test0()
{
    using namespace stdex;
    thread::id mid = this_thread::get_id();

    return 0;
}

int test1()
{
    using namespace stdex;

    try
    {
        thread t;
        DYNAMIC_VERIFY(!t.joinable());
    }
    catch (const system_error&)
    {
        DYNAMIC_VERIFY(false);
    }
    catch (...)
    {
        DYNAMIC_VERIFY(false);
    }

    return 0;
}

int test2()
{
    using namespace stdex;

    try
    {
        thread::id id1;

        active_thread_left++;
        thread t(free_function, thread_tests_std::ref(id1));
        thread::id id2 = t.get_id();
        DYNAMIC_VERIFY(t.joinable());
        t.join();
        DYNAMIC_VERIFY(!t.joinable());
        DYNAMIC_VERIFY(id1 == id2);
    
    }
    catch (const system_error&)
    {
        DYNAMIC_VERIFY(false);
    }
    catch (const char *)
    {
        DYNAMIC_VERIFY(false);
    }
    catch (...)
    {
        DYNAMIC_VERIFY(false);
    }

    return 0;
}

int test3()
{
    using namespace stdex;

    try
    {
        thread::id t1_id1;
        copyable c1;
        active_thread_left++;
        thread t1(c1, thread_tests_std::ref(t1_id1));
        active_thread_left++;
        thread tt1(c1, t1_id1);
        thread::id t1_id2 = t1.get_id();
        DYNAMIC_VERIFY(t1.joinable());
        t1.join();
        DYNAMIC_VERIFY(!t1.joinable());
        DYNAMIC_VERIFY(t1_id1 == t1_id2);
        //DYNAMIC_VERIFY(copyable::copy_count == 0);

        thread::id t2_id1;
        copyable c2;
        active_thread_left++;
        thread t2(c2, thread_tests_std::ref(t2_id1));
        thread::id t2_id2 = t2.get_id();
        DYNAMIC_VERIFY(t2.joinable());
        t2.join();
        DYNAMIC_VERIFY(!t2.joinable());
        DYNAMIC_VERIFY(t2_id1 == t2_id2);
        DYNAMIC_VERIFY(copyable::copy_count > 0);
        copyable::copy_count = 0;

        tt1.join();
    }
    catch (const system_error&)
    {
        DYNAMIC_VERIFY(false);
    }
    catch (const char *)
    {
        DYNAMIC_VERIFY(false);
    }
    catch (...)
    {
        DYNAMIC_VERIFY(false);
    }

    return 0;
}

int test4()
{
    using namespace stdex;

    try
    {
        active_thread_left++;
        thread t(f);
        t.join();
        DYNAMIC_VERIFY(f_was_called);
        f_was_called = false;
    }
    catch (const system_error&)
    {
        DYNAMIC_VERIFY(false);
    }
    catch (const char *)
    {
        DYNAMIC_VERIFY(false);
    }
    catch (...)
    {
        DYNAMIC_VERIFY(false);
    }

    return 0;
}

int test5()
{
    using namespace stdex;

    try
    {
        // first
        active_thread_left++;
        moveable m1(60);
        thread t1(m1);
        t1.join();
        DYNAMIC_VERIFY(total == 60);

        // second
        active_thread_left++;
        moveable m2(600);
        thread t2(m2);
        t2.join();
        DYNAMIC_VERIFY(total == 660); // Not 120...

        total = 0;
    }
    catch (const system_error&)
    {
        DYNAMIC_VERIFY(false);
    }
    catch (const char *)
    {
        DYNAMIC_VERIFY(false);
    }
    catch (...)
    {
        DYNAMIC_VERIFY(false);
    }

    return 0;
}

int test6()
{
    using namespace stdex;

    {
        thread t1;
        thread t2(thread_func1, 1);
        swap(static_cast<thread&>(t1), static_cast<thread&>(t2));

        t1.join();
    }

    /*{
        thread t1(thread_func7, 1, 1, 1, 1, 1, nullptr, nullptr);

        t1.join();
    }*/

    {
        thread t1(thread_func8, nullptr, nullptr, nullptr, nullptr);

        t1.join();
    }

    return 0;
}

int test7()
{
    using namespace stdex;

#if CHECK_FOR_COMPILE_ERROR_TESTS == 1
		{
			typedef thread test_type;
			test_type t1;
			test_type t2;
			t1 = t2;
		}

		{
			typedef thread test_type;
			test_type t1;
			test_type t2(t1);
		}
#endif
    return 0;
}

int test_thread_id()
{
    // thread id
    using namespace stdex;

    thread::id id1;
    thread::id id2;
    bool res;

    res = id1 == id2;
    res = id1 != id2;
    res = id1 < id2;
    res = id1 > id2;
    res = id1 >= id2;
    res = id1 <= id2;

    res = res;

    return 0;
}

int test8()
{
    using namespace stdex;

    try
    {
        active_thread_left++;
        thread t(f);
        DYNAMIC_VERIFY(t.joinable());
        t.detach();
        DYNAMIC_VERIFY(!t.joinable());
    }
    catch (const system_error&)
    {
        DYNAMIC_VERIFY(false);
    }
    catch (const char *)
    {
        DYNAMIC_VERIFY(false);
    }
    catch (...)
    {
        DYNAMIC_VERIFY(false);
    }

    return 0;
}

int test9()
{
    using namespace stdex;

    try
    {
        active_thread_left++;
        thread t(f);
        DYNAMIC_VERIFY(t.get_id() != thread::id());
        t.join();
        DYNAMIC_VERIFY(t.get_id() == thread::id());
    }
    catch (const system_error&)
    {
        DYNAMIC_VERIFY(false);
    }
    catch (const char *)
    {
        DYNAMIC_VERIFY(false);
    }
    catch (...)
    {
        DYNAMIC_VERIFY(false);
    }
    return 0;
}

int test10()
{
    using namespace stdex;
#if CHECK_FOR_THROW_EVENTS != 0
		{
			bool test = false;

			thread t;
			try
			{
				t.join();
			}
			catch (const system_error&)
			{
				test = true;
			}
			catch (const char *)
			{
				DYNAMIC_VERIFY(false);
			}

			DYNAMIC_VERIFY(test);
		}

		{
			bool test = false;

			thread t;

			try
			{
				t.detach();
			}
			catch (const system_error&)
			{
				test = true;
			}
			catch (const char *)
			{
				DYNAMIC_VERIFY(false);
			}

			DYNAMIC_VERIFY(test);
		}
#endif
    return 0;
}

int test11()
{
    using namespace stdex;

    {
        float p;
        ClassType cl;
        thread t1(thread_func_nullptr_check, nullptr, &p, nullptr, &cl);

        t1.join();
    }

    return thread_func_nullptr_check_ret;
}

int test12()
{
    using namespace stdex;

    thread_func_nullptr_check_ret = 0;
    
    {
        float p;
        ClassType cl;
        thread t1(functor(), &p, nullptr, nullptr, nullptr, &cl);

        t1.join();
    }

    return thread_func_nullptr_check_ret;
}

void dummy_func_0() {}
void dummy_func_1(void*) {}
void dummy_func_2(void*, int*) {}
void dummy_func_3(void*, double*, float*) {}
void dummy_func_4(struct dummy_func_4_t0*, struct dummy_func_4_t1*, struct dummy_func_4_t2*, struct dummy_func_4_t3*) {}
void dummy_func_5(struct dummy_func_5_t0*, struct dummy_func_5_t1*, struct dummy_func_5_t2*, struct dummy_func_5_t3*, struct dummy_func_5_t4*) {}
void dummy_func_6(struct dummy_func_5_t0*, struct dummy_func_5_t1*, struct dummy_func_5_t2 const*, struct dummy_func_5_t3*, struct dummy_func_5_t4*, struct dummy_func_5_t5 const*) {}
void dummy_func_7(struct dummy_func_7_t0*, struct dummy_func_7_t1*, struct dummy_func_7_t2 const*, struct dummy_func_7_t3*, struct dummy_func_7_t4*, struct dummy_func_7_t5 const*, struct dummy_func_7_t6 const*) {}
void dummy_func_8(struct dummy_func_7_t0*, struct dummy_func_7_t1*, struct dummy_func_7_t2 const*, struct dummy_func_7_t3*, struct dummy_func_7_t4*, struct dummy_func_7_t5 const*, struct dummy_func_7_t6 const**, void*) {}

int test13()
{
    using namespace stdex;

    {
        thread tt(&dummy_func_0);
        tt.join();
    }

    {
        thread tt(&dummy_func_1, nullptr);
        tt.join();
    }

    {
        thread tt(&dummy_func_2, nullptr, nullptr);
        tt.join();
    }

    {
        thread tt(&dummy_func_3, nullptr, nullptr, nullptr);
        tt.join();
    }

    {
        thread tt(&dummy_func_4, nullptr, nullptr, nullptr, nullptr);
        tt.join();
    }

    {
        thread tt(&dummy_func_5, nullptr, nullptr, nullptr, nullptr, nullptr);
        tt.join();
    }

    {
        thread tt(&dummy_func_6, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
        tt.join();
    }

    {
        thread tt(&dummy_func_7, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
        tt.join();
    }

    {
        thread tt(&dummy_func_8, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
        tt.join();
    }

    return 0;
}

int test14()
{
    using namespace stdex;
    using namespace stdex::chrono;

    system_clock::time_point start = system_clock::now();

    for(std::size_t i = 0; i < 1000; ++i)
    {
        this_thread::sleep_for(milliseconds(25));
    }
    system_clock::duration dur = 
        system_clock::now() - start;
    DYNAMIC_VERIFY(duration_cast<milliseconds>(dur).count() >= 25 * 1000);
    DYNAMIC_VERIFY(duration_cast<milliseconds>(dur).count() < 26 * 1000);

    return 0;
}

int main(void)
{
    using namespace stdex;
    
    test0();    

    for (size_t i = 0; i < 50; ++i)
    {
        std::cout << "step " << i << std::endl;

        RUN_TEST(test1);
        RUN_TEST(test2);
        RUN_TEST(test3);
        RUN_TEST(test4);
        RUN_TEST(test5);
        RUN_TEST(test6);
        RUN_TEST(test7);
        RUN_TEST(test8);
        RUN_TEST(test9);
        RUN_TEST(test10);
        RUN_TEST(test11);
        RUN_TEST(test12);
        RUN_TEST(test13);
        RUN_TEST(test14);

        DYNAMIC_VERIFY(thread::hardware_concurrency() >= 1);
    }
    
    test_thread_id();

    return 0;
}

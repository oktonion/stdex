#include "../stdex/include/core.h"
#include "../stdex/include/thread.hpp"
#include "../stdex/include/system_error.hpp"

#include <cstdlib>
#include <iostream>
#include <ctime>

#define DYNAMIC_VERIFY(cond) if(!(cond)) return __LINE__;
#define RUN_TEST(test) {std::cout << #test << std::endl; int line = test(); if(line != 0) return line;}

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
		std::srand(std::time(nullptr));

		stdex::this_thread::sleep_for(stdex::chrono::milliseconds(1 + (std::rand() % (330 - 1 + 1))));
	}
	active_thread_left--;
}

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
    catch (const char *msg)
    {
        DYNAMIC_VERIFY(false);
    }
    catch (...)
    {
        DYNAMIC_VERIFY(false);
    }

    return 0;
}


int main(void)
{

    test0();    

    for (size_t i = 0; i < 50; ++i)
    {
        std::cout << "step " << i << std::endl;

        RUN_TEST(test1);
        RUN_TEST(test2);
    }
    


    return 0;
}
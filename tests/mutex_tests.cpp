#include "../stdex/include/core.h"
#include "../stdex/include/system_error.hpp"
#include "../stdex/include/mutex.hpp"
#include "../stdex/include/thread.hpp"

#include <iostream>
#include <string>

#define VERIFY(cond) STATIC_ASSERT((cond), check)
#define DYNAMIC_VERIFY(cond) if(!(cond)) {std::cout << "check condition \'" << #cond << "\' failed at line " << __LINE__ << std::endl; return -1;}
#define RUN_TEST(test) {std::cout << #test << std::endl; int line = test(); if(line != 0) {std::cout << "failed at line " << line << std::endl; return line;}}

namespace mutex_tests
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

	// Check for required typedefs
    typedef stdex::mutex mutex_type;
    typedef mutex_type::native_handle_type native_handle_type;

	mutex_type m;

	void f()
	{
		stdex::lock_guard<mutex_type> l(m);
	}

    int try_lock_func_res = 0;

	int try_lock_func(mutex_type &m, bool &b)
	{
		try
		{
			b = m.try_lock();
		}
		catch (const stdex::system_error& e)
		{
            try_lock_func_res = -1;
			DYNAMIC_VERIFY(false);
		}

        return 0;
	}

    int test1()
    {
        using namespace stdex;

        try
        {
            mutex_type m1;
        }
        catch (const stdex::system_error& e)
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

#if CHECK_FOR_COMPILE_ERROR_TESTS == 1
		{
			// assign
			mutex_type m1;
			mutex_type m2;
			m1 = m2;			// { dg-error "deleted" }
		}

		{
			// assign
			mutex_type m1;
			mutex_type m2(m1);		// { dg-error "deleted" }
		}
#endif

        return 0;
    }

    int test3()
    {
        using namespace stdex;

        try
        {
            mutex_type m;
            m.lock();
        }
        catch (const system_error& e)
        {
            // Destroying locked mutex raises system error, or undefined.
            // POSIX == may fail with EBUSY.
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
            mutex_type m;
            m.lock();

            // Lock already locked mutex.
            try
            {
                // XXX Will block.
                // m.lock();
            }
            catch (const system_error& e)
            {
                DYNAMIC_VERIFY(false);
            }

            m.unlock();
        }
        catch (const system_error& e)
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
            mutex_type m;
            mutex_type::native_handle_type n = m.native_handle();
            (void) n;
        }
        catch (const system_error& e)
        {
            DYNAMIC_VERIFY(false);
        }
        catch (...)
        {
            DYNAMIC_VERIFY(false);
        }

        return 0;
    }

    int try_lock_test1()
    {
        using namespace stdex;
        
        try
        {
            mutex_type m;
            bool b = m.try_lock();
            DYNAMIC_VERIFY(b);
            m.unlock();
        }
        catch (const system_error& e)
        {
            DYNAMIC_VERIFY(false);
        }
        catch (...)
        {
            DYNAMIC_VERIFY(false);
        }

        return 0;
    }

    int try_lock_test2()
    {
        using namespace stdex;
        
        try
        {
            mutex_type m;
            m.lock();
            bool b;

            thread t(mutex_tests::try_lock_func, mutex_tests::ref(m), mutex_tests::ref(b));
            t.join();
            DYNAMIC_VERIFY(!b);

            m.unlock();
        }
        catch (const system_error& e)
        {
            DYNAMIC_VERIFY(false);
        }
        catch (...)
        {
            DYNAMIC_VERIFY(false);
        }

        return try_lock_func_res;
    }

    int unlock_test1()
    {
        using namespace stdex;
        
        try
        {
            // Unlock mutex that hasn't been locked.
            mutex_type m;
            m.unlock();
        }
        catch (const system_error& e)
        {
            // POSIX == EPERM
            DYNAMIC_VERIFY(true);
        }
        catch (...)
        {
            DYNAMIC_VERIFY(false);
        }

        return 0;
    }

    int unlock_test2()
    {
        using namespace stdex;
        
        thread t1(mutex_tests::f);
        thread t2(mutex_tests::f);
        t1.join();
        t2.join();

        return 0;
    }
}


int main(void)
{
    using namespace stdex;
    using namespace mutex_tests;
    
    RUN_TEST(test1);
    RUN_TEST(test2);
    RUN_TEST(test3);
    RUN_TEST(test4);
    RUN_TEST(test5);
    RUN_TEST(try_lock_test1);
    RUN_TEST(try_lock_test2);
    RUN_TEST(unlock_test1);
    RUN_TEST(unlock_test2);

    return 0;
}

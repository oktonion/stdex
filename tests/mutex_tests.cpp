#include "../stdex/include/core.h"
#include "../stdex/include/system_error.hpp"
#include "../stdex/include/mutex.hpp"
#include "../stdex/include/thread.hpp"

#include <iostream>
#include <string>

#define VERIFY(cond) STATIC_ASSERT((cond), check)
#define DYNAMIC_VERIFY(cond) if(!(cond)) {std::cout << "check condition \'" << #cond << "\' failed at line " << __LINE__ << std::endl; return -1;}
#define RUN_TEST(test) {std::cout << #test << std::endl; int line = test(); if(line != 0) {std::cout << "failed at line " << line << std::endl; return line;}}
#define DYNAMIC_VERIFY_FAIL DYNAMIC_VERIFY( sizeof(false) == 0 )

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

    int try_lock_func_res = 0;

	int try_lock_func(mutex_type &mmm, bool &b)
	{
		try
		{
			b = mmm.try_lock();
		}
		catch (const stdex::system_error&)
		{
            try_lock_func_res = -1;
			DYNAMIC_VERIFY(try_lock_func_res == 0);
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
        catch (const stdex::system_error&)
        {
            DYNAMIC_VERIFY_FAIL;
        }
        catch (...)
        {
            DYNAMIC_VERIFY_FAIL;
        }

        return 0;
    }

    int test2()
    {
        using namespace stdex;

        try
        {
            mutex_type mmm;
            mmm.lock();
        }
        catch (const system_error&)
        {
            // Destroying locked mutex raises system error, or undefined.
            // POSIX == may fail with EBUSY.
        }
        catch (...)
        {
            DYNAMIC_VERIFY_FAIL;
        }

        return 0;
    }

    int test3()
    {
        using namespace stdex;

        int ln = __LINE__;

        try
        {
            ln = __LINE__;
            mutex_type mmm;
            ln = __LINE__;
            mmm.lock();
            ln = __LINE__;

            // Lock already locked mutex.
            try
            {
                // XXX Will block.
                // m.lock();
            }
            catch (const system_error&)
            {
                DYNAMIC_VERIFY_FAIL;
            }

            ln = __LINE__;
            mmm.unlock();
            ln = __LINE__;
        }
        catch (const system_error& e)
        {
            std::cout << "unexpected system error '" << e.what() << "' at line " << ln << std::endl;
            DYNAMIC_VERIFY_FAIL;
        }
        catch (const std::exception& e)
        {
            std::cout << "unexpected exception '" << e.what() << "' at line " << ln << std::endl;
            DYNAMIC_VERIFY_FAIL;
        }
        catch (...)
        {
            std::cout << "unexpected exception at " << ln << std::endl;
            DYNAMIC_VERIFY_FAIL;
        }

        return 0;
    }

    int test4()
    {
        using namespace stdex;

        try
        {
            mutex_type mmm;
            mutex_type::native_handle_type n = mmm.native_handle();
            (void) n;
        }
        catch (const system_error&)
        {
            DYNAMIC_VERIFY_FAIL;
        }
        catch (...)
        {
            DYNAMIC_VERIFY_FAIL;
        }

        return 0;
    }

    int try_lock_test1()
    {
        using namespace stdex;
        
        try
        {
            mutex_type mmm;
            bool b = mmm.try_lock();
            DYNAMIC_VERIFY(b);
            mmm.unlock();
        }
        catch (const system_error&)
        {
            DYNAMIC_VERIFY_FAIL;
        }
        catch (...)
        {
            DYNAMIC_VERIFY_FAIL;
        }

        return 0;
    }

    int try_lock_test2()
    {
        using namespace stdex;
        
        try
        {
            mutex_type mmm;
            mmm.lock();
            bool b;

            thread t(mutex_tests::try_lock_func, mutex_tests::ref(mmm), mutex_tests::ref(b));
            t.join();
            DYNAMIC_VERIFY(!b);

            mmm.unlock();
        }
        catch (const system_error&)
        {
            DYNAMIC_VERIFY_FAIL;
        }
        catch (...)
        {
            DYNAMIC_VERIFY_FAIL;
        }

        return try_lock_func_res;
    }

    int unlock_test1()
    {
        using namespace stdex;
        
        try
        {
            // Unlock mutex that hasn't been locked.
            mutex_type mmm;
            mmm.unlock();
        }
        catch (const system_error&)
        {
            // POSIX == EPERM
            return 0; // may throw - not specified by standard
        }
        catch (...)
        {
            DYNAMIC_VERIFY_FAIL;
        }

        return 0; // may not - not specified by standard
    }

    mutex_type f_m;
    void f(){
        stdex::lock_guard<mutex_type> l(f_m);
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
    RUN_TEST(try_lock_test1);
    RUN_TEST(try_lock_test2);
    RUN_TEST(unlock_test1);
    RUN_TEST(unlock_test2);

    return 0;
}

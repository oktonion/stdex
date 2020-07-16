#include "../stdex/include/core.h"
#include "../stdex/include/system_error.hpp"
#include "../stdex/include/mutex.hpp"
#include "../stdex/include/thread.hpp"

#include <iostream>
#include <string>
#include <cassert>

#define VERIFY(cond) STATIC_ASSERT((cond), check)
#define DYNAMIC_VERIFY(cond) if(!(cond)) {std::cout << "check condition \'" << #cond << "\' failed at line " << __LINE__ << std::endl; return -1;}
#define RUN_TEST(test) {std::cout << #test << std::endl; int line = test(); if(line != 0) {std::cout << "failed at line " << line << std::endl; return line;}}
#define DYNAMIC_VERIFY_FAIL {std::cout << "check condition " << "failed at line " << __LINE__ << std::endl; return -1;}

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

    mutex_type f_m;
    void f(){
        stdex::lock_guard<mutex_type> l(f_m);
    }

    int unlock_test1()
    {
        using namespace stdex;
        
        thread t1(mutex_tests::f);
        thread t2(mutex_tests::f);
        t1.join();
        t2.join();

        return 0;
    }
}

int lock_test1()
{
    typedef stdex::mutex mutex_type;
    typedef stdex::unique_lock<mutex_type> lock_type;

    try
    {
        mutex_type m1, m2, m3;
        lock_type l1(m1, stdex::defer_lock), 
        l2(m2, stdex::defer_lock),
        l3(m3, stdex::defer_lock);

        try
        {
            stdex::lock(l1, l2, l3);
            DYNAMIC_VERIFY( l1.owns_lock() );
            DYNAMIC_VERIFY( l2.owns_lock() );
            DYNAMIC_VERIFY( l3.owns_lock() );
        }
        catch (const stdex::system_error&)
        {
            DYNAMIC_VERIFY_FAIL;
        }
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

stdex::mutex line_err_m;
std::size_t line_err = 0;

int locker(stdex::mutex& m1, stdex::mutex& m2, stdex::mutex& m3)
{
  typedef stdex::unique_lock<stdex::mutex> lock_type;

  lock_type l1(m1, stdex::defer_lock);
  lock_type l2(m2, stdex::defer_lock);
  lock_type l3(m3, stdex::defer_lock);
  stdex::lock(l1, l2, l3);
  DYNAMIC_VERIFY( l1.owns_lock() );
  DYNAMIC_VERIFY( l2.owns_lock() );
  DYNAMIC_VERIFY( l3.owns_lock() );

  return 0;
}

void locker_wrap(stdex::mutex& m1, stdex::mutex& m2, stdex::mutex& m3)
{
    int ln = locker(m1, m2, m3);

    stdex::unique_lock<stdex::mutex> lk(line_err_m);
    if(line_err == 0)
        line_err = ln;
}


int lock_test2()
{
  stdex::mutex m1, m2, m3;
  stdex::thread t1(locker_wrap, mutex_tests::ref(m1), mutex_tests::ref(m2), mutex_tests::ref(m3));
  stdex::thread t2(locker_wrap, mutex_tests::ref(m3), mutex_tests::ref(m2), mutex_tests::ref(m1));
  t1.join();
  t2.join();

  DYNAMIC_VERIFY( line_err == 0 );

  return 0;
}

struct user_lock
{
  user_lock() : is_locked(false) { }

  void lock()
  {
    assert( !is_locked );
    is_locked = true;
  }

  bool try_lock() 
  { return is_locked ? false : (is_locked = true); }

  void unlock()
  {
    assert( is_locked );
    is_locked = false;
  }

private:
  bool is_locked;
};

int lock_test3()
{
    try
    {
        stdex::mutex m1;
        stdex::recursive_mutex m2;
        user_lock m3;

        try
        {
            //heterogeneous types
            stdex::lock(m1, m2, m3);
            m1.unlock();
            m2.unlock();
            m3.unlock();
        }
        catch (const stdex::system_error&)
        {
            DYNAMIC_VERIFY_FAIL;
        }
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

struct unreliable_lock
{
    stdex::mutex m;
    stdex::unique_lock<stdex::mutex> l;

    static int count;
    static int throw_on;
    static int lock_on;

    unreliable_lock() : l(m, stdex::defer_lock) { }

    ~unreliable_lock()
    {
        assert( !l.owns_lock() );
    }

    void lock()
    {
        if (count == throw_on)
        throw throw_on;
        ++count;
        l.lock();
    }
    bool try_lock()
    {
        if (count == throw_on)
        throw throw_on;
        stdex::unique_lock<stdex::mutex> l2(m, stdex::defer_lock);
        if (count == lock_on)
        l2.lock();
        ++count;
        return l.try_lock();
    }

    void unlock()
    {
        assert( l.owns_lock() );
        l.unlock();
    }

};

int unreliable_lock::count = 0;
int unreliable_lock::throw_on = -1;
int unreliable_lock::lock_on = -1;

int lock_test4()
{
    unreliable_lock l1, l2, l3;

    try
    {
        unreliable_lock::count = 0;
        stdex::lock(l1, l2, l3);
        DYNAMIC_VERIFY( unreliable_lock::count == 3 );
        l1.unlock();
        l2.unlock();
        l3.unlock();
    }
    catch (...)
    {
        DYNAMIC_VERIFY_FAIL;
    }

    return 0;
}

int lock_test5()
{
    // test behaviour when a lock is already held
    try
    {
        unreliable_lock::lock_on = 1;
        while (unreliable_lock::lock_on < 3)
        {
            unreliable_lock::count = 0;
            unreliable_lock l1, l2, l3;
            stdex::lock(l1, l2, l3);
            DYNAMIC_VERIFY( unreliable_lock::count > 3 );
            l1.unlock();
            l2.unlock();
            l3.unlock();
            ++unreliable_lock::lock_on;
        }
    }
    catch (...)
    {
        DYNAMIC_VERIFY_FAIL;
    }

    return 0;
}

int lock_test6()
{
  // test behaviour when an exception is thrown
  unreliable_lock::throw_on = 0;
  while (unreliable_lock::throw_on < 3)
  {
    unreliable_lock::count = 0;
    unreliable_lock l1, l2, l3;
    bool test = false;
    try
    {
        stdex::lock(l1, l2, l3);
    }
    catch (...)
    {
        test = true;
    }
    DYNAMIC_VERIFY( test );
    ++unreliable_lock::throw_on;
  }

  return 0;
}

int main(void)
{
    {
        // require

        typedef stdex::mutex                  mutex;
        typedef stdex::timed_mutex            timed_mutex;
        typedef stdex::recursive_mutex        recursive_mutex;
        typedef stdex::recursive_timed_mutex  recursive_timed_mutex;
        typedef stdex::lock_guard<mutex>      lock_guard;
        typedef stdex::unique_lock<mutex>     unique_lock;
        //typedef stdex::scoped_lock<mutex>     scoped_lock; // c++ 17 - ni
        
        typedef stdex::defer_lock_t           defer_lock_t;
        typedef stdex::try_to_lock_t          try_to_lock_t;
        typedef stdex::adopt_lock_t           adopt_lock_t;
        
        using stdex::defer_lock;
        using stdex::try_to_lock;
        using stdex::adopt_lock;
        
        //typedef stdex::once_flag              once_flag; // ni

        using stdex::try_lock;
        using stdex::lock;
        //using stdex::call_once; // ni
    }

    using namespace stdex;
    using namespace mutex_tests;
    
    RUN_TEST(test1);
    RUN_TEST(test2);
    RUN_TEST(test3);
    RUN_TEST(test4);
    RUN_TEST(try_lock_test1);
    RUN_TEST(try_lock_test2);
    RUN_TEST(unlock_test1);
    RUN_TEST(lock_test1);
    RUN_TEST(lock_test2);
    RUN_TEST(lock_test3);
    RUN_TEST(lock_test4);
    RUN_TEST(lock_test5);
    RUN_TEST(lock_test6);

    return 0;
}

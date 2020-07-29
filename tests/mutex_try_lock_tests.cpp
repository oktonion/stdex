#include "../stdex/include/core.h"
#include "../stdex/include/system_error.hpp"
#include "../stdex/include/mutex.hpp"
#include "../stdex/include/thread.hpp"

#include <iostream>
#include <string>

#define VERIFY(cond) STATIC_ASSERT((cond), check)
#define DYNAMIC_VERIFY(cond) if(!(cond)) {std::cout << "check condition \'" << #cond << "\' failed at line " << __LINE__ << std::endl; return -1;}
#define RUN_TEST(test) {std::cout << #test << std::endl; int line = test(); if(line != 0) {std::cout << "failed at line " << line << std::endl; return line;}}
#define DYNAMIC_VERIFY_FAIL {std::cout << "check condition " << "failed at line " << __LINE__ << std::endl; return -1;}
#define DYNAMIC_VERIFY_ABORT(cond) if(!(cond)) {std::cout << "check condition \'" << #cond << "\' failed at line " << __LINE__ << std::endl; std::abort();}

struct user_lock
{
  user_lock() : is_locked(false) { }

  void lock()
  {
    DYNAMIC_VERIFY_ABORT( !is_locked );
    is_locked = true;
  }

  bool try_lock() 
  { return is_locked ? false : (is_locked = true); }

  void unlock()
  {
    DYNAMIC_VERIFY_ABORT( is_locked );
    is_locked = false;
  }

private:
  bool is_locked;
};

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
        DYNAMIC_VERIFY_ABORT(!l.owns_lock());
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
        DYNAMIC_VERIFY_ABORT( l.owns_lock() );
        l.unlock();
    }

};

int unreliable_lock::count = 0;
int unreliable_lock::throw_on = -1;
int unreliable_lock::lock_on = -1;


int try_lock_test1()
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
            int result = stdex::try_lock(l1, l2, l3);
            DYNAMIC_VERIFY( result == -1 );
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

typedef stdex::unique_lock<stdex::mutex> lock_type;

int try_lock_test2()
{
    try
    {
        stdex::mutex m1, m2, m3;
        lock_type l1(m1);
        int result = stdex::try_lock(m1, m2, m3);
        DYNAMIC_VERIFY( result == 0 );
        DYNAMIC_VERIFY( l1.owns_lock() );
        lock_type l2(m2);
        lock_type l3(m3);
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

int try_lock_test3()
{
    try
    {
        stdex::mutex m1, m2, m3;
        lock_type l2(m2);
        int result = stdex::try_lock(m1, m2, m3);
        DYNAMIC_VERIFY( result == 1 );
        DYNAMIC_VERIFY( l2.owns_lock() );
        lock_type l1(m1);
        lock_type l3(m3);
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

int try_lock_test4()
{
    try
    {
        stdex::mutex m1, m2, m3;
        lock_type l3(m3);
        int result = stdex::try_lock(m1, m2, m3);
        DYNAMIC_VERIFY( result == 2 );
        DYNAMIC_VERIFY( l3.owns_lock() );
        lock_type l1(m1);
        lock_type l2(m2);
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

int try_lock_test5()
{
    try
    {
        stdex::mutex m1;
        stdex::recursive_mutex m2;
        user_lock m3;

        try
        {
            //heterogeneous types
            int result = stdex::try_lock(m1, m2, m3);
            DYNAMIC_VERIFY( result == -1 );
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

int try_lock_test6()
{
    unreliable_lock l1, l2, l3;

    try
    {
        unreliable_lock::count = 0;
        int result = stdex::try_lock(l1, l2, l3);
        DYNAMIC_VERIFY( result == -1 );
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

int try_lock_test7()
{
    unreliable_lock l1, l2, l3;

    try
    {
        // test behaviour when a lock is already held
        unreliable_lock::lock_on = 0;
        while (unreliable_lock::lock_on < 3)
        {
            unreliable_lock::count = 0;
            int failed = stdex::try_lock(l1, l2, l3);
            DYNAMIC_VERIFY( failed == unreliable_lock::lock_on );
            ++unreliable_lock::lock_on;
        }
    }
    catch (...)
    {
        DYNAMIC_VERIFY_FAIL;
    }

    return 0;
}

int try_lock_test8()
{
    unreliable_lock l1, l2, l3;

    try
    {
        // test behaviour when an exception is thrown
        unreliable_lock::throw_on = 0;
        while (unreliable_lock::throw_on < 3)
        {
            unreliable_lock::count = 0;
            try
            {
                stdex::try_lock(l1, l2, l3);
                DYNAMIC_VERIFY_FAIL;
            }
            catch (int e)
            {
                DYNAMIC_VERIFY( e == unreliable_lock::throw_on );
            }
            ++unreliable_lock::throw_on;
        }
    }
    catch (...)
    {
        DYNAMIC_VERIFY_FAIL;
    }
    return 0;
}

int try_lock_test9()
{
    unreliable_lock l1, l2, l3, l4, l5, l6, l7, l8, l9, l10, l11, l12, l13, l14, l15, l16, l17, l18, l19, l20, l21, l22, l23, l24;

    try
    {
        // test behaviour when an exception is thrown
        unreliable_lock::throw_on = 0;
        while (unreliable_lock::throw_on < 24)
        {
            unreliable_lock::count = 0;
            try
            {
                stdex::try_lock(l1, l2, l3, l4, l5, l6, l7, l8, l9, l10, l11, l12, l13, l14, l15, l16, l17, l18, l19, l20, l21, l22, l23, l24);
                DYNAMIC_VERIFY(unreliable_lock::throw_on >= 4);
            }
            catch (int e)
            {
                DYNAMIC_VERIFY( e == unreliable_lock::throw_on );
            }
            ++unreliable_lock::throw_on;
        }
    }
    catch (...)
    {
        DYNAMIC_VERIFY_FAIL;
    }
    return 0;
}

int main(void)
{
    using namespace stdex;
    
    RUN_TEST(try_lock_test1);
    RUN_TEST(try_lock_test2);
    RUN_TEST(try_lock_test3);
    RUN_TEST(try_lock_test4);
    RUN_TEST(try_lock_test5);
    RUN_TEST(try_lock_test6);
    RUN_TEST(try_lock_test7);
    RUN_TEST(try_lock_test8);
    RUN_TEST(try_lock_test9);

    return 0;
}

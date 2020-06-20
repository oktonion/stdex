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

namespace recursive_mutex_tests
{
    typedef stdex::recursive_mutex mutex_type;

    int test1()
    {
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
        try 
        {
            mutex_type m;
            m.lock();
        }
        catch (const stdex::system_error&)
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
        try 
        {
            mutex_type m;
            m.lock();

            // Lock already locked recursive mutex.
            try
            {
                // XXX Will not block.
                m.lock();
            }
            catch (const stdex::system_error&)
            {
                DYNAMIC_VERIFY_FAIL;
            }

            m.unlock();
            m.unlock();
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

    int test4()
    {
        try 
        {
            mutex_type m;
            mutex_type::native_handle_type n = m.native_handle();
            (void)(&n);
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

    int test5()
    {
        try 
        {
            mutex_type m;
            mutex_type::native_handle_type n = m.native_handle();
            (void)(&n);
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

    int try_lock_test1()
    {
        try 
        {
            mutex_type m;
            bool b = m.try_lock();
            DYNAMIC_VERIFY( b );
            m.unlock();
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

    int try_lock_test2()
    {
        try 
        {
            mutex_type m;
            m.lock();
            bool b;

            try
            {
                b = m.try_lock();
                DYNAMIC_VERIFY( b );
                m.unlock();
            }
            catch (const stdex::system_error&)
            {
                DYNAMIC_VERIFY_FAIL;
            }
            catch(...)
            {
                DYNAMIC_VERIFY_FAIL;
            }

            m.unlock();
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

    int unlock_test1()
    {
        try 
        {
            // Unlock mutex that hasn't been locked.
            mutex_type m;
            m.unlock();
        }
        catch (const stdex::system_error&)
        {
            // POSIX == EPERM
            DYNAMIC_VERIFY(true);
        }
        catch (...)
        {
            DYNAMIC_VERIFY_FAIL;
        }

        return 0;
    }

    mutex_type m;

    void f()
    {
        stdex::lock_guard<mutex_type> l(m);
    }

    int unlock_test2()
    {
        stdex::thread t1(f);
        stdex::thread t2(f);
        t1.join();
        t2.join();

        return 0;
    }
}

int main(void)
{
    using namespace stdex;
    using namespace recursive_mutex_tests;

    typedef stdex::recursive_mutex test_type;
    typedef test_type::native_handle_type type;
    
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
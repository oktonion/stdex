#include "../stdex/include/core.h"
#include "../stdex/include/system_error.hpp"
#include "../stdex/include/mutex.hpp"
#include "../stdex/include/thread.hpp"
#include "../stdex/include/chrono.hpp"

#include <iostream>
#include <string>

#define VERIFY(cond) STATIC_ASSERT((cond), check)
#define DYNAMIC_VERIFY(cond) if(!(cond)) {std::cout << "check condition \'" << #cond << "\' failed at line " << __LINE__ << std::endl; return -1;}
#define RUN_TEST(test) {std::cout << #test << std::endl; int line = test(); if(line != 0) {std::cout << "failed at line " << line << std::endl; return line;}}
#define DYNAMIC_VERIFY_FAIL {std::cout << "check condition " << "failed at line " << __LINE__ << std::endl; return -1;}

namespace timed_mutex_tests
{
    typedef stdex::timed_mutex mutex_type;

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
                //m.lock();
            }
            catch (const stdex::system_error&)
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
                DYNAMIC_VERIFY( !b );
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

    int try_lock_for_test1()
    {
        try 
        {
            mutex_type m;
            bool b = m.try_lock_for(stdex::chrono::seconds(1));
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

    int try_lock_for_test2()
    {
        typedef stdex::chrono::duration<stdex::int32_t, stdex::nano> nanoseconds;

        try 
        {
            mutex_type m;
            bool b = m.try_lock_for(nanoseconds(1));
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

    int try_lock_for_test3()
    {
        stdex::chrono::seconds s(1);

        try 
        {
            mutex_type m;
            m.lock();
            bool b;

            try
            {
                b = m.try_lock_for(s);
                DYNAMIC_VERIFY( !b );
            }
            catch (const stdex::system_error&)
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

    int try_lock_until_test1()
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

    int try_lock_until_test2()
    {
        try 
        {
            mutex_type m;
            m.lock();
            bool b;

            try
            {
                b = m.try_lock();
                DYNAMIC_VERIFY( !b );
            }
            catch (const stdex::system_error&)
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

    //template <class clock_type>
    struct try_lock_until_test3_pred{
        typedef stdex::chrono::milliseconds milliseconds;
        typedef stdex::chrono::system_clock clock_type;
        try_lock_until_test3_pred(mutex_type &m_): m(m_)
        {}

        int test_func() const
        {
            try
            { 
                const milliseconds timeout = milliseconds(100);
                const typename clock_type::time_point start = clock_type::now();
                const bool b = m.try_lock_until(start + timeout);
                const typename clock_type::duration t = clock_type::now() - start;
                
                DYNAMIC_VERIFY( !b );
                std::cout << stdex::chrono::duration_cast<milliseconds>(t).count() << " >= " << timeout.count() << std::endl;
                DYNAMIC_VERIFY( t >= timeout );
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

        void operator()() const
        {
            const int res = test_func();
            if(0 != res)
                throw("");
        }

        mutex_type &m;
    };

    template <class clock_type>
    int try_lock_until_test3()
    {
        
        try
        {
            mutex_type m;

            m.lock();

            try_lock_until_test3_pred<clock_type> 
                pred(m);

            stdex::thread thr(pred);
            thr.join();
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

    template <class clock_type>
    int try_lock_until_test4()
    {
        try
        {
            using namespace stdex::chrono;
            mutex_type m;

            // Confirm that try_lock_until acts like try_lock if the timeout has
            // already passed.

            // First test with a timeout that is definitely in the past.
            DYNAMIC_VERIFY( m.try_lock_until( clock_type::now() - milliseconds(1) ) );
            m.unlock();

            // Then attempt to test with a timeout that might exactly match the
            // current time.
            DYNAMIC_VERIFY( m.try_lock_until( clock_type::now() ) );
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

    namespace C = stdex::chrono;

    // custom clock with epoch 10s before system_clock's
    struct clock
    {
        typedef C::system_clock::rep rep;
        typedef C::system_clock::period period;
        typedef C::system_clock::duration duration;
        typedef C::time_point<clock> time_point;
        static const bool is_steady;

        static time_point now()
        {
            C::system_clock::duration sys_time = C::system_clock::now().time_since_epoch();
            return time_point(sys_time + C::seconds(10));
        }
    };

    const bool clock::is_steady = C::system_clock::is_steady;

    stdex::timed_mutex mx;
    bool locked = false;

    template <class ClockType>
    void f()
    {
        locked = mx.try_lock_until(ClockType::now() + C::milliseconds(1));
    }

    template <class ClockType>
    int try_lock_until_test5()
    {
        //typedef C::system_clock ClockType;
        stdex::lock_guard<stdex::timed_mutex> l(mx);
        typename ClockType::time_point start = ClockType::now();
        void (&_f)() = f<ClockType>;
        stdex::thread t(&_f);
        t.join();
        typename ClockType::time_point stop = ClockType::now();
        DYNAMIC_VERIFY( (stop - start) < C::seconds(9) );
        DYNAMIC_VERIFY( !locked );

        return 0;
    }

    mutex_type m;

    void ff()
    {
        stdex::lock_guard<mutex_type> l(m);
    }

    int unlock_test1()
    {
        stdex::thread t1(ff);
        stdex::thread t2(ff);
        t1.join();
        t2.join();

        return 0;
    }
}

int main(void)
{
    using namespace stdex;
    using namespace timed_mutex_tests;

    typedef stdex::timed_mutex test_type;
    typedef test_type::native_handle_type type;
    
    RUN_TEST(test1);
    RUN_TEST(test2);
    RUN_TEST(test3);
    RUN_TEST(test4);
    RUN_TEST(try_lock_test1);
    RUN_TEST(try_lock_test2);
    RUN_TEST(try_lock_for_test1);
    RUN_TEST(try_lock_for_test2);
    RUN_TEST(try_lock_for_test3);
    RUN_TEST(try_lock_until_test1);
    RUN_TEST(try_lock_until_test2);
    RUN_TEST(try_lock_until_test3<stdex::chrono::system_clock>);
    RUN_TEST(try_lock_until_test3<stdex::chrono::steady_clock>);
    RUN_TEST(try_lock_until_test4<stdex::chrono::system_clock>);
    RUN_TEST(try_lock_until_test4<stdex::chrono::steady_clock>);
    RUN_TEST(try_lock_until_test5<stdex::chrono::system_clock>);
    RUN_TEST(try_lock_until_test5<stdex::chrono::steady_clock>);
    RUN_TEST(unlock_test1);

    return 0;
}
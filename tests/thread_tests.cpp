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
#define DYNAMIC_VERIFY_ABORT(cond) if(!(cond)) {std::cout << "check condition \'" << #cond << "\' failed at line " << __LINE__ << std::endl; std::abort();}
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
        id = stdex::this_thread::get_id();
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
        DYNAMIC_VERIFY_FAIL ;
    }
    catch (const char *)
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
        DYNAMIC_VERIFY_FAIL ;
    }
    catch (const char *)
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
        DYNAMIC_VERIFY_FAIL ;
    }
    catch (const char *)
    {
        DYNAMIC_VERIFY_FAIL ;
    }
    catch (...)
    {
        DYNAMIC_VERIFY_FAIL ;
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
        DYNAMIC_VERIFY_FAIL ;
    }
    catch (const char *)
    {
        DYNAMIC_VERIFY_FAIL ;
    }
    catch (...)
    {
        DYNAMIC_VERIFY_FAIL ;
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

    {
        thread t1(thread_func7, 1, 1, 1, 1.f, 1, nullptr, nullptr);

        t1.join();
    }

    {
        thread t1(thread_func8, nullptr, nullptr, nullptr, nullptr);

        t1.join();
    }

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

    (void)(&res);

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
        DYNAMIC_VERIFY_FAIL ;
    }
    catch (const char *)
    {
        DYNAMIC_VERIFY_FAIL ;
    }
    catch (...)
    {
        DYNAMIC_VERIFY_FAIL ;
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
        DYNAMIC_VERIFY_FAIL ;
    }
    catch (const char *)
    {
        DYNAMIC_VERIFY_FAIL ;
    }
    catch (...)
    {
        DYNAMIC_VERIFY_FAIL ;
    }
    return 0;
}

#define CHECK_FOR_THROW_EVENTS 1





void dummy_func_0() {}
void dummy_func_1(void*) {}
void dummy_func_2(void*, int*) {}
void dummy_func_3(void*, double*, float*) {}
void dummy_func_4(struct dummy_func_4_t0*, struct dummy_func_4_t1*, struct dummy_func_4_t2*, struct dummy_func_4_t3*) {}
void dummy_func_5(struct dummy_func_5_t0*, struct dummy_func_5_t1*, struct dummy_func_5_t2*, struct dummy_func_5_t3*, struct dummy_func_5_t4*) {}
void dummy_func_6(struct dummy_func_6_t0*, struct dummy_func_6_t1*, struct dummy_func_6_t2 const*, struct dummy_func_6_t3*, struct dummy_func_6_t4*, struct dummy_func_6_t5 const*) {}
void dummy_func_7(struct dummy_func_7_t0*, struct dummy_func_7_t1*, struct dummy_func_7_t2 const*, struct dummy_func_7_t3*, struct dummy_func_7_t4*, struct dummy_func_7_t5 const*, struct dummy_func_7_t6 const*) {}
void dummy_func_8(struct dummy_func_8_t0*, struct dummy_func_8_t1*, struct dummy_func_8_t2 const*, struct dummy_func_8_t3*, struct dummy_func_8_t4*, struct dummy_func_8_t5 const*, struct dummy_func_8_t6 const**, void*) {}

struct dummy_functor{
    int operator()() {return 0;}
    void operator()(void* arg1) {
        DYNAMIC_VERIFY_ABORT(arg1 == 0);
    }
    void operator()(void* arg1, int* arg2) {
        DYNAMIC_VERIFY_ABORT(arg1 == 0);
        DYNAMIC_VERIFY_ABORT(arg2 == 0);
    }
    int operator()(void* arg1, double* arg2, float* arg3) {
        DYNAMIC_VERIFY_ABORT(arg1 == 0);
        DYNAMIC_VERIFY_ABORT(arg2 == 0);
        DYNAMIC_VERIFY_ABORT(arg3 == 0);
        return 0;
    }
    void operator()(
        struct dummy_func_4_t0* arg1, 
        struct dummy_func_4_t1* arg2, 
        struct dummy_func_4_t2* arg3, 
        struct dummy_func_4_t3* arg4) {
        DYNAMIC_VERIFY_ABORT(arg1 == 0);
        DYNAMIC_VERIFY_ABORT(arg2 == 0);
        DYNAMIC_VERIFY_ABORT(arg3 == 0);
        DYNAMIC_VERIFY_ABORT(arg4 == 0);
    }
    void operator()(
        struct dummy_func_5_t0* arg1, 
        struct dummy_func_5_t1* arg2, 
        struct dummy_func_5_t2* arg3, 
        struct dummy_func_5_t3* arg4, 
        struct dummy_func_5_t4* arg5) {
        DYNAMIC_VERIFY_ABORT(arg1 == 0);
        DYNAMIC_VERIFY_ABORT(arg2 == 0);
        DYNAMIC_VERIFY_ABORT(arg3 == 0);
        DYNAMIC_VERIFY_ABORT(arg4 == 0);
        DYNAMIC_VERIFY_ABORT(arg5 == 0);
    }
    float operator()(
        struct dummy_func_6_t0* arg1, 
        struct dummy_func_6_t1* arg2, 
        struct dummy_func_6_t2 const* arg3, 
        struct dummy_func_6_t3* arg4, 
        struct dummy_func_6_t4* arg5, 
        struct dummy_func_6_t5 const* arg6) {
            DYNAMIC_VERIFY_ABORT(arg1 == 0);
            DYNAMIC_VERIFY_ABORT(arg2 == 0);
            DYNAMIC_VERIFY_ABORT(arg3 == 0);
            DYNAMIC_VERIFY_ABORT(arg4 == 0);
            DYNAMIC_VERIFY_ABORT(arg5 == 0);
            DYNAMIC_VERIFY_ABORT(arg6 == 0);
            return 0.f;
        }
    void operator()(
        struct dummy_func_7_t0* arg1, 
        struct dummy_func_7_t1* arg2, 
        struct dummy_func_7_t2 const* arg3, 
        struct dummy_func_7_t3* arg4, 
        struct dummy_func_7_t4* arg5, 
        struct dummy_func_7_t5 const* arg6, 
        struct dummy_func_7_t6 const* arg7) {
            DYNAMIC_VERIFY_ABORT(arg1 == 0);
            DYNAMIC_VERIFY_ABORT(arg2 == 0);
            DYNAMIC_VERIFY_ABORT(arg3 == 0);
            DYNAMIC_VERIFY_ABORT(arg4 == 0);
            DYNAMIC_VERIFY_ABORT(arg5 == 0);
            DYNAMIC_VERIFY_ABORT(arg6 == 0);
            DYNAMIC_VERIFY_ABORT(arg7 == 0);
        }
    void operator()(
        struct dummy_func_8_t0* arg1, 
        struct dummy_func_8_t1* arg2, 
        struct dummy_func_8_t2 const* arg3, 
        struct dummy_func_8_t3* arg4, 
        struct dummy_func_8_t4* arg5, 
        struct dummy_func_8_t5 const* arg6, 
        struct dummy_func_8_t6 const** arg7, 
        void* arg8) {
            DYNAMIC_VERIFY_ABORT(arg1 == 0);
            DYNAMIC_VERIFY_ABORT(arg2 == 0);
            DYNAMIC_VERIFY_ABORT(arg3 == 0);
            DYNAMIC_VERIFY_ABORT(arg4 == 0);
            DYNAMIC_VERIFY_ABORT(arg5 == 0);
            DYNAMIC_VERIFY_ABORT(arg6 == 0);
            DYNAMIC_VERIFY_ABORT(arg7 == 0);
            DYNAMIC_VERIFY_ABORT(arg8 == 0);
        }

};

struct dummy_functor2
{
    void operator()(
        int arg1, long arg2, std::ptrdiff_t arg3, short arg4,
        unsigned int arg5, unsigned long arg6, unsigned short arg7)
    {
        DYNAMIC_VERIFY_ABORT(arg1 != 0);
        DYNAMIC_VERIFY_ABORT(arg2 != 0);
        DYNAMIC_VERIFY_ABORT(arg3 != 0);
        DYNAMIC_VERIFY_ABORT(arg4 != 0);
        DYNAMIC_VERIFY_ABORT(arg5 != 0);
        DYNAMIC_VERIFY_ABORT(arg6 != 0);
        DYNAMIC_VERIFY_ABORT(arg7 != 0);
    }
};

int test13()
{
    using namespace stdex;

    {
        thread tt(&dummy_func_0);
        tt.join();
    }
    {
        dummy_functor ff;
        thread tt(ff);
        tt.join();
    }

    {
        thread tt(&dummy_func_1, nullptr);
        tt.join();
    }
    {
        dummy_functor ff;
        thread tt(ff, nullptr);
        tt.join();
    }

    {
        thread tt(&dummy_func_2, nullptr, nullptr);
        tt.join();
    }
    {
        dummy_functor ff;
        thread tt(ff, nullptr, nullptr);
        tt.join();
    }

    {
        thread tt(&dummy_func_3, nullptr, nullptr, nullptr);
        tt.join();
    }
    {
        dummy_functor ff;
        thread tt(ff, nullptr, nullptr, nullptr);
        tt.join();
    }
    
    {
        thread tt(&dummy_func_4, nullptr, nullptr, nullptr, nullptr);
        tt.join();
    }
    {
        dummy_functor ff;
        thread tt(ff, nullptr, nullptr, nullptr, nullptr);
        tt.join();
    }
    
    {
        thread tt(&dummy_func_5, nullptr, nullptr, nullptr, nullptr, nullptr);
        tt.join();
    }
    {
        dummy_functor ff;
        thread tt(ff, nullptr, nullptr, nullptr, nullptr, nullptr);
        tt.join();
    }
    
    {
        thread tt(&dummy_func_6, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
        tt.join();
    }
    {
        dummy_functor ff;
        thread tt(ff, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
        tt.join();
    }
    
    {
        thread tt(&dummy_func_7, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
        tt.join();
    }
    {
        dummy_functor ff;
        thread tt(ff, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
        tt.join();
    }
    
    {
        thread tt(&dummy_func_8, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
        tt.join();
    }
    {
        dummy_functor ff;
        thread tt(ff, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
        tt.join();
    }

    // lambdas
    {
        struct lambdas
        {
            static void call(
                int arg1, long arg2, std::ptrdiff_t arg3, short arg4,
                unsigned int arg5, unsigned long arg6, unsigned short arg7)
            {
                DYNAMIC_VERIFY_ABORT(arg1 != 0);
                DYNAMIC_VERIFY_ABORT(arg2 != 0);
                DYNAMIC_VERIFY_ABORT(arg3 != 0);
                DYNAMIC_VERIFY_ABORT(arg4 != 0);
                DYNAMIC_VERIFY_ABORT(arg5 != 0);
                DYNAMIC_VERIFY_ABORT(arg6 != 0);
                DYNAMIC_VERIFY_ABORT(arg7 != 0);
            }
        };
        thread tt(&lambdas::call, 1, 2, 3, 4, 5, 6, 7);
        tt.join();
    }

    {
        struct lambdas
        {
            static void call(
                int arg1, long arg2, std::ptrdiff_t arg3, short arg4,
                unsigned int arg5, unsigned long arg6, unsigned short arg7)
            {
                DYNAMIC_VERIFY_ABORT(arg1 == 0);
                DYNAMIC_VERIFY_ABORT(arg2 == 1);
                DYNAMIC_VERIFY_ABORT(arg3 == 2);
                DYNAMIC_VERIFY_ABORT(arg4 == 3);
                DYNAMIC_VERIFY_ABORT(arg5 == 4);
                DYNAMIC_VERIFY_ABORT(arg6 == 0);
                DYNAMIC_VERIFY_ABORT(arg7 == 5);
            }
        };
        thread tt(&lambdas::call, 0, 1, 2, 3, 4, 0, 5);
        tt.join();
    }

    {
        struct lambdas
        {
            static void call(
                int arg1, long arg2, std::ptrdiff_t arg3, short arg4,
                unsigned int arg5, unsigned long arg6, unsigned short arg7, void *arg8)
            {
                DYNAMIC_VERIFY_ABORT(arg1 == 1);
                DYNAMIC_VERIFY_ABORT(arg2 == 2);
                DYNAMIC_VERIFY_ABORT(arg3 == 3);
                DYNAMIC_VERIFY_ABORT(arg4 == 4);
                DYNAMIC_VERIFY_ABORT(arg5 == 5);
                DYNAMIC_VERIFY_ABORT(arg6 == 6);
                DYNAMIC_VERIFY_ABORT(arg7 == 7);
                DYNAMIC_VERIFY_ABORT(arg8);
            }
        };
        char wild_ptr[1];
        thread tt(
            &lambdas::call, 
            int(1), long(2), 
            std::ptrdiff_t(3), 
            short(4), 
            (unsigned int)(5), 
            (unsigned long)(6), 
            (unsigned short)(7), 
            static_cast<void*>(&wild_ptr[0]));
        tt.join();
    }
#ifdef _STDEX_FUNCTIONAL_H // disable current test for now till <functional.hpp> is ready
    {
        // for some reason GCC can not use local class as functor for templated thread constructor
        // so we have to improvise
        typedef dummy_functor2 dummy_functor_local;
        dummy_functor_local ff;

        thread tt(ff, 1, 2, 3, 4, 5, 6, 7);
        tt.join();

        thread tt2(ff, int(1), long(2), std::ptrdiff_t(3), short(4), (unsigned int)(5), (unsigned long)(6), (unsigned short)(7));
        tt2.join();
    }
#endif

    return 0;
}



stdex::chrono::milliseconds standard_sleep_for_accuracy()
{
#if defined(_STDEX_NATIVE_CPP11_SUPPORT) || defined(__MACH__)
    namespace clocks = std::chrono;
    typedef clocks::steady_clock clock_type;

    clock_type::time_point begin = clock_type::now();

    std::this_thread::sleep_for(clocks::milliseconds(25000));

    clocks::milliseconds dur_25000_ms = 
        clocks::duration_cast<clocks::milliseconds>(clock_type::now() - begin);


    begin = clock_type::now();

    for(std::size_t i = 0; i < 100; ++i)
        std::this_thread::sleep_for(clocks::milliseconds(250));
    
    clocks::milliseconds dur_250x100_ms = 
        clocks::duration_cast<clocks::milliseconds>(clock_type::now() - begin);
    
    return (dur_250x100_ms - dur_25000_ms).count();
#else
    return 1500;
#endif
}

stdex::chrono::milliseconds stdex_sleep_for_accuracy()
{
#if defined(_STDEX_NATIVE_CPP11_SUPPORT) || defined(__MACH__)
    namespace clocks = std::chrono;
#else
    namespace clocks = stdex::chrono;
#endif
    typedef clocks::steady_clock clock_type;

    clock_type::time_point begin = clock_type::now();

    stdex::this_thread::sleep_for(stdex::chrono::milliseconds(25000));

    clocks::milliseconds dur_25000_ms = 
        clocks::duration_cast<clocks::milliseconds>(clock_type::now() - begin);


    begin = clock_type::now();

    for(std::size_t i = 0; i < 100; ++i)
        stdex::this_thread::sleep_for(stdex::chrono::milliseconds(250));
    
    clocks::milliseconds dur_250x100_ms = 
        clocks::duration_cast<clocks::milliseconds>(clock_type::now() - begin);
    
    return (dur_250x100_ms - dur_25000_ms).count();
}

int check_stdex__sleep_for__accuracy()
{
    stdex::chrono::milliseconds 
        average_std_sleep_for_acc,
        average_stdex_sleep_for_acc;

    const std::size_t avrg_n = 5;

    for(std::size_t i = 0; i < avrg_n; ++i)
        average_std_sleep_for_acc += standard_sleep_for_accuracy();
    
    average_std_sleep_for_acc /= avrg_n;

    for(std::size_t i = 0; i < avrg_n; ++i)
        average_stdex_sleep_for_acc += stdex_sleep_for_accuracy();
    
    average_stdex_sleep_for_acc /= avrg_n;
    std::size_t acceptable_delta = 
        average_std_sleep_for_acc.count()*0.95;

    DYNAMIC_VERIFY(
        (average_stdex_sleep_for_acc.count() <=  (average_std_sleep_for_acc.count() + acceptable_delta)) ? 
            true : 
            (std::cout << "CONDITION: \n" << average_stdex_sleep_for_acc.count() << " <= " << average_std_sleep_for_acc.count() << " + " << acceptable_delta << "\n Failed" <<  std::endl, false)
    );

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
        RUN_TEST(test8);
        RUN_TEST(test9);
        //RUN_TEST(test10);
        //RUN_TEST(test11);
        //RUN_TEST(test12);
        RUN_TEST(test13);
        
    }

    DYNAMIC_VERIFY(thread::hardware_concurrency() >= 1);
    RUN_TEST(check_stdex__sleep_for__accuracy);
    
    test_thread_id();

    return 0;
}

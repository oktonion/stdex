#include "../stdex/include/core.h"
#include "../stdex/include/functional.hpp"
#include "../stdex/include/type_traits.hpp"

#include <iostream>

#define VERIFY(cond) STATIC_ASSERT((cond), check)
#define DYNAMIC_VERIFY(cond) if(!(cond)) {std::cout << "check condition \'" << #cond << "\' failed at line " << __LINE__ << std::endl; return -1;}
#define RUN_TEST(test) {std::cout << #test << std::endl; int line = test(); if(line != 0) {std::cout << "failed at line " << line << std::endl; return line;}}
#define DYNAMIC_VERIFY_FAIL {std::cout << "check condition " << "failed at line " << __LINE__ << std::endl; return -1;}
#define DYNAMIC_VERIFY_ABORT(cond) if(!(cond)) {std::cout << "check condition \'" << #cond << "\' failed at line " << __LINE__ << std::endl; std::abort();}


int dummy = 42;

int return_value(int) { return dummy; }
int& return_lvalue(int) { return dummy; }
const int& return_const_lvalue(int) { return dummy; }
#ifdef STDEX_RV_REF
STDEX_RV_REF(int) return_rvalue(int) { return stdex::move(dummy); }
#endif
#ifdef STDEX_RV_REF_CONST
STDEX_RV_REF_CONST(int) return_const_rvalue(int) { return stdex::move(dummy); }
#endif

template <class Expect, class Fn>
int do_test(Fn* func) {
    using namespace stdex::placeholders;
    
    Expect exp = stdex::bind(func, _1)(100); // the input value is ignored. dummy is returned.
    Expect exp2 = stdex::bind(func, _1)(101, 102);
    Expect exp_r = stdex::bind<Expect>(func, _1)(100);
    Expect exp_r2 = stdex::bind<Expect>(func, _1)(101, 102);

    DYNAMIC_VERIFY(exp == 42);
    DYNAMIC_VERIFY(exp2 == 42);
    DYNAMIC_VERIFY(exp_r == 42);
    DYNAMIC_VERIFY(exp_r2 == 42);

    if ((stdex::is_reference<Expect>::value && func)) {
        DYNAMIC_VERIFY(&exp == &dummy);
        DYNAMIC_VERIFY(&exp2 == &dummy);
        DYNAMIC_VERIFY(&exp_r == &dummy);
        DYNAMIC_VERIFY(&exp_r2 == &dummy);
    }

    return 0;
}


// Test but with an explicit return type which differs from the real one.
template <class Expect, class Fn>
int do_test_r(Fn* func) {
    using namespace stdex::placeholders;

    Expect exp = stdex::bind<Expect>(func, _1)(100); // the input value is ignored
    Expect exp2 = stdex::bind<Expect>(func, _1)(101, 102);

    DYNAMIC_VERIFY(exp == 42);
    DYNAMIC_VERIFY(exp2 == 42);

    return 0;
}

struct return_line
{
    int line;
    return_line(int line_) : line(line_) {}
    int operator()() const {return line;}
};

return_line run_test(int line) 
{
    return line;
}

#define RUN_TEST_PARAM(test) RUN_TEST(run_test(test))

template<class R, class FuncT>
void verify_function_return(FuncT)
{
    VERIFY((stdex::is_same<typename stdex::detail::_function_return<FuncT>::type, R>::value));
}

int bind_return()
{
    verify_function_return<int>(&return_value);
    verify_function_return<int&>(&return_lvalue);

    stdex::bind<int>(&return_const_lvalue, stdex::placeholders::_1)(2);

    RUN_TEST_PARAM(do_test<int>(&return_value));
    RUN_TEST_PARAM(do_test<int&>(&return_lvalue));
    RUN_TEST_PARAM(do_test<const int&>(&return_const_lvalue));
#ifdef STDEX_RV_REF    
    RUN_TEST_PARAM(do_test<STDEX_RV_REF(int)>(&return_rvalue));
#endif
#ifdef STDEX_RV_REF_CONST
    RUN_TEST_PARAM(do_test<STDEX_RV_REF_CONST(int)>(&return_const_rvalue));
#endif

    RUN_TEST_PARAM(do_test_r<long>(&return_value));
    RUN_TEST_PARAM(do_test_r<long>(&return_lvalue));
    RUN_TEST_PARAM(do_test_r<long>(&return_const_lvalue));
#ifdef STDEX_RV_REF
    RUN_TEST_PARAM(do_test_r<long>(&return_rvalue));
#endif
#ifdef STDEX_RV_REF_CONST
    RUN_TEST_PARAM(do_test_r<long>(&return_const_rvalue));
#endif

    return 0;
}

float mult(float a, float b)
{
    return a * b;
}

int copy_test()
{
    typedef stdex::function<float(*)(float, float)> stdex_function_type;
    typedef stdex::binder<float, stdex_function_type, float, float> stdex_binder_type;
    stdex_function_type fnc = mult;
    DYNAMIC_VERIFY(fnc(2.f, 4.f) == 8);
    stdex_binder_type task = stdex::bind(fnc, 2.f, 4.f);
    stdex_binder_type task2(task);
    DYNAMIC_VERIFY(task() == 8);
    DYNAMIC_VERIFY(task2() == 8);

    return 0;
}

struct DummyUnaryFunction
{
    template <typename S>
    int operator()(S const&) const { return 0; }
};

struct BadUnaryFunction
{
    template <typename S>
    int operator()(S const&) const
    {
        // Trigger a compile error if this function is instantiated.
        // The constexpr is needed so that it is instantiated while checking
        // __invoke_of<BadUnaryFunction &, ...>.
        STATIC_ASSERT((stdex::is_same<S, S>::value == bool(false)), should_not_be);
        return 0;
    }
};

int invoke_function_object()
{
    stdex::binder<int, DummyUnaryFunction, BadUnaryFunction> b =
        stdex::bind<int>(DummyUnaryFunction(), BadUnaryFunction());
    DYNAMIC_VERIFY(b(0) == 0);
    stdex::binder<long, DummyUnaryFunction, BadUnaryFunction> b2 =
        stdex::bind<long>(DummyUnaryFunction(), BadUnaryFunction());
    DYNAMIC_VERIFY(b2(0) == 0);

    return 0;
}

template <class R, class F>
void
test(F f, R expected)
{
    DYNAMIC_VERIFY_ABORT(f() == expected);
}

template <class R, class F>
void
test_const(const F& f, R expected)
{
    DYNAMIC_VERIFY_ABORT(f() == expected);
}

int f() { return 1; }

struct A_int_0
{
    int operator()() { return 4; }
    int operator()() const { return 5; }
};

int invoke_int_0()
{
    test(stdex::bind(f), 1);
    test(stdex::bind(&f), 1);

    test(stdex::bind<int>(f), 1);
    test(stdex::bind<int>(&f), 1);
    test(stdex::bind<int>(A_int_0()), 4);
    test_const(stdex::bind<int>(A_int_0()), 5);

    return 0;
}

int main()
{
    RUN_TEST(bind_return);
    RUN_TEST(copy_test);
    RUN_TEST(invoke_function_object);
    RUN_TEST(invoke_int_0);

    return 0;
}

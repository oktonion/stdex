#include "../stdex/include/core.h"
#include "../stdex/include/functional.hpp"
#include "../stdex/include/system_error.hpp"
#include "../stdex/include/string.hpp"

#include <iostream>
#include <string>
#include <bitset>
#include <limits>

#define VERIFY(cond) STATIC_ASSERT((cond), check)
#define DYNAMIC_VERIFY(cond) if(!(cond)) {std::cout << "check condition \'" << #cond << "\' failed at line " << __LINE__ << std::endl; return -1;}
#define RUN_TEST(test) {std::cout << #test << std::endl; int line = test(); if(line != 0) {std::cout << "failed at line " << line << std::endl; return line;}}
#define DYNAMIC_VERIFY_FAIL {std::cout << "check condition " << "failed at line " << __LINE__ << std::endl; return -1;}
#define DYNAMIC_VERIFY_ABORT(cond) if(!(cond)) {std::cout << "check condition \'" << #cond << "\' failed at line " << __LINE__ << std::endl; std::abort();}

// Verify that we can instantiate hash for every required type.
STATIC_ASSERT(sizeof(struct stdex::hash<bool>), should_be);
STATIC_ASSERT(sizeof(struct stdex::hash<char>), should_be);
STATIC_ASSERT(sizeof(struct stdex::hash<signed char>), should_be);
STATIC_ASSERT(sizeof(struct stdex::hash<unsigned char>), should_be);
#ifdef _STDEX_NATIVE_CPP11_TYPES_SUPPORT
//template struct stdex::hash<stdex_char16_t>;
//template struct stdex::hash<stdex_char32_t>;
#endif
STATIC_ASSERT(sizeof(struct stdex::hash<short>), should_be);
STATIC_ASSERT(sizeof(struct stdex::hash<int>), should_be);
STATIC_ASSERT(sizeof(struct stdex::hash<long>), should_be);
STATIC_ASSERT(sizeof(struct stdex::hash<unsigned short>), should_be);
STATIC_ASSERT(sizeof(struct stdex::hash<unsigned int>), should_be);
STATIC_ASSERT(sizeof(struct stdex::hash<unsigned long>), should_be);
STATIC_ASSERT(sizeof(struct stdex::hash<float>), should_be);
STATIC_ASSERT(sizeof(struct stdex::hash<double>), should_be);
STATIC_ASSERT(sizeof(struct stdex::hash<long double>), should_be);
STATIC_ASSERT(sizeof(struct stdex::hash<void*>), should_be);
STATIC_ASSERT(sizeof(struct stdex::hash<stdex::string>), should_be);
STATIC_ASSERT(sizeof(struct stdex::hash<stdex::error_code>), should_be);
STATIC_ASSERT(sizeof(struct stdex::hash<stdex::error_condition>), should_be);
STATIC_ASSERT(sizeof(struct stdex::hash<wchar_t>), should_be);
STATIC_ASSERT(sizeof(struct stdex::hash<stdex::wstring>), should_be);

template<typename T>
int
do_test()
{
    typedef T value_type;
    typedef stdex::hash<value_type> hash_type;
    using std::size_t;

    value_type v; // default initialized is fine, same value all that matters.
    hash_type h1;
    size_t r1 = size_t(h1(v));

    hash_type h2;
    size_t r2 = size_t(h2(v));

    DYNAMIC_VERIFY( r1 == r2 );

    return 0;
}
  
int test01()
{
  DYNAMIC_VERIFY(do_test<stdex::error_code>() == 0);
  DYNAMIC_VERIFY(do_test<stdex::error_condition>() == 0);

  return 0;
}

struct operations_counter {
    static std::size_t copy_count;
    static std::size_t delete_count;
    static std::size_t construct_count;

    operations_counter() {
        construct_count++;
    }
    operations_counter(const operations_counter&)
    {
        construct_count++;
        copy_count++;
    }
    ~operations_counter() {
        delete_count++;
    }

    static void reset() {
        copy_count = 0; delete_count = 0; construct_count = 0;
    }
};

int test02()
{
    typedef stdex::function<stdex::remove_pointer<void(*)(int&, void*)>::type> function;
    typedef stdex::function<stdex::remove_pointer<void(*)(operations_counter&, void*)>::type> function_oc;
    //typedef std::function<void(int&, void*)> function;
    struct lambdas
    {
        static void func0(operations_counter& a, void* ptr) {
            a.reset();
            DYNAMIC_VERIFY_ABORT(ptr == nullptr);
        }

        static void func(int& a, void* ptr){
            a++;
            DYNAMIC_VERIFY_ABORT(ptr == nullptr);
        }

        static void func1(int& a, void* ptr) {
            a++;
            DYNAMIC_VERIFY_ABORT(ptr != nullptr);
        }
    };

    operations_counter oc;
    function_oc f0(&lambdas::func0);
    f0(oc, nullptr);

    int a = 0;
    function f1(&lambdas::func);
    f1(a, nullptr);

    DYNAMIC_VERIFY(a == 1);

    function f2(&lambdas::func);
    f2(a, nullptr);

    DYNAMIC_VERIFY(a == 2);

    function f3(&lambdas::func1);
    f3(a, &a);

    DYNAMIC_VERIFY(a == 3);

    function f4(&lambdas::func1);
    f4(a, &a);

    DYNAMIC_VERIFY(a == 4);

    function f5(&lambdas::func);

    f5(a, nullptr);

    DYNAMIC_VERIFY(a == 5);

    return 0;
}

// hack to check for stdex version of nullptr passed to function
template<bool Enabled = 
    stdex::intern::_has_feature<stdex::intern::_stdex_nullptr_implemented_as_distinct_type>::value> 
struct np_tests_impl:
    stdex::detail::_arg<stdex::detail::_nullptr_place_holder, 0>
{
    typedef stdex::detail::_arg<stdex::detail::_nullptr_place_holder, 0> base_type;
    np_tests_impl() : base_type(nullptr) {}

    template<class T>
    static int test01_impl(T*)
    {
        typedef stdex::function<void(*)(stdex::nullptr_t, float&)> function;

        struct lambdas{
            static void func(void* ptr, float &val)
            {
                DYNAMIC_VERIFY_ABORT(ptr == 0 || ptr == &val);
                val = 1.f;
            }
        };

        

        function f(&lambdas::func);

        float val = 0.0f;

        void(*target)(void*, float&) =
            f.target<typename stdex::remove_pointer<void(*)(void*, float&)>::type>();

        stdex::invoke(target, 0, val); 
        f(nullptr, val);

        DYNAMIC_VERIFY(val != 0.0f);

        return 0;
    }

    template<class T>
    static int test01_impl(stdex::detail::functional_detail::_any){std::cout << "test01 disabled" <<std::endl; return 0;}

    template<class T>
    int test01()
    {
        return test01_impl<T>(base_type::value);
    }
};

template<>
struct np_tests_impl<false>
{
    template<class T>
    int test01()
    {
        std::cout << "test01 disabled" <<std::endl; return 0;
    }  
};

np_tests_impl<> np_tests;

std::size_t operations_counter::copy_count = 0;
std::size_t operations_counter::delete_count = 0;
std::size_t operations_counter::construct_count = 0;

int test03()
{
   

    struct lambdas{
        static void func1(operations_counter&)
        { }

        static void func2(const operations_counter&)
        { }
    };

    {
        typedef stdex::function<void(*)(operations_counter&)> function;
        function f(&lambdas::func1);
        operations_counter cc;
        cc.reset();
        f(cc);
        DYNAMIC_VERIFY(cc.copy_count == 0 ? true : (std::cout << cc.copy_count << " != 0" << std::endl, false));
    }

    {
        typedef stdex::function<void(*)(operations_counter)> function;
        function f(&lambdas::func1);
        operations_counter cc;
        cc.reset();
        f(cc);
        DYNAMIC_VERIFY(cc.copy_count == 1 ? true : (std::cout << cc.copy_count << " != 1" << std::endl, false));
    }

    {
        typedef stdex::function<void(*)(operations_counter)> function;
        function f(&lambdas::func2);
        operations_counter cc;
        cc.reset();
        f(cc);
        DYNAMIC_VERIFY(cc.copy_count == 1 ? true : (std::cout << cc.copy_count << " != 1" << std::endl, false));
    }

    return 0;
}

int test04()
{


    struct lambdas {
        static void func1(int*, operations_counter&)
        { }

        static void func2(operations_counter&, int*)
        { }
    };

    {
        typedef stdex::function<void(*)(int*, operations_counter&)> function;
        function f(&lambdas::func1);
        operations_counter cc;
        cc.reset();
        f(0, cc);
        DYNAMIC_VERIFY(cc.copy_count == 0 ? true : (std::cout << cc.copy_count << " != 0" << std::endl, false));
    }

    {
        typedef stdex::function<void(*)(int*, operations_counter)> function;
        function f(&lambdas::func1);
        operations_counter cc;
        cc.reset();
        f(0, cc);
        DYNAMIC_VERIFY(cc.copy_count == 1 ? true : (std::cout << cc.copy_count << " != 1" << std::endl, false));
    }

    {
        typedef stdex::function<void(*)(operations_counter, int*)> function;
        function f(&lambdas::func2);
        operations_counter cc;
        cc.reset();
        f(cc, 0);
        DYNAMIC_VERIFY(cc.copy_count == 1 ? true : (std::cout << cc.copy_count << " != 1" << std::endl, false));
    }

    return 0;
}

int test05()
{


    struct lambdas {
        static operations_counter func1(int*)
        { 
            operations_counter value;
            return value;
        }

        static operations_counter func2()
        { 
            operations_counter value;
            return value;
        }

        static operations_counter* func3()
        {
            static operations_counter value;
            value.construct_count--;
            return &value;
        }

        static operations_counter func4(void*)
        {
            operations_counter value;
            return value;
        }
    };

    {
        typedef stdex::function<operations_counter(*)(int*)> function;
        function f(&lambdas::func1);
        operations_counter cc;
        cc.reset();
        f(0);
        DYNAMIC_VERIFY(cc.construct_count == cc.delete_count ? true : (std::cout << cc.construct_count << " != " << cc.delete_count << std::endl, false));
        DYNAMIC_VERIFY(cc.copy_count <= 4 ? true : (std::cout << cc.copy_count << " > 4" << std::endl, false));
    }

    {
        typedef stdex::function<operations_counter(*)()> function;
        function f(&lambdas::func2);
        operations_counter cc;
        cc.reset();
        f();
        DYNAMIC_VERIFY(cc.construct_count == cc.delete_count ? true : (std::cout << cc.construct_count << " != " << cc.delete_count << std::endl, false));
        DYNAMIC_VERIFY(cc.copy_count <= 4 ? true : (std::cout << cc.copy_count << " > 4" << std::endl, false));
    }

    {
        typedef stdex::function<operations_counter*(*)()> function;
        function f(&lambdas::func3);
        operations_counter cc;
        cc.reset();
        f();
        DYNAMIC_VERIFY(cc.construct_count == cc.delete_count ? true : (std::cout << cc.construct_count << " != " << cc.delete_count << std::endl, false));
        DYNAMIC_VERIFY(cc.copy_count == 0 ? true : (std::cout << cc.copy_count << " != 0" << std::endl, false));

        DYNAMIC_VERIFY(f.target< operations_counter* ()>() == &lambdas::func3);
    }

    

    return 0;
}

struct dummy_functor {
    std::size_t call_count;
    void call(operations_counter&) { call_count++; }
    void reset() { call_count = 0; }
};

int test06()
{
    {
        typedef stdex::function<void(*)(dummy_functor&, operations_counter&)> function;
        function f(&dummy_functor::call);
        operations_counter cc;
        dummy_functor func;
        cc.reset();
        func.reset();
        f(func, cc);
        DYNAMIC_VERIFY(cc.copy_count == 0 ? true : (std::cout << cc.copy_count << " != 0" << std::endl, false));
        DYNAMIC_VERIFY(func.call_count == 1 ? true : (std::cout << func.call_count << " != 1" << std::endl, false));
    }

    {
        typedef stdex::function<void(*)(dummy_functor*, operations_counter&)> function;
        function f(&dummy_functor::call);
        operations_counter cc;
        dummy_functor func;
        cc.reset();
        func.reset();
        f(&func, cc);
        DYNAMIC_VERIFY(cc.copy_count == 0 ? true : (std::cout << cc.copy_count << " != 0" << std::endl, false));
        DYNAMIC_VERIFY(func.call_count == 1 ? true : (std::cout << func.call_count << " != 1" << std::endl, false));
    }
    return 0;
}

struct classical_functor {
    std::size_t call_count;
    void operator()(operations_counter&) { call_count++; }
    void operator()() { call_count++; }
    int operator()(int value) {return value;}
    int operator()(int value) const {return value + 1;}
    void reset() { call_count = 0; }
};

int test07()
{
    classical_functor func;
    func.reset();
    operations_counter cc;

    stdex::invoke<void>(func, cc);
    stdex::invoke<void>(func);
    DYNAMIC_VERIFY(stdex::invoke<int>(func, 1) == 1);
    DYNAMIC_VERIFY(stdex::invoke<int>(stdex::ref(func), 1) == 1);

    const classical_functor func_const = func;
    stdex::invoke<void>(func_const, cc);
    stdex::invoke<void>(func_const);
    DYNAMIC_VERIFY(stdex::invoke<int>(func_const, 2) == 2);
    DYNAMIC_VERIFY(stdex::invoke<int>(stdex::cref(func), 2) == 3);
    
    return 0;
}

int test08()
{
    dummy_functor func;
    operations_counter counter;

    stdex::mem_fn(&dummy_functor::call)(func, counter);
    stdex::mem_fn(&dummy_functor::call)(&func, counter);
    stdex::mem_fn(&dummy_functor::call)(stdex::ref(func), counter);

    return 0;
}

class replaced{};

int test09()
{
    dummy_functor func;
    operations_counter counter;

    struct lambdas
    {
        static void func_none(){}
        static void func0(int, float){}
    };
    int val;
    
    stdex::function<void(*)(double)> functor = 
        stdex::bind(&lambdas::func0, val, stdex::placeholders::_1);
    
    functor(0.0);

    stdex::bind(&lambdas::func0, 0, stdex::placeholders::_1)(val);

    stdex::bind(&lambdas::func_none)(0);

    return 0;
}

void ff(const float&, ...) {}

int main()
{
    float a = 0;
    stdex::invoke<void>(ff, a, 0, 0, 0, 0, 0, 0, 0);
    RUN_TEST(test01);
    RUN_TEST(test02);
    RUN_TEST(test03);
    RUN_TEST(test04);
    RUN_TEST(test05);
    RUN_TEST(test06);
    RUN_TEST(test07);
    RUN_TEST(test08);
    RUN_TEST(np_tests.test01<void>);
    RUN_TEST(np_tests.test01<int>);

    const std::string::size_type big = 
        std::numeric_limits<std::string::size_type>::max();
    std::string s;
    try {
        s.resize(big, 'a');
    } catch (const std::bad_alloc&) {
        return 0; // try to avoid a FAIL if memory allocation fails
    } catch (const std::length_error&) {
        return 0; // try to avoid a FAIL if memory allocation fails
    }
    // PR libstdc++/89629
    (void) stdex::hash<std::string>()(s);

    return 0;
}

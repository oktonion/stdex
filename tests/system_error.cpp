#include "../stdex/include/core.h"
#include "../stdex/include/system_error.hpp"

#include <iostream>
#include <string>

#define VERIFY(cond) STATIC_ASSERT((cond), check)
#define DYNAMIC_VERIFY(cond) if(!(cond)) {std::cout << "check condition \'" << #cond << "\' failed at line " << __LINE__ << std::endl; return -1;}
#define RUN_TEST(test) {std::cout << #test << std::endl; int line = test(); if(line != 0) {std::cout << "failed at line " << line << std::endl; return line;}}
using std::size_t;

struct ClassType {};
enum my_errc { my_err = 0 };
enum my_not_errc { my_not_err = 0 };

class my_error_category_impl
	: public stdex::error_category
{
public:
	const char* name() const stdex_noexcept { return ""; }
	std::string message(int) const { return ""; }
} my_error_category_instance;

namespace stdex
{
	stdex::error_code
		make_error_code(my_errc e)
	{
		return stdex::error_code(static_cast<int>(e),
			my_error_category_instance);
	}
}

using stdex::make_error_code;

namespace stdex
{
	stdex::error_condition
		make_error_condition(my_errc e)
	{
		return stdex::error_condition(static_cast<int>(e),
			my_error_category_instance);
	}
}

using stdex::make_error_condition;

namespace stdex
{
	template<>
	struct is_error_code_enum<my_errc>
		: public true_type {};
}

namespace stdex
{
	template<>
	struct is_error_condition_enum<my_errc>
		: public true_type { };
}

class fuzzy_logic :
    public stdex::system_error
{
public:
	fuzzy_logic() : stdex::system_error(stdex::error_code(), "whoa") { }
};

void allocate_on_stack(void)
{
	const size_t num = 512;
	char array[num];
	for (size_t i = 0; i < num; i++)
		array[i] = 0;
	// Suppress unused warnings.
	for (size_t i = 0; i < num; i++)
		array[i] = array[i];
}

int test0()
{
    using namespace stdex;

    error_code ec;
    ec = errc::not_supported;
    ec = my_err;
    if (ec == errc::not_supported)
    { }

    return 0;
}

int test1()
{
    using namespace stdex;

    const std::string s("too late: boulangerie out of pain au raisin");
    const error_code
        e(make_error_code(errc::operation_not_supported));

    // 1
    {
        system_error err1(e, s);
        DYNAMIC_VERIFY(err1.code() == e);
        DYNAMIC_VERIFY(std::string(err1.what()).find(s) != std::string::npos);
    }

    // 2
    {
        system_error err2(95, system_category(), s);
        DYNAMIC_VERIFY(err2.code() == error_code(95, system_category()));
        DYNAMIC_VERIFY(std::string((err2.what(), s)).find(s) != std::string::npos);
    }

    return 0;
}

int test2()
{
    using namespace stdex;

    std::string s("lack of sunlight, no water error");

    // 1
    system_error obj1 = system_error(error_code(), s);

    // 2
    system_error obj2(error_code(), s);

    DYNAMIC_VERIFY(std::string(obj1.what()).find(s.data()) != std::string::npos);
    DYNAMIC_VERIFY(std::string(obj2.what()).find(s.data()) != std::string::npos);

    return 0;
}

int test3()
{
    using namespace stdex;

    std::string s("lack of sunlight error");
    system_error x(error_code(), s);

    DYNAMIC_VERIFY(std::string(x.what()).find(s.data()) != std::string::npos);

    return 0;
}

int test4()
{
    using namespace stdex;

#if CHECK_FOR_COMPILE_ERROR_TESTS == 1
    try
    {
        throw fuzzy_logic();
    }
    catch (const fuzzy_logic& obj)
    {
        DYNAMIC_VERIFY(std::string(obj.what()).find("whoa") != std::string::npos);
    }
    catch (...)
    {
        DYNAMIC_VERIFY(false);
    }

    {
        error_code e;
        int i = e;  // error "cannot convert"
    }

    {
        error_condition e;
        int i = e; // error "cannot convert"
    }

#endif

    return 0;
}

int test5()
{
    using namespace stdex;

    const std::string s("CA ISO emergency once again:immediate power down");
    const char* strlit1 = "wish I lived in Palo Alto";
    const char* strlit2 = "...or Santa Barbara";
    system_error obj1(error_code(), s);

    // block 01
    {
        const std::string s2(strlit1);
        system_error obj2(error_code(), s2);
        obj1 = obj2;
    }
    allocate_on_stack();
    DYNAMIC_VERIFY(std::string(obj1.what()).find(strlit1) != std::string::npos);

    // block 02
    {
        const std::string s3(strlit2);
        system_error obj3 = system_error(error_code(), s3);
        obj1 = obj3;
    }
    allocate_on_stack();
    DYNAMIC_VERIFY(std::string(obj1.what()).find(strlit2) != std::string::npos);

    return 0;
}

int test6()
{
    using namespace stdex;

    std::string s("after nine thirty, this request cannot be met");

    system_error obj =
        system_error(make_error_code(errc::invalid_argument), s);
    std::string s1(obj.what());
    std::string s2(obj.what());
    DYNAMIC_VERIFY(s1 == s2);

    return 0;
}

int test7()
{
    using namespace stdex;

    typedef system_error test_type;

    const std::string xxx(255, 'x');
    test_type t(error_code(), xxx);
    std::string what_str = t.what();
    size_t 
        len1 = what_str.length(),
        len2 = xxx.length();
    ((void)(len1));
    ((void)(len2));
    DYNAMIC_VERIFY((what_str.find(xxx) != std::string::npos));
    return 0;
}

int test8()
{
    using namespace stdex;

    // 1
    error_code e1;
    DYNAMIC_VERIFY(e1.value() == 0);
    DYNAMIC_VERIFY(e1.category() == system_category());

    // 3
    error_code e3(make_error_code(errc::operation_not_supported));
    DYNAMIC_VERIFY(e3.value() == int(errc::operation_not_supported));
    DYNAMIC_VERIFY(e3.category() == generic_category());

    return 0;
}

int test9()
{
    using namespace stdex;

    error_code ec2;
    ec2 = my_err;
    DYNAMIC_VERIFY(ec2 == make_error_code(my_err));

    return 0;
}

int test10()
{
    using namespace stdex;
    // 1
    error_code e1;
    if (static_cast<bool>(e1))
    {
        DYNAMIC_VERIFY(false);
    }

    // 2
    error_code e2(make_error_code(errc::operation_not_supported));
    if (static_cast<bool>(e2))
    {
        DYNAMIC_VERIFY(true);
    }

    return 0;
}

int test11()
{
    using namespace stdex;

    error_code e1;
    error_code e2(make_error_code(errc::operation_not_supported));

    DYNAMIC_VERIFY(e1 == e1);
    DYNAMIC_VERIFY(!(e1 == e2));

    error_code e3(e2.value(), system_category());
    DYNAMIC_VERIFY(!(e2 == e3));

    return 0;
}

int test12()
{
    using namespace stdex;

    error_code e1;
    error_code e2(make_error_code(errc::operation_not_supported));

    DYNAMIC_VERIFY(!(e1 != e1));
    DYNAMIC_VERIFY(e1 != e2);

    error_code e3(e2.value(), system_category());
    DYNAMIC_VERIFY(e2 != e3);

    return 0;
}

int test13()
{
    using namespace stdex;

    VERIFY((is_error_code_enum<errc>::value));

    VERIFY((!is_error_code_enum<int>::value));

    VERIFY((is_error_condition_enum<errc>::value));

    VERIFY((!is_error_condition_enum<int>::value));

    return 0;
}

int test14()
{
    using namespace stdex;

    // 1
    error_condition e1;
    DYNAMIC_VERIFY(e1.value() == 0);
    DYNAMIC_VERIFY(e1.category() == generic_category());

    // 2
    error_condition e2(e1.value(), system_category());
    DYNAMIC_VERIFY(e2.value() == e1.value());
    DYNAMIC_VERIFY(e2.category() == system_category());

    // 3
    error_condition e3(errc::operation_not_supported);
    DYNAMIC_VERIFY(e3.value() == int(errc::operation_not_supported));
    DYNAMIC_VERIFY(e3.category() == generic_category());

    return 0;
}

int test15()
{
    using namespace stdex;

    {
        error_condition ec1(my_err);
        DYNAMIC_VERIFY(ec1 == make_error_condition(my_err));
    }

    {
        error_condition ec2;
        ec2 = my_err;
        DYNAMIC_VERIFY(ec2 == make_error_condition(my_err));
    }

    return 0;
}

int test16()
{
    using namespace stdex;

    // 1
    error_condition e1;
    if (static_cast<bool>(e1))
    {
        DYNAMIC_VERIFY(false);
    }

    // 2
    error_condition e2(errc::operation_not_supported);
    if (static_cast<bool>(e2))
    {
        DYNAMIC_VERIFY(true);
    }

    return 0;
}

int test17()
{
    using namespace stdex;

    error_condition e1;
    error_condition e2(errc::operation_not_supported);

    DYNAMIC_VERIFY(e1 == e1);
    DYNAMIC_VERIFY(!(e1 == e2));

    error_condition e3(e2.value(), system_category());
    DYNAMIC_VERIFY(!(e2 == e3));

    return 0;
}

int test18()
{
    using namespace stdex;

    error_condition e1;
    error_condition e2(errc::operation_not_supported);

    DYNAMIC_VERIFY(!(e1 != e1));
    DYNAMIC_VERIFY(e1 != e2);

    error_condition e3(e2.value(), system_category());
    DYNAMIC_VERIFY(e2 != e3);

    return 0;
}

int test19()
{
    using namespace stdex;

    error_code e1 = error_code(errno, generic_category());

    DYNAMIC_VERIFY(e1.value() == errno);

    return 0;
}

int main(void)
{
    using namespace stdex;
    
    RUN_TEST(test1);
    RUN_TEST(test2);
    RUN_TEST(test3);
    RUN_TEST(test4);
    RUN_TEST(test5);
    RUN_TEST(test6);
    RUN_TEST(test7);
    RUN_TEST(test8);
    RUN_TEST(test9);
    RUN_TEST(test10);
    RUN_TEST(test11);
    RUN_TEST(test12);
    RUN_TEST(test13);
    RUN_TEST(test14);
    RUN_TEST(test15);
    RUN_TEST(test16);
    RUN_TEST(test17);
    RUN_TEST(test18);
    RUN_TEST(test19);

    return 0;
}

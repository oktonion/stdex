#include "../stdex/include/ctime.hpp"

#include "../stdex/include/core.h"

#include <iostream>

#define DYNAMIC_VERIFY(cond) if(!(cond)) {std::cout << "check condition \'" << #cond << "\' failed at line " << __LINE__ << std::endl; return __LINE__;}
#define RUN_TEST(test) {std::cout << #test << std::endl; int line = test(); if(line != 0) {std::cout << "failed at line " << line << std::endl; return line;}}

void test01()
{
  typedef stdex::clock_t         clock_t;
  typedef stdex::size_t          size_t;
  typedef stdex::time_t          time_t;
  typedef stdex::tm              tm;
  typedef stdex::timespec        timespec;
}

namespace type_traits
{
    template<class T1, class T2>
    struct is_same
    {
        static const bool value = false;
    };

    template<class T>
    struct is_same<T, T>
    {
        static const bool value = true;
    };
}

void test02()
{
    std::cout << "using original timespec: " << (type_traits::is_same<stdex::timespec, ::timespec>::value ? "true" : "false") << std::endl;
}

int test03()
{
    using namespace stdex;

    

    return 0;
}



int main(void)
{
    test01();
    test02();
    RUN_TEST(test03);


    return 0;
}

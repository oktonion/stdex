#include "../stdex/include/typeindex.hpp"

#include <iostream>

#define THROW_VERIFY(cond) if(!(cond)) {std::cout << "check condition \'" << #cond << "\' failed at line " << __LINE__ << std::endl; throw(__LINE__);}
#define DYNAMIC_VERIFY(cond) if(!(cond)) {std::cout << "check condition \'" << #cond << "\' failed at line " << __LINE__ << std::endl; return __LINE__;}
#define RUN_TEST(test) {std::cout << #test << std::endl; int line = test(); if(line != 0) {std::cout << "failed at line " << line << std::endl; return line;}}

#define TEST3(TI1, TI2, TO1, TO2)               \
  DYNAMIC_VERIFY( (TI1 == TI2) == (TO1 == TO2) );       \
  DYNAMIC_VERIFY( (TI1 != TI2) == (TO1 != TO2) );	\
  DYNAMIC_VERIFY( (TI1 < TI2) == (TO1.before(TO2)) );   \
  DYNAMIC_VERIFY( (TI1 <= TI2) == (!TO2.before(TO1)) ); \
  DYNAMIC_VERIFY( (TI1 > TI2) == (TO2.before(TO1)) );	\
  DYNAMIC_VERIFY( (TI1 >= TI2) == (!TO1.before(TO2)) );

#define TEST2(TI, TO)	  \
  TEST3(TI, ti1, TO, to1) \
  TEST3(TI, ti2, TO, to2) \
  TEST3(TI, ti3, TO, to3) \
  TEST3(TI, ti4, TO, to4) \
  TEST3(TI, ti5, TO, to5) \
  TEST3(TI, ti6, TO, to6) \
  TEST3(TI, ti7, TO, to7)


int test01()
{
    using namespace stdex;

    using std::type_info;

    class Cadabra { };
    Cadabra a1, a2_;
    const Cadabra a2 = a2_;

    const type_info& to1 = typeid(int);
    const type_index ti1(to1);

    const type_info& to2 = typeid(double);
    const type_index ti2(to2);

    const type_info& to3 = typeid(Cadabra);
    const type_index ti3(to3);

    const type_info& to4 = typeid(const Cadabra);
    const type_index ti4(to4);

    const type_info& to5 = typeid(const Cadabra&);
    const type_index ti5(to5);

    const type_info& to6 = typeid(a1);
    const type_index ti6(to6);

    const type_info& to7 = typeid(a2);
    const type_index ti7(to7);

    TEST2(ti1, to1);
    TEST2(ti2, to2);
    TEST2(ti3, to3);
    TEST2(ti4, to4);
    TEST2(ti5, to5);
    TEST2(ti6, to6);
    TEST2(ti7, to7);

    return 0;
}

int test02()
{
    using namespace stdex;

    class Alakazam { };
    Alakazam a1, a2_;
    const Alakazam a2 = a2_;

    const type_info& to1 = typeid(int);
    const type_index ti1(to1);
    DYNAMIC_VERIFY( ti1.name() == to1.name() );

    const type_info& to2 = typeid(double);
    const type_index ti2(to2);
    DYNAMIC_VERIFY( ti2.name() == to2.name() );

    const type_info& to3 = typeid(Alakazam);
    const type_index ti3(to3);
    DYNAMIC_VERIFY( ti3.name() == to3.name() );

    const type_info& to4 = typeid(const Alakazam);
    const type_index ti4(to4);
    DYNAMIC_VERIFY( ti4.name() == to4.name() );

    const type_info& to5 = typeid(const Alakazam&);
    const type_index ti5(to5);
    DYNAMIC_VERIFY( ti5.name() == to5.name() );

    const type_info& to6 = typeid(a1);
    const type_index ti6(to6);
    DYNAMIC_VERIFY( ti6.name() == to6.name() );

    const type_info& to7 = typeid(a2);
    const type_index ti7(to7);
    DYNAMIC_VERIFY( ti7.name() == to7.name() );

    return 0;
}

int main()
{
    RUN_TEST(test01);
    RUN_TEST(test02);

    return 0;
}

#include "../stdex/include/core.h"
#include "../stdex/include/type_traits.hpp"

#include <iostream>
#define DYNAMIC_VERIFY(cond)                                                                         \
    if (!(cond))                                                                                     \
    {                                                                                                \
        std::cout << "check condition \'" << #cond << "\' failed at line " << __LINE__ << std::endl; \
        return __LINE__;                                                                             \
    }
#define RUN_TEST(test)                                           \
    {                                                            \
        std::cout << #test << std::endl;                         \
        int line = test();                                       \
        if (line != 0)                                           \
        {                                                        \
            std::cout << "failed at line " << line << std::endl; \
            return line;                                         \
        }                                                        \
    }

struct empty_UDT
{
    empty_UDT();
    empty_UDT(const empty_UDT &);
    ~empty_UDT();
    empty_UDT &operator=(const empty_UDT &);
    bool operator==(const empty_UDT &) const;
};

empty_UDT::empty_UDT() {}
empty_UDT::~empty_UDT() {}
empty_UDT::empty_UDT(const empty_UDT &) {}
empty_UDT &empty_UDT::operator=(const empty_UDT &) { return *this; }
bool empty_UDT::operator==(const empty_UDT &) const { return true; }

struct VB
{
    virtual ~VB(){};
};

struct VD : public VB
{
    ~VD(){};
};

enum enum_UDT
{
    one,
    two,
    three
};

struct UDT
{
    UDT();
    ~UDT();
    UDT(const UDT &);
    UDT &operator=(const UDT &);
    int i;

    void f1();
    int f2();
    int f3(int);
    int f4(int, float);
};

typedef int (UDT::*mf2)();

struct POD_UDT
{
    int x;
};

union union_UDT {
    int x;
    double y;
    ~union_UDT() {}
};

template <class T>
struct align_calc
{
    char padding;
    T instance;
    static std::ptrdiff_t get()
    {
        static align_calc<T> a;
        return reinterpret_cast<const char *>(&(a.instance)) - reinterpret_cast<const char *>(&(a.padding));
    }
};

#define ALIGNOF(x) align_calc<x>::get()

int align_of_test()
{
    using namespace stdex;

    DYNAMIC_VERIFY(alignment_of<char>::value == ALIGNOF(char));
    DYNAMIC_VERIFY(alignment_of<short>::value == ALIGNOF(short));
    DYNAMIC_VERIFY(alignment_of<int>::value == ALIGNOF(int));
    DYNAMIC_VERIFY(alignment_of<long>::value == ALIGNOF(long));
    DYNAMIC_VERIFY(alignment_of<float>::value == ALIGNOF(float));
    DYNAMIC_VERIFY(alignment_of<double>::value == ALIGNOF(double));
    DYNAMIC_VERIFY(alignment_of<long double>::value == ALIGNOF(long double));
#ifdef LLONG_MAX
    DYNAMIC_VERIFY(alignment_of<long long>::value == ALIGNOF(long long));
#endif
    DYNAMIC_VERIFY(alignment_of<int[4]>::value == ALIGNOF(int[4]));
    DYNAMIC_VERIFY(alignment_of<int (*)(int)>::value == ALIGNOF(int (*)(int)));
    DYNAMIC_VERIFY(alignment_of<int *>::value == ALIGNOF(int *));
    DYNAMIC_VERIFY(alignment_of<VB>::value == ALIGNOF(VB));
    DYNAMIC_VERIFY(alignment_of<VD>::value == ALIGNOF(VD));
    DYNAMIC_VERIFY(alignment_of<enum_UDT>::value == ALIGNOF(enum_UDT));
    DYNAMIC_VERIFY(alignment_of<mf2>::value == ALIGNOF(mf2));
    DYNAMIC_VERIFY(alignment_of<POD_UDT>::value == ALIGNOF(POD_UDT));
    DYNAMIC_VERIFY(alignment_of<empty_UDT>::value == ALIGNOF(empty_UDT));
    DYNAMIC_VERIFY(alignment_of<union_UDT>::value == ALIGNOF(union_UDT));

    return 0;
}

int main(void)
{
    using namespace stdex;

    RUN_TEST(align_of_test);
    return 0;
}

#include "../stdex/include/core.h"
#include "../stdex/include/move.hpp"

#include <cstdlib>
#include <iostream>
#include <cerrno>
#include <limits>
#include <map>

#define DYNAMIC_VERIFY(cond) if(!(cond)) {std::cout << "check condition \'" << #cond << "\' failed at line " << __LINE__ << std::endl; return __LINE__;}
#define RUN_TEST(test) {std::cout << #test << std::endl; int line = test(); if(line != 0) {std::cout << "failed at line " << line << std::endl; return line;}}

#ifndef __BORLANDC__
    //#undef STDEX_RV_REF
    //#define STDEX_RV_REF(Type) Type&&
    //#undef STDEX_RV_REF_CONST
    //#define STDEX_RV_REF_CONST(Type) Type const&&
#endif

#define MY_STD stdex

class movable
{
    char data;
public:
    movable(int)
    {
        std::cout << "movable(int)" << std::endl;
    }
    movable(const movable & other):
        data(other.data)
    { 
        std::cout << "movable(lv_ref)" << std::endl;
    }
    movable(STDEX_RV_REF(movable) other)
    {
        //movable &other = other_;
        std::cout << "movable(rv_ref)" << std::endl;
        this->swap(other);
    }

    movable& operator=(const movable &other)
    {
        //movable &other = other_;
        std::cout << "movable = lv_ref" << std::endl;
        movable tmp(other);
        this->swap(tmp);

        return *this;
    }

    movable& operator=(STDEX_RV_REF(movable) other)
    {
        //movable &other = other_;
        std::cout << "movable = rv_ref" << std::endl;
        this->swap(other);

        return *this;
    }

    friend void swap(movable &lhs, movable &rhs)
    {
        lhs.swap(rhs);
    }

    void swap(movable &other)
    {
        using std::swap;
        swap(data, other.data);
    }
};

class movable_only
{
    STDEX_NOT_COPYABLE
    char data;
public:
    movable_only(int): STDEX_DELETE_ICC()
    {
        std::cout << "movable_only(int)" << std::endl;
    }
    movable_only(STDEX_RV_REF(movable_only) other): STDEX_DELETE_ICC()
    {
        //movable &other = other_;
        std::cout << "movable_only(rv_ref)" << std::endl;
        this->swap(other);
    }


    movable_only& operator=(STDEX_RV_REF(movable_only) other)
    {
        //movable &other = other_;
        std::cout << "movable_only = rv_ref" << std::endl;
        this->swap(other);

        return *this;
    }

    friend void swap(movable_only &lhs, movable_only &rhs)
    {
        lhs.swap(rhs);
    }

    void swap(movable_only &other)
    {
        using std::swap;
        swap(data, other.data);
    }
};

class movable_not_copyable:
    public movable
{
    STDEX_NOT_COPYABLE
    bool data2;
public:
    movable_not_copyable(int v = 0): movable(v), STDEX_DELETE_ICC()
    {
        std::cout << "movable_not_copyable(int)" << std::endl;
    }
    movable_not_copyable(STDEX_RV_REF(movable_not_copyable) other):
        movable(MY_STD::move(other)),
        STDEX_DELETE_ICC(),
        data2(static_cast<const movable_not_copyable&>(other).data2)
    {
        //movable_not_copyable &other = other_;
        std::cout << "movable_not_copyable(rv_ref)" << std::endl;
    }

    movable_not_copyable& operator=(STDEX_RV_REF(movable_not_copyable) other)
    {
        //movable_not_copyable &other = other_;
        std::cout << "movable_not_copyable = rv_ref" << std::endl;
        movable_not_copyable tmp(MY_STD::move(other));

        using std::swap;
        this->swap(tmp);

        return *this;
    }

    friend void swap(movable_not_copyable &lhs, movable_not_copyable &rhs)
    {
        lhs.swap(rhs);
    }

    void swap(movable_not_copyable &other)
    {
        using std::swap;
        typedef movable base_type;
        swap(static_cast<base_type&>(*this), static_cast<base_type&>(other));
        swap(data2, other.data2);
    }
};

class movable_with_const_rv_ref:
    public movable
{
    int bbb;
public:
    movable_with_const_rv_ref():
        movable(0)
    { }

    movable_with_const_rv_ref(STDEX_RV_REF_CONST(movable_with_const_rv_ref) other_):
        movable(MY_STD::move(other_)),
        bbb(static_cast<const movable_with_const_rv_ref&>(other_).bbb)
    {
        //movable_not_copyable &other = other_;
        std::cout << "movable_not_copyable(rv_ref)" << std::endl;

        const movable_with_const_rv_ref &other = other_;
        
    }

    movable_with_const_rv_ref& operator=(STDEX_RV_REF_CONST(movable_with_const_rv_ref) other_)
    {
        //movable_not_copyable &other = other_;
        std::cout << "movable_not_copyable = rv_ref" << std::endl;
        movable_with_const_rv_ref tmp(MY_STD::move(other_));

        using std::swap;
        swap(*this, tmp);

        return *this;
    }    
};

int test0()
{
    typedef movable_only mv_t;
    
    mv_t mv = mv_t(0), mv3(0);
    const mv_t  mv2(0); // shouldn't work
    std::cout << "should be rv_ref" << std::endl;
    //mv = // works
        MY_STD::move(mv_t(0)); // works

    //mv = mv2; // shouldn't work

    //mv = mv3; // shouldn't work

    //mv = // shouldn't work
        MY_STD::move(mv2); // works
    std::cout << "should be rv_ref" << std::endl;
    mv =  // works
        MY_STD::move(mv3); // works


    return 0;
}

int test1()
{
    typedef movable mv_t;
    
    mv_t mv = mv_t(0), mv3(0);
    const mv_t  mv2(0);
    std::cout << "should be rv_ref" << std::endl;
    mv = // works
        MY_STD::move(mv_t(0)); // works
    std::cout << "should be lv_ref" << std::endl;
    mv = mv2; // works
    std::cout << "should be lv_ref" << std::endl;
    mv = mv3; // works
    std::cout << "should be lv_ref" << std::endl;
    mv = // works
        MY_STD::move(mv2); // works
    std::cout << "should be rv_ref" << std::endl;
    mv =  // works
        MY_STD::move(mv3); // works


    return 0;
}

int test2()
{
    typedef movable_not_copyable mv_t;
    
    std::cout << __LINE__ << std::endl;
    mv_t mv = mv_t(0); 
    std::cout << __LINE__ << std::endl;
    mv_t mv3(0);
    std::cout << __LINE__ << std::endl;
    const mv_t  mv2(0);
    std::cout << __LINE__ << std::endl;
    //mv = // works
        MY_STD::move(mv_t(0)); // works
    //mv = mv2; // shouldn't work
    //mv = mv3; // shouldn't work
    std::cout << __LINE__ << std::endl;
    //mv =  // shouldn't work
        MY_STD::move(mv2); // works
    std::cout << __LINE__ << std::endl;
    mv =  // works
        MY_STD::move(mv3); // works


    return 0;
}

int test3()
{
    typedef movable_with_const_rv_ref mv_t;
    
    mv_t mv = mv_t(), mv3;
    const mv_t  mv2;
    mv = // works
        MY_STD::move(mv_t()); // works
    //mv = mv2; // shouldn't work
    //mv = mv3; // shouldn't work
    mv = // works
        MY_STD::move(mv2); // works
    mv = // works
        MY_STD::move(mv3); // works


    return 0;
}

int test4()
{
    typedef movable_not_copyable mv_t;

    mv_t mv = mv_t(0), mv3(0);
    const mv_t  mv2(0);

    std::map<int, mv_t> mv_map;

    mv_map[0] =  // works
        MY_STD::move(mv);  // works
    mv_map[0] =  // works
        MY_STD::move(mv_map[1]);  // works

    //mv_map[0] = mv;// shouldn't work
    //mv_map[0] = mv_map[1];// shouldn't work

    return 0;
}

struct X
{
    X(int d = 0) : id(instances++)
    {
        std::cout << "X" << id << ": construct\n";
    }
    
    X(X const& rhs) : id(instances++)
    {
        std::cout << "X" << id << ": <- " << "X" << rhs.id << ": **copy**\n";
        ++copies;
    }

    // This particular test doesn't exercise assignment, but for
    // completeness:
    X& operator=(const X& rhs)
    {
        std::cout << "X" << id << ": <- " << "X" << rhs.id << ": assign\n";
        return *this;
    }

    X& operator=(STDEX_RV_REF(X) rhs)
    {
        //X& rhs = rhs_;
        std::cout << "X" << id << ": <- " << "X" << rhs.id << ": move assign\n";
        return *this;
    }
    
    X(STDEX_RV_REF(X) rhs) : id(instances++)
    {
        //X& rhs = rhs_;
        std::cout << "X" << id << ": <- " << "X" << rhs.id << ": ..move construct..\n";
        ++copies;
    }

    ~X() { std::cout << "X" << id << ": destroy\n"; }

    unsigned id;
    
    static unsigned copies;
    static unsigned instances;
};

unsigned X::copies = 0;
unsigned X::instances = 0;

#define CHECK_COPIES( stmt, min, max, comment )                         \
{                                                                       \
    unsigned const old_copies = X::copies;                              \
                                                                        \
    std::cout << "\n" comment "\n" #stmt "\n===========\n";             \
    {                                                                   \
        stmt;                                                           \
    }                                                                   \
    unsigned const n = X::copies - old_copies;                          \
    volatile unsigned const minv(min), maxv(max);                       \
    DYNAMIC_VERIFY(n <= maxv);                                              \
    if (n > maxv)                                                       \
        std::cout << "*** max is too low or compiler is buggy ***\n";   \
    DYNAMIC_VERIFY(n >= minv);                                              \
    if (n < minv)                                                       \
        std::cout << "*** min is too high or compiler is buggy ***\n";  \
                                                                        \
    std::cout << "-----------\n"                                        \
              << n << "/" << max                                        \
              << " possible copies/moves made\n"                        \
              << max - n << "/" << max - min                            \
              << " possible elisions performed\n\n";                    \
                                                                        \
    if (n > minv)                                                       \
        std::cout << "*** " << n - min                                  \
                  << " possible elisions missed! ***\n";                \
}

struct trace
{
    trace(char const* name)
        : m_name(name)
    {
        std::cout << "->: " << m_name << "\n";
    }
    
    ~trace()
    {
        std::cout << "<-: " << m_name << "\n";
    }
    
    char const* m_name;
};

void sink(X)
{
  trace t("sink");
}

X nrvo_source()
{
    trace t("nrvo_source");
    X a;
    return a;
}

X urvo_source()
{
    trace t("urvo_source");
    return X();
}

X identity(X a)
{
    trace t("identity");
    return a;
}

X lvalue_;
X& lvalue()
{
    return lvalue_;
}
typedef X rvalue;

X ternary( bool y )
{
    X a, b;
    MY_STD::move(y ? a : b);
    return a;
}

int main(void)
{
    using namespace stdex;

    int argc = 1000;
    
    RUN_TEST(test1);
    RUN_TEST(test2);
    RUN_TEST(test3);
    RUN_TEST(test4);
    // Double parens prevent "most vexing parse"
    CHECK_COPIES( X a(( lvalue() )), 1U, 1U, "Direct initialization from lvalue");
    CHECK_COPIES( X a(( rvalue(0) )), 0U, 1U, "Direct initialization from rvalue");
    
    CHECK_COPIES( X a = lvalue(), 1U, 1U, "Copy initialization from lvalue" );
    CHECK_COPIES( X a = rvalue(), 0U, 1U, "Copy initialization from rvalue" );

    CHECK_COPIES( sink( lvalue() ), 1U, 1U, "Pass lvalue by value" );
    CHECK_COPIES( sink( rvalue() ), 0U, 1U, "Pass rvalue by value" );

    CHECK_COPIES( nrvo_source(), 0U, 1U, "Named return value optimization (NRVO)" );
    CHECK_COPIES( urvo_source(), 0U, 1U, "Unnamed return value optimization (URVO)" );

    // Just to prove these things compose properly
    CHECK_COPIES( X a(urvo_source()), 0U, 2U, "Return value used as ctor arg" );
    
    // Expect to miss one possible elision here
    CHECK_COPIES( identity( rvalue() ), 0U, 2U, "Return rvalue passed by value" );

    // Expect to miss an elision in at least one of the following lines
    CHECK_COPIES( X a = ternary( argc == 1000 ), 0U, 2U, "Return result of ternary operation" );
    CHECK_COPIES( X a = ternary( argc != 1000 ), 0U, 2U, "Return result of ternary operation again" );
    return 0;
}

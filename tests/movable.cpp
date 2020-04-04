#include "../stdex/include/core.h"
#include "../stdex/include/move.hpp"

#include <cstdlib>
#include <iostream>
#include <cerrno>
#include <limits>
#include <map>
#include <typeinfo>

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
    ~movable(){} // eliminates compiler generated move-members

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
    ~movable_only(){} // eliminates compiler generated move-members

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
    ~movable_not_copyable(){} // eliminates compiler generated move-members


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

class movable_not_copyable_child:
    public movable_not_copyable
{

public:
    movable_not_copyable_child(int v = 0): movable_not_copyable(v)
    {
        std::cout << "movable_not_copyable_child(int)" << std::endl;
    }
    movable_not_copyable_child(STDEX_RV_REF(movable_not_copyable_child) other):
        movable_not_copyable(MY_STD::move(other))
    {
        //movable_not_copyable &other = other_;
        std::cout << "movable_not_copyable_child(rv_ref)" << std::endl;
    }
    ~movable_not_copyable_child(){} // eliminates compiler generated move-members

    movable_not_copyable_child& operator=(STDEX_RV_REF(movable_not_copyable_child) other)
    {
        //movable_not_copyable &other = other_;
        std::cout << "movable_not_copyable_child = rv_ref" << std::endl;
        movable_not_copyable_child tmp(MY_STD::move(other));

        using std::swap;
        this->swap(tmp);

        return *this;
    }

    friend void swap(movable_not_copyable_child &lhs, movable_not_copyable_child &rhs)
    {
        lhs.swap(rhs);
    }

    void swap(movable_not_copyable_child &other)
    {
        using std::swap;
        typedef movable_not_copyable base_type;
        static_cast<base_type&>(*this).swap(other);
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

        (void)(&other);
        
    }

    ~movable_with_const_rv_ref(){} // eliminates compiler generated move-members

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
        (void)(&d);
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

class Foo
{
public:
   int i;
   explicit Foo(int val)      : i(val)   {}

   Foo(STDEX_RV_REF(Foo) obj) : i(obj.i) {}

   Foo& operator=(STDEX_RV_REF(Foo) rhs)
   {  i = rhs.i; rhs.i = 0; return *this; }

   Foo& operator=(const Foo &rhs)
   {  i = rhs.i; return *this;   } //(1)

   template<class U> //(*) TEMPLATED ASSIGNMENT, potential problem
   Foo& operator=(const U& rhs)
   {  i = -rhs.i; return *this;  } //(2)
};

int test5()
{
    Foo foo1(1);
    Foo foo2(2);
    foo2 = foo1; // Calls (1) in C++11 but (2) in C++98
    DYNAMIC_VERIFY(foo2.i == foo1.i);
    const Foo foo5(5);
    foo2 = foo5; // Calls (1) in C++11 but (2) in C++98
    DYNAMIC_VERIFY(foo2.i == foo5.i);

    return 0;
}

void test6_func(STDEX_RV_REF(movable))
{

}

int test6()
{
    // Binding of rvalue references to lvalues
    movable m(0);
    STDEX_RV_REF(movable) r = MY_STD::move(m);
    test6_func(r);

    return 0;
}

int test7()
{
    typedef movable_not_copyable_child mv_t;
    
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

enum ret_status {by_lvalue, by_rvalue, by_const_rvalue};

template<class T>
struct Overloaded
{
    static ret_status call( T const &) { std::cout << "by_lvalue" << std::endl; return by_lvalue; }
    static ret_status call( STDEX_RV_REF(T) val) { std::cout << "by_rvalue" << std::endl; val++; return by_rvalue; }
    static ret_status call( STDEX_RV_REF_CONST(T)) { std::cout << "by_const_rvalue" << std::endl; return by_const_rvalue; }
};
 
template< class T, typename t >
ret_status forwarding_via_forward( STDEX_FWD_REF(t) arg ) {
    std::cout << "forwarding_via_forward: class T = " << typeid(T).name() << ", typename t = " << typeid(t).name() << std::endl;
    return Overloaded<T>::call( MY_STD::forward< t >( arg ) );
}

template< class T, typename t >
ret_status forwarding_via_move( STDEX_FWD_REF(t) arg ) {
    std::cout << "forwarding_via_move: class T = " << typeid(T).name() << ", typename t = " << typeid(t).name() << std::endl;
    return Overloaded<T>::call( MY_STD::move( arg ) );
}

template< class T, typename t >
ret_status forwarding_simple(STDEX_FWD_REF(t) arg ) {
    std::cout << "forwarding_via_move: class T = " << typeid(T).name() << ", typename t = " << typeid(t).name() << std::endl;
    return Overloaded<T>::call( arg );
}

template<class T>
int forwarding_test(const T &initial_value) 
{
    {
        T x = initial_value;
        STDEX_RV_REF(T) rval = MY_STD::move(x);
        DYNAMIC_VERIFY(rval == x);
        DYNAMIC_VERIFY(x == rval);
        DYNAMIC_VERIFY(rval == initial_value);
        DYNAMIC_VERIFY(initial_value == rval);

        std::cout << "initial caller passes rvalue:\n";
        DYNAMIC_VERIFY(forwarding_via_forward<T>( rval ) == by_rvalue);
        DYNAMIC_VERIFY(rval != initial_value);
        DYNAMIC_VERIFY(x != initial_value);
        x = initial_value;
        DYNAMIC_VERIFY(rval == initial_value);
        DYNAMIC_VERIFY(forwarding_via_move<T>( rval ) == by_rvalue);
        DYNAMIC_VERIFY(initial_value != rval);
        DYNAMIC_VERIFY(initial_value != x);
        x = initial_value;
        DYNAMIC_VERIFY(rval == initial_value);
        //DYNAMIC_VERIFY(forwarding_simple<T>( rval ) == by_lvalue); // does not work for stdex
        DYNAMIC_VERIFY(rval == initial_value);
        DYNAMIC_VERIFY(x == initial_value);
    }

    {
        std::cout << "initial caller passes lvalue:\n";
        T x = initial_value;
        DYNAMIC_VERIFY(forwarding_via_forward<T>( x ) == by_lvalue);
        DYNAMIC_VERIFY(x == initial_value);
        DYNAMIC_VERIFY(forwarding_via_move<T>( x ) == by_rvalue);
        DYNAMIC_VERIFY(x != initial_value);
        x = initial_value;
        DYNAMIC_VERIFY(x == initial_value);
        DYNAMIC_VERIFY(forwarding_simple<T>( x ) == by_lvalue);
        DYNAMIC_VERIFY(x == initial_value);
    }

    {
        std::cout << "initial caller passes const lvalue:\n";
        const T x = initial_value;
        DYNAMIC_VERIFY(forwarding_via_forward<T>( x ) == by_lvalue);
        DYNAMIC_VERIFY(x == initial_value);
        DYNAMIC_VERIFY(forwarding_via_move<T>( x ) == by_const_rvalue);
        DYNAMIC_VERIFY(x == initial_value);
        DYNAMIC_VERIFY(forwarding_simple<T>( x ) == by_lvalue);
        DYNAMIC_VERIFY(x == initial_value);
    }

    {
        std::cout << "initial caller passes const rvalue:\n";
        const T x = initial_value;
        STDEX_RV_REF_CONST(T) crval = MY_STD::move(x);
        DYNAMIC_VERIFY(crval == x);
        DYNAMIC_VERIFY(x == crval);

        DYNAMIC_VERIFY(forwarding_via_forward<T>( crval ) == by_const_rvalue);
        DYNAMIC_VERIFY(crval == initial_value);
        DYNAMIC_VERIFY(forwarding_via_move<T>( crval ) == by_const_rvalue);
        DYNAMIC_VERIFY(initial_value == crval);
        //DYNAMIC_VERIFY(forwarding_simple<T>( crval ) == by_lvalue); // does not work for stdex
        DYNAMIC_VERIFY(x == initial_value);
    }

    return 0;
}

int test8()
{
    int val = 5;
    return forwarding_test(val);
}

struct forwardable:
    public movable_not_copyable_child
{
    std::size_t i;

    forwardable &operator=(const forwardable &other)
    {
        i = other.i;
        return *this;
    }

    forwardable &operator++(int)
    {
        i++;
        return *this;
    }

    bool operator==(const forwardable &other) const
    {
        return i == other.i;
    }

    bool operator!=(const forwardable &other) const
    {
        return i != other.i;
    }
};

int test9()
{
    forwardable val;
    val.i = 5;
    return forwarding_test(val);
}

int test9_1()
{
    forwardable val1, val2;
    val1.i = 5;
    val2.i = 5;
    STDEX_RV_REF(forwardable) 
        val1_rv = stdex::move(val1);
    STDEX_RV_REF(forwardable) 
        val2_rv = stdex::move(val2);
    DYNAMIC_VERIFY(val1_rv.i == val2_rv.i);
    DYNAMIC_VERIFY(val1_rv.i == val2.i);
    DYNAMIC_VERIFY(val2_rv.i == val1.i);
    DYNAMIC_VERIFY(val1_rv.i == val1.i);
    DYNAMIC_VERIFY(val2_rv.i == val2.i);
    DYNAMIC_VERIFY(&val1_rv.i == &val1.i);
    DYNAMIC_VERIFY(&val2_rv.i == &val2.i);

    return 0;
}

struct forwardable_v:
    public virtual movable_not_copyable_child
{
    std::size_t i;

    virtual
    forwardable_v &operator=(const forwardable_v &other)
    {
        i = other.i;
        return *this;
    }

    virtual
    forwardable_v &operator++(int)
    {
        i++;
        return *this;
    }

    virtual
    bool operator==(const forwardable_v &other) const
    {
        return i == other.i;
    }

    virtual
    bool operator!=(const forwardable_v &other) const
    {
        return i != other.i;
    }

    virtual ~forwardable_v() {}
};

struct forwardable_vv:
    public virtual forwardable_v,
    public Foo,
    public virtual movable
{
    forwardable_vv(int i_ = 0):
        Foo(i_),
        movable(i_)
    { 
        Foo::i = i_;
        forwardable_v::i = Foo::i;
    }

    virtual
    forwardable_vv &operator=(const forwardable_vv &other)
    {
        Foo::i = other.forwardable_v::i;
        forwardable_v::i = Foo::i;
        return *this;
    }

    virtual
    forwardable_v &operator=(const forwardable_v &other)
    {
        Foo::i = other.forwardable_v::i;
        forwardable_v::i = Foo::i;
        return *this;
    }

    virtual
    forwardable_v &operator++(int)
    {
        Foo::i++;
        forwardable_v::i = Foo::i;
        return *this;
    }

    virtual
    bool operator==(const forwardable_v &other) const
    {
        return Foo::i == int(other.forwardable_v::i);
    }

    virtual
    bool operator!=(const forwardable_v &other) const
    {
        return Foo::i != int(other.forwardable_v::i);
    }
    
    virtual ~forwardable_vv() {}
};

int test10()
{
    forwardable_v val;
    val.i = 5;
    return forwarding_test(val);
}

int test11()
{
    forwardable_vv val(5);
    return forwarding_test(val);
}

int main(void)
{
    using namespace stdex;

    int argc = 1000;
    
    RUN_TEST(test1);
    RUN_TEST(test2);
    RUN_TEST(test3);
    RUN_TEST(test4);
    RUN_TEST(test5);
    RUN_TEST(test6);
    RUN_TEST(test7);
    RUN_TEST(test8);
    RUN_TEST(test9);
    RUN_TEST(test9_1);
    RUN_TEST(test10);
    RUN_TEST(test11);
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

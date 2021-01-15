#include "../stdex/include/core.h"
#include "../stdex/include/move.hpp"

#include <cstdlib>
#include <iostream>
#include <cerrno>
#include <limits>

#define DYNAMIC_VERIFY(cond) if(!(cond)) {std::cout << "check condition \'" << #cond << "\' failed at line " << __LINE__ << std::endl; return __LINE__;}
#define RUN_TEST(test) {std::cout << #test << std::endl; int line = test(); if(line != 0) {std::cout << "failed at line " << line << std::endl; return line;}}

#ifndef __BORLANDC__
    //#undef STDEX_RV_REF
    //#define STDEX_RV_REF(Type) Type&&
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

void test_func(STDEX_RV_REF(movable))
{

}

int main()
{
    // Binding of rvalue references to lvalues
    movable m(0);
    STDEX_RV_REF(movable) r = MY_STD::move(m);
    test_func(r);  // shouldn't work

    return 0;
}

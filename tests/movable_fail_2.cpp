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
    movable(STDEX_RV_REF(movable) other)
    {
        //movable &other = other_;
        std::cout << "movable(rv_ref)" << std::endl;
        this->swap(other);
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

class movable_not_copyable:
    public movable
{
    STDEX_NOT_COPYABLE
    bool data2;
public:
    movable_not_copyable(int): movable(0), STDEX_DELETE_ICC()
    {
        std::cout << "movable_not_copyable(int)" << std::endl;
    }
    movable_not_copyable(STDEX_RV_REF(movable_not_copyable) other):
        movable(MY_STD::move(other)),
        STDEX_DELETE_ICC()
    {
        //movable_not_copyable &other = other_;
        std::cout << "movable_not_copyable(rv_ref)" << std::endl;
        this->swap(other);
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

int test1()
{
    typedef movable_not_copyable mv_t;
    
    mv_t mv = mv_t(0), mv3(0);
    const mv_t  mv2(0);
    mv = mv3;


    return 0;
}

int main(void)
{
    
    RUN_TEST(test1);
    return 0;
}

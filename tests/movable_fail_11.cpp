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
    movable_only(STDEX_RV_REF_CONST(movable_only) other) : STDEX_DELETE_ICC()
    {
        //movable &other = other_;
        std::cout << "movable_only(rv_ref)" << std::endl;
        //this->swap(other);
    }
    ~movable_only(){} // eliminates compiler generated move-members

    movable_only& operator=(STDEX_RV_REF(movable_only) other)
    {
        //movable &other = other_;
        std::cout << "movable_only = rv_ref" << std::endl;
        this->swap(other);

        return *this;
    }

    movable_only& operator=(STDEX_RV_REF_CONST(movable_only) other)
    {
        //movable &other = other_;
        std::cout << "movable_only = rv_ref_const" << std::endl;
        movable_only tmp = MY_STD::move(other);
        this->swap(tmp);

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


int main()
{
    typedef movable_only mv_t;
    const mv_t  mv2(0);
    mv_t mv4 = mv2; // shouldn't work

    return 0;
}

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
struct move_only {
private:
    STDEX_MOVABLE_BUT_NOT_COPYABLE(move_only)

public:

    move_only() : STDEX_DELETE_ICC() {
        std::cout << "move_only()" << std::endl;
    }
    move_only(STDEX_RV_REF(move_only) other) : STDEX_DELETE_ICC(){
        std::cout << "move_only(STDEX_RV_REF(move_only) other)" << std::endl;
    }
    move_only& operator=(STDEX_RV_REF(move_only) other) {
        std::cout << "move_only& operator=(STDEX_RV_REF(move_only) other)" << std::endl;
        return *this;
    }
    ~move_only() {
        std::cout << "~move_only()" << std::endl;
    }

    //move_only(move_only&&) : STDEX_DELETE_ICC() {
    //    std::cout << "move_only(move_only&&)" << std::endl;
    //}
    //move_only& operator=(move_only&& other) {
    //    std::cout << __func__ << std::endl;
    //    return *this;
    //}
};

move_only source() {return move_only();}
const move_only csource() {return move_only();}

void test(move_only) {}

int main()
{
  move_only ca = move_only();
  // expected-error@+1 {{call to implicitly-deleted copy constructor of 'move_only'}}
  test(ca);

  return 0;
}

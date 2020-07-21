#include "../stdex/include/core.h"
#include "../stdex/include/move.hpp"

#include <cstdlib>
#include <iostream>
#include <cerrno>
#include <limits>

#define DYNAMIC_VERIFY(cond) if(!(cond)) {std::cout << "check condition \'" << #cond << "\' failed at line " << __LINE__ << std::endl; return __LINE__;}
#define RUN_TEST(test) {std::cout << #test << std::endl; int line = test(); if(line != 0) {std::cout << "failed at line " << line << std::endl; return line;}}

struct not_copyable {
private:
    STDEX_NOT_COPYABLE

public:
    not_copyable() : STDEX_DELETE_ICC() {
    }

    //not_copyable(not_copyable&) = delete;
};

void test(not_copyable){}

int main()
{
    not_copyable nc;

    test(nc);

    return 0;
}

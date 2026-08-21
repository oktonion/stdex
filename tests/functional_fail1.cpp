#include "../stdex/include/core.h"
#include "../stdex/include/functional.hpp"

int main()
{
    struct lambdas
    {
        static void func_none(){}
        static void func0(int, float){}
    };

    stdex::bind(&lambdas::func_none, 0)();

    return 0;
}

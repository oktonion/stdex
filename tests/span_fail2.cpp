#include "../stdex/include/span.hpp"


int main(void)
{


    {
        int a[4];
        stdex::span<int, 4> S(a);
        S.subspan<5, 0>();
    }
    
    return 0;
}
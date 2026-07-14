#include "../stdex/include/span.hpp"


int main(void)
{


    {
        int a[4];
        stdex::span<int, 4> S(a);
        S.subspan<3, 2>();
    }
    
    return 0;
}
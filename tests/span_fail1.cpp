#include "../stdex/include/span.hpp"


int main(void)
{

    {
        int const A[] = {1, 2, 3, 4, 5};
        stdex::span<int> S(A);

    }
    
    return 0;
}
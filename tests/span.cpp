#include "../stdex/include/span.hpp"

#include <iostream>
#define DYNAMIC_VERIFY(cond) if(!(cond)) {std::cout << "check condition \'" << #cond << "\' failed at line " << __LINE__ << std::endl; return -1;}


int main(void)
{
    {
        stdex::span<int> S;
        
        DYNAMIC_VERIFY(S.empty());
        DYNAMIC_VERIFY(S.size() == 0U);
        DYNAMIC_VERIFY(S.begin() == S.end());
    }

    {
        int A[] = {1, 2, 3, 4, 5};
        stdex::span<int> S(A);
        DYNAMIC_VERIFY(!S.empty());
        DYNAMIC_VERIFY(S.size() == 5U);
        DYNAMIC_VERIFY(S.end() - S.begin() == 5U);
        DYNAMIC_VERIFY(S.data() == &A[0]);
        for (unsigned I = 0; I != S.size(); ++I)
            DYNAMIC_VERIFY(S[I] == A[I]);

    }

    {
        int A[] = {1, 2, 3, 4, 5};
        stdex::span<const int> S(A);
        DYNAMIC_VERIFY(!S.empty());
        DYNAMIC_VERIFY(S.size() == 5U);
        DYNAMIC_VERIFY(S.end() - S.begin() == 5U);
        DYNAMIC_VERIFY(S.data() == &A[0]);
        for (unsigned I = 0; I != S.size(); ++I)
            DYNAMIC_VERIFY(S[I] == A[I]);

    }

    {
        int const A[] = {1, 2, 3, 4, 5};
        stdex::span<const int> S(A);
        DYNAMIC_VERIFY(!S.empty());
        DYNAMIC_VERIFY(S.size() == 5U);
        DYNAMIC_VERIFY(S.end() - S.begin() == 5U);
        DYNAMIC_VERIFY(S.data() == &A[0]);
        for (unsigned I = 0; I != S.size(); ++I)
            DYNAMIC_VERIFY(S[I] == A[I]);

    }

    {
        int A[] = {1, 2, 3, 4, 5};
        stdex::span<int> S(&A[0], 5);
        DYNAMIC_VERIFY(!S.empty());
        DYNAMIC_VERIFY(S.size() == 5U);
        DYNAMIC_VERIFY(S.end() - S.begin() == 5U);
        DYNAMIC_VERIFY(S.data() == &A[0]);
        for (unsigned I = 0; I != S.size(); ++I)
            DYNAMIC_VERIFY(S[I] == A[I]);
    }
    
    return 0;
}
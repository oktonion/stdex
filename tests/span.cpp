#include "../stdex/include/span.hpp"

#include <vector>

#include <iostream>
#define DYNAMIC_VERIFY(cond) if(!(cond)) {std::cout << "check condition \'" << #cond << "\' failed at line " << __LINE__ << std::endl; return -1;}

template<class T>
struct span_tests
{
    typedef T type;
    static int test1(T (&data)[9])
    {
        stdex::span<T, 9> data_span = stdex::make_span<T>(data);
        DYNAMIC_VERIFY(data_span.size() == 9);
        DYNAMIC_VERIFY(data_span.size_bytes() == 9 * sizeof(T));
        DYNAMIC_VERIFY(*data_span.begin() == 0);
        DYNAMIC_VERIFY(*data_span.data() == 0);
        DYNAMIC_VERIFY(data_span.front() == 0);
        DYNAMIC_VERIFY(data_span.back() == 8);
        DYNAMIC_VERIFY(data_span[0] == data[0]);
        DYNAMIC_VERIFY(data_span[1] == data[1]);
        DYNAMIC_VERIFY(data_span[2] == data[2]);
        DYNAMIC_VERIFY(data_span[3] == data[3]);
        DYNAMIC_VERIFY(data_span[4] == data[4]);
        DYNAMIC_VERIFY(data_span[5] == data[5]);
        DYNAMIC_VERIFY(data_span[6] == data[6]);
        DYNAMIC_VERIFY(data_span[7] == data[7]);
        DYNAMIC_VERIFY(data_span[8] == data[8]);
        DYNAMIC_VERIFY(!data_span.empty());
        DYNAMIC_VERIFY(data_span.extent == 9);

        stdex::span<T, 3> data_span_first = data_span.template first<3>();
        DYNAMIC_VERIFY(data_span_first.extent == 3);
        DYNAMIC_VERIFY(data_span_first.size() == 3);
        DYNAMIC_VERIFY(data_span_first.front() == 0);
        DYNAMIC_VERIFY(data_span_first.back() == 2);

        stdex::span<T> data_span_first_dyn = data_span.first(4);
        DYNAMIC_VERIFY(data_span_first_dyn.extent == stdex::dynamic_extent);
        DYNAMIC_VERIFY(data_span_first_dyn.size() == 4);
        DYNAMIC_VERIFY(data_span_first_dyn.front() == 0);
        DYNAMIC_VERIFY(data_span_first_dyn.back() == 3);

        stdex::span<T, 5> data_span_last = data_span.template last<5>();
        DYNAMIC_VERIFY(data_span_last.extent == 5);
        DYNAMIC_VERIFY(data_span_last.size() == 5);
        DYNAMIC_VERIFY(data_span_last.front() == 4);
        DYNAMIC_VERIFY(data_span_last.back() == 8);

        stdex::span<T> data_span_last_dyn = data_span.last(6);
        DYNAMIC_VERIFY(data_span_last_dyn.extent == stdex::dynamic_extent);
        DYNAMIC_VERIFY(data_span_last_dyn.size() == 6);
        DYNAMIC_VERIFY(data_span_last_dyn.front() == 3);
        DYNAMIC_VERIFY(data_span_last_dyn.back() == 8);

        stdex::span<T, 3> data_span_subspan = data_span.template subspan<1, 3>();
        DYNAMIC_VERIFY(data_span_subspan.extent == 3);
        DYNAMIC_VERIFY(data_span_subspan.size() == 3);
        DYNAMIC_VERIFY(data_span_subspan.front() == 1);
        DYNAMIC_VERIFY(data_span_subspan.back() == 3);

        stdex::span<T, 1> data_span_subspan_offset = data_span.template subspan<8>();
        DYNAMIC_VERIFY(data_span_subspan_offset.extent == 1);
        DYNAMIC_VERIFY(data_span_subspan_offset.size() == 1);
        DYNAMIC_VERIFY(data_span_subspan_offset.front() == 8);
        DYNAMIC_VERIFY(data_span_subspan_offset.back() == 8);

        stdex::span<T> data_span_subspan_empty = data_span.subspan(9, 0);
        DYNAMIC_VERIFY(data_span_subspan_empty.extent == stdex::dynamic_extent);
        DYNAMIC_VERIFY(data_span_subspan_empty.size() == 0);

        stdex::span<T, 0> data_span_subspan_empty_static = data_span.template subspan<9>();
        DYNAMIC_VERIFY(data_span_subspan_empty_static.extent == 0);
        DYNAMIC_VERIFY(data_span_subspan_empty.size() == 0);

        return 0;
    }

    template<class R>
    static int test2(R &range)
    {
        stdex::span<T> range_span(range);

        DYNAMIC_VERIFY(range_span.size() == range.size());

        return 0;
    }
};

int main(void)
{
    {
        typedef span_tests<const int> tests;

        tests::type data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
        DYNAMIC_VERIFY(0 == tests::test1(data));
    }

    {
        typedef span_tests<int> tests;

        tests::type data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
        DYNAMIC_VERIFY(0 == tests::test1(data));

        //std::vector<tests::type> range(10);
        //DYNAMIC_VERIFY(0 == tests::test2(range));
    }
    
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
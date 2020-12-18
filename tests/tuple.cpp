#include "./../stdex/include/tuple.hpp"

#include "./../stdex/include/string.hpp"

#include <set>
#include <iostream>

#define VERIFY(cond) STATIC_ASSERT((cond), check)
#define DYNAMIC_VERIFY(cond) if(!(cond)) {std::cout << "check condition \'" << #cond << "\' failed at line " << __LINE__ << std::endl; return -1;}
#define RUN_TEST(test) {std::cout << #test << std::endl; int line = test(); if(line != 0) {std::cout << "failed at line " << line << std::endl; return line;}}
#define DYNAMIC_VERIFY_FAIL {std::cout << "check condition " << "failed at line " << __LINE__ << std::endl; return -1;}
#define DYNAMIC_VERIFY_ABORT(cond) if(!(cond)) {std::cout << "check condition \'" << #cond << "\' failed at line " << __LINE__ << std::endl; std::abort();}


struct S {
    int n;
    stdex::string s;
    float d;
    bool operator<(const S& rhs) const
    {
        // compares n to rhs.n,
        // then s to rhs.s,
        // then d to rhs.d
        return stdex::tie(n, s, d) < stdex::tie(rhs.n, rhs.s, rhs.d);
    }

    S(int n_, stdex::string s_, float d_):
        n(n_), s(s_), d(d_) {}
};

int main()
{
    {
        typedef stdex::tuple<int> tuple;

        tuple t = tuple();
    }

    {
        typedef stdex::tuple<int, float> tuple;

        tuple t = tuple();
    }

    {
        typedef stdex::tuple<int, float> tuple1;
        typedef stdex::tuple<double, int> tuple2;
        typedef std::pair<float, double> tuple3;

        tuple1 t1 = tuple2();
        tuple2 t2 = tuple1();
        tuple1 t1_1 = tuple3();
        tuple2 t2_1 = tuple3();

        tuple1 t1_3 = stdex::make_tuple(0, 0);
        tuple2 t2_3 = stdex::make_tuple(0, 1);

        DYNAMIC_VERIFY(stdex::get<0>(t1_3) == 0);
        DYNAMIC_VERIFY(stdex::get<1>(t1_3) == 1.f);
    }

    {
        std::set<S> set_of_s; // S is LessThanComparable
 
        S value = S(42, "Test", 3.14);
        std::set<S>::iterator iter;
        bool inserted = false;
    
        // unpacks the return value of insert into iter and inserted
        stdex::tie(iter, inserted) = set_of_s.insert(value);

        DYNAMIC_VERIFY(inserted == true);
    }
    return 0;
}

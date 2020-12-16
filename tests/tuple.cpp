#include "./../stdex/include/tuple.hpp"

#include "./../stdex/include/string.hpp"
#include <set>

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
        tuple2 t2_3 = stdex::make_tuple(0, 0);

        stdex::get<0>(t1_3) == 0;
    }

    {
        std::set<S> set_of_s; // S is LessThanComparable
 
        S value = S(42, "Test", 3.14);
        std::set<S>::iterator iter;
        bool inserted;
    
        // unpacks the return value of insert into iter and inserted
        stdex::tie(iter, inserted) = set_of_s.insert(value);

        
    }
    return 0;
}

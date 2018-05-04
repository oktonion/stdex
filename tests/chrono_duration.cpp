#include "../stdex/include/core.h"
#include "../stdex/include/chrono.hpp"

#include <iostream>
#define DYNAMIC_VERIFY(cond) if(!(cond)) {std::cout << #cond << " failed at line " << __LINE__ << std::endl; return -1;}

class ClockTime
{
	typedef stdex::chrono::hours hours;
	typedef stdex::chrono::minutes minutes;
	typedef stdex::chrono::seconds seconds;

public:
	hours hours_;
	minutes minutes_;
	seconds seconds_;

	template<typename Rep, typename Period>
	explicit
		ClockTime(const stdex::chrono::duration<Rep, Period>& d)
		: hours_(stdex::chrono::duration_cast<hours>  (d)),
		minutes_(stdex::chrono::duration_cast<minutes>(d % hours(1))),
		seconds_(stdex::chrono::duration_cast<seconds>(d % minutes(1))) { }
};

template<typename T>
struct type_emulator
{
	type_emulator()
		: i(T(0)) { }

	type_emulator(T j)
		: i(j) { }

	type_emulator(const type_emulator& e)
		: i(e.i) { }

	type_emulator&
		operator*=(type_emulator a)
	{
		i *= a.i;
		return *this;
	}

	type_emulator&
		operator+=(type_emulator a)
	{
		i += a.i;
		return *this;
	}

	operator T ()
	{
		return i;
	}

	T i;
};

template<typename T>
bool
operator==(type_emulator<T> a, type_emulator<T> b)
{
	return a.i == b.i;
}

template<typename T>
bool
operator<(type_emulator<T> a, type_emulator<T> b)
{
	return a.i < b.i;
}

template<typename T>
type_emulator<T>
operator+(type_emulator<T> a, type_emulator<T> b)
{
	return a += b;
}

template<typename T>
type_emulator<T>
operator*(type_emulator<T> a, type_emulator<T> b)
{
	return a *= b;
}

namespace stdex
{
	template<typename T, typename U>
	struct common_type<type_emulator<T>, U>
	{
		typedef typename common_type<T, U>::type type;
	};

	template<typename T, typename U>
	struct common_type<U, type_emulator<T> >
	{
		typedef typename common_type<U, T>::type type;
	};

	template<typename T, typename U>
	struct common_type<type_emulator<T>, type_emulator<U> >
	{
		typedef typename common_type<T, U>::type type;
	};
}
typedef type_emulator<int> int_emulator;

int main(void)
{
    using namespace stdex;
    using namespace stdex::chrono;

    // duration
    {
        duration<int> d0(3);
        duration<int> d1 = -d0;
        DYNAMIC_VERIFY(d0.count() == 3);
        DYNAMIC_VERIFY(d1.count() == -3);

        duration<int> d2 = (+d0);
        DYNAMIC_VERIFY(d2.count() == 3);

        duration<int> d3(++d2);
        DYNAMIC_VERIFY(d2.count() == 4);
        DYNAMIC_VERIFY(d3.count() == 4);

        duration<int> d4(d3++);
        DYNAMIC_VERIFY(d3.count() == 5);
        DYNAMIC_VERIFY(d4.count() == 4);

        duration<int> d5(--d4);
        DYNAMIC_VERIFY(d4.count() == 3);
        DYNAMIC_VERIFY(d5.count() == 3);

        duration<int> d6(d5--);
        DYNAMIC_VERIFY(d5.count() == 2);
        DYNAMIC_VERIFY(d6.count() == 3);
    }

    /*{
        duration<int> d0(12);
        duration<int> d1(3);
        int i = 3;

        duration<int> d2 = d0 + d1;
        DYNAMIC_VERIFY(d2.count() == 15);

        duration<int> d3 = d0 - d1;
        DYNAMIC_VERIFY(d3.count() == 9);

        duration<int> d4 = d0 * i;
        DYNAMIC_VERIFY(d4.count() == 36);

        duration<int> d5 = i * d0;
        DYNAMIC_VERIFY(d5.count() == 36);

        duration<int> d6 = d0 / i;
        DYNAMIC_VERIFY(d6.count() == 4);

        int j = d0 / d1;
        DYNAMIC_VERIFY(j == 4);
    }*/

    {
        chrono::duration<long, ratio_divide<kilo, milli> >   d1;
        chrono::duration<long, ratio_multiply<kilo, milli> > d2;
        chrono::duration<long, ratio_add<kilo, milli> >      d3;
        chrono::duration<long, ratio_subtract<kilo, milli> > d4;
    }

    {
        typedef duration<stdex::intmax_t, ratio<36 * 24 * 36525> > Years;

        Years years(23);

        DYNAMIC_VERIFY(duration_cast<seconds>(years).count()
            == duration_cast<minutes>(years).count() * 60);

        DYNAMIC_VERIFY(duration_cast<minutes>(years).count()
            == duration_cast<seconds>(years).count() / 60);
    }

    {
        duration<int> d0(12);
        duration<int> d1(3);
        int i = 5;

        DYNAMIC_VERIFY((d0 + d1).count() == 15);

        DYNAMIC_VERIFY((d0 - d1).count() == 9);

        DYNAMIC_VERIFY((d0 * i).count() == 60);

        DYNAMIC_VERIFY((i * d0).count() == 60);

        DYNAMIC_VERIFY((d0 % i).count() == 2);

        DYNAMIC_VERIFY((d0 % d1).count() == 0);

        DYNAMIC_VERIFY((d0 / i).count() == 2);

        DYNAMIC_VERIFY((d0 / d1) == 4);
    }

    {
        chrono::duration<int> d;
        ClockTime ct(d);
    }

    {
        const duration<int> d0(17);
        duration<int> d3(d0);
        d3 %= 5;
        DYNAMIC_VERIFY(d3.count() == 2);

        const duration<int> d4(7);
        duration<int> d5(d0);
        d5 %= d4;
        DYNAMIC_VERIFY(d5.count() == 3);

        const duration<int> d6 = d0 % 6;
        DYNAMIC_VERIFY(d6.count() == 5);

        const duration<int> d7(11);
        const duration<int> d8 = d0 % d7;
        DYNAMIC_VERIFY(d8.count() == 6);
    }

    {
        duration<int> d0(12);
        duration<int> d1(3);
        duration<int> d2(3);

        DYNAMIC_VERIFY(d1 < d0);
        DYNAMIC_VERIFY(d0 > d1);

        DYNAMIC_VERIFY(d0 != d1);
        DYNAMIC_VERIFY(d1 == d2);

        DYNAMIC_VERIFY(d1 <= d2);
        DYNAMIC_VERIFY(d1 >= d2);

        DYNAMIC_VERIFY(d1 <= d0);
        DYNAMIC_VERIFY(d0 >= d1);
    }

    {
        int r = 3;
        duration<int> d1(r);
        DYNAMIC_VERIFY(d1.count() == static_cast<duration<int>::rep>(r));

        double s = 8.0;
        duration<double> d2(s);
        DYNAMIC_VERIFY(d2.count() == static_cast<duration<double>::rep>(s));

        int_emulator ie(3);
        duration<int_emulator> d3(ie);
        DYNAMIC_VERIFY(d3.count() == static_cast<duration<int_emulator>::rep>(ie));

#if (CHECK_FOR_COMPILE_ERROR_TESTS == 1)
        dbl_emulator de(4.0);
        duration<dbl_emulator> d4(de);
        DYNAMIC_VERIFY(d4.count() == static_cast<duration<dbl_emulator>::rep>(de));
#endif
    }

    {
        duration<int> d0(3);
        duration<int> d0_copy(d0);
        DYNAMIC_VERIFY(d0_copy.count() == d0.count());

        duration<int, milli> d1(5);
        duration<int, micro> d1_copy(d1);
        DYNAMIC_VERIFY(d1.count() * 1000 == d1_copy.count());

        duration<double, micro> d2(8.0);
        duration<double, milli> d2_copy(d2);
        double c1 = d2.count();
        double c2 = d2_copy.count() * 1000.0;
        DYNAMIC_VERIFY(c1 == c2);

        duration<int_emulator, milli> d3(5);
        duration<int_emulator, micro> d3_copy(d3);
        DYNAMIC_VERIFY(d3.count() * 1000 == d3_copy.count());

#if (CHECK_FOR_COMPILE_ERROR_TESTS == 1)
        duration<dbl_emulator, micro> d4(5.0);
        duration<dbl_emulator, milli> d4_copy(d4);
        DYNAMIC_VERIFY(d4.count() == d4_copy.count() * dbl_emulator(1000.0));
#endif
    }
#if (CHECK_FOR_COMPILE_ERROR_TESTS == 1)
    {
        duration<double> d(3.5);
        duration<int> i = d;
    }
#endif
    return 0;
}
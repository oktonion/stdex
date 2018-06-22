#include "../stdex/include/ratio.hpp"
#include "../stdex/include/core.h"

#define VERIFY(cond) STATIC_ASSERT((cond), check)

static const stdex::intmax_t M = stdex::_intmax_t_info::signed_max;

int main(void)
{
	using namespace stdex;

	// ratio
	{
		STATIC_ASSERT((ratio_equal<ratio<2, 6>, ratio<1, 3>>::value == 1), check);
		STATIC_ASSERT((ratio_equal<ratio<2, 6>, ratio<1, 4>>::value == 0), check);

		STATIC_ASSERT((ratio_not_equal<ratio<2, 6>, ratio<1, 3>>::value == 0), check);
		STATIC_ASSERT((ratio_not_equal<ratio<2, 6>, ratio<1, 4>>::value == 1), check);

		VERIFY((ratio_less<ratio<1, 4>, ratio<1, 3>>::value == true));
		VERIFY((ratio_less<ratio<-1, 3>, ratio<1, 3>>::value == 1));

		VERIFY((ratio_less<ratio<1, 3>, ratio<1, 4>>::value == 0));
		VERIFY((ratio_less<ratio<1, 3>, ratio<-1, 3>>::value == 0));

		VERIFY((ratio_less<ratio<-1, 3>, ratio<-1, 4>>::value == 1));
		VERIFY((ratio_less<ratio<0, 4>, ratio<0, 3>>::value == 0));
		VERIFY((ratio_less<ratio<1, 3>, ratio<0, 3>>::value == 0));
		VERIFY((ratio_less<ratio<0, 3>, ratio<-1, 4>>::value == 0));

		VERIFY((ratio_less_equal<ratio<-1, 3>, ratio<-1, 3>>::value == 1));
		VERIFY((ratio_less_equal<ratio<1, 4>, ratio<1, 3>>::value == 1));

		VERIFY((ratio_less_equal<ratio<1, 4>, ratio<-1, 3>>::value == 0));
		VERIFY((ratio_less_equal<ratio<1, 3>, ratio<-1, 3>>::value == 0));

		VERIFY((ratio_greater<ratio<1, 3>, ratio<1, 4>>::value == 1));
		VERIFY((ratio_greater<ratio<1, 3>, ratio<-1, 3>>::value == 1));

		VERIFY((ratio_greater<ratio<1, 4>, ratio<1, 3>>::value == 0));
		VERIFY((ratio_greater<ratio<-1, 3>, ratio<1, 3>>::value == 0));

		VERIFY((ratio_greater_equal<ratio<1, 3>, ratio<1, 3>>::value == 1));
		VERIFY((ratio_greater_equal<ratio<1, 3>, ratio<-1, 3>>::value == 1));

		VERIFY((ratio_greater_equal<ratio<-1, 3>, ratio<1, 3>>::value == 0));
		VERIFY((ratio_greater_equal<ratio<1, 4>, ratio<1, 3>>::value == 0));

		// No overflow with same denominator
		VERIFY((ratio_less<ratio<M - 2, M>, ratio<M - 1, M>>::value == 1));

		VERIFY((ratio_less<ratio<M - 1, M>, ratio<M - 2, M>>::value == 0));

		// No overflow if signs differ
		VERIFY((ratio_less<ratio<-M, M - 1>, ratio<M - 1, M - 2>>::value == 1)); // failing then no long long

		VERIFY((ratio_less<ratio<M - 1, M - 2>, ratio<-M, M - 1>>::value == 0)); // failing then no long long

		// No overflow
		VERIFY((ratio_less<ratio<M, M - 1>, ratio<M, M - 2>>::value == 1));

		VERIFY((ratio_less<ratio<-M, M - 1>, ratio<-M, M - 2>>::value == 0));

		VERIFY((ratio_less<ratio<M - 2, M - 1>, ratio<M - 1, M>>::value == 1)); // failing then no long long

		VERIFY((ratio_less<ratio<59, 29131>, ratio<59, 29129>>::value == 1));
		VERIFY((ratio_less<ratio<4733, 13>, ratio<4751, 13>>::value == 1));
		VERIFY((ratio_less<ratio<25703, 25717>, ratio<25733, 25741>>::value == 1));
		VERIFY((ratio_less<ratio<631, 769>, ratio<673, 773>>::value == 1));
		VERIFY((ratio_less<ratio<8353, 16903>, ratio<17891, 32099>>::value == 1));
		VERIFY((ratio_less<ratio<2311, 18701>, ratio<18457, 19571>>::value == 1));

		VERIFY((ratio_less<ratio<60, 29132>, ratio<60, 29130>>::value == 1));
		VERIFY((ratio_less<ratio<4734, 14>, ratio<4752, 14>>::value == 1));
		VERIFY((ratio_less<ratio<25704, 25718>, ratio<25732, 25742>>::value == 1));
		VERIFY((ratio_less<ratio<632, 770>, ratio<674, 774>>::value == 1));
		VERIFY((ratio_less<ratio<8352, 16904>, ratio<17892, 32100>>::value == 1));
		VERIFY((ratio_less<ratio<2312, 18702>, ratio<18458, 19572>>::value == 1));

		VERIFY((ratio_less<ratio<58, 29130>, ratio<58, 29128>>::value == 1));
		VERIFY((ratio_less<ratio<4732, 12>, ratio<4750, 12>>::value == 1));
		VERIFY((ratio_less<ratio<25702, 25716>, ratio<25734, 25740>>::value == 1));
		VERIFY((ratio_less<ratio<630, 768>, ratio<672, 772>>::value == 1));
		VERIFY((ratio_less<ratio<8354, 16902>, ratio<17890, 32102>>::value == 1));
		VERIFY((ratio_less<ratio<2310, 18700>, ratio<18456, 19570>>::value == 1));

		{
			typedef ratio<1, 3> r0;
			typedef ratio<2, 6> r1;
			typedef ratio<2, -6> r2;
			typedef ratio<-2, 6> r3;

			VERIFY((r0::num == 1));
			VERIFY((r0::den == 3));

			VERIFY((r1::num == r0::num));
			VERIFY((r1::den == r0::den));
			VERIFY((r2::num == -r0::num));
			VERIFY((r2::den == r0::den));
			VERIFY((r3::num == -r0::num));
			VERIFY((r3::den == r0::den));
		}

		{
			typedef ratio<1, 4>::type r_type1;
			typedef ratio<3, 2>::type r_type2;

			typedef ratio_add<r_type1, r_type2> ra_type;

			VERIFY(ra_type::num == ra_type::type::num);
			VERIFY(ra_type::den == ra_type::type::den);
			VERIFY(ra_type::num == 7);
			VERIFY(ra_type::den == 4);

			typedef ratio_subtract<r_type1, r_type2> rs_type;

			VERIFY(rs_type::num == rs_type::type::num);
			VERIFY(rs_type::den == rs_type::type::den);
			VERIFY(rs_type::num == -5);
			VERIFY(rs_type::den == 4);

			typedef ratio_multiply<r_type1, r_type2> rm_type;

			VERIFY(rm_type::num == rm_type::type::num);
			VERIFY(rm_type::den == rm_type::type::den);
			VERIFY(rm_type::num == 3);
			VERIFY(rm_type::den == 8);

			typedef ratio_divide<r_type1, r_type2> rd_type;

			VERIFY(rd_type::num == rd_type::type::num);
			VERIFY(rd_type::den == rd_type::type::den);
			VERIFY(rd_type::num == 1);
			VERIFY(rd_type::den == 6);
		}

		{
			using stdex::intmax_t;

			const intmax_t m1 = (intmax_t)1 << (4 * sizeof(intmax_t) - 1);
			typedef ratio_add<ratio<1, (m1 - 1) * (m1 - 2)>,
							  ratio<1, (m1 - 3) * (m1 - 2)>>
				ra_type1;
			VERIFY(ra_type1::num == 2);
			VERIFY(ra_type1::den == (m1 - 1) * (m1 - 3));

			/*typedef ratio_add<ratio<M, 2>,
				ratio<-M, 3> > ra_type2;
			VERIFY(ra_type2::num == M);
			VERIFY(ra_type2::den == 6);

			typedef ratio_add<ratio<M / 7 * 5 - 1, 5>,
				ratio<-M + 2, 7> > ra_type3;
			ra_type3();

			const intmax_t m3 = M - 1;
			typedef ratio_add<ratio<-m3 / 7 * 5 - 1, 5>,
				ratio<m3, 7> > ra_type4;
			ra_type4();

			const intmax_t m4 = M / 2;
			typedef ratio_add<ratio<m4 - 5, 15>,
				ratio<m4, 35> > ra_type5;
			VERIFY(ra_type5::num == (2 * m4 - 7));
			VERIFY(ra_type5::den == 21);*/
		}

		{
			typedef ratio_add<ratio<3, 8>, ratio<5, 12>>::type r;

			VERIFY(r::num == 19);
			VERIFY(r::den == 24);
		}

		{
			typedef ratio_subtract<ratio<3, 8>, ratio<5, 12>>::type r;

			VERIFY(r::num == -1);
			VERIFY(r::den == 24);
		}

		{
			typedef ratio_multiply<ratio<3, 8>, ratio<5, 12>>::type r;

			VERIFY(r::num == 5);
			VERIFY(r::den == 32);
		}

		{
			typedef ratio_divide<ratio<3, 8>, ratio<5, 12>>::type r;

			VERIFY(r::num == 9);
			VERIFY(r::den == 10);
		}

		{
			typedef ratio<1, M> one_over_max;
			typedef ratio<2, M> two_over_max;
			typedef ratio<M, 1> max_over_one;
			typedef ratio<M, 2> max_over_two;

			typedef ratio_add<one_over_max, one_over_max>::type r1;

			VERIFY(r1::num == two_over_max::num);
			VERIFY(r1::den == two_over_max::den);

			typedef ratio_add<
				ratio<M / 2, M / 2>,
				ratio<M / 2, M / 2 + 1>>::type r2;

			VERIFY(r2::num == M);
			VERIFY(r2::den == (M / 2) + 1);

			typedef ratio_subtract<one_over_max, one_over_max>::type r3;

			VERIFY(r3::num == 0);
			VERIFY(r3::den == 1);

			typedef ratio_subtract<
				ratio<M / 2, M / 2>,
				ratio<M / 2, M / 2 + 1>>::type r4;

			VERIFY(r4::num == 1);
			VERIFY(r4::den == (M / 2) + 1);
		}

		// sanity check
		{
			VERIFY((ratio_equal<ratio<1, 3>, ratio<2, 6>>::value == (true)));
			VERIFY((ratio_not_equal<ratio<1, 3>, ratio<2, 5>>::value == (true)));
			VERIFY((ratio_less<ratio<1, 4>, ratio<1, 3>>::value == (true)));
			VERIFY((ratio_less_equal<ratio<1, 4>, ratio<1, 4>>::value && ratio_less_equal<ratio<1, 4>, ratio<1, 3>>::value == (true)));
			VERIFY((ratio_greater<ratio<1, 3>, ratio<1, 4>>::value == (true)));
			VERIFY((ratio_greater_equal<ratio<1, 4>, ratio<1, 4>>::value && ratio_greater_equal<ratio<1, 3>, ratio<1, 4>>::value == (true)));
		}

#if (CHECK_FOR_COMPILE_ERROR_TESTS == 1)
		{
			// Negative tests:
			volatile ratio<M, M> r1;  //??
			volatile ratio<-M, M> r2; //??
			volatile ratio<M, 1> r3;  //??
			volatile ratio<1, M> r4;  //??
			volatile ratio<1, 0> r5;  // error

			volatile ratio_add<ratio<M, 1>, ratio<1>>::type r6;			 // error
			volatile ratio_multiply<ratio<-M, 2>, ratio<3, 2>>::type r1; // error
			volatile ratio_multiply<ratio<M>, ratio<M>>::type r2;		 // error
		}
#endif
	}
	return 0;
}

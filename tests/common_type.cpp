#include "../stdex/include/core.h"
#include "../stdex/include/type_traits.hpp"

#define JOIN(X, Y) DO_JOIN(X, Y)
#define DO_JOIN(X, Y) DO_JOIN2(X, Y)
#define DO_JOIN2(X, Y) X##Y

#define VERIFY(cond) STATIC_ASSERT((cond), check)

struct A
{
};
struct B : A
{
};

int main(void)
{
    using namespace stdex;

    // common_type
    {
        typedef common_type<int>::type JOIN(test_int, 1_type);
        typedef common_type<const int>::type JOIN(test_const_int, 1_type);
        typedef common_type<volatile int>::type JOIN(test_volatile_int, 1_type);
        typedef common_type<const volatile int>::type JOIN(test_const_volatile_int, 1_type);

        typedef common_type<int &>::type JOIN(test_int, 1_ref_type);
        typedef common_type<const int &>::type JOIN(test_const_int, 1_ref_type);
        typedef common_type<volatile int &>::type JOIN(test_volatile_int, 1_ref_type);
        typedef common_type<const volatile int &>::type JOIN(test_const_volatile_int, 1_ref_type);

        VERIFY((is_same<test_int1_type, test_int1_type>::value));
        VERIFY((is_same<test_const_int1_type, test_const_int1_type>::value));
        VERIFY((is_same<test_const_volatile_int1_type, test_const_volatile_int1_type>::value));

        VERIFY((is_same<test_int1_ref_type, test_int1_ref_type>::value));
        VERIFY((is_same<test_const_int1_ref_type, test_const_int1_ref_type>::value));
        VERIFY((is_same<test_const_volatile_int1_ref_type, test_const_volatile_int1_ref_type>::value));

        VERIFY((is_same<test_int1_type, test_int1_ref_type>::value));
        VERIFY((is_same<test_const_volatile_int1_ref_type, test_const_int1_type>::value));
        VERIFY((is_same<test_const_volatile_int1_type, test_const_int1_ref_type>::value));

        VERIFY((is_same<test_const_volatile_int1_ref_type, test_int1_ref_type>::value));
        VERIFY((is_same<test_const_volatile_int1_ref_type, test_const_int1_type>::value));
        VERIFY((is_same<test_const_int1_type, test_const_int1_ref_type>::value));
    }

    {
        typedef common_type<B>::type JOIN(test_B, 1_type);
        typedef common_type<const B>::type JOIN(test_const_B, 1_type);
        typedef common_type<volatile B>::type JOIN(test_volatile_B, 1_type);
        typedef common_type<const volatile B>::type JOIN(test_const_volatile_B, 1_type);

        typedef common_type<B &>::type JOIN(test_B, 1_ref_type);
        typedef common_type<const B &>::type JOIN(test_const_B, 1_ref_type);
        typedef common_type<volatile B &>::type JOIN(test_volatile_B, 1_ref_type);
        typedef common_type<const volatile B &>::type JOIN(test_const_volatile_B, 1_ref_type);

        VERIFY((is_same<test_B1_type, test_B1_type>::value));
        VERIFY((is_same<test_const_B1_type, test_const_B1_type>::value));
        VERIFY((is_same<test_const_volatile_B1_type, test_const_volatile_B1_type>::value));

        VERIFY((is_same<test_B1_ref_type, test_B1_ref_type>::value));
        VERIFY((is_same<test_const_B1_ref_type, test_const_B1_ref_type>::value));
        VERIFY((is_same<test_const_volatile_B1_ref_type, test_const_volatile_B1_ref_type>::value));

        VERIFY((is_same<test_B1_type, test_B1_ref_type>::value));
        VERIFY((is_same<test_const_volatile_B1_ref_type, test_const_B1_type>::value));
        VERIFY((is_same<test_const_volatile_B1_type, test_const_B1_ref_type>::value));

        VERIFY((is_same<test_const_volatile_B1_ref_type, test_B1_ref_type>::value));
        VERIFY((is_same<test_const_volatile_B1_ref_type, test_const_B1_type>::value));
        VERIFY((is_same<test_const_B1_type, test_const_B1_ref_type>::value));
    }

    {
        typedef int type1;
        typedef int type2;
        typedef int type3;

        typedef common_type<type1, type2>::type JOIN(JOIN(test_, uid), _t1);
        typedef common_type<type2, type1>::type JOIN(JOIN(test_, uid), _t2);
        typedef common_type<type1 &, type2>::type JOIN(JOIN(test_, uid), _t1_ref);
        typedef common_type<type2 &, type1>::type JOIN(JOIN(test_, uid), _t2_ref);

        typedef common_type<const type1, type2>::type JOIN(JOIN(test_c_, uid), _t1);
        typedef common_type<const type2, type1>::type JOIN(JOIN(test_c_, uid), _t2);
        typedef common_type<const type1 &, type2>::type JOIN(JOIN(test_c_, uid), _t1_ref);
        typedef common_type<const type2 &, type1>::type JOIN(JOIN(test_c_, uid), _t2_ref);

        typedef common_type<volatile type1, type2>::type JOIN(JOIN(test_v_, uid), _t1);
        typedef common_type<volatile type2, type1>::type JOIN(JOIN(test_v_, uid), _t2);
        typedef common_type<volatile type1 &, type2>::type JOIN(JOIN(test_v_, uid), _t1_ref);
        typedef common_type<volatile type2 &, type1>::type JOIN(JOIN(test_v_, uid), _t2_ref);

        typedef common_type<const volatile type1, type2>::type JOIN(JOIN(test_cv_, uid), _t1);
        typedef common_type<const volatile type2, type1>::type JOIN(JOIN(test_cv_, uid), _t2);
        typedef common_type<const volatile type1 &, type2>::type JOIN(JOIN(test_cv_, uid), _t1_ref);
        typedef common_type<const volatile type2 &, type1>::type JOIN(JOIN(test_cv_, uid), _t2_ref);

        VERIFY((is_same<test_uid_t1, type3>::value));
        VERIFY((is_same<test_uid_t2, type3>::value));
        VERIFY((is_same<test_uid_t1_ref, type3>::value));
        VERIFY((is_same<test_uid_t2_ref, type3>::value));

        VERIFY((is_same<test_c_uid_t1, type3>::value));
        VERIFY((is_same<test_c_uid_t2, type3>::value));
        VERIFY((is_same<test_c_uid_t1_ref, type3>::value));
        VERIFY((is_same<test_c_uid_t2_ref, type3>::value));

        VERIFY((is_same<test_v_uid_t1, type3>::value));
        VERIFY((is_same<test_v_uid_t2, type3>::value));
        VERIFY((is_same<test_v_uid_t1_ref, type3>::value));
        VERIFY((is_same<test_v_uid_t2_ref, type3>::value));

        VERIFY((is_same<test_cv_uid_t1, type3>::value));
        VERIFY((is_same<test_cv_uid_t2, type3>::value));
        VERIFY((is_same<test_cv_uid_t1_ref, type3>::value));
        VERIFY((is_same<test_cv_uid_t2_ref, type3>::value));
    }

    {
        typedef int type1;
        typedef double type2;
        typedef double type3;

        typedef common_type<type1, type2>::type JOIN(JOIN(test_, uid), _t1);
        typedef common_type<type2, type1>::type JOIN(JOIN(test_, uid), _t2);
        typedef common_type<type1 &, type2>::type JOIN(JOIN(test_, uid), _t1_ref);
        typedef common_type<type2 &, type1>::type JOIN(JOIN(test_, uid), _t2_ref);

        typedef common_type<const type1, type2>::type JOIN(JOIN(test_c_, uid), _t1);
        typedef common_type<const type2, type1>::type JOIN(JOIN(test_c_, uid), _t2);
        typedef common_type<const type1 &, type2>::type JOIN(JOIN(test_c_, uid), _t1_ref);
        typedef common_type<const type2 &, type1>::type JOIN(JOIN(test_c_, uid), _t2_ref);

        typedef common_type<volatile type1, type2>::type JOIN(JOIN(test_v_, uid), _t1);
        typedef common_type<volatile type2, type1>::type JOIN(JOIN(test_v_, uid), _t2);
        typedef common_type<volatile type1 &, type2>::type JOIN(JOIN(test_v_, uid), _t1_ref);
        typedef common_type<volatile type2 &, type1>::type JOIN(JOIN(test_v_, uid), _t2_ref);

        typedef common_type<const volatile type1, type2>::type JOIN(JOIN(test_cv_, uid), _t1);
        typedef common_type<const volatile type2, type1>::type JOIN(JOIN(test_cv_, uid), _t2);
        typedef common_type<const volatile type1 &, type2>::type JOIN(JOIN(test_cv_, uid), _t1_ref);
        typedef common_type<const volatile type2 &, type1>::type JOIN(JOIN(test_cv_, uid), _t2_ref);

        VERIFY((is_same<test_uid_t1, type3>::value));
        VERIFY((is_same<test_uid_t2, type3>::value));
        VERIFY((is_same<test_uid_t1_ref, type3>::value));
        VERIFY((is_same<test_uid_t2_ref, type3>::value));

        VERIFY((is_same<test_c_uid_t1, type3>::value));
        VERIFY((is_same<test_c_uid_t2, type3>::value));
        VERIFY((is_same<test_c_uid_t1_ref, type3>::value));
        VERIFY((is_same<test_c_uid_t2_ref, type3>::value));

        VERIFY((is_same<test_v_uid_t1, type3>::value));
        VERIFY((is_same<test_v_uid_t2, type3>::value));
        VERIFY((is_same<test_v_uid_t1_ref, type3>::value));
        VERIFY((is_same<test_v_uid_t2_ref, type3>::value));

        VERIFY((is_same<test_cv_uid_t1, type3>::value));
        VERIFY((is_same<test_cv_uid_t2, type3>::value));
        VERIFY((is_same<test_cv_uid_t1_ref, type3>::value));
        VERIFY((is_same<test_cv_uid_t2_ref, type3>::value));
    }

    {
        typedef A type1;
        typedef A type2;
        typedef A type3;

        typedef common_type<type1, type2>::type JOIN(JOIN(test_, uid), _t1);
        typedef common_type<type2, type1>::type JOIN(JOIN(test_, uid), _t2);
        typedef common_type<type1 &, type2>::type JOIN(JOIN(test_, uid), _t1_ref);
        typedef common_type<type2 &, type1>::type JOIN(JOIN(test_, uid), _t2_ref);

        typedef common_type<const type1, type2>::type JOIN(JOIN(test_c_, uid), _t1);
        typedef common_type<const type2, type1>::type JOIN(JOIN(test_c_, uid), _t2);
        typedef common_type<const type1 &, type2>::type JOIN(JOIN(test_c_, uid), _t1_ref);
        typedef common_type<const type2 &, type1>::type JOIN(JOIN(test_c_, uid), _t2_ref);

        typedef common_type<volatile type1, type2>::type JOIN(JOIN(test_v_, uid), _t1);
        typedef common_type<volatile type2, type1>::type JOIN(JOIN(test_v_, uid), _t2);
        typedef common_type<volatile type1 &, type2>::type JOIN(JOIN(test_v_, uid), _t1_ref);
        typedef common_type<volatile type2 &, type1>::type JOIN(JOIN(test_v_, uid), _t2_ref);

        typedef common_type<const volatile type1, type2>::type JOIN(JOIN(test_cv_, uid), _t1);
        typedef common_type<const volatile type2, type1>::type JOIN(JOIN(test_cv_, uid), _t2);
        typedef common_type<const volatile type1 &, type2>::type JOIN(JOIN(test_cv_, uid), _t1_ref);
        typedef common_type<const volatile type2 &, type1>::type JOIN(JOIN(test_cv_, uid), _t2_ref);

        VERIFY((is_same<test_uid_t1, type3>::value));
        VERIFY((is_same<test_uid_t2, type3>::value));
        VERIFY((is_same<test_uid_t1_ref, type3>::value));
        VERIFY((is_same<test_uid_t2_ref, type3>::value));

        VERIFY((is_same<test_c_uid_t1, type3>::value));
        VERIFY((is_same<test_c_uid_t2, type3>::value));
        VERIFY((is_same<test_c_uid_t1_ref, type3>::value));
        VERIFY((is_same<test_c_uid_t2_ref, type3>::value));

        /*
        VERIFY((is_same<test_v_uid_t1, type3>::value));
        VERIFY((is_same<test_v_uid_t2, type3>::value));
        VERIFY((is_same<test_v_uid_t1_ref, type3>::value));
        VERIFY((is_same<test_v_uid_t2_ref, type3>::value));

        VERIFY((is_same<test_cv_uid_t1, type3>::value));
        VERIFY((is_same<test_cv_uid_t2, type3>::value));
        VERIFY((is_same<test_cv_uid_t1_ref, type3>::value));
        VERIFY((is_same<test_cv_uid_t2_ref, type3>::value));
        */
    }

    {

        //COMMON_TYPE_TEST_ALL_2(int, int, int, 1);
        //COMMON_TYPE_TEST_ALL_2(int, double, double, 2);
        //COMMON_TYPE_TEST_2(NO_CV, A, A, A, 3);
        //COMMON_TYPE_TEST_2(const, A, A, A, 4);
        //COMMON_TYPE_TEST_2(NO_CV, B, A, A, 5);
    }
    return 0;
}

#include "../stdex/include/core.h"
#include "../stdex/include/type_traits.hpp"

#define JOIN( X, Y ) DO_JOIN( X, Y )
#define DO_JOIN( X, Y ) DO_JOIN2(X,Y)
#define DO_JOIN2( X, Y ) X##Y

#define VERIFY(cond) STATIC_ASSERT((cond), check)

struct A { };
struct B : A { };
struct C : A { };
struct D : B { };

template<class type1>
void COMMON_TYPE_TEST_1_CPP98() 
{
  using namespace stdex;
#define uid cpp98
  typedef typename common_type<type1>::type JOIN(test_t,uid); 
  {STATIC_ASSERT( (is_same<JOIN(test_t,uid), JOIN(test_t,uid)>::value), check_1);} 
  typedef typename common_type<const type1>::type JOIN(test_t,JOIN(uid,c));
  {STATIC_ASSERT( (is_same<JOIN(test_t,JOIN(uid,c)), 
                   JOIN(test_t,JOIN(uid,c))>::value), check_2);} 
  typedef typename common_type<volatile type1>::type JOIN(test_t,JOIN(uid,v));
  {STATIC_ASSERT( (is_same<JOIN(test_t,JOIN(uid,v)), 
                   JOIN(test_t,JOIN(uid,v))>::value), check_3);} 
  typedef typename common_type<const volatile type1>::type JOIN(test_t,JOIN(uid,cv));
  {STATIC_ASSERT( (is_same<JOIN(test_t,JOIN(uid,cv)), 
                   JOIN(test_t,JOIN(uid,cv))>::value), check_4);} 
  typedef typename common_type<type1 &>::type JOIN(test_t,JOIN(uid,l));
  {STATIC_ASSERT( (is_same<JOIN(test_t,JOIN(uid,l)), 
                   JOIN(test_t,JOIN(uid,l))>::value), check_5);} 
  typedef typename common_type<const type1 &>::type JOIN(test_t,JOIN(uid,lc));
  {STATIC_ASSERT( (is_same<JOIN(test_t,JOIN(uid,lc)), 
                   JOIN(test_t,JOIN(uid,lc))>::value), check_6);} 
  typedef typename common_type<volatile type1 &>::type JOIN(test_t,JOIN(uid,lv));
  {STATIC_ASSERT( (is_same<JOIN(test_t,JOIN(uid,lv)), 
                   JOIN(test_t,JOIN(uid,lv))>::value), check_7);} 
  typedef typename common_type<const volatile type1 &>::type JOIN(test_t,JOIN(uid,lcv));
  {STATIC_ASSERT( (is_same<JOIN(test_t,JOIN(uid,lcv)), 
                   JOIN(test_t,JOIN(uid,lcv))>::value), check_8);} 
#undef uid
}

template<class type1>
void COMMON_TYPE_TEST_1_CPP11() 
{
#ifdef _STDEX_NATIVE_CPP11_SUPPORT
  using namespace stdex;
#define uid cpp11
  typedef typename common_type<type1 &&>::type JOIN(test_t,JOIN(uid,r));
  {STATIC_ASSERT( (is_same<JOIN(test_t,JOIN(uid,r)), 
                   JOIN(test_t,JOIN(uid,r))>::value), check_9);} 
  typedef typename common_type<const type1 &&>::type JOIN(test_t,JOIN(uid,rc));
  {STATIC_ASSERT( (is_same<JOIN(test_t,JOIN(uid,rc)), 
                   JOIN(test_t,JOIN(uid,rc))>::value), check_10);} 
  typedef typename common_type<volatile type1 &&>::type JOIN(test_t,JOIN(uid,rv));
  {STATIC_ASSERT( (is_same<JOIN(test_t,JOIN(uid,rv)), 
                   JOIN(test_t,JOIN(uid,rv))>::value), check_11);} 
  typedef typename common_type<const volatile type1 &&>::type JOIN(test_t,JOIN(uid,rcv));
  {STATIC_ASSERT( (is_same<JOIN(test_t,JOIN(uid,rcv)), 
                   JOIN(test_t,JOIN(uid,rcv))>::value), check_12);}
#undef uid
#endif // _STDEX_NATIVE_CPP11_SUPPORT
}

template<class type1, class type2, class type3>
void COMMON_TYPE_TEST_2_IMPL()
{
  using namespace stdex;
#define uid COMMON_TYPE_TEST_2_IMPL
  typedef typename common_type<type1, type2>::type  	JOIN(JOIN(test, uid),_t1);
  typedef typename common_type<type2, type1>::type  	JOIN(JOIN(test, uid),_t2);
  {STATIC_ASSERT( (is_same<JOIN(JOIN(test, uid),_t1), type3>::value), check1);}
  {STATIC_ASSERT( (is_same<JOIN(JOIN(test, uid),_t2), type3>::value), check2);}
#undef uid
}


#ifdef _STDEX_NATIVE_CPP11_SUPPORT

#define COMMON_TYPE_TEST_1(type1) \
    COMMON_TYPE_TEST_1_CPP98<type1>(); \
    COMMON_TYPE_TEST_1_CPP11<type1>()

#define COMMON_TYPE_TEST_2(cv_qual, type1, type2, type3) \
  COMMON_TYPE_TEST_2_IMPL<cv_qual type1, type2, type3>(); \
  COMMON_TYPE_TEST_2_IMPL<cv_qual type1 &, type2, type3>(); \
  COMMON_TYPE_TEST_2_IMPL<cv_qual type1 &&, type2, type3>()

#else // _STDEX_NATIVE_CPP11_SUPPORT

#define COMMON_TYPE_TEST_1(type1) \
    COMMON_TYPE_TEST_1_CPP98<type1>()

#define COMMON_TYPE_TEST_2(cv_qual, type1, type2, type3) \
  COMMON_TYPE_TEST_2_IMPL<cv_qual type1, type2, type3>(); \
  COMMON_TYPE_TEST_2_IMPL<cv_qual type1 &, type2, type3>();

#endif // _STDEX_NATIVE_CPP11_SUPPORT

int main(void)
{
    using namespace stdex;

    // Positive tests.
    {
        COMMON_TYPE_TEST_1(int);
        COMMON_TYPE_TEST_1(double);
        COMMON_TYPE_TEST_1(A);
        COMMON_TYPE_TEST_1(B);
    }
    
    // common_type
   {
        typedef common_type<int>::type JOIN(test_int, 1_type);
        typedef common_type<const int>::type JOIN(test_const_int, 1_type);
        typedef common_type<volatile int>::type JOIN(test_volatile_int, 1_type);
        typedef common_type<const volatile int>::type JOIN(test_const_volatile_int, 1_type);

        typedef common_type<int&>::type JOIN(test_int, 1_ref_type);
        typedef common_type<const int&>::type JOIN(test_const_int, 1_ref_type);
        typedef common_type<volatile int&>::type JOIN(test_volatile_int, 1_ref_type);
        typedef common_type<const volatile int&>::type JOIN(test_const_volatile_int, 1_ref_type);

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

        typedef common_type<B&>::type JOIN(test_B, 1_ref_type);
        typedef common_type<const B&>::type JOIN(test_const_B, 1_ref_type);
        typedef common_type<volatile B&>::type JOIN(test_volatile_B, 1_ref_type);
        typedef common_type<const volatile B&>::type JOIN(test_const_volatile_B, 1_ref_type);

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

        typedef common_type<type1, type2>::type      JOIN(JOIN(test_, uid), _t1);
        typedef common_type<type2, type1>::type      JOIN(JOIN(test_, uid), _t2);
        typedef common_type<type1&, type2>::type      JOIN(JOIN(test_, uid), _t1_ref);
        typedef common_type<type2&, type1>::type      JOIN(JOIN(test_, uid), _t2_ref);

        typedef common_type<const type1, type2>::type          JOIN(JOIN(test_c_, uid), _t1);
        typedef common_type<const type2, type1>::type          JOIN(JOIN(test_c_, uid), _t2);
        typedef common_type<const type1&, type2>::type      JOIN(JOIN(test_c_, uid), _t1_ref);
        typedef common_type<const type2&, type1>::type      JOIN(JOIN(test_c_, uid), _t2_ref);

        typedef common_type<volatile type1, type2>::type          JOIN(JOIN(test_v_, uid), _t1);
        typedef common_type<volatile type2, type1>::type          JOIN(JOIN(test_v_, uid), _t2);
        typedef common_type<volatile type1&, type2>::type          JOIN(JOIN(test_v_, uid), _t1_ref);
        typedef common_type<volatile type2&, type1>::type          JOIN(JOIN(test_v_, uid), _t2_ref);

        typedef common_type<const volatile type1, type2>::type          JOIN(JOIN(test_cv_, uid), _t1);
        typedef common_type<const volatile type2, type1>::type          JOIN(JOIN(test_cv_, uid), _t2);
        typedef common_type<const volatile type1&, type2>::type          JOIN(JOIN(test_cv_, uid), _t1_ref);
        typedef common_type<const volatile type2&, type1>::type          JOIN(JOIN(test_cv_, uid), _t2_ref);

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

        typedef common_type<type1, type2>::type      JOIN(JOIN(test_, uid), _t1);
        typedef common_type<type2, type1>::type      JOIN(JOIN(test_, uid), _t2);
        typedef common_type<type1&, type2>::type      JOIN(JOIN(test_, uid), _t1_ref);
        typedef common_type<type2&, type1>::type      JOIN(JOIN(test_, uid), _t2_ref);

        typedef common_type<const type1, type2>::type          JOIN(JOIN(test_c_, uid), _t1);
        typedef common_type<const type2, type1>::type          JOIN(JOIN(test_c_, uid), _t2);
        typedef common_type<const type1&, type2>::type      JOIN(JOIN(test_c_, uid), _t1_ref);
        typedef common_type<const type2&, type1>::type      JOIN(JOIN(test_c_, uid), _t2_ref);

        typedef common_type<volatile type1, type2>::type          JOIN(JOIN(test_v_, uid), _t1);
        typedef common_type<volatile type2, type1>::type          JOIN(JOIN(test_v_, uid), _t2);
        typedef common_type<volatile type1&, type2>::type          JOIN(JOIN(test_v_, uid), _t1_ref);
        typedef common_type<volatile type2&, type1>::type          JOIN(JOIN(test_v_, uid), _t2_ref);

        typedef common_type<const volatile type1, type2>::type          JOIN(JOIN(test_cv_, uid), _t1);
        typedef common_type<const volatile type2, type1>::type          JOIN(JOIN(test_cv_, uid), _t2);
        typedef common_type<const volatile type1&, type2>::type          JOIN(JOIN(test_cv_, uid), _t1_ref);
        typedef common_type<const volatile type2&, type1>::type          JOIN(JOIN(test_cv_, uid), _t2_ref);

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

        typedef common_type<type1, type2>::type      JOIN(JOIN(test_, uid), _t1);
        typedef common_type<type2, type1>::type      JOIN(JOIN(test_, uid), _t2);
        typedef common_type<type1&, type2>::type      JOIN(JOIN(test_, uid), _t1_ref);
        typedef common_type<type2&, type1>::type      JOIN(JOIN(test_, uid), _t2_ref);

        typedef common_type<const type1, type2>::type          JOIN(JOIN(test_c_, uid), _t1);
        typedef common_type<const type2, type1>::type          JOIN(JOIN(test_c_, uid), _t2);
        typedef common_type<const type1&, type2>::type      JOIN(JOIN(test_c_, uid), _t1_ref);
        typedef common_type<const type2&, type1>::type      JOIN(JOIN(test_c_, uid), _t2_ref);

        typedef common_type<volatile type1, type2>::type          JOIN(JOIN(test_v_, uid), _t1);
        typedef common_type<volatile type2, type1>::type          JOIN(JOIN(test_v_, uid), _t2);
        typedef common_type<volatile type1&, type2>::type          JOIN(JOIN(test_v_, uid), _t1_ref);
        typedef common_type<volatile type2&, type1>::type          JOIN(JOIN(test_v_, uid), _t2_ref);

        typedef common_type<const volatile type1, type2>::type          JOIN(JOIN(test_cv_, uid), _t1);
        typedef common_type<const volatile type2, type1>::type          JOIN(JOIN(test_cv_, uid), _t2);
        typedef common_type<const volatile type1&, type2>::type          JOIN(JOIN(test_cv_, uid), _t1_ref);
        typedef common_type<const volatile type2&, type1>::type          JOIN(JOIN(test_cv_, uid), _t2_ref);

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

#define NO_CV

#define COMMON_TYPE_TEST_ALL_2(type1, type2, type3) \
  COMMON_TYPE_TEST_2(NO_CV, type1, type2, type3); \
  COMMON_TYPE_TEST_2(const, type1, type2, type3); \
  COMMON_TYPE_TEST_2(volatile, type1, type2, type3); \
  COMMON_TYPE_TEST_2(const volatile, type1, type2, type3)

    {
        
        COMMON_TYPE_TEST_ALL_2(int, int, int);
        COMMON_TYPE_TEST_ALL_2(int, double, double);
        COMMON_TYPE_TEST_2(NO_CV, A, A, A);
        COMMON_TYPE_TEST_2(const, A, A, A);
        STATIC_ASSERT((is_same<common_type<A, B>::type, A>::value), common_type_for_A_and_B_should_be_A);
        COMMON_TYPE_TEST_2(NO_CV, B, A, A);
    }

    // variadic
    STATIC_ASSERT((is_same<common_type<A, B, C, D>::type, A>::value), common_type_for_A_and_B_and_C_and_D_should_be_A);
    STATIC_ASSERT((is_same<common_type<B, A, C, D>::type, A>::value), common_type_for_A_and_B_and_C_and_D_should_be_A);
    STATIC_ASSERT((is_same<common_type<C, A, B, D>::type, A>::value), common_type_for_A_and_B_and_C_and_D_should_be_A);
    STATIC_ASSERT((is_same<common_type<D, A, B, C>::type, A>::value), common_type_for_A_and_B_and_C_and_D_should_be_A);

    STATIC_ASSERT((is_same<common_type<A&, B, C, D>::type, A>::value), common_type_for_A_and_B_and_C_and_D_should_be_A);
    STATIC_ASSERT((is_same<common_type<B&, A, C, D>::type, A>::value), common_type_for_A_and_B_and_C_and_D_should_be_A);
    STATIC_ASSERT((is_same<common_type<C&, A, B, D>::type, A>::value), common_type_for_A_and_B_and_C_and_D_should_be_A);
    STATIC_ASSERT((is_same<common_type<D&, A, B, C>::type, A>::value), common_type_for_A_and_B_and_C_and_D_should_be_A);

    STATIC_ASSERT((is_same<common_type<A&, B&, C, D>::type, A>::value), common_type_for_A_and_B_and_C_and_D_should_be_A);
    STATIC_ASSERT((is_same<common_type<B&, A&, C, D>::type, A>::value), common_type_for_A_and_B_and_C_and_D_should_be_A);
    STATIC_ASSERT((is_same<common_type<C&, A&, B, D>::type, A>::value), common_type_for_A_and_B_and_C_and_D_should_be_A);
    STATIC_ASSERT((is_same<common_type<D&, A&, B, C>::type, A>::value), common_type_for_A_and_B_and_C_and_D_should_be_A);

    STATIC_ASSERT((is_same<common_type<A, B, C&, D>::type, A>::value), common_type_for_A_and_B_and_C_and_D_should_be_A);
    STATIC_ASSERT((is_same<common_type<B, A, C&, D>::type, A>::value), common_type_for_A_and_B_and_C_and_D_should_be_A);
    STATIC_ASSERT((is_same<common_type<C, A, B&, D>::type, A>::value), common_type_for_A_and_B_and_C_and_D_should_be_A);
    STATIC_ASSERT((is_same<common_type<D, A, B&, C>::type, A>::value), common_type_for_A_and_B_and_C_and_D_should_be_A);

    return 0;
}

#include "../stdex/include/core.h"
#include "../stdex/include/type_traits.hpp"

struct ClassType
{
};
typedef ClassType AbstractClass;

typedef void foo0_t();
typedef void foo1_t(int);
typedef float foo2_t(int &, double);
typedef void foo3_t(int &, bool, int, int);
typedef int &foo4_t(int, bool, int *, int[], int, int, int, int, int);
typedef void foo5_t(int, bool, int *, int[], ...);

struct Test
{
    typedef char(mfunc_type)(int, int, int);
    typedef char(mfunc_margs_type)(int, int, int);
};

int main(void)
{
    using namespace stdex;

    // is_function
    // Positive tests.
    {

        STATIC_ASSERT(is_function<foo0_t>::value == (true), should_be_function);
        STATIC_ASSERT(is_function<foo1_t>::value == (true), should_be_function);
        STATIC_ASSERT(is_function<foo2_t>::value == (true), should_be_function);
        STATIC_ASSERT(is_function<foo3_t>::value == (true), should_be_function);
        STATIC_ASSERT(is_function<foo4_t>::value == (true), should_be_function);
        STATIC_ASSERT(is_function<foo5_t>::value == (true), should_be_function);
        STATIC_ASSERT(is_function<Test::mfunc_type>::value == (true), should_be_function);
        STATIC_ASSERT(is_function<Test::mfunc_margs_type>::value == (true), should_be_function);
    }

    // Negative tests.
    {
        STATIC_ASSERT(is_function<int &>::value == (false), can_not_be_function);
        STATIC_ASSERT(is_function<void>::value == (false), can_not_be_function);
        STATIC_ASSERT(is_function<const void>::value == (false), can_not_be_function);

        STATIC_ASSERT(is_function<AbstractClass>::value == (false), can_not_be_function);
        STATIC_ASSERT(is_function<int (&)(int)>::value == (false), can_not_be_function);

        typedef int (ClassType::*mfunc1)(int);
        typedef int (ClassType::*mfunc2)(int) const;
        typedef int (ClassType::*mfunc3)(float, ...);
        typedef ClassType (ClassType::*mfunc4)(ClassType) volatile;
        typedef float (ClassType::*mfunc5)(int, float, int[], int &);

        STATIC_ASSERT(is_function<mfunc1>::value == (false), can_not_be_function);
        STATIC_ASSERT(is_function<mfunc2>::value == (false), can_not_be_function);
        STATIC_ASSERT(is_function<mfunc3>::value == (false), can_not_be_function);
        STATIC_ASSERT(is_function<mfunc4>::value == (false), can_not_be_function);

        // Sanity check.
        STATIC_ASSERT(is_function<ClassType>::value == (false), can_not_be_function);
    }

    return 0;
}

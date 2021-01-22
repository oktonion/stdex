#include "../stdex/include/core.h"
#include "../stdex/include/type_traits.hpp"

struct ClassType {};
typedef ClassType AbstractClass;

typedef void foo0_t();
typedef void foo1_t(int);
typedef float foo2_t(int&, double);
typedef void foo3_t(int&, bool, int, int);
typedef int& foo4_t(int, bool, int*, int [], int, int, int, int, int);
typedef void foo5_t(int, bool, int*, int [], ...);

struct Test
{
    typedef char(mfunc_type)(int, int, int);
    typedef char(mfunc_margs_type)(int, int, int);
};

#ifdef _STDEX_STDCALL
namespace stdcall_f
{
    typedef void _STDEX_STDCALL foo0_t();
    typedef void _STDEX_STDCALL foo1_t(int);
    typedef float _STDEX_STDCALL foo2_t(int&, double);
    typedef void _STDEX_STDCALL foo3_t(int&, bool, int, int);
    typedef int& _STDEX_STDCALL foo4_t(int, bool, int*, int [], int, int, int, int, int);
    typedef void _STDEX_STDCALL foo5_t(int, bool, int*, int [], ...);

    struct Test
    {
        typedef char(_STDEX_STDCALL mfunc_type)(int, int, int);
        typedef char(_STDEX_STDCALL mfunc_margs_type)(int, int, int);
    };
}
#endif

#ifdef _STDEX_CDECL
namespace cdecl_f
{
    typedef void _STDEX_CDECL foo0_t();
    typedef void _STDEX_CDECL foo1_t(int);
    typedef float _STDEX_CDECL foo2_t(int&, double);
    typedef void _STDEX_CDECL foo3_t(int&, bool, int, int);
    typedef int& _STDEX_CDECL foo4_t(int, bool, int*, int [], int, int, int, int, int);
    typedef void _STDEX_CDECL foo5_t(int, bool, int*, int [], ...);

    struct Test
    {
        typedef char(_STDEX_CDECL mfunc_type)(int, int, int);
        typedef char(_STDEX_CDECL mfunc_margs_type)(int, int, int);
    };
}
#endif

#ifdef _STDEX_FASTCALL
namespace fastcall_f
{
    typedef void _STDEX_FASTCALL foo0_t();
    typedef void _STDEX_FASTCALL foo1_t(int);
    typedef float _STDEX_FASTCALL foo2_t(int&, double);
    typedef void _STDEX_FASTCALL foo3_t(int&, bool, int, int);
    typedef int& _STDEX_FASTCALL foo4_t(int, bool, int*, int [], int, int, int, int, int);
    typedef void _STDEX_FASTCALL foo5_t(int, bool, int*, int [], ...);

    struct Test
    {
        typedef char(_STDEX_FASTCALL mfunc_type)(int, int, int);
        typedef char(_STDEX_FASTCALL mfunc_margs_type)(int, int, int);
    };
}
#endif

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

        STATIC_ASSERT(is_function<const foo0_t>::value == (true), should_be_function);
        STATIC_ASSERT(is_function<const foo1_t>::value == (true), should_be_function);
        STATIC_ASSERT(is_function<const foo2_t>::value == (true), should_be_function);
        STATIC_ASSERT(is_function<const foo3_t>::value == (true), should_be_function);
        STATIC_ASSERT(is_function<const foo4_t>::value == (true), should_be_function);
        STATIC_ASSERT(is_function<const foo5_t>::value == (true), should_be_function);
        STATIC_ASSERT(is_function<const Test::mfunc_type>::value == (true), should_be_function);
        STATIC_ASSERT(is_function<const Test::mfunc_margs_type>::value == (true), should_be_function);

        STATIC_ASSERT(is_function<volatile foo0_t>::value == (true), should_be_function);
        STATIC_ASSERT(is_function<volatile foo1_t>::value == (true), should_be_function);
        STATIC_ASSERT(is_function<volatile foo2_t>::value == (true), should_be_function);
        STATIC_ASSERT(is_function<volatile foo3_t>::value == (true), should_be_function);
        STATIC_ASSERT(is_function<volatile foo4_t>::value == (true), should_be_function);
        STATIC_ASSERT(is_function<volatile foo5_t>::value == (true), should_be_function);
        STATIC_ASSERT(is_function<volatile Test::mfunc_type>::value == (true), should_be_function);
        STATIC_ASSERT(is_function<volatile Test::mfunc_margs_type>::value == (true), should_be_function);

#ifdef _STDEX_STDCALL
        STATIC_ASSERT(is_function<stdcall_f::foo0_t>::value == (true), should_be_function);
        STATIC_ASSERT(is_function<stdcall_f::foo1_t>::value == (true), should_be_function);
        STATIC_ASSERT(is_function<stdcall_f::foo2_t>::value == (true), should_be_function);
        STATIC_ASSERT(is_function<stdcall_f::foo3_t>::value == (true), should_be_function);
        STATIC_ASSERT(is_function<stdcall_f::foo4_t>::value == (true), should_be_function);
        STATIC_ASSERT(is_function<stdcall_f::foo5_t>::value == (true), should_be_function);
        STATIC_ASSERT(is_function<stdcall_f::Test::mfunc_type>::value == (true), should_be_function);
        STATIC_ASSERT(is_function<stdcall_f::Test::mfunc_margs_type>::value == (true), should_be_function);
#endif

#ifdef _STDEX_CDECL
        STATIC_ASSERT(is_function<cdecl_f::foo0_t>::value == (true), should_be_function);
        STATIC_ASSERT(is_function<cdecl_f::foo1_t>::value == (true), should_be_function);
        STATIC_ASSERT(is_function<cdecl_f::foo2_t>::value == (true), should_be_function);
        STATIC_ASSERT(is_function<cdecl_f::foo3_t>::value == (true), should_be_function);
        STATIC_ASSERT(is_function<cdecl_f::foo4_t>::value == (true), should_be_function);
        STATIC_ASSERT(is_function<cdecl_f::foo5_t>::value == (true), should_be_function);
        STATIC_ASSERT(is_function<cdecl_f::Test::mfunc_type>::value == (true), should_be_function);
        STATIC_ASSERT(is_function<cdecl_f::Test::mfunc_margs_type>::value == (true), should_be_function);
#endif

#ifdef _STDEX_FASTCALL
        STATIC_ASSERT(is_function<fastcall_f::foo0_t>::value == (true), should_be_function);
        STATIC_ASSERT(is_function<fastcall_f::foo1_t>::value == (true), should_be_function);
        STATIC_ASSERT(is_function<fastcall_f::foo2_t>::value == (true), should_be_function);
        STATIC_ASSERT(is_function<fastcall_f::foo3_t>::value == (true), should_be_function);
        STATIC_ASSERT(is_function<fastcall_f::foo4_t>::value == (true), should_be_function);
        STATIC_ASSERT(is_function<fastcall_f::foo5_t>::value == (true), should_be_function);
        STATIC_ASSERT(is_function<fastcall_f::Test::mfunc_type>::value == (true), should_be_function);
        STATIC_ASSERT(is_function<fastcall_f::Test::mfunc_margs_type>::value == (true), should_be_function);
#endif
    }
                                
    // Negative tests.        
    {
        STATIC_ASSERT(is_function<int&>::value == (false), can_not_be_function);
        STATIC_ASSERT(is_function<void>::value == (false), can_not_be_function);
        STATIC_ASSERT(is_function<const void>::value == (false), can_not_be_function);

        STATIC_ASSERT(is_function<AbstractClass>::value == (false), can_not_be_function);
        STATIC_ASSERT(is_function<int(&)(int)>::value == (false), can_not_be_function);

        typedef int (ClassType::*mfunc1) (int);
        typedef int (ClassType::*mfunc2) (int) const;
        typedef int (ClassType::*mfunc3) (float, ...);
        typedef ClassType(ClassType::*mfunc4) (ClassType) volatile;
        typedef float (ClassType::*mfunc5) (int, float, int [], int&);

        STATIC_ASSERT(is_function<mfunc1>::value == (false), can_not_be_function);
        STATIC_ASSERT(is_function<mfunc2>::value == (false), can_not_be_function);
        STATIC_ASSERT(is_function<mfunc3>::value == (false), can_not_be_function);
        STATIC_ASSERT(is_function<mfunc4>::value == (false), can_not_be_function);

        STATIC_ASSERT(is_function<int[4]>::value == (false), array_can_not_be_function);

        // Sanity check.
        STATIC_ASSERT(is_function<ClassType>::value == (false), can_not_be_function);
    }    

    return 0;
}

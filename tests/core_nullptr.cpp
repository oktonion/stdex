#define forever while(1)

#define nullptr 0

#include "../stdex/include/core.h"

struct Test
{
    typedef char(Test::*mfunc_ptr_type)(int, int, int);

    void mfunc() {}
    void mfunc_const() const {}
};

int func(int, int, int) { return 0; }

typedef int(*func_type)(int, void*);

int main(void)
{// core

    volatile const bool* p = nullptr;
    ((void)(p));

    // static_assert check:
    STATIC_ASSERT(1 < 2, one_is_less_than_two);

    // nullptr checks:
    volatile void *ptr = nullptr; ((void)(ptr)); 
    STATIC_ASSERT(nullptr == nullptr && !(nullptr != nullptr), nullptr_should_be_equal_itself);
    STATIC_ASSERT(0 == nullptr && nullptr == 0, nullptr_should_be_equal_zero);
    //STATIC_ASSERT(!(nullptr > nullptr), nullptr_should_not_be_more_than_itself);
    //STATIC_ASSERT(!(nullptr < nullptr), nullptr_should_not_be_less_than_itself);
    //STATIC_ASSERT(nullptr >= nullptr, nullptr_should_more_equal_than_itself);
    //STATIC_ASSERT(nullptr <= nullptr, nullptr_should_less_equal_than_itself);
    //STATIC_ASSERT(nullptr >= 0, nullptr_should_more_equal_zero);
    //STATIC_ASSERT(nullptr <= 0, nullptr_should_less_equal_zero);
    //STATIC_ASSERT(!nullptr && nullptr == false && false == nullptr, nullptr_should_be_equal_false);
    STATIC_ASSERT(sizeof(nullptr) == sizeof(void*), nullptr_should_be_sized_equal_to_void_ptr);
    {
        int i = 0;
        int *ptr2 = &i;
        if(nullptr == ptr2)
            ptr2 = nullptr;
        (void)(&i);
        (void)(&ptr2);
    }
    
    if(! (&Test::mfunc != nullptr) || &func == nullptr) return 1;
    Test::mfunc_ptr_type mf = nullptr;
    func_type f = nullptr;
    const Test::mfunc_ptr_type cmf = nullptr;
    const func_type cf = nullptr;
    f = cf; f = f;
    mf = cmf; mf = mf;
    (void)(&f);

    struct lambdas{
        static void npt_test(void*, float*, struct npt_classss*, void(*)(), void (lambdas::*)()) {}
    };

    lambdas::npt_test(nullptr, nullptr, nullptr, nullptr, nullptr);

    // countof checks:
    {
        char carr[20];
        int iarr[20];
        float farr[20];
        volatile static const int i = int(countof(carr));
        (void)(&i);
        carr[0] = char();
        iarr[0] = int();
        farr[0] = float();

        STATIC_ASSERT(20 == countof(carr), count_of_elements_check);
        STATIC_ASSERT(20 == countof(iarr), count_of_elements_check);
        STATIC_ASSERT(20 == countof(farr), count_of_elements_check);
    }

    // forever check:
    forever
    {
        static int i = 0;
        (void)(&i);
        if(++i < 10)
            break;

        continue;
    }

    return 0;
} 

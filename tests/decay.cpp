#include "../stdex/include/core.h"
#include "../stdex/include/type_traits.hpp"

int main(void)
{
    using namespace stdex;
    
    // decay
    // Positive tests.
    {
        typedef decay<bool>::type     	test1_type;
        STATIC_ASSERT((is_same<test1_type, bool>::value == (true)),  decay_bool);

        // NB: DR 705.
        typedef decay<const int>::type  	test2_type;
        STATIC_ASSERT((is_same<test2_type, int>::value == (true)), decay_const_int);

        typedef decay<int[4]>::type     	test3_type;
        STATIC_ASSERT((is_same<test3_type, remove_extent<int[4]>::type*>::value == (true)), decay_int_4);

        typedef void (fn_type)();
        typedef decay<fn_type>::type  	test4_type;
        STATIC_ASSERT((is_same<test4_type, add_pointer<fn_type>::type>::value == (true)), decay_fn_type);
    }
    return 0;
}

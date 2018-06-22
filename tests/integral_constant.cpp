#include "../stdex/include/core.h"
#include "../stdex/include/type_traits.hpp"

int main(void)
{
    using namespace stdex;
    
    // integral_constant
    typedef true_type                      true_type;
    typedef false_type                     false_type;

    typedef true_type::value_type               true_value_type;
    typedef true_type::type                     true_type;
    typedef true_type::type::value_type         true_type_value_type;
    typedef true_type::type::type               true_type_type;

    typedef false_type::value_type              false_value_type;
    typedef false_type::type                    false_type;
    typedef false_type::type::value_type        false_type_value_type;
    typedef false_type::type::type false_type_type;

    typedef integral_constant<int, 1>       ic_one;
    typedef integral_constant<int, 0>       ic_zero;
    typedef integral_constant<int, -1>      ic_minus_one;

    typedef integral_constant<bool, true>   ic_true;
    typedef integral_constant<bool, false>  ic_false;

    STATIC_ASSERT(ic_one::value == 1, integral_constant_1);
    STATIC_ASSERT(ic_zero::value == 0, integral_constant_0);
    STATIC_ASSERT(ic_minus_one::value == -1, integral_constant_minus_1);

    STATIC_ASSERT(ic_true::value == true, integral_constant_true);
    STATIC_ASSERT(ic_false::value == false, integral_constant_false);
    return 0;
}

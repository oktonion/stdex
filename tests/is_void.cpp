#include "../stdex/include/core.h"
#include "../stdex/include/type_traits.hpp"

struct ClassType {};

int main(void)
{
    using namespace stdex;

    STATIC_ASSERT(is_void<void>::value == (true), void_should_be_void);
							 
    STATIC_ASSERT(is_void<char>::value == (false), can_not_be_void);
    STATIC_ASSERT(is_void<signed char>::value == (false), can_not_be_void);
    STATIC_ASSERT(is_void<unsigned char>::value == (false), can_not_be_void);
    STATIC_ASSERT(is_void<wchar_t>::value == (false), can_not_be_void);
    STATIC_ASSERT(is_void<short>::value == (false), can_not_be_void);
    STATIC_ASSERT(is_void<unsigned short>::value == (false), can_not_be_void);
    STATIC_ASSERT(is_void<int>::value == (false), can_not_be_void);
    STATIC_ASSERT(is_void<unsigned int>::value == (false), can_not_be_void);
    STATIC_ASSERT(is_void<long>::value == (false), can_not_be_void);
    STATIC_ASSERT(is_void<unsigned long>::value == (false), can_not_be_void);
    STATIC_ASSERT(is_void<float>::value == (false), can_not_be_void);
    STATIC_ASSERT(is_void<double>::value == (false), can_not_be_void);
    STATIC_ASSERT(is_void<long double>::value == (false), can_not_be_void);

    // Sanity check.
    STATIC_ASSERT(is_void<ClassType>::value == (false), can_not_be_void);

    return 0;
}

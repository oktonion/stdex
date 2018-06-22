#include "../stdex/include/core.h"
#include "../stdex/include/type_traits.hpp"

struct ClassType
{
};

int main(void)
{
    using namespace stdex;

    // is_fundamental
    STATIC_ASSERT(is_fundamental<void>::value == (true), should_be_fundamental);
    STATIC_ASSERT(is_fundamental<char>::value == (true), should_be_fundamental);
    STATIC_ASSERT(is_fundamental<signed char>::value == (true), should_be_fundamental);
    STATIC_ASSERT(is_fundamental<unsigned char>::value == (true), should_be_fundamental);
    STATIC_ASSERT(is_fundamental<wchar_t>::value == (true), should_be_fundamental);
    STATIC_ASSERT(is_fundamental<short>::value == (true), should_be_fundamental);
    STATIC_ASSERT(is_fundamental<unsigned short>::value == (true), should_be_fundamental);
    STATIC_ASSERT(is_fundamental<int>::value == (true), should_be_fundamental);
    STATIC_ASSERT(is_fundamental<unsigned int>::value == (true), should_be_fundamental);
    STATIC_ASSERT(is_fundamental<long>::value == (true), should_be_fundamental);
    STATIC_ASSERT(is_fundamental<unsigned long>::value == (true), should_be_fundamental);
    STATIC_ASSERT(is_fundamental<long long>::value == (true), should_be_fundamental);
    STATIC_ASSERT(is_fundamental<unsigned long long>::value == (true), should_be_fundamental);
    STATIC_ASSERT(is_fundamental<float>::value == (true), should_be_fundamental);
    STATIC_ASSERT(is_fundamental<double>::value == (true), should_be_fundamental);
    STATIC_ASSERT(is_fundamental<long double>::value == (true), should_be_fundamental);

    // libstdc++/56609
    STATIC_ASSERT(is_fundamental<stdex::nullptr_t>::value == (true), should_be_fundamental);

    // Sanity check.
    STATIC_ASSERT(is_fundamental<ClassType>::value == (false), can_not_be_fundamental);
    return 0;
}

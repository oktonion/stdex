#include "../stdex/include/core.h"
#include "../stdex/include/type_traits.hpp"

struct ClassType {};
struct ConvType 
{
	template<class T>
	operator T() { return T(); }
};
enum EnumType {};

typedef union {
	float a;
	ClassType b;
}
UnionType;

int main(void)
{
    using namespace stdex;
    
    // Positive tests.
    STATIC_ASSERT(is_enum<EnumType>::value == (true), should_be_enum);

    // Negative tests.
    STATIC_ASSERT(is_enum<void>::value == (false), should_not_be_enum);
    STATIC_ASSERT(is_enum<int>::value == (false), should_not_be_enum);
    STATIC_ASSERT(is_enum<float>::value == (false), should_not_be_enum);
    STATIC_ASSERT(is_enum<int[2]>::value == (false), should_not_be_enum);
    STATIC_ASSERT(is_enum<int*>::value == (false), should_not_be_enum);
    STATIC_ASSERT(is_enum<int(*)(int)>::value == (false), should_not_be_enum);
    STATIC_ASSERT(is_enum<float&>::value == (false), should_not_be_enum);
    STATIC_ASSERT(is_enum<float(&)(float)>::value == (false), should_not_be_enum);
    //STATIC_ASSERT(is_enum<int(ClassType::*)>::value == (false), should_not_be_enum);
    //STATIC_ASSERT(is_enum<int (ClassType::*) (int)>::value == (false), should_not_be_enum);
    //STATIC_ASSERT(is_enum<int(int)>::value == (false), should_not_be_enum);

    STATIC_ASSERT(is_enum<ConvType>::value == (false), should_not_be_enum);

    // Sanity check.
    STATIC_ASSERT(is_enum<ClassType>::value == (false), should_not_be_enum);

    return 0;
}

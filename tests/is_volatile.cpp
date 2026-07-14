#include "../stdex/include/core.h"
#include "../stdex/include/type_traits.hpp"

struct ClassType {};
typedef const ClassType cClassType;
typedef volatile ClassType& vClassTypeRef;
typedef const volatile ClassType cvClassType;
typedef volatile ClassType vClassType;
typedef void(*FuncPtrType)(int);
typedef stdex::remove_pointer<FuncPtrType>::type FuncType;
typedef volatile FuncPtrType vFuncPtrType;
typedef volatile FuncType vFuncType;

int main(void)
{
    using namespace stdex;
    
    // is_volatile
    // Positive tests.
    STATIC_ASSERT(is_volatile<volatile int>::value == (true), should_be_volatile);
    STATIC_ASSERT(is_volatile<const volatile int>::value == (true), should_be_volatile);
    STATIC_ASSERT(is_volatile<cvClassType>::value == (true), should_be_volatile);
    STATIC_ASSERT(is_volatile<vClassType>::value == (true), should_be_volatile);

    STATIC_ASSERT(is_volatile<vFuncPtrType>::value == (true), should_be_volatile);

    // Negative tests.
    STATIC_ASSERT(is_volatile<const int>::value == (false), can_not_be_volatile);
    STATIC_ASSERT(is_volatile<int>::value == (false), can_not_be_volatile);
    STATIC_ASSERT(is_volatile<ClassType>::value == (false), can_not_be_volatile);
    STATIC_ASSERT(is_volatile<cClassType>::value == (false), can_not_be_volatile);
    STATIC_ASSERT(is_volatile<vClassTypeRef>::value == (false), can_not_be_volatile);
    STATIC_ASSERT(is_volatile<cClassType>::value == (false), can_not_be_volatile);
    STATIC_ASSERT(is_volatile<cClassType&>::value == (false), can_not_be_volatile);
    STATIC_ASSERT(is_volatile<const cClassType&>::value == (false), can_not_be_volatile);

    STATIC_ASSERT(is_volatile<FuncPtrType>::value == (false), can_not_be_volatile);
    STATIC_ASSERT(is_volatile<FuncType>::value == (false), can_not_be_volatile);
    STATIC_ASSERT(is_volatile<vFuncType>::value == (false), can_not_be_volatile);
    STATIC_ASSERT(is_volatile<vFuncPtrType&>::value == (false), can_not_be_volatile);
    STATIC_ASSERT(is_volatile<vFuncType&>::value == (false), can_not_be_volatile);
    return 0;
}

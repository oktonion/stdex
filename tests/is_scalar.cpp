#include "../stdex/include/core.h"
#include "../stdex/include/type_traits.hpp"

struct ClassType {};
enum EnumType {};

struct PODType { int data; };

struct ConvTypeInt
{
	ConvTypeInt(int) {}
	template<class T>
	operator T() { return T(); }
};

struct ClassType1 { ClassType1(int); };

struct ConvClassPtrLike { 
    ConvClassPtrLike(int); 
    template<class T>
    ConvClassPtrLike(T*);

    template<class T>
    operator T*();

    int &operator *() const;
};


int main(void)
{
    using namespace stdex;
    
    // is_scalar
    STATIC_ASSERT(is_scalar<int>::value == (true), should_be_scalar);
    STATIC_ASSERT(is_scalar<float>::value == (true), should_be_scalar);
    STATIC_ASSERT(is_scalar<EnumType>::value == (true), should_be_scalar);
    STATIC_ASSERT(is_scalar<int*>::value == (true), should_be_scalar);
    STATIC_ASSERT(is_scalar<int(*)(int)>::value == (true), should_be_scalar);
    STATIC_ASSERT(is_scalar<stdex::nullptr_t>::value == (true), should_be_scalar);

    {
        typedef int(ClassType::*memobjt);
        typedef int (ClassType::*memfunct) (int);

        STATIC_ASSERT(is_scalar<memobjt>::value == (true), should_be_scalar);
        STATIC_ASSERT(is_scalar<memfunct>::value == (true), should_be_scalar);
    }

    // Sanity check.
    STATIC_ASSERT(is_scalar<ClassType>::value == (false), can_not_be_scalar);
    STATIC_ASSERT(is_scalar<PODType>::value == (false), can_not_be_scalar);
    STATIC_ASSERT(is_scalar<ConvTypeInt>::value == (false), can_not_be_scalar);
    STATIC_ASSERT(is_scalar<ClassType1>::value == (false), can_not_be_scalar);
    STATIC_ASSERT(is_scalar<ConvClassPtrLike>::value == (false), can_not_be_scalar);
    return 0;
}

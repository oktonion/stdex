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
    
    // is_pointer
    STATIC_ASSERT(is_pointer<int*>::value == (true), should_be_pointer);
    STATIC_ASSERT(is_pointer<ClassType*>::value == (true), should_be_pointer);
    STATIC_ASSERT(is_pointer<int(*)(int)>::value == (true), should_be_pointer);
                            
    // Sanity check.            
    STATIC_ASSERT(is_pointer<ClassType>::value == (false), can_not_be_pointer);
    STATIC_ASSERT(is_pointer<EnumType>::value == (false), can_not_be_pointer);
    STATIC_ASSERT(is_pointer<PODType>::value == (false), can_not_be_pointer);
    STATIC_ASSERT(is_pointer<ConvTypeInt>::value == (false), can_not_be_pointer);
    STATIC_ASSERT(is_pointer<ClassType1>::value == (false), can_not_be_pointer);
    STATIC_ASSERT(is_pointer<ConvClassPtrLike>::value == (false), can_not_be_pointer);
    return 0;
}

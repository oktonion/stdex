#include "../stdex/include/core.h"
#include "../stdex/include/type_traits.hpp"

typedef void foo0_t();

struct ClassType
{
};
typedef ClassType AbstractClass;
typedef const ClassType cClassType;
typedef const volatile ClassType cvClassType;
typedef volatile ClassType vClassType;

struct DerivedType : ClassType
{
};

struct ConvType
{
    template <class T>
    operator T() { return T(); }
};

class PolymorphicClass
{
    virtual int func() { return 0; }
};

class DerivedPolymorphic : PolymorphicClass
{
};

enum EnumType
{
};

struct PODType
{
    int data;
};

typedef PODType TType;

int main(void)
{
    using namespace stdex;

    // is_class
    // Positive tests.
    STATIC_ASSERT(is_class<ClassType>::value == (true), should_be_class);
    STATIC_ASSERT(is_class<DerivedType>::value == (true), should_be_class);
    STATIC_ASSERT(is_class<ConvType>::value == (true), should_be_class);
    STATIC_ASSERT(is_class<AbstractClass>::value == (true), should_be_class);
    STATIC_ASSERT(is_class<PolymorphicClass>::value == (true), should_be_class);
    STATIC_ASSERT(is_class<DerivedPolymorphic>::value == (true), should_be_class);

    // Negative tests.
    {
        typedef int(ClassType::*member1_t);
        typedef const int(ClassType::*member2_t);
        typedef ClassType(ClassType::*member3_t);
        typedef int (ClassType::*member4_t)(int);
        typedef int (ClassType::*member5_t)(int) const;
        typedef int (ClassType::*member6_t)(float, ...);

        //STATIC_ASSERT(is_class<UnionType>::value == (false), can_not_be_class);
        STATIC_ASSERT(is_class<void>::value == (false), can_not_be_class);
        STATIC_ASSERT(is_class<int>::value == (false), can_not_be_class);
        STATIC_ASSERT(is_class<float>::value == (false), can_not_be_class);
        STATIC_ASSERT(is_class<int[2]>::value == (false), can_not_be_class);
        STATIC_ASSERT(is_class<int *>::value == (false), can_not_be_class);
        STATIC_ASSERT(is_class<int (*)(int)>::value == (false), can_not_be_class);
        STATIC_ASSERT(is_class<float &>::value == (false), can_not_be_class);
        STATIC_ASSERT(is_class<foo0_t>::value == (false), can_not_be_class);
        STATIC_ASSERT(is_class<member1_t>::value == (false), can_not_be_class);
        STATIC_ASSERT(is_class<member2_t>::value == (false), can_not_be_class);
        STATIC_ASSERT(is_class<member3_t>::value == (false), can_not_be_class);
        STATIC_ASSERT(is_class<member4_t>::value == (false), can_not_be_class);
        STATIC_ASSERT(is_class<member5_t>::value == (false), can_not_be_class);
        STATIC_ASSERT(is_class<member6_t>::value == (false), can_not_be_class);
        //STATIC_ASSERT(is_class<int(int)>::value == (false), can_not_be_class);
        //STATIC_ASSERT(is_class<EnumType>::value == (false), can_not_be_class); // implement is_enum!!!!
    }
    return 0;
}

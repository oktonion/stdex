#include "../stdex/include/core.h"
#include "../stdex/include/type_traits.hpp"

typedef void foo0_t();

struct ClassType {};
typedef ClassType AbstractClass;
typedef const ClassType cClassType;
typedef const volatile ClassType cvClassType;
typedef volatile ClassType vClassType;

struct DerivedType : ClassType {};

struct ConvType 
{
    template<class T>
    operator T() { return T(); }
};

class PolymorphicClass
{
    virtual int func();
};

class DerivedPolymorphic : PolymorphicClass {};

enum EnumType {};

struct PODType { int data; };

typedef PODType TType;

struct ConvTypeInt
{
    ConvTypeInt(int) {}
    template<class T>
    operator T() { return T(); }
};

struct ClassType1 { ClassType1(int); };

union ConvUnionType {
    float a;
    ClassType b;
    template<class T>
    operator T() { return T(); }
    ConvUnionType(int){}
};

typedef union {
    float a;
    ClassType b;
}
UnionType;

template<bool EnableTests>
struct UnionTestsImpl
{
    static void test()
    {
        using namespace stdex;

        // Positive tests.
        STATIC_ASSERT((is_union<UnionType>::value == (true)) || (EnableTests == (false)), should_be_union);
        STATIC_ASSERT((is_union<ConvUnionType>::value == (true)) || (EnableTests == (false)), should_be_union);
    }
};

struct UnionTests:
    UnionTestsImpl<stdex::detail::_is_union_intrinsic<UnionType>::value>
{};

int main(void)
{
    using namespace stdex;
    
    // is_class
    // Positive tests.
    UnionTests::test();


    // Negative tests.
    {
        typedef int(ClassType::*member1_t);
        typedef const int(ClassType::*member2_t);
        typedef ClassType(ClassType::*member3_t);
        typedef int (ClassType::*member4_t) (int);
        typedef int (ClassType::*member5_t) (int) const;
        typedef int (ClassType::*member6_t) (float, ...);

        STATIC_ASSERT(is_union<void>::value == (false), can_not_be_union);
        STATIC_ASSERT(is_union<int>::value == (false), can_not_be_union);
        STATIC_ASSERT(is_union<float>::value == (false), can_not_be_union);
        STATIC_ASSERT(is_union<int[2]>::value == (false), can_not_be_union);
        STATIC_ASSERT(is_union<int*>::value == (false), can_not_be_union);
        STATIC_ASSERT(is_union<int(*)(int)>::value == (false), can_not_be_union);
        STATIC_ASSERT(is_union<float&>::value == (false), can_not_be_union);
        STATIC_ASSERT(is_union<foo0_t>::value == (false), can_not_be_union);
        STATIC_ASSERT(is_union<member1_t>::value == (false), can_not_be_union);
        STATIC_ASSERT(is_union<member2_t>::value == (false), can_not_be_union);
        STATIC_ASSERT(is_union<member3_t>::value == (false), can_not_be_union);
        STATIC_ASSERT(is_union<member4_t>::value == (false), can_not_be_union);
        STATIC_ASSERT(is_union<member5_t>::value == (false), can_not_be_union);
        STATIC_ASSERT(is_union<member6_t>::value == (false), can_not_be_union);
        STATIC_ASSERT(is_class<EnumType>::value == (false), can_not_be_class);

        STATIC_ASSERT(is_union<ClassType>::value == (false), can_not_be_union);
        STATIC_ASSERT(is_union<DerivedType>::value == (false), can_not_be_union);
        STATIC_ASSERT(is_union<ConvType>::value == (false), can_not_be_union);
        STATIC_ASSERT(is_union<AbstractClass>::value == (false), can_not_be_union);
        STATIC_ASSERT(is_union<PolymorphicClass>::value == (false), can_not_be_union);
        STATIC_ASSERT(is_union<DerivedPolymorphic>::value == (false), can_not_be_union);
        STATIC_ASSERT(is_union<ConvTypeInt>::value == (false), can_not_be_union);
        STATIC_ASSERT(is_union<ClassType1>::value == (false), can_not_be_union);
    }
    return 0;
}

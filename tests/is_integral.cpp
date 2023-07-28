
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
    
    STATIC_ASSERT(is_integral<void>::value == (false), can_not_be_integral);
                                 
    STATIC_ASSERT(is_integral<bool>::value == (true), should_be_integral);
    STATIC_ASSERT(is_integral<char>::value == (true), should_be_integral);
    STATIC_ASSERT(is_integral<signed char>::value == (true), should_be_integral);
    STATIC_ASSERT(is_integral<unsigned char>::value == (true), should_be_integral);
    STATIC_ASSERT(is_integral<wchar_t>::value == (true), should_be_integral);
    STATIC_ASSERT(is_integral<stdex_char16_t>::value == (true), should_be_integral);
    STATIC_ASSERT(is_integral<stdex_char32_t>::value == (true), should_be_integral);
    STATIC_ASSERT(is_integral<short>::value == (true), should_be_integral);
    STATIC_ASSERT(is_integral<unsigned short>::value == (true), should_be_integral);
    STATIC_ASSERT(is_integral<int>::value == (true), should_be_integral);
    STATIC_ASSERT(is_integral<unsigned int>::value == (true), should_be_integral);
    STATIC_ASSERT(is_integral<long>::value == (true), should_be_integral);
    STATIC_ASSERT(is_integral<unsigned long>::value == (true), should_be_integral);
                                
    STATIC_ASSERT(is_integral<float>::value == (false), should_be_integral);
    STATIC_ASSERT(is_integral<double>::value == (false), should_be_integral);
    STATIC_ASSERT(is_integral<long double>::value == (false), should_be_integral);

    // Sanity check.
    STATIC_ASSERT(is_integral<ClassType>::value == (false), can_not_be_integral);
    STATIC_ASSERT(is_integral<int ClassType::*>::value == (false), can_not_be_integral);
    STATIC_ASSERT(is_integral<int*>::value == (false), can_not_be_integral);
    STATIC_ASSERT(is_integral<int&>::value == (false), can_not_be_integral);
    STATIC_ASSERT(is_integral<EnumType>::value == (false), can_not_be_integral);
    STATIC_ASSERT(is_integral<PODType>::value == (false), can_not_be_integral);
    STATIC_ASSERT(is_integral<ConvTypeInt>::value == (false), can_not_be_integral);
    STATIC_ASSERT(is_integral<ClassType1>::value == (false), can_not_be_integral);
    STATIC_ASSERT(is_integral<ConvClassPtrLike>::value == (false), can_not_be_integral);

    typedef int (ClassType::*member_t) (int);
    STATIC_ASSERT(is_integral<member_t>::value == (false), can_not_be_integral);

    return 0;
}

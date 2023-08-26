#include "../stdex/include/core.h"
#include "../stdex/include/type_traits.hpp"

class E { public: template<class T> E(const T&) {} };
class A {};
class B : public A {};
class C {};
class D { public: operator C() { return c; } C c; };
class F : private D {};

int main(void)
{
    using namespace stdex;
    
    STATIC_ASSERT((is_convertible<B*, A*>::value == true), should_be_convertible);
    STATIC_ASSERT((is_convertible<A*, B*>::value == false), should_not_be_convertible);
    STATIC_ASSERT((is_convertible<D, C>::value == true), should_be_convertible);
    STATIC_ASSERT((is_convertible<B*, C*>::value == false), should_not_be_convertible);
    STATIC_ASSERT((is_convertible<A, E>::value == true), should_be_convertible);
    STATIC_ASSERT((is_convertible<int, float>::value == true), should_be_convertible);
    STATIC_ASSERT((is_convertible<int, E>::value == true), should_be_convertible);
    STATIC_ASSERT((is_convertible<D*, F*>::value == false), should_not_be_convertible);
    STATIC_ASSERT((is_convertible<F*, D*>::value == false), should_not_be_convertible);
    return 0;
}

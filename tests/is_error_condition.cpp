#include "../stdex/include/core.h"
#include "../stdex/include/system_error.hpp"
#include "../stdex/include/string.hpp"

template <bool Expected, class T>
void
test()
{
    STATIC_ASSERT((stdex::is_error_condition_enum<T>::value == Expected), should_be);

}

class A {
    A();
    operator stdex::error_condition () const { return stdex::error_condition(); }
};

// Specialize the template for my class
namespace stdex
{
  template <>
  struct is_error_condition_enum<A> : public stdex::true_type {};
}


int main(void)
{
    test<false, void>();
    test<false, int>();
    test<false, stdex::nullptr_t>();
    test<false, stdex::string>();

    test<true, A>();

    return 0;
}

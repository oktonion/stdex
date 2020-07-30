#include "../stdex/include/core.h"
#include "../stdex/include/functional.hpp"
#include "../stdex/include/system_error.hpp"
#include "../stdex/include/string.hpp"

#include <iostream>
#include <string>
#include <bitset>

#define VERIFY(cond) STATIC_ASSERT((cond), check)
#define DYNAMIC_VERIFY(cond) if(!(cond)) {std::cout << "check condition \'" << #cond << "\' failed at line " << __LINE__ << std::endl; return -1;}
#define RUN_TEST(test) {std::cout << #test << std::endl; int line = test(); if(line != 0) {std::cout << "failed at line " << line << std::endl; return line;}}
#define DYNAMIC_VERIFY_FAIL {std::cout << "check condition " << "failed at line " << __LINE__ << std::endl; return -1;}
#define DYNAMIC_VERIFY_ABORT(cond) if(!(cond)) {std::cout << "check condition \'" << #cond << "\' failed at line " << __LINE__ << std::endl; std::abort();}

// Verify that we can instantiate hash for every required type.
template struct stdex::hash<bool>;
template struct stdex::hash<char>;
template struct stdex::hash<signed char>;
template struct stdex::hash<unsigned char>;
template struct stdex::hash<stdex_char16_t>;
template struct stdex::hash<stdex_char32_t>;
template struct stdex::hash<short>;
template struct stdex::hash<int>;
template struct stdex::hash<long>;
template struct stdex::hash<unsigned short>;
template struct stdex::hash<unsigned int>;
template struct stdex::hash<unsigned long>;
template struct stdex::hash<float>;
template struct stdex::hash<double>;
template struct stdex::hash<long double>;
template struct stdex::hash<void*>;
template struct stdex::hash<stdex::string>;
template struct stdex::hash<stdex::error_code>;
template struct stdex::hash<stdex::error_condition>;
template struct stdex::hash<wchar_t>;
template struct stdex::hash<stdex::wstring>;

template<typename T>
int
do_test()
{
    typedef T value_type;
    typedef stdex::hash<value_type> hash_type;
    using std::size_t;

    value_type v; // default initialized is fine, same value all that matters.
    hash_type h1;
    size_t r1 = size_t(h1(v));

    hash_type h2;
    size_t r2 = size_t(h2(v));

    DYNAMIC_VERIFY( r1 == r2 );

    return 0;
}
  
int test01()
{
  DYNAMIC_VERIFY(do_test<stdex::error_code>() == 0);
  DYNAMIC_VERIFY(do_test<stdex::error_condition>() == 0);

  return 0;
}


int main()
{

    RUN_TEST(test01);

    const std::size_t big = std::size_t(1) << 31;
    std::string s;
    try {
        s.resize(big, 'a');
    } catch (const std::bad_alloc&) {
        return 0; // try to avoid a FAIL if memory allocation fails
    }
    // PR libstdc++/89629
    (void) stdex::hash<std::string>()(s);

    return 0;
}
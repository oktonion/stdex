#include "../stdex/include/core.h"
#include "../stdex/include/functional.hpp"
#include "../stdex/include/system_error.hpp"
#include "../stdex/include/string.hpp"

#include <iostream>
#include <string>
#include <bitset>

std::size_t
test_compile01()
{
  std::bitset<1> b;
  stdex::hash<std::bitset<1>/**/> h;
  return h(b);
}


int main()
{

    return 0;
}
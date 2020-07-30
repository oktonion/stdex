#include "../stdex/include/core.h"
#include "../stdex/include/functional.hpp"
#include "../stdex/include/system_error.hpp"
#include "../stdex/include/string.hpp"

#include <iostream>
#include <string>
#include <vector>

std::size_t
test_compile02()
{
  std::vector<bool> b;
  stdex::hash<std::vector<bool>/**/> h;
  return h(b);
}


int main()
{

    return 0;
}
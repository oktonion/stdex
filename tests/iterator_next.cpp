//#include "../stdex/include/core.h"
#include "../stdex/include/iterator.hpp"

#include <iostream>
#include <vector>
#include <list>

#define DYNAMIC_VERIFY(cond) if(!(cond)) {std::cout << "check condition \'" << #cond << "\' failed at line " << __LINE__ << std::endl; return __LINE__;}
#define RUN_TEST(test) {std::cout << #test << std::endl; int line = test(); if(line != 0) {std::cout << "failed at line " << line << std::endl; return line;}}

int test01()
{
  using namespace stdex;
  std::vector<int> c(3);
  std::vector<int>::iterator i = c.begin(), j;

  j = stdex::next(i, 3);
  DYNAMIC_VERIFY( (i == c.begin()) );
  DYNAMIC_VERIFY( (j == c.end()) );

  return 0;
}

int test02()
{
  std::list<int> c(1);    
  std::list<int>::iterator i = c.begin(), j;

  j = stdex::next(i);
  DYNAMIC_VERIFY( (i == c.begin()) );
  DYNAMIC_VERIFY( (j == c.end()) );

  return 0;
}

int main()
{
  RUN_TEST(test01);
  RUN_TEST(test02);
  return 0;
}

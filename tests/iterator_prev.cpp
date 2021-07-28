//#include "../stdex/include/core.h"
#include "../stdex/include/iterator.hpp"

#include <iostream>
#include <vector>
#include <list>

#define DYNAMIC_VERIFY(cond) if(!(cond)) {std::cout << "check condition \'" << #cond << "\' failed at line " << __LINE__ << std::endl; return __LINE__;}
#define RUN_TEST(test) {std::cout << #test << std::endl; int line = test(); if(line != 0) {std::cout << "failed at line " << line << std::endl; return line;}}

int test01()
{
  typedef std::vector<int>::size_type size_type;

  std::vector<int> c(static_cast<size_type>(3));    
  std::vector<int>::iterator i = c.end(), j;

  j = stdex::prev(i, 3);
  DYNAMIC_VERIFY( (i == c.end()) );
  DYNAMIC_VERIFY( (j == c.begin()) );

  return 0;
}

int test02()
{
  typedef std::list<int>::size_type size_type;

  std::list<int> c(static_cast<size_type>(3));    
  std::list<int>::iterator i = c.end(), j;

  j = stdex::prev(i, 3);
  DYNAMIC_VERIFY( (i == c.end()) );
  DYNAMIC_VERIFY( (j == c.begin()) );

  return 0;
}

int test03()
{
  typedef std::list<int>::size_type size_type;

  std::list<int> c(static_cast<size_type>(1));    
  std::list<int>::iterator i = c.end(), j;

  j = stdex::prev(i);
  DYNAMIC_VERIFY( (i == c.end()) );
  DYNAMIC_VERIFY( (j == c.begin()) );

  return 0;
}

int main()
{
  RUN_TEST(test01);
  RUN_TEST(test02);
  RUN_TEST(test03);
  return 0;
}

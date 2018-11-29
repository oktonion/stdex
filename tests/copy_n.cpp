#include "../stdex/include/core.h"
#include "../stdex/include/algorithm.hpp"
#include "../stdex/include/sstream.hpp"

#include <cstdlib>
#include <iterator>
#include <iostream>
#include <vector>
#include <deque>
#include <list>

#define DYNAMIC_VERIFY(cond) if(!(cond)) {std::cout << "check condition \'" << #cond << "\' failed at line " << __LINE__ << std::endl; return __LINE__;}
#define RUN_TEST(test) {std::cout << #test << std::endl; int line = test(); if(line != 0) {std::cout << "failed at line " << line << std::endl; return line;}}

int
test1()
{
  using namespace stdex;

  const int A[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17};
  const int N = sizeof(A) / sizeof(int);

  int i1[N];
  copy_n(A, N, i1);
  DYNAMIC_VERIFY( (equal(i1, i1 + N, A)) );

  std::vector<int> v1(N);
  copy_n(A, N, v1.begin());
  DYNAMIC_VERIFY( (equal(v1.begin(), v1.end(), A)) );

  short s1[N];
  copy_n(A, N, s1);
  DYNAMIC_VERIFY( (equal(s1, s1 + N, A)) );

  return 0;
}

int
test2()
{
  {
    const int A[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17};
    const int N = sizeof(A) / sizeof(int);
    const std::vector<int> a(A, A + N);
    int i1[N];

    copy_n(a.begin(), N, i1);
  }
  using namespace stdex;

  const int A[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17};
  const int N = sizeof(A) / sizeof(int);
  const std::vector<int> a(A, A + N);

  int i1[N];
  copy_n(a.begin(), N, i1);
  DYNAMIC_VERIFY( (equal(i1, i1 + N, a.begin())) );

  std::vector<int> v1(N);
  copy_n(a.begin(), N, v1.begin());
  DYNAMIC_VERIFY( (equal(v1.begin(), v1.end(), a.begin())) );

  short s1[N];
  copy_n(a.begin(), N, s1);
  DYNAMIC_VERIFY( (equal(s1, s1 + N, a.begin())) );
  
  return 0;
}

int
test3()
{
  using namespace stdex;

  const int A[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17};
  const int N = sizeof(A) / sizeof(int);
  const std::deque<int> a(A, A + N);

  int i1[N];
  copy_n(a.begin(), N, i1);
  DYNAMIC_VERIFY( equal(i1, i1 + N, a.begin()) );

  std::vector<int> v1(N);
  copy_n(a.begin(), N, v1.begin());
  DYNAMIC_VERIFY( equal(v1.begin(), v1.end(), a.begin()) );

  short s1[N];
  copy_n(a.begin(), N, s1);
  DYNAMIC_VERIFY( equal(s1, s1 + N, a.begin()) );
    
  return 0;
}

int
test4()
{
  using namespace stdex;

  const int A[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17};
  const int N = sizeof(A) / sizeof(int);
  const std::list<int> a(A, A + N);
  
  int i1[N];
  copy_n(a.begin(), N, i1);
  DYNAMIC_VERIFY( equal(i1, i1 + N, a.begin()) );

  std::vector<int> v1(N);
  copy_n(a.begin(), N, v1.begin());
  DYNAMIC_VERIFY( equal(v1.begin(), v1.end(), a.begin()) );

  short s1[N];
  copy_n(a.begin(), N, s1);
  DYNAMIC_VERIFY( equal(s1, s1 + N, a.begin()) );

      
  return 0;
}

/*int 
test5()
{
  using namespace stdex;

  std::vector<int> v;
  istringstream s("1 2 3 4 5");

  copy_n(std::istream_iterator<int>(s), 2, back_inserter(v));
  DYNAMIC_VERIFY( v.size() == 2 );
  DYNAMIC_VERIFY( v[0] == 1 );
  DYNAMIC_VERIFY( v[1] == 2 );

  copy_n(std::istream_iterator<int>(s), 2, back_inserter(v));
  DYNAMIC_VERIFY( v.size() == 4 );
  DYNAMIC_VERIFY( v[0] == 1 );
  DYNAMIC_VERIFY( v[1] == 2 );
  DYNAMIC_VERIFY( v[2] == 3 );
  DYNAMIC_VERIFY( v[3] == 4 );
        
  return 0;
}*/

int 
main()
{
  RUN_TEST(test1);
  RUN_TEST(test2);
  RUN_TEST(test3);
  RUN_TEST(test4);
  //RUN_TEST(test5);
  return 0;
}

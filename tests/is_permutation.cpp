#include "../stdex/include/core.h"
#include "../stdex/include/algorithm.hpp"

#include <iostream>

#define THROW_VERIFY(cond) if(!(cond)) {std::cout << "check condition \'" << #cond << "\' failed at line " << __LINE__ << std::endl; throw(__LINE__);}
#define DYNAMIC_VERIFY(cond) if(!(cond)) {std::cout << "check condition \'" << #cond << "\' failed at line " << __LINE__ << std::endl; return __LINE__;}
#define RUN_TEST(test) {std::cout << #test << std::endl; int line = test(); if(line != 0) {std::cout << "failed at line " << line << std::endl; return line;}}


struct my_equal_to
{
    bool
    operator()(int _x, int _y) const
    { return _x % 10 == _y % 10; }
};

const int arr0[] = { 11, 22, 33, 44, 55 };

int
do_test(int arr1[5], bool np = true)
{
    do
    {
        DYNAMIC_VERIFY( stdex::is_permutation(arr1, arr1 + 5, arr0) == np );
    }
    while (stdex::next_permutation(arr1, arr1 + 5));
    return 0;
}

template<class Predicate>
int
do_test(int arr1[5], Predicate pred, bool np = true)
{
    do
    {
        DYNAMIC_VERIFY( stdex::is_permutation(arr1, arr1 + 5, arr0, pred) == np );
    }
    while (stdex::next_permutation(arr1, arr1 + 5));
    return 0;
}

int
do_test2(int arr1[5], bool np = true)
{
    do
    {
        DYNAMIC_VERIFY( stdex::is_permutation(arr1, arr1 + 5, arr0) == np );
    }
    while (stdex::next_permutation(arr1, arr1 + 5));
    return 0;
}

template<class Predicate>
int
do_test2(int arr1[5], Predicate pred, bool np = true)
{
    do
    {
        DYNAMIC_VERIFY( stdex::is_permutation(arr1, arr1 + 5, arr0, pred) == np );
    }
    while (stdex::next_permutation(arr1, arr1 + 5));
    return 0;
}

int test01()
{
  int arr1[] = { 11, 22, 33, 44, 55 };
  DYNAMIC_VERIFY(do_test(arr1) == 0);
  DYNAMIC_VERIFY(do_test2(arr1) == 0);

  int arr2[] = { 11, 33, 33, 44, 55 };
  DYNAMIC_VERIFY(do_test(arr2, false) == 0);
  DYNAMIC_VERIFY(do_test2(arr2, false) == 0);

  int arr3[] = { 33, 33, 33, 44, 44 };
  DYNAMIC_VERIFY(do_test(arr3, false) == 0);
  DYNAMIC_VERIFY(do_test2(arr3, false) == 0);

  int arr4[] = { 11, 22, 33, 44, 55 };
  DYNAMIC_VERIFY(do_test(arr4, std::equal_to<int>()) == 0);
  DYNAMIC_VERIFY(do_test2(arr4, std::equal_to<int>()) == 0);

  int arr5[] = { 11, 33, 33, 44, 55 };
  DYNAMIC_VERIFY(do_test(arr5, std::equal_to<int>(), false) == 0);
  DYNAMIC_VERIFY(do_test2(arr5, std::equal_to<int>(), false) == 0);

  int arr6[] = { 33, 33, 33, 44, 44 };
  DYNAMIC_VERIFY(do_test(arr6, std::equal_to<int>(), false) == 0);
  DYNAMIC_VERIFY(do_test2(arr6, std::equal_to<int>(), false) == 0);

  int arr7[] = { 1, 2, 3, 4, 5 };
  DYNAMIC_VERIFY(do_test(arr7, my_equal_to()) == 0);
  DYNAMIC_VERIFY(do_test2(arr7, my_equal_to()) == 0);

  int arr8[] = { 1, 3, 3, 4, 5 };
  DYNAMIC_VERIFY(do_test(arr8, my_equal_to(), false) == 0);
  DYNAMIC_VERIFY(do_test2(arr8, my_equal_to(), false) == 0);

  int arr9[] = { 3, 3, 3, 4, 4 };
  DYNAMIC_VERIFY(do_test(arr9, my_equal_to(), false) == 0);
  DYNAMIC_VERIFY(do_test2(arr9, my_equal_to(), false) == 0);

  int arr10[] = { 111, 222, 333, 444, 555 };
  DYNAMIC_VERIFY(do_test(arr10, my_equal_to()) == 0);
  DYNAMIC_VERIFY(do_test2(arr10, my_equal_to()) == 0);

  int arr11[] = { 1, 222, 33, 4, 55 };
  DYNAMIC_VERIFY(do_test(arr11, my_equal_to()) == 0);
  DYNAMIC_VERIFY(do_test2(arr11, my_equal_to()) == 0);

  int arr12[] = { 111, 333, 333, 444, 555 };
  DYNAMIC_VERIFY(do_test(arr12, my_equal_to(), false) == 0);
  DYNAMIC_VERIFY(do_test2(arr12, my_equal_to(), false) == 0);

  int arr13[] = { 333, 333, 333, 444, 444 };
  DYNAMIC_VERIFY(do_test(arr13, my_equal_to(), false) == 0);
  DYNAMIC_VERIFY(do_test2(arr13, my_equal_to(), false) == 0);

  return 0;
}

int main()
{
  RUN_TEST(test01);
  return 0;
}

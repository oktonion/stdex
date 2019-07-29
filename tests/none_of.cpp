#include "../stdex/include/core.h"
#include "../stdex/include/algorithm.hpp"

#include <cstdlib>
#include <iterator>
#include <iostream>

#define THROW_VERIFY(cond) if(!(cond)) {std::cout << "check condition \'" << #cond << "\' failed at line " << __LINE__ << std::endl; throw(__LINE__);}
#define DYNAMIC_VERIFY(cond) if(!(cond)) {std::cout << "check condition \'" << #cond << "\' failed at line " << __LINE__ << std::endl; return __LINE__;}
#define RUN_TEST(test) {std::cout << #test << std::endl; int line = test(); if(line != 0) {std::cout << "failed at line " << line << std::endl; return line;}}

template<class T>
  struct BoundsContainer
  {
    T* first;
    T* last;
    BoundsContainer(T* _first, T* _last) : first(_first), last(_last)
    { }
};

template<class T> struct remove_cv { typedef T type; };
template<class T> struct remove_cv<const T> { typedef T type; };
template<class T> struct remove_cv<volatile T> { typedef T type; };
template<class T> struct remove_cv<const volatile T> { typedef T type; };

template<class T>
class input_iterator_wrapper
: public std::iterator<std::input_iterator_tag, typename remove_cv<T>::type,
      std::ptrdiff_t, T*, T&>
{
protected:
  input_iterator_wrapper()
  { }

public:
  typedef BoundsContainer<T> ContainerType;
  T* ptr;
  ContainerType* SharedInfo;

  input_iterator_wrapper(T* _ptr, ContainerType* SharedInfo_in)
  : ptr(_ptr), SharedInfo(SharedInfo_in)
  { THROW_VERIFY(ptr >= SharedInfo->first && ptr <= SharedInfo->last); }

  input_iterator_wrapper(const input_iterator_wrapper& in)
  : ptr(in.ptr), SharedInfo(in.SharedInfo)
  { }

  bool
  operator==(const input_iterator_wrapper& in) const
  {
    THROW_VERIFY(SharedInfo && SharedInfo == in.SharedInfo);
    THROW_VERIFY(ptr>=SharedInfo->first && in.ptr>=SharedInfo->first);
    return ptr == in.ptr;
  }

  bool
  operator!=(const input_iterator_wrapper& in) const
  {
    return !(*this == in);
  }

  T&
  operator*() const
  {
    THROW_VERIFY(SharedInfo && ptr < SharedInfo->last);
    THROW_VERIFY(ptr >= SharedInfo->first);
    return *ptr;
  }

  T*
  operator->() const
  {
    return &**this;
  }

  input_iterator_wrapper&
  operator=(const input_iterator_wrapper& in)
  {
    ptr = in.ptr;
    SharedInfo = in.SharedInfo;
    return *this;
  }

  input_iterator_wrapper&
  operator++()
  {
    THROW_VERIFY(SharedInfo && ptr < SharedInfo->last);
    THROW_VERIFY(ptr>=SharedInfo->first);
    ptr++;
    SharedInfo->first=ptr;
    return *this;
  }

  void
  operator++(int)
  {
    ++*this;
  }

private:
  template<class U>
    void operator,(const U&) const;
};

template <class T>
struct test_container
{
  typedef input_iterator_wrapper<T> it_type_t;

  typename it_type_t::ContainerType bounds;
  test_container(T* _first, T* _last) : bounds(_first, _last)
  { }

  template<int N>
  explicit
  test_container(T (&arr)[N]) : bounds(arr, arr+N)
  { }

  it_type_t
  it(int pos)
  {
    THROW_VERIFY(pos >= 0 && pos <= (bounds.last - bounds.first));
    return it_type_t(bounds.first + pos, &bounds);
  }

  it_type_t
  it(T* pos)
  {
    THROW_VERIFY(pos >= bounds.first && pos <= bounds.last);
    return it_type_t(pos, &bounds);
  }

  const T&
  val(int pos)
  { return (bounds.first)[pos]; }

  it_type_t
  begin()
  { return it(bounds.first); }

  it_type_t
  end()
  { return it(bounds.last); }
};

typedef test_container<int> Container;
int array[] = {0, 0, 0, 1, 0, 1};

bool
predicate(const int& i) 
{ return i == 1; }

int test1()
{
  try
  {
    Container con(array, array);
    DYNAMIC_VERIFY( stdex::none_of(con.begin(), con.end(), predicate) );
  }
  catch(int line)
  {
    return line;
  }

  return 0;
}

int test2()
{
  try
  {
    Container con(array, array + 1);
    DYNAMIC_VERIFY( stdex::none_of(con.begin(), con.end(), predicate) );
  }
  catch(int line)
  {
    return line;
  }

   return 0;
}

int test3()
{
  try
  {
    Container con(array, array + 6);
    DYNAMIC_VERIFY( !stdex::none_of(con.begin(), con.end(), predicate) );
  }
  catch(int line)
  {
    return line;
  }

  return 0;
}

int 
main()
{
  RUN_TEST(test1);
  RUN_TEST(test2);
  RUN_TEST(test3);
  return 0;
}

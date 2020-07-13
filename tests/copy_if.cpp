#include "../stdex/include/core.h"
#include "../stdex/include/algorithm.hpp"

#include <cstdlib>
#include <iterator>
#include <iostream>
#include <vector>

#define THROW_VERIFY(cond) if(!(cond)) {std::cout << "check condition \'" << #cond << "\' failed at line " << __LINE__ << std::endl; throw(__LINE__);}
#define DYNAMIC_VERIFY(cond) if(!(cond)) {std::cout << "check condition \'" << #cond << "\' failed at line " << __LINE__ << std::endl; return __LINE__;}
#define RUN_TEST(test) {std::cout << #test << std::endl; X line = test(); if(line != 0) {std::cout << "failed at line " << line << std::endl; return line;}}

struct X {};

struct Z
{
  Z&
  operator=(const X&)
  { return *this; }
};

template<class T>
struct BoundsContainer
{
  T* first;
  T* last;

  BoundsContainer(T* _first, T* _last) : first(_first), last(_last)
  { }

  std::size_t size() const { return last - first; }
};

template<class T>
struct OutputContainer : public BoundsContainer<T>
{
  T* incrementedto;
  bool* writtento;

  OutputContainer(T* _first, T* _last)
  : BoundsContainer<T>(_first, _last), incrementedto(_first),
  writtento(new bool[this->size()])
  { }

  ~OutputContainer()
  { delete[] writtento; }
};

namespace type_traits
{
	template<class T> struct remove_cv { typedef T type; };
	template<class T> struct remove_cv<const T> { typedef T type; };
	template<class T> struct remove_cv<volatile T> { typedef T type; };
	template<class T> struct remove_cv<const volatile T> { typedef T type; };
}

template<class T>
class input_iterator_wrapper
: public std::iterator<std::input_iterator_tag, typename type_traits::remove_cv<T>::type,
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
  { THROW_VERIFY((ptr >= SharedInfo->first && ptr <= SharedInfo->last)); }

  input_iterator_wrapper(const input_iterator_wrapper& in)
  : ptr(in.ptr), SharedInfo(in.SharedInfo)
  { }

  bool
  operator==(const input_iterator_wrapper& in) const
  {
    THROW_VERIFY((SharedInfo && SharedInfo == in.SharedInfo));
    THROW_VERIFY((ptr>=SharedInfo->first && in.ptr>=SharedInfo->first));
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
    THROW_VERIFY((SharedInfo && ptr < SharedInfo->last));
    THROW_VERIFY((ptr >= SharedInfo->first));
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
    THROW_VERIFY((SharedInfo && ptr < SharedInfo->last));
    THROW_VERIFY((ptr>=SharedInfo->first));
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

template<class T>
class WritableObject
{
  T* ptr;

public:
  OutputContainer<T>* SharedInfo;

  WritableObject(T* ptr_in, OutputContainer<T>* SharedInfo_in):
  ptr(ptr_in), SharedInfo(SharedInfo_in)
  { }
  template<class U>
  void
  operator=(const U& new_val)
  {
    assert(SharedInfo->writtento[ptr - SharedInfo->first] == 0);
    SharedInfo->writtento[ptr - SharedInfo->first] = 1;
    *ptr = new_val;
  }
};

template<class T>
struct output_iterator_wrapper
: public std::iterator<std::output_iterator_tag, T, std::ptrdiff_t, T*, T&>
{
    typedef OutputContainer<T> ContainerType;
    T* ptr;
    ContainerType* SharedInfo;

    output_iterator_wrapper(T* _ptr, ContainerType* SharedInfo_in)
    : ptr(_ptr), SharedInfo(SharedInfo_in)
    {
        THROW_VERIFY((ptr >= SharedInfo->first && ptr <= SharedInfo->last));
    }

    output_iterator_wrapper(const output_iterator_wrapper& in)
    : ptr(in.ptr), SharedInfo(in.SharedInfo)
    { }

    WritableObject<T>
    operator*() const
    {
        THROW_VERIFY((ptr < SharedInfo->last));
        THROW_VERIFY((SharedInfo->writtento[ptr - SharedInfo->first] == false));
        return WritableObject<T>(ptr, SharedInfo);
    }

    output_iterator_wrapper&
    operator=(const output_iterator_wrapper& in)
    {
        ptr = in.ptr;
        SharedInfo = in.SharedInfo;
        return *this;
    }

    output_iterator_wrapper&
    operator++()
    {
        THROW_VERIFY((SharedInfo && ptr < SharedInfo->last));
        THROW_VERIFY((ptr>=SharedInfo->incrementedto));
        ptr++;
        SharedInfo->incrementedto=ptr;
        return *this;
    }

    output_iterator_wrapper
    operator++(int)
    {
        output_iterator_wrapper<T> tmp = *this;
        ++*this;
        return tmp;
    }
};



bool
pred_function(const X&)
{ return true; }

struct pred_obj
{
  bool 
  operator()(const X&)
  { return true; }
};

output_iterator_wrapper<Z>
test1(input_iterator_wrapper<X>& begin,
      input_iterator_wrapper<X>& end,
      output_iterator_wrapper<Z>& output)
{ return stdex::copy_if(begin, end, output, pred_function); }

output_iterator_wrapper<Z>
test2(input_iterator_wrapper<X>& begin,
      input_iterator_wrapper<X>& end,
      output_iterator_wrapper<Z>& output)
{ return stdex::copy_if(begin, end, output, pred_obj()); }

int
main()
{
  X x[2];
  Z z[2];
  OutputContainer<Z> cont_z = OutputContainer<Z>(&z[0], &z[1]);
  OutputContainer<X> cont_x = OutputContainer<X>(&x[0], &x[1]);
  WritableObject<X> obj1 = WritableObject<X>(&x[0], &cont_x), obj2 = obj1;
  BoundsContainer<X> bcont = BoundsContainer<X>(&x[0], &x[1]);
  (void)(&x);
  (void)(&z);
  (void)(&cont_x);
  (void)(&cont_z);
  (void)(&obj1);
  (void)(&obj2);
  (void)(&bcont);
  obj2 = obj1;


  output_iterator_wrapper<Z> owr = output_iterator_wrapper<Z>(&z[0], &cont_z);
  input_iterator_wrapper<X> iwr = input_iterator_wrapper<X>(&x[0], &bcont);
  owr++;
  iwr++;
  (void)(&owr);
  (void)(&iwr);

  test1(iwr, iwr, owr);
  test2(iwr, iwr, owr);

  return 0;
}

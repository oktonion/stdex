#include "../stdex/include/core.h"
#include "../stdex/include/future.hpp"
#include "../stdex/include/chrono.hpp"

#include "../stdex/include/string.hpp"

#include <iostream>
#include <string>

namespace toolbox
{
  namespace detail
  {
    template<class LhsT, class RhsT>
    bool eq(const LhsT &lhs, const RhsT &rhs) { return lhs == (rhs); }
    template<class LhsT, class RhsT>
    bool ne(const LhsT &lhs, const RhsT &rhs) { return lhs != (rhs); }
    template<class LhsT, class RhsT>
    bool lt(const LhsT &lhs, const RhsT &rhs) { return lhs <  (rhs); }
    template<class LhsT, class RhsT>
    bool gt(const LhsT &lhs, const RhsT &rhs) { return lhs >  (rhs); }
    template<class LhsT, class RhsT>
    bool le(const LhsT &lhs, const RhsT &rhs) { return lhs <= (rhs); }
    template<class LhsT, class RhsT>
    bool ge(const LhsT &lhs, const RhsT &rhs) { return lhs >= (rhs); }
    //inline bool eq(const char* lhs, const char* rhs) { return std::string(lhs) == std::string(rhs); }
    //inline bool ne(const char* lhs, const char* rhs) { return std::string(lhs) != std::string(rhs); }
    //inline bool lt(const char* lhs, const char* rhs) { return std::string(lhs) < std::string(rhs); }
    //inline bool gt(const char* lhs, const char* rhs) { return std::string(lhs) > std::string(rhs); }
    //inline bool le(const char* lhs, const char* rhs) { return std::string(lhs) <= std::string(rhs); }
    //inline bool ge(const char* lhs, const char* rhs) { return std::string(lhs) >= std::string(rhs); }

    template<class T>
    std::string to_string(const T &value)
    {
        return stdex::to_string(value);
    }

    class void_type;
  }
#define TESTS_TOOLBOX_CMP_EQ(l, r) toolbox::detail::eq(l, r)
#define TESTS_TOOLBOX_CMP_NE(l, r) toolbox::detail::ne(l, r)
#define TESTS_TOOLBOX_CMP_GT(l, r) toolbox::detail::gt(l, r)
#define TESTS_TOOLBOX_CMP_LT(l, r) toolbox::detail::lt(l, r)
#define TESTS_TOOLBOX_CMP_GE(l, r) toolbox::detail::ge(l, r)
#define TESTS_TOOLBOX_CMP_LE(l, r) toolbox::detail::le(l, r)
#define TESTS_TOOLBOX_DO_BINARY_EXPRESSION_COMPARISON(op, op_str, op_macro) \
  template<class RhsT> \
  result& operator op (const RhsT &rhs) \
  { \
    string += (op_str + toolbox::detail::to_string(rhs)); \
    (outcome = outcome && op_macro(lhs, rhs)); \
    return *this; \
  }

  template<class LhsT = toolbox::detail::void_type>
  struct result;


  template<>
  struct result<toolbox::detail::void_type>
  {
      std::string string;
      bool outcome;  

      result() : outcome(true) {
      }
  };

  struct final_result
      : result<>
  {
      typedef result<> base;
      final_result(const base &other = base())
          : base(other) {}

      bool operator !() const {
          return !outcome;
      }
      operator std::string() const {
          return string;
      }
  };

  template<class LhsT>
  struct result
      : result<>
  {
    typedef result<> base;
    LhsT lhs;
    TESTS_TOOLBOX_DO_BINARY_EXPRESSION_COMPARISON(== , " == ", TESTS_TOOLBOX_CMP_EQ)
    TESTS_TOOLBOX_DO_BINARY_EXPRESSION_COMPARISON(!= , " != ", TESTS_TOOLBOX_CMP_NE)
    TESTS_TOOLBOX_DO_BINARY_EXPRESSION_COMPARISON(>  , " >  ", TESTS_TOOLBOX_CMP_GT) 
    TESTS_TOOLBOX_DO_BINARY_EXPRESSION_COMPARISON(<  , " <  ", TESTS_TOOLBOX_CMP_LT) 
    TESTS_TOOLBOX_DO_BINARY_EXPRESSION_COMPARISON(>= , " >= ", TESTS_TOOLBOX_CMP_GE)
    TESTS_TOOLBOX_DO_BINARY_EXPRESSION_COMPARISON(<= , " <= ", TESTS_TOOLBOX_CMP_LE)

    result(const LhsT &lhs_in)
      : lhs(lhs_in)
    {
        string = (toolbox::detail::to_string(lhs_in));
    }

    template<class OtherLhsT>
    result(const result<OtherLhsT> &other)
        : base(other)
        , lhs(other.lhs)
    {
    }

    operator final_result() const
    {
        return static_cast<const base&>(*this);
    }
  };

  struct expression_decomposer
  {
    template<class LhsT>
    result<LhsT> operator/(const LhsT &lhs) const
    {
      return result<LhsT>(lhs);
    }
  };

#define TESTS_TOOLBOX_MAKE_RESULT(cond) toolbox::final_result result = (toolbox::expression_decomposer() / cond);
}

#define VERIFY(cond) STATIC_ASSERT((cond), check)
#define DYNAMIC_VERIFY_IMPL(cond, op) { \
    TESTS_TOOLBOX_MAKE_RESULT(cond) \
    if(!(result)) {std::cout << "check condition \'" << #cond << "\' failed at line " << __LINE__ << std::endl << \
        "logged: condition '" << result.string << "' failed" << std::endl; op;} \
}
#define DYNAMIC_VERIFY_RETURN_NEGATIVE return 1
#define DYNAMIC_VERIFY_CALL_ABORT {using namespace std; abort();}
#define DYNAMIC_VERIFY(cond) DYNAMIC_VERIFY_IMPL(cond, DYNAMIC_VERIFY_RETURN_NEGATIVE)
#define RUN_TEST(test) {std::cout << #test << std::endl; int line = test(); if(line != 0) {std::cout << "failed at line " << line << std::endl; return line;}}
#define DYNAMIC_VERIFY_FAIL {std::cout << "check condition " << "failed at line " << __LINE__ << std::endl; DYNAMIC_VERIFY_RETURN_NEGATIVE;}
#define DYNAMIC_VERIFY_ABORT(cond) DYNAMIC_VERIFY_IMPL(cond, DYNAMIC_VERIFY_CALL_ABORT)

int iterations = 200;

using namespace stdex;

template<typename Duration>
double
print(const char* desc, Duration dur)
{
  chrono::nanoseconds::rep ns = chrono::duration_cast<chrono::nanoseconds>(dur).count();
  double d = double(ns) / iterations;
  std::cout << desc << ": " << ns << "ns for " << iterations
    << " calls, avg " << d << "ns per call" << std::endl;
  return d;
}

int main()
{
  promise<int> p;
  future<int> f;
  p.get_future().swap(f);

 start_over:
  chrono::high_resolution_clock::time_point start = chrono::high_resolution_clock::now();
  for(int i = 0; i < iterations; i++)
    f.wait_for(chrono::seconds(0));
  chrono::high_resolution_clock::time_point stop = chrono::high_resolution_clock::now();

  /* We've run too few iterations for the clock resolution.
     Attempt to calibrate it.  */
  if (start == stop)
    {
      /* After set_value, wait_for is faster, so use that for the
     calibration to avoid zero at low clock resultions.  */
      promise<int> pc;
      future<int> fc;
      pc.get_future().swap(fc);
      pc.set_value(1);

      /* Loop until the clock advances, so that start is right after a
     time increment.  */
      do
    start = chrono::high_resolution_clock::now();
      while (start == stop);
      int i = 0;
      /* Now until the clock advances again, so that stop is right
     after another time increment.  */
      do
    {
      fc.wait_for(chrono::seconds(0));
      stop = chrono::high_resolution_clock::now();
      i++;
    }
      while (start == stop);
      /* Go for some 10 cycles, but if we're already past that and
     still get into the calibration loop, double the iteration
     count and try again.  */
      if (iterations < i * 10)
    iterations = i * 10;
      else
    iterations *= 2;
      goto start_over;
    }

  double wait_for_0 = print("wait_for(0s)", stop - start);

  start = chrono::high_resolution_clock::now();
  for(int i = 0; i < iterations; i++)
    f.wait_until(chrono::system_clock::time_point::min());
  stop = chrono::high_resolution_clock::now();
  double wait_until_sys_min 
    = print("wait_until(system_clock minimum)", stop - start);

  start = chrono::high_resolution_clock::now();
  for(int i = 0; i < iterations; i++)
    f.wait_until(chrono::steady_clock::time_point::min());
  stop = chrono::high_resolution_clock::now();
  double wait_until_steady_min 
    = print("wait_until(steady_clock minimum)", stop - start);

  start = chrono::high_resolution_clock::now();
  for(int i = 0; i < iterations; i++)
    f.wait_until(chrono::system_clock::time_point());
  stop = chrono::high_resolution_clock::now();
  double wait_until_sys_epoch 
    = print("wait_until(system_clock epoch)", stop - start);

  start = chrono::high_resolution_clock::now();
  for(int i = 0; i < iterations; i++)
    f.wait_until(chrono::steady_clock::time_point());
  stop = chrono::high_resolution_clock::now();
  double wait_until_steady_epoch 
    = print("wait_until(steady_clock epoch", stop - start);

  (void)wait_until_steady_epoch;
  (void)wait_until_sys_epoch;

  p.set_value(1);

  start = chrono::high_resolution_clock::now();
  for(int i = 0; i < iterations; i++)
    f.wait_for(chrono::seconds(0));
  stop = chrono::high_resolution_clock::now();
  double ready = print("wait_for when ready", stop - start) + 1.0;

  // Polling before ready with wait_for(0s) should be almost as fast as
  // after the result is ready.
  DYNAMIC_VERIFY( wait_for_0 < (ready * 30) );

  // Polling before ready using wait_until(min) should not be terribly slow.
  DYNAMIC_VERIFY( wait_until_sys_min < (ready * 100) );
  DYNAMIC_VERIFY( wait_until_steady_min < (ready * 100) );

#if 1
  // Polling before ready using wait_until(epoch) should not be terribly slow.
  DYNAMIC_VERIFY( wait_until_sys_epoch < (ready * 100) );
  DYNAMIC_VERIFY( wait_until_steady_epoch < (ready * 100) );
#endif
    return 0;
}

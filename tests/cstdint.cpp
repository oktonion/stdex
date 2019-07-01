#include "../stdex/include/cstdint.hpp"

void test01()
{
  typedef stdex::int8_t          my_int8_t;
  typedef stdex::int16_t         my_int16_t;
  typedef stdex::int32_t         my_int32_t;
  typedef stdex::int64_t         my_int64_t;
  typedef stdex::int_fast8_t     my_int_fast8_t;
  typedef stdex::int_fast16_t    my_int_fast16_t;
  typedef stdex::int_fast32_t    my_int_fast32_t;
  typedef stdex::int_fast64_t    my_int_fast64_t;	
  typedef stdex::int_least8_t    my_int_least8_t;
  typedef stdex::int_least16_t   my_int_least16_t;
  typedef stdex::int_least32_t   my_int_least32_t;
  typedef stdex::int_least64_t   my_int_least64_t;
  typedef stdex::intmax_t        my_intmax_t;
  typedef stdex::intptr_t        my_intptr_t;
  typedef stdex::uint8_t         my_uint8_t;
  typedef stdex::uint16_t        my_uint16_t;
  typedef stdex::uint32_t        my_uint32_t;
  typedef stdex::uint64_t        my_uint64_t;
  typedef stdex::uint_fast8_t    my_uint_fast8_t;
  typedef stdex::uint_fast16_t   my_uint_fast16_t;
  typedef stdex::uint_fast32_t   my_uint_fast32_t;
  typedef stdex::uint_fast64_t   my_uint_fast64_t;	
  typedef stdex::uint_least8_t   my_uint_least8_t;
  typedef stdex::uint_least16_t  my_uint_least16_t;
  typedef stdex::uint_least32_t  my_uint_least32_t;
  typedef stdex::uint_least64_t  my_uint_least64_t;
  typedef stdex::uintmax_t       my_uintmax_t;
  typedef stdex::uintptr_t       my_uintptr_t;
}

int main(void)
{
    test01();
    return 0;
}

#include "../stdex/include/core.h"
#include "../stdex/include/functional.hpp"
#include "../stdex/include/system_error.hpp"
#include "../stdex/include/string.hpp"

#include <iostream>
#include <string>
#include <bitset>
#include <set>

#define VERIFY(cond) STATIC_ASSERT((cond), check)
#define DYNAMIC_VERIFY(cond) if(!(cond)) {std::cout << "check condition \'" << #cond << "\' failed at line " << __LINE__ << std::endl; return -1;}
#define RUN_TEST(test) {std::cout << #test << std::endl; int line = test(); if(line != 0) {std::cout << "failed at line " << line << std::endl; return line;}}
#define DYNAMIC_VERIFY_FAIL {std::cout << "check condition " << "failed at line " << __LINE__ << std::endl; return -1;}
#define DYNAMIC_VERIFY_ABORT(cond) if(!(cond)) {std::cout << "check condition \'" << #cond << "\' failed at line " << __LINE__ << std::endl; std::abort();}

#ifndef NTESTS
#define NTESTS 15
#endif
#ifndef NSTRINGS
#define NSTRINGS 400
#endif
#ifndef STRSIZE
#define STRSIZE 42
#endif

const unsigned int num_quality_tests = NTESTS;
const unsigned int num_strings_for_quality_tests = NSTRINGS;
const unsigned int string_size = STRSIZE;

std::vector<std::string>
random_strings(unsigned int n, unsigned int len)
{
  std::string s(len, '\0');
  std::set<std::string> result_set;
  while (result_set.size() < n)
    {
      result_set.insert(s);
      unsigned int tmp = std::rand();
      tmp %= len * 256;
      s[tmp / 256] = tmp % 256;
    }
  return std::vector<std::string>(result_set.begin(), result_set.end());
}

double
score_from_varying_position(std::string s, unsigned int index)
{
  unsigned int bits_in_hash_code = sizeof(std::size_t) * 8;

  // We'll iterate through all 256 vals for s[index], leaving the rest
  // of s fixed.  Then, for example, out of the 128 times that
  // s[index] has its 3rd bit equal to 0 we would like roughly half 1s
  // and half 0s in bit 9 of the hash codes.
  //
  // Bookkeeping: Conceptually we want a 3D array of ints.  We want to
  // count the number of times each output position (of which there are
  // bits_in_hash_code) is 1 for each bit position within s[index] (of 
  // which there are 8) and value of that bit (of which there are 2).
  const unsigned int jj = 2;
  const unsigned int kk = jj * bits_in_hash_code;
  const unsigned int array_size = 8 * kk;
  std::vector<int> ones(array_size, 0);

  for (int i = 0; i < 256; i++)
    {
      s[index] = i;
      std::size_t h = stdex::hash<std::string>()(s);
      for (int j = 0; h != 0; j++, h >>= 1)
        {
          if (h & 1)
            {
              for (int k = 0; k < 8; k++)
                ++ones[k * kk + j * jj + ((i >> k) & 1)];
            }
        }
    }

  // At most, the innermost statement in the above loop nest can
  // execute 256 * bits_in_hash_code * 8 times.  If the hash is good,
  // it'll execute about half that many times, with a pretty even
  // spread across the elements of ones[].
  DYNAMIC_VERIFY_ABORT( 256 * bits_in_hash_code * 8 / array_size == 128 );
  int max_ones_possible = 128;
  int good = 0, bad = 0;
  for (int bit = 0; bit <= 1; bit++)
    {
      for (unsigned int j = 0; j < bits_in_hash_code; j++)
        {
          for (int bitpos = 0; bitpos < 8; bitpos++)
            {
              int z = ones[bitpos * kk + j * jj + bit];
              if (z <= max_ones_possible / 6
		  || z >= max_ones_possible * 5 / 6)
                {
                  // The hash function screwed up, or was just unlucky,
                  // as 128 flips of a perfect coin occasionally yield
                  // far from 64 heads.
                  bad++;
                }
              else
                good++;
            }
        }
    }
  return good / (double)(good + bad);
}

double
score_from_varying_position(const std::vector<std::string>& v, unsigned int index)
{
  double score = 0;
  for (unsigned int i = 0; i < v.size(); i++)
    score += score_from_varying_position(v[i], index);
  return score / v.size();
}

double
quality_test(unsigned int num_strings, unsigned int string_size)
{
  // Construct random strings.
  std::vector<std::string> v = random_strings(num_strings, string_size);
  double sum_of_scores = 0;
  for (unsigned int i = 0; i < string_size; i++)
    sum_of_scores += score_from_varying_position(v, i);

  // A good hash function should have a score very close to 1, and a bad
  // hash function will have a score close to 0.
  return sum_of_scores / string_size;
}

void
quality_test()
{
  std::srand(137);
  double sum_of_scores = 0;
  for (unsigned int i = 0; i < num_quality_tests; i++)
    {
      double score = quality_test(num_strings_for_quality_tests,
				  string_size);
      sum_of_scores += score;
      DYNAMIC_VERIFY_ABORT( score > 0.97 ? true : (std::cout << score << " < 0.97" << std::endl, false) );
    }

  if (num_quality_tests > 1)
    {
      double mean_quality = sum_of_scores / num_quality_tests;
      DYNAMIC_VERIFY_ABORT( mean_quality > 0.97 ? true : (std::cout << mean_quality << " < 0.97" << std::endl, false) );
    }
}

int
main()
{
  quality_test();
  return 0;
}
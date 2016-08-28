/**
   Parse gaps in a given BAM alignment and output the resulting gaps in BED or
   VCF formats.
 */
#include "parse_gaps.h"

// Returns n! (the factorial of n).  For negative n, n! is defined to be 1.
int Factorial(int n) {
  int result = 1;
  for (int i = 1; i <= n; i++) {
    result *= i;
  }

  return result;
}

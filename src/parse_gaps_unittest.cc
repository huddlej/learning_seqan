// Unit testing for parse_gaps.
//
// Author: huddlej@gmail.com (John Huddleston)

#include <limits.h>
#include "parse_gaps.h"
#include "gtest/gtest.h"

// Tests ParseCigar().

// Tests parsing of deletions in CIGAR strings.
TEST(ParseCigarTest, Deletions) {
  EXPECT_EQ(2, ParseCigar("50M2D48M"));
  EXPECT_EQ(20, ParseCigar("50M20D30M"));
}

// Tests parsing of insertions in CIGAR strings.
TEST(ParseCigarTest, Insertions) {
  EXPECT_EQ(2, ParseCigar("50M2I48M"));
  EXPECT_EQ(20, ParseCigar("50M20I30M"));
}

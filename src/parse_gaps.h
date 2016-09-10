// A CIGAR string parser for BAMs.
//
// Author: huddlej@gmail.com (John Huddleston)

#ifndef PARSE_GAPS_H_
#define PARSE_GAPS_H_

// Returns length of the first deletion or insertion in the given CIGAR string.
int ParseCigar(char* cigar);

#endif  // PARSE_GAPS_H_

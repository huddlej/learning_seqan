#include <iostream>
#include <seqan/sequence.h>
#include <seqan/stream.h>

using namespace seqan;

// Function to print simple alignment between two sequences with the same length
template <typename TText1, typename TText2>
void printAlign(TText1 const & genomeFragment, TText2 const & read)
{
    std::cout <<  "Alignment " << std::endl;
    std::cout << "  genome : " << genomeFragment << std::endl;
    std::cout << "  read   : " << read << std::endl;
}

int main()
{
    // Build reads and genomes
    DnaString chr1 = "TATAATATTGCTATCGCGATATCGCTAGCTAGCTACGGATTATGCGCTCTGCGATATATCGCGCTAGATGTGCAGCTCGATCGAATGCACGTGTGTGCGATCGATTAGCGTCGATCATCGATCTATATTAGCGCGCGGTATCGGACGATCATATTAGCGGTCTAGCATTTAG";

    // Build List containing all reads
    typedef String<DnaString> TDnaList;
    TDnaList readList;
    resize(readList, 4);
    readList[0] = "TTGCTATCGCGATATCGCTAGCTAGCTACGGATTATGCGCTCTGCGATATATCGCGCT";
    readList[1] = "TCGATTAGCGTCGATCATCGATCTATATTAGCGCGCGGTATCGGACGATCATATTAGCGGTCTAGCATT";
    readList[2] = "AGCCTGCGTACGTTGCAGTGCGTGCGTAGACTGTTGCAAGCCGGGGGTTCATGTGCGCTGAAGCACACATGCACA";
    readList[3] = "CGTGCACTGCTGACGTCGTGGTTGTCACATCGTCGTGCGTGCGTACTGCTGCTGACA";

    // Append a second chromosome sequence fragment to chr1
    DnaString chr2 = "AGCCTGCGTACGTTGCAGTGCGTGCGTAGACTGTTGCAAGCCGGGGGTTCATGTGCGCTGAAGCACACATGCACACGTCTCTGTGTTCCGACGTGTGTCACGTGCACTGCTGACGTCGTGGTTGTCACATCGTCGTGCGTGCGTACTGCTGCTGACACATGCTGCTG";
    append(chr1, chr2);

    // Print readlist
    std::cout << " \n Read list: " << std::endl;
    for (unsigned i = 0; i < length(readList); ++i)
        std::cout << readList[i] << std::endl;

    // 1. Assume we have mapped the 4 reads to chr1 (and chr2) and now have the mapping start positions (no gaps).
    // Store the start position in a String alignPosList: 7, 100, 172, 272
    String<unsigned> alignPosList;
    resize(alignPosList, 4);
    alignPosList[0] = 7;
    alignPosList[1] = 100;
    alignPosList[2] = 172;
    alignPosList[3] = 272;

    // 2. Build another String bsChr1 as a copy of chr1, and exchange every ‘C’
    // with a ‘T’, as in a bisulfite treated genome.
    DnaString bsChr1;
    assign(bsChr1, chr1);
    for (unsigned i = 0; i < length(chr1); ++i) {
        if (chr1[i] == 'C') {
            bsChr1[i] = 'T';
        }
    }

    // 3. Print alignments of the reads with chr1 (or bsChr1) sequence using the
    // function printAlign and the positions in alignPosList.

    // To do that, you have to create a copy of the fragment in chr1 (bsChr1) that is aligned to the read.
    std::cout << " \n Print alignment: " << std::endl;
    for (unsigned i = 0; i < length(readList); ++i) {
        // Begin position beginPosition of a given alignment between the read and the genome
        unsigned beginPosition = alignPosList[i];

        // Genome fragment
        DnaString genomeFragment;
        resize(genomeFragment, length(readList[i]));
        for (unsigned j = 0; j < length(readList[i]); ++j) {
            genomeFragment[j] = bsChr1[beginPosition + j];
        }

        // Call of our function to print the simple alignment
        printAlign(genomeFragment, readList[i]);
    }

    return 0;
}

#include <seqan/sequence.h>
#include <seqan/basic.h>
#include <seqan/stream.h>
#include <seqan/file.h>
#include <seqan/modifier.h>

using namespace seqan;

Dna getRevCompl(Dna const & nucleotide)
{
    if (nucleotide == 'A')
        return 'T';
    if (nucleotide == 'T')
        return 'A';
    if (nucleotide == 'C')
        return 'G';
    return 'C';
}

int main()
{
    DnaString genome = "TATATACGCGCGAGTCGT";
    DnaString revComplGenome;

    // Use the resize function to resize the revComplGenome variable.
    resize(revComplGenome, length(genome));

    // Using the getRevCompl function, get the reverse complement for every
    // nucleotide genome and store it in reverse order revComplGenome.
    for (int i = 0; i < length(genome); ++i) {
        revComplGenome[i] = getRevCompl(genome[length(genome) - i - 1]);
    }

    std::cout << genome << std::endl;
    std::cout << revComplGenome << std::endl;

    // And to check if your output is correct,
    // use the given SeqAn function reverseComplement(),
    // which modifies the sequence in-place:
    reverseComplement(genome);
    std::cout << genome << std::endl;
    return 0;
}

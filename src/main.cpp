#include <seqan/basic.h>
#include <seqan/sequence.h>
#include <seqan/stream.h>
#include <seqan/bam_io.h>
#include <seqan/bed_io.h>

using namespace seqan;

int main()
{
    std::cout << CharString("Hello SeqAn!") << std::endl;

    CharString bamFileName = "/Users/jlhudd/Documents/code/genotyper/CHM1_illumina_alignments_chr20.bam";
    unsigned min_length = 20;

    // Open input file, BamFileIn can read SAM and BAM files.
    BamFileIn bamFileIn;
    if (!open(bamFileIn, toCString(bamFileName))) {
        std::cerr << "ERROR: Could not open " << bamFileName << std::endl;
        return 1;
    }

    BedFileOut out(std::cout, Bed());

    try {
        // Copy header.
        BamHeader header;
        readHeader(header, bamFileIn);

        // Get BAM context.
        typedef FormattedFileContext<BamFileIn, void>::Type TBamContext;
        TBamContext const & bamContext = context(bamFileIn);

        // Copy records.
        BamAlignmentRecord record;
        unsigned query_position;
        unsigned target_position;
        String<Dna5> query_sequence;
        Infix<String<Dna5> >::Type gap_sequence;

        while (!atEnd(bamFileIn)) {
            readRecord(record, bamFileIn);
            query_sequence = record.seq;
            query_position = 0;
            target_position = record.beginPos;

            for (unsigned i = 0; i < length(record.cigar); ++i) {
                if (record.cigar[i].operation == 'N' || record.cigar[i].operation == 'S') {
                    // Increment query position for operations that don't occur
                    // in the target sequence.
                    query_position += record.cigar[i].count;
                }
                else if (record.cigar[i].operation == 'M' || record.cigar[i].operation == '=' || record.cigar[i].operation == 'X') {
                    // Matches or mismatches increment both query and target
                    // sequences.
                    query_position += record.cigar[i].count;
                    target_position += record.cigar[i].count;
                }
                else if (record.cigar[i].operation == 'I') {
                    if (record.cigar[i].count >= min_length) {
                        // Emit the insertion if it matches minimum length.
                        gap_sequence = infix(query_sequence, query_position, query_position + record.cigar[i].count);

                        BedRecord<Bed5> gap_record;
                        gap_record.ref = contigNames(bamContext)[record.rID];
                        gap_record.beginPos = target_position;
                        gap_record.endPos = target_position + 1;
                        gap_record.name = gap_sequence;
                        gap_record.score = record.cigar[i].count;
                        writeRecord(out, gap_record);
                        //std::cout << record.qName << query_position <<  << gap_sequence << std::endl;
                    }

                    query_position += record.cigar[i].count;
                }
                else if (record.cigar[i].operation == 'D') {
                    if (record.cigar[i].count >= min_length) {
                    }

                    //std::cout << record.cigar[i].count << record.cigar[i].operation << std::endl;
                    target_position += record.cigar[i].count;
                }
                else if (record.cigar[i].operation == 'H') {
                    // Hard clipping doesn't increment either sequence count.
                }
            }
        }
    }
    catch (Exception const & e) {
        std::cout << "ERROR: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

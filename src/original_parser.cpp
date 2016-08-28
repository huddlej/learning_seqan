#include <seqan/basic.h>
#include <seqan/sequence.h>
#include <seqan/bam_io.h>
#include <seqan/bed_io.h>
#include <seqan/seq_io.h>
#include <seqan/stream.h>

using namespace seqan;

int main()
{
    std::cout << CharString("Hello SeqAn!") << std::endl;

    CharString bamFileName = "/Users/jlhudd/Documents/code/genotyper/CHM1_illumina_alignments_chr20.bam";
    CharString referenceFileName = "/Users/jlhudd/Documents/code/genotyper/chr20.fasta";
    unsigned min_length = 20;

    // Try to load index and create on the fly if necessary.
    FaiIndex faiIndex;
    if (!open(faiIndex, toCString(referenceFileName))) {
        std::cerr << "Indexing FASTA file: " << referenceFileName << std::endl;

        if (!build(faiIndex, toCString(referenceFileName))) {
            std::cerr << "ERROR: Index could not be loaded or built.\n";
            return 1;
        }
        if (!save(faiIndex)) {
            std::cerr << "ERROR: Index could not be written do disk.\n";
            return 1;
        }
    }

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
        Dna5String target_sequence;
        unsigned operation_length;
        unsigned idx;

        while (!atEnd(bamFileIn)) {
            readRecord(record, bamFileIn);
            query_sequence = record.seq;
            query_position = 0;
            target_position = record.beginPos;

            for (unsigned i = 0; i < length(record.cigar); ++i) {
                operation_length = record.cigar[i].count;

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
                        std::cerr << "Deletion" << std::endl;

                        // Emit the insertion if it matches minimum length.
                        gap_sequence = infix(query_sequence, query_position, query_position + record.cigar[i].count);

                        BedRecord<Bed5> gap_record;
                        gap_record.ref = contigNames(bamContext)[record.rID];
                        gap_record.beginPos = target_position;
                        gap_record.endPos = target_position + 1;
                        gap_record.name = gap_sequence;
                        gap_record.score = std::to_string(operation_length);
                        writeRecord(out, gap_record);
                    }

                    query_position += record.cigar[i].count;
                }
                else if (record.cigar[i].operation == 'D') {
                    if (record.cigar[i].count >= min_length) {
                        std::cerr << "Deletion" << std::endl;

                        // Emit the deletion if it matches minimum length.
                        if (!getIdByName(idx, faiIndex, contigNames(bamContext)[record.rID])) {
                            std::cerr << "ERROR: Index does not know about sequence " << contigNames(bamContext)[record.rID] << std::endl;
                            return 1;
                        }
                        readRegion(target_sequence, faiIndex, idx, target_position, target_position + operation_length);

                        BedRecord<Bed5> gap_record;
                        gap_record.ref = contigNames(bamContext)[record.rID];
                        gap_record.beginPos = target_position;
                        gap_record.endPos = target_position + operation_length;
                        gap_record.name = gap_sequence;
                        gap_record.score = std::to_string(operation_length);
                        writeRecord(out, gap_record);
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

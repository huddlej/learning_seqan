/**
   Parse gaps in a given BAM alignment and output the resulting gaps in BED or
   VCF formats.
 */
#include <string>
#include <tclap/CmdLine.h>
#include <seqan/sequence.h>
#include <seqan/seq_io.h>
#include <seqan/bam_io.h>

using namespace seqan;

int main(int argc, char** argv) {
    try {
        TCLAP::CmdLine cmd("Parse gaps in BAM alignments", ' ', "0.1");
        TCLAP::UnlabeledValueArg<std::string> arg_reference("reference", "path to a FASTA file for the reference used by the given BAM", true, "", "reference");
        TCLAP::UnlabeledValueArg<std::string> arg_bam("bam", "path to a BAM file to parse", true, "", "bam");

        cmd.add(arg_reference);
        cmd.add(arg_bam);
        cmd.parse(argc, argv);

        /**
           Open the FASTA file for the given reference.
         */
        std::string reference_file_name = arg_reference.getValue();
        FaiIndex fai_index;
        if (!open(fai_index, toCString(reference_file_name))) {
            std::cerr << "Error: Could not open FASTA index for reference: " << reference_file_name << std::endl;
            return 1;
        }

        /**
           Open the BAM file of alignments to the given reference.
         */
        std::string bam_file_name = arg_bam.getValue();
        BamFileIn bam_file_in;
        if (!open(bam_file_in, toCString(bam_file_name))) {
            std::cerr << "Error: Could not open BAM: " << bam_file_name << std::endl;
            return 1;
        }
    }
    catch (TCLAP::ArgException &e) {
        std::cerr << "Error: " << e.error() << " for arg " << e.argId() << std::endl;
    }

    return 0;
}

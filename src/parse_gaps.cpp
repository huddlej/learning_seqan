/**
   Parse gaps in a given BAM alignment and output the resulting gaps in BED or
   VCF formats.
 */
#include <string>
#include <tclap/CmdLine.h>

int main(int argc, char** argv) {
    try {
        TCLAP::CmdLine cmd("Parse gaps in BAM alignments", ' ', "0.1");
        TCLAP::UnlabeledValueArg<std::string> arg_reference("reference", "path to a FASTA file for the reference used by the given BAM", true, "", "reference");
        TCLAP::UnlabeledValueArg<std::string> arg_bam("bam", "path to a BAM file to parse", true, "", "bam");

        cmd.add(arg_reference);
        cmd.add(arg_bam);
        cmd.parse(argc, argv);
    }
    catch (TCLAP::ArgException &e) {
        std::cerr << "Error: " << e.error() << " for arg " << e.argId() << std::endl;
    }

    return 0;
}

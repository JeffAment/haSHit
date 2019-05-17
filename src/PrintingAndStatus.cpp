/**
 * @author https://github.com/jeffament
 */

#include "Headers/PrintingAndStatus.h"

void PrintingAndStatus::printUsage() {
    std::cout << "\nUSAGE:\n";
    std::cout << "\thashit [options][command] /path/to/files (if no path is specified the current path is used)\n\n";
    std::cout << "OPTIONS:\n";
    std::cout << "\t-r, --recursive\t\tTraverses the directory and all of it's subdirectories recursively\n";
    std::cout << "\t-q, --quiet\t\t\tDecreases verbosity\n";
    std::cout << "\t-o, --overwrite\t\tOverwrites existing .md5 files when using generate command\n";
    std::cout << "\nCOMMANDS:\n";
    std::cout << "\tgenerate\t\tGenerates a checksum file for each file in the directory/directories\n";
    std::cout << "\tcheck\t\t\tChecks files against their stored md5 hashes\n";
    std::cout << "\tdelete\t\t\tDeletes all checksum files in the chosen directory/directories\n";
    std::cout << "\tdeleteorphans\tDeletes all orphaned checksum files in the chosen directory/directories\n";
    std::cout << "\nEXAMPLE COMMANDS:\n";
    std::cout << "\thashit -rq generate\t\t\t\t(Generates checksum files for every file in the current directory using "
                 "the -r and -q options)\n";
    std::cout << "\thashit check /home/Samantha\t\t(Checks files in Samantha's home directory against their "
                 "corresponding checksum files, but does not recurse into subdirectories)\n";
    std::cout << "\thashit -r delete /home/pictures\t(Deletes all checksum files in the pictures directory and all of "
                 "its subdirectories)\n";
}

void PrintingAndStatus::showProgress(const bfs::directory_entry &de, const int &blocksProcessed,
                                     const size_t &fileSize, const size_t &bufferSize) {
    if (!CommandState::getInstance().quiet && blocksProcessed % 512 == 0) {
        auto progress = (blocksProcessed * bufferSize * 100) / fileSize;
        std::cout << "\r" << progress << "% " << de.path().filename() << std::flush;
    }
    if (!CommandState::getInstance().quiet && fileSize / bufferSize <= blocksProcessed) {
        std::cout << "\r" << std::flush;
    }
}

void PrintingAndStatus::printString(const std::string &string, const bool &overrideQuiet) {
    if (!CommandState::getInstance().quiet || overrideQuiet) std::cout << string;
}

void PrintingAndStatus::printCheckStatusReport() {
    auto &cs = CommandState::getInstance();
    std::cout << std::endl << "###################################################" << std::endl;
    std::cout << cs.filesThatAreOK << " files are OK" << std::endl;
    std::cout << cs.filesWithNoHashes << " files with no checksums" << std::endl;
    std::cout << cs.filesWithMismatchedHashes << " files with mismatched checksums" << std::endl;
    std::cout << cs.orphanedChecksums << " orphaned checksum files";
    std::cout << std::endl << "###################################################" << std::endl;
}

void PrintingAndStatus::printGenerateStatusReport() {
    auto &cs = CommandState::getInstance();
    std::cout << std::endl << "###################################################" << std::endl;
    std::cout << cs.md5FilesCreated << " checksum files successfully generated" << std::endl;
    std::cout << cs.md5FilesThatCouldntBeCreated
              << " checksum files couldn't be generated (possible access error; check permissions)" << std::endl;
    std::cout << cs.md5FilesThatAlreadyExist << " checksum files already exist; nothing to generate" << std::endl;
    std::cout << cs.orphanedChecksums << " orphaned checksum files";
    std::cout << std::endl << "###################################################" << std::endl;
}

void PrintingAndStatus::printDeleteStatusReport() {
    auto &cs = CommandState::getInstance();
    std::cout << std::endl << "###################################################" << std::endl;
    std::cout << cs.md5FilesDeleted << " checksum files successfully deleted" << std::endl;
    std::cout << cs.md5FilesNotDeleted
              << " checksum files couldn't be deleted (possible access error; check permissions)";
    std::cout << std::endl << "###################################################" << std::endl;
}

void PrintingAndStatus::printDeleteOrphansStatusReport() {
    auto &cs = CommandState::getInstance();
    std::cout << std::endl << "###################################################" << std::endl;
    std::cout << cs.md5FilesDeleted << " orphaned checksum files successfully deleted" << std::endl;
    std::cout << cs.md5FilesNotDeleted
              << " orphaned checksum files couldn't be deleted (possible access error; check permissions)";
    std::cout << std::endl << "###################################################" << std::endl;
}
/**
 * @author https://github.com/jeffament
 */

#include "Headers/CommandBuilderAndExecutor.h"

//todo: add "checkgen" command that checks files against any pre-existing .md5 files while generating
//todo: record certain error information in a vector for optional logging option to be implemented later
//todo: add support for "exclude file" in shouldIgnoreFile() method
//todo: better command parsing
//todo: eliminate some repetitive code that could be made into their own functions
//todo: try to get the c++17 filesystem library to work so that boost is not necessary (they *should* be interchangeable)
//todo: refactor some variable names that have no meaning, and in some cases are inaccurate/misleading
//todo: possibly add option to change block size and multithreading
//todo: clean up messy singleton implementation

void CommandBuilderAndExecutor::execute() {
    auto &cs = CommandState::getInstance();

    if (cs.command == cs.GENERATE_COMMAND) {
        Commands::countOrphanedChecksums();
        Commands::generateCommand();
        PrintingAndStatus::printGenerateStatusReport();
    } else if (cs.command == cs.CHECK_COMMAND) {
        Commands::countOrphanedChecksums();
        Commands::checkCommand();
        PrintingAndStatus::printCheckStatusReport();
    } else if (cs.command == cs.DELETE_COMMAND) {
        Commands::deleteCommand();
        PrintingAndStatus::printDeleteStatusReport();
    } else if (cs.command == cs.DELETE_ORPHANS_COMMAND) {
        Commands::deleteOrphansCommand();
        PrintingAndStatus::printDeleteOrphansStatusReport();
    }
}
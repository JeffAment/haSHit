/**
 * @author https://github.com/jeffament
 */

#include "main.h"
#include "CommandBuilderAndExecutor.h"

int main(int argc, char *argv[]) {

    if (argc < 2) { // no args passed; show usage notes and exit
        main::printUsage();
        return 1;
    }

    CommandBuilderAndExecutor cb;

    static const struct option opts[] = {
            {"recursive", no_argument, 0, 'r'},
            {"quiet",     no_argument, 0, 'q'},
            {"overwrite", no_argument, 0, 'o'},
            {0, 0,                     0, 0} // sentinel value
    };

    int optidx;
    char c;

    while ((c = static_cast<char>(getopt_long(argc, argv, "rqo?:", opts, &optidx))) != -1) {
        switch (c) {
            case 'r':
                cb.setRecursionTrue();
                break;
            case 'q':
                cb.setQuietTrue();
                break;
            case 'o':
                cb.setGenerateOverwriteTrue();
                break;
            default: // option is invalid
                main::printUsage();
                return 1;
        }
    }

    if (optind == argc - 1) { // one arg; if it's a command, execute with current directory as path
        if (cb.setCommand(argv[optind]))
            cb.execute();
    } else if (optind == argc - 2) { // two args; if it is a valid command followed by a valid path, execute it
        if (cb.setCommand(argv[optind]) && cb.setPath(argv[optind + 1]))
            cb.execute();
    } else { // invalid usage - too many args; show usage notes and exit
        main::printUsage();
        return 1;
    }
    return 0;
}

void main::printUsage() {
    std::cout << "\nUSAGE:\n";
    std::cout << "\thashit [options][command] /path/to/files (if no path is specified the current path is used)\n\n";
    std::cout << "OPTIONS:\n";
    std::cout << "\t-r, --recursive\t\tTraverses the directory and all of it's subdirectories recursively\n";
    std::cout << "\t-q, --quiet\t\t\tDecreases verbosity\n";
    std::cout << "\t-o, --overwrite\t\tOverwrites existing .md5 files when using generate command\n";
    std::cout << "\nCOMMANDS:\n";
    std::cout << "\tgenerate\tGenerates a checksum file for each file in the directory/directories\n";
    std::cout << "\tcheck\t\tChecks files against their stored md5 hashes\n";
    std::cout << "\tdelete\t\tDeletes all checksum files in the chosen directory/directories\n";
    std::cout << "\tdeleteorphans\t\tDeletes all orphaned checksum files in the chosen directory/directories\n";
    std::cout << "\nEXAMPLE COMMANDS:\n";
    std::cout << "\thashit -rq generate\t\t\t\t(Generates checksum files for every file in the current directory using "
                 "the -r and -q options)\n";
    std::cout << "\thashit check /home/Samantha\t\t(Checks files in Samantha's home directory against their "
                 "corresponding checksum files, but does not recurse into subdirectories)\n";
    std::cout << "\thashit -r delete /home/pictures\t\t(Deletes all checksum files in the pictures directory and all of "
                 "its subdirectories)\n";
}
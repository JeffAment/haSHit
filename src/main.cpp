/**
 * @author https://github.com/jeffament
 */

#include "Headers/main.h"

int main(int argc, char *argv[]) {

    if (argc < 2) { // no args passed; show usage notes and exit
        PrintingAndStatus::printUsage();
        return 1;
    }

    CommandBuilderAndExecutor cb;
    CommandState &cs = CommandState::getInstance();

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
                cs.setRecursionTrue();
                break;
            case 'q':
                cs.setQuietTrue();
                break;
            case 'o':
                cs.setGenerateOverwriteTrue();
                break;
            default: // option is invalid
                PrintingAndStatus::printUsage();
                return 1;
        }
    }

    if (optind == argc - 1) { // one arg; if it's a command, execute with current directory as path
        if (cs.setCommand(argv[optind]))
            cb.execute();
    } else if (optind == argc - 2) { // two args; if it is a valid command followed by a valid path, execute it
        if (cs.setCommand(argv[optind]) && cs.setPath(argv[optind + 1]))
            cb.execute();
    } else { // invalid usage - too many args; show usage notes and exit
        PrintingAndStatus::printUsage();
        return 1;
    }
    return 0;
}

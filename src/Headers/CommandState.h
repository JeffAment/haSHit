/**
 * @author https://github.com/jeffament
 */

#ifndef HASHIT_COMMANDSTATE_H
#define HASHIT_COMMANDSTATE_H

#include <boost/filesystem.hpp>
#include <algorithm>
#include <string>

namespace bfs = boost::filesystem;

class CommandState {

private:
    // singleton stuff
    CommandState(){}

public:
    // singleton stuff
    static CommandState& getInstance();
    CommandState(CommandState const&) = delete;
    void operator=(CommandState const&) = delete;

    bool setPath(const std::string &path);

    bool setCommand(const std::string &command);

    const bool isValidCommand(const std::string &command);

    void setQuietTrue();

    void setRecursionTrue();

    void setGenerateOverwriteTrue();

    // option and command state
    std::string path = ".";
    std::string command;
    bool quiet = false;
    bool recursion = false;
    bool generateOverwrite = false;

    int orphanedChecksums = 0;

    // check operation variables
    int filesWithNoHashes = 0;
    int filesWithMismatchedHashes = 0;
    int filesThatAreOK = 0;

    // generate operation variables
    int filesThatCouldntBeOpened = 0; // not currently used
    int md5FilesCreated = 0;
    int md5FilesThatCouldntBeCreated = 0;
    int md5FilesThatAlreadyExist = 0;

    // delete operation variables
    int md5FilesDeleted{};
    int md5FilesNotDeleted{};


    // constants
    const std::string GENERATE_COMMAND = "generate";
    const std::string CHECK_COMMAND = "check";
    const std::string DELETE_COMMAND = "delete";
    const std::string DELETE_ORPHANS_COMMAND = "deleteorphans";
    const std::vector<std::string> VALID_COMMANDS_VECTOR = {GENERATE_COMMAND, CHECK_COMMAND, DELETE_COMMAND,
                                                            DELETE_ORPHANS_COMMAND};
    const std::string CHECKSUM_EXTENSION = ".hmd5";
};


#endif //HASHIT_COMMANDSTATE_H

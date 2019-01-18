/**
 * @author https://github.com/jeffament
 */

#ifndef HASHIT_COMMANDBUILDERANDEXECUTOR_H
#define HASHIT_COMMANDBUILDERANDEXECUTOR_H

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <boost/filesystem.hpp>
#include <stdio.h>
#include <algorithm>
#include <openssl/md5.h>
#include <fstream>
#include <sstream>

namespace bfs = boost::filesystem;

class CommandBuilderAndExecutor {
public:
    CommandBuilderAndExecutor() = default;
    bool setPath(const std::string &path);
    bool setCommand(const std::string &command);
    void setQuietTrue();
    void setRecursionTrue();
    void setGenerateOverwriteTrue();
    void execute();

private:
    // option and command state
    std::string path = ".";
    std::string command;
    bool quiet = false;
    bool recursion = false;
    bool generateOverwrite = false;

    // command functions
    void generateCommand();
    void checkCommand();

    // helper functions
    std::string generateMD5(const bfs::directory_entry &de);
    void createMD5File(const bfs::directory_entry &de, const std::string &hashAsString);
    bool shouldIgnoreFile(const bfs::directory_entry &de) const;
    bool md5FileExists(const bfs::directory_entry &de);
    bool isMd5File(const bfs::directory_entry &de) const;
    void printCheckStatusReport() const;
    void printGenerateStatusReport() const;
    void showProgress(const bfs::directory_entry &de, const int &blocksProcessed, const size_t &fileSize,
            const size_t &bufferSize) const;

    // check operation variables
    int filesWithNoHashes = 0;
    int filesWithMismatchedHashes = 0;
    int filesThatAreOK = 0;

    // generate operation variables
    int filesThatCouldntBeOpened = 0; // not currently used
    int md5FilesCreated = 0;
    int md5FilesThatCouldntBeCreated = 0;
    int md5FilesThatAlreadyExist = 0;

    // constants
    const std::string GENERATE_COMMAND = "generate";
    const std::string CHECK_COMMAND = "check";
    const std::vector<std::string> VALID_COMMANDS_VECTOR = {GENERATE_COMMAND, CHECK_COMMAND};
};

#endif //HASHIT_COMMANDBUILDERANDEXECUTOR_H
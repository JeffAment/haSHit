/**
 * @author https://github.com/jeffament
 */

#include "CommandBuilderAndExecutor.h"

//todo: add option to delete orphaned .md5 files with no matching files
//todo: add delete command to delete all .md5 files
//todo: make more functions const and try to update variables in less places using boolean return vals from const functions
//todo: add "checkgen" command that checks files against any pre-existing .md5 files while generating
//todo: record certain error information in a vector for optional logging option to be implemented later
//todo: consider adding time and date in .md5 files
//todo: add support for "exclude file" in shouldIgnoreFile() method
//todo: better command parsing
//todo: eliminate some repetitive code that could be made into their own functions
//todo: try to get the c++17 filesystem library to work so that boost is not necessary (they *should* be interchangeable)
//todo: refactor some variable names that have no meaning, and in some cases are inaccurate/misleading
//todo: possibly add option to change block size and multithreading

bool CommandBuilderAndExecutor::setPath(const std::string &path) {
    if (bfs::is_directory(path)) {
        this->path = path;
        return true;
    }
    return false;
}

bool CommandBuilderAndExecutor::setCommand(const std::string &command) {
    if (std::find(VALID_COMMANDS_VECTOR.begin(), VALID_COMMANDS_VECTOR.end(), command) != VALID_COMMANDS_VECTOR.end()) {
        this->command = command;
        return true;
    }
    return false;
}

void CommandBuilderAndExecutor::setQuietTrue() {
    quiet = true;
}

void CommandBuilderAndExecutor::setRecursionTrue() {
    recursion = true;
}

void CommandBuilderAndExecutor::setGenerateOverwriteTrue() {
    generateOverwrite = true;
}

void CommandBuilderAndExecutor::execute() {
    if (command == GENERATE_COMMAND) {
        generateCommand();
        printGenerateStatusReport();
    } else if (command == CHECK_COMMAND) {
        checkCommand();
        printCheckStatusReport();
    }
}

void CommandBuilderAndExecutor::generateCommand() {
    for (bfs::recursive_directory_iterator end, dir(path); dir != end; ++dir) {
        dir.disable_recursion_pending(!recursion);
        if (!bfs::is_directory(*dir) && !shouldIgnoreFile(*dir)) {
            if (generateOverwrite || !md5FileExists(*dir)) {
                std::string hash = generateMD5(*dir);
                createMD5File(*dir, hash);
            }
        }
    }
}

void CommandBuilderAndExecutor::checkCommand() {
    for (bfs::recursive_directory_iterator end, dir(path); dir != end; ++dir) {
        dir.disable_recursion_pending(!recursion);
        if (!bfs::is_directory(*dir) && !shouldIgnoreFile(*dir)) {
            std::string filePath = (*dir).path().string();
            std::string fileNameWithPath = filePath + ".md5";
            std::ifstream infile(fileNameWithPath);
            if (infile.good()) {
                std::string md5Hash;
                getline(infile, md5Hash);
                if (md5Hash == generateMD5(*dir)) {
                    if (!quiet) std::cout << "OK - " << filePath << std::endl;
                    ++filesThatAreOK;
                } else {
                    if (!quiet) std::cout << "ERROR, hash doesn't match - " << filePath << std::endl;
                    ++filesWithMismatchedHashes;
                }
            } else {
                if (!quiet) std::cout << "No .md5 file for " << filePath << std::endl;
                ++filesWithNoHashes;
            }
            infile.close();
        }
    }
}

std::string CommandBuilderAndExecutor::generateMD5(const bfs::directory_entry &de) {
    std::string filePath = de.path().string();
    auto fileSize = bfs::file_size(de);
    unsigned char out[MD5_DIGEST_LENGTH];
    size_t bufferSize = 512 * 100;
    char buffer[bufferSize];
    int blocksProcessed{};
    MD5_CTX context;
    MD5_Init(&context);
    std::ifstream inFile(filePath, std::ios::binary | std::ios::in);

    if (inFile.fail() || !inFile.is_open() || inFile.bad()) {
        ++filesThatCouldntBeOpened;
        if (!quiet) std::cout << "ERROR - File could not be opened!" << std::endl;
        return "Error";
    }

    do { // todo: I don't like anything about this!  But it works, so I'll leave it for now
        inFile.read(buffer, sizeof(buffer));
        if (inFile.gcount() < bufferSize)
            MD5_Update(&context, buffer, static_cast<size_t>(inFile.gcount()));
        else
            MD5_Update(&context, buffer, sizeof(buffer));
        showProgress(de, ++blocksProcessed, fileSize, bufferSize);
    } while (inFile);

    MD5_Final(out, &context);
    inFile.close();
    std::stringstream ss;

    for (auto a: out)
        ss << std::hex << std::setfill('0') << std::setw(2) << (int) a;

    return ss.str();
}

void CommandBuilderAndExecutor::createMD5File(const bfs::directory_entry &de, const std::string &hashAsString) {
    if (hashAsString.length() / 2 == MD5_DIGEST_LENGTH) {
        std::string fileName = de.path().string();
        std::string fileNameWithPath = fileName + ".md5";
        std::ofstream md5file(fileNameWithPath);

        if (md5file.good()) {
            md5file << hashAsString;
            md5file.close();
            ++md5FilesCreated;
            if (!quiet) std::cout << "OK - hash file successfully created for " << fileName << std::endl;
        } else {
            ++md5FilesThatCouldntBeCreated;
            if (!quiet) std::cout << "ERROR - hash file could not be created for " << fileName << std::endl;
        }
    }
}

bool CommandBuilderAndExecutor::md5FileExists(const bfs::directory_entry &de) {
    std::string filePath = de.path().string();
    std::string filePathWithMd5Appendage = filePath + ".md5";
    if (bfs::is_regular_file(filePathWithMd5Appendage)) {
        ++md5FilesThatAlreadyExist;
        return true;
    }
    return false;
}

bool CommandBuilderAndExecutor::shouldIgnoreFile(const bfs::directory_entry &de) const {
    return isMd5File(de);
}

bool CommandBuilderAndExecutor::isMd5File(const bfs::directory_entry &de) const {
    return de.path().extension() == ".md5";
}

void CommandBuilderAndExecutor::printCheckStatusReport() const {
    std::cout << std::endl << "###################################################" << std::endl;
    std::cout << filesThatAreOK << " files are OK" << std::endl;
    std::cout << filesWithNoHashes << " files with no hashes" << std::endl;
    std::cout << filesWithMismatchedHashes << " files with mismatched hashes";
    std::cout << std::endl << "###################################################" << std::endl;
}

void CommandBuilderAndExecutor::printGenerateStatusReport() const {
    std::cout << std::endl << "###################################################" << std::endl;
    std::cout << md5FilesCreated << " .md5 files successfully generated" << std::endl;
    std::cout << md5FilesThatCouldntBeCreated
              << " .md5 files couldn't be generated (possible access error; check permissions)" << std::endl;
    std::cout << md5FilesThatAlreadyExist << " .md5 files already exist; nothing to generate";
    std::cout << std::endl << "###################################################" << std::endl;
}

void CommandBuilderAndExecutor::showProgress(const bfs::directory_entry &de, const int &blocksProcessed,
                                             const size_t &fileSize, const size_t &bufferSize) const {
    if (!quiet && blocksProcessed % 512 == 0) {
        auto progress = (blocksProcessed * bufferSize * 100) / fileSize;
        std::cout << "\r" << progress << "% " << de.path().filename() << std::flush;
    }
    if (!quiet && fileSize / bufferSize <= blocksProcessed) {
        std::cout << "\r" << std::flush;
    }
}
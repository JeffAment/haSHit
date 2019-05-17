/**
 * @author https://github.com/jeffament
 */

#include "Headers/Commands.h"

void Commands::generateCommand() {
    auto &cs = CommandState::getInstance();
    for (bfs::recursive_directory_iterator end, dir(cs.path); dir != end; ++dir) {
        dir.disable_recursion_pending(!cs.recursion);
        if (!bfs::is_directory(*dir) && !shouldIgnoreFile(*dir)) {
            if (cs.generateOverwrite || !md5FileExists(*dir)) {
                std::string hash = generateMD5(*dir);
                createMD5File(*dir, hash);
            }
        }
    }
}

void Commands::checkCommand() {
    auto &cs = CommandState::getInstance();
    for (bfs::recursive_directory_iterator end, dir(cs.path); dir != end; ++dir) {
        dir.disable_recursion_pending(!cs.recursion);
        if (!bfs::is_directory(*dir) && !shouldIgnoreFile(*dir)) {
            std::string filePath = (*dir).path().string();
            std::string fileNameWithPath = filePath + cs.CHECKSUM_EXTENSION;
            std::ifstream infile(fileNameWithPath);
            if (infile.good()) {
                std::string md5Hash;
                getline(infile, md5Hash);
                md5Hash = md5Hash.substr(0, MD5_DIGEST_LENGTH*2); //todo: make this into a function
                if (md5Hash == generateMD5(*dir)) {
                    PrintingAndStatus::printString("OK - " + filePath + "\n");
                    ++cs.filesThatAreOK;
                } else {
                    PrintingAndStatus::printString("ERROR, hash doesn't match - " + filePath + "\n");
                    ++cs.filesWithMismatchedHashes;
                }
            } else {
                PrintingAndStatus::printString("No checksum file for " + filePath + "\n");
                ++cs.filesWithNoHashes;
            }
            infile.close();
        }
    }
}

void Commands::deleteCommand() {
    auto &cs = CommandState::getInstance();
    std::vector<bfs::path> md5FilesToDelete;
    for (bfs::recursive_directory_iterator end, dir(cs.path); dir != end; ++dir) {
        dir.disable_recursion_pending(!cs.recursion);
        if (!bfs::is_directory(*dir) && isMd5File(*dir)) {
            md5FilesToDelete.push_back((*dir).path());
        }
    }
    for (const auto& a : md5FilesToDelete) {
        if (bfs::remove(a))
            ++cs.md5FilesDeleted;
        else
            ++cs.md5FilesNotDeleted;
    }
}

void Commands::deleteOrphansCommand() {
    auto &cs = CommandState::getInstance();
    std::vector<bfs::path> orphansToDelete;
    for (bfs::recursive_directory_iterator end, dir(cs.path); dir != end; ++dir) {
        dir.disable_recursion_pending(!cs.recursion);
        if (!bfs::is_directory(*dir) && isMd5File(*dir) && isOrphanedChecksum(*dir)) {
            orphansToDelete.push_back((*dir).path());
        }
    }
    for (const auto& a : orphansToDelete) {
        if (bfs::remove(a))
            ++cs.md5FilesDeleted;
        else
            ++cs.md5FilesNotDeleted;
    }
}

std::string Commands::generateMD5(const bfs::directory_entry &de) {
    auto &cs = CommandState::getInstance();
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
        ++cs.filesThatCouldntBeOpened;
        PrintingAndStatus::printString("ERROR - File could not be opened!\n");
        return "Error";
    }

    do { // todo: I don't like anything about this!  But it works, so I'll leave it for now
        inFile.read(buffer, sizeof(buffer));
        if (inFile.gcount() < bufferSize)
            MD5_Update(&context, buffer, static_cast<size_t>(inFile.gcount()));
        else
            MD5_Update(&context, buffer, sizeof(buffer));
        PrintingAndStatus::showProgress(de, ++blocksProcessed, fileSize, bufferSize);
    } while (inFile);

    MD5_Final(out, &context);
    inFile.close();
    std::stringstream ss;

    for (auto a: out)
        ss << std::hex << std::setfill('0') << std::setw(2) << (int) a;

    return ss.str();
}

void Commands::createMD5File(const bfs::directory_entry &de, const std::string &hashAsString) {
    auto &cs = CommandState::getInstance();
    if (hashAsString.length() / 2 == MD5_DIGEST_LENGTH) {
        std::string fileName = de.path().filename().string();
        std::string filePath = de.path().string();
        std::string filePathWithExtension = filePath + cs.CHECKSUM_EXTENSION;
        std::ofstream md5file(filePathWithExtension);

        if (md5file.good()) {
            md5file << hashAsString << "  " << fileName;
            md5file.close();
            ++cs.md5FilesCreated;
            PrintingAndStatus::printString("OK - hash file successfully created for " + filePath + "\n");
        } else {
            ++cs.md5FilesThatCouldntBeCreated;
            PrintingAndStatus::printString("ERROR - hash file could not be created for " + filePath + "\n");
        }
    }
}

bool Commands::md5FileExists(const bfs::directory_entry &de) {
    auto &cs = CommandState::getInstance();
    std::string filePath = de.path().string();
    std::string filePathWithMd5Appendage = filePath + cs.CHECKSUM_EXTENSION;
    if (bfs::is_regular_file(filePathWithMd5Appendage)) {
        ++cs.md5FilesThatAlreadyExist;
        return true;
    }
    return false;
}

bool Commands::shouldIgnoreFile(const bfs::directory_entry &de) {
    return isMd5File(de);
}

bool Commands::isMd5File(const bfs::directory_entry &de) {
    return de.path().extension() == CommandState::getInstance().CHECKSUM_EXTENSION;
}

bool Commands::isOrphanedChecksum(const bfs::directory_entry &de) {
    // todo: Eliminate unnecessary substring using de.path.filename or whatever it's called
    std::string checksumFile = de.path().string();
    std::string nameOfCorrespondingFile = checksumFile.substr(0, checksumFile.size() - CommandState::getInstance().CHECKSUM_EXTENSION.size());
    return !bfs::is_regular_file(nameOfCorrespondingFile);
}

void Commands::countOrphanedChecksums() {
    auto &cs = CommandState::getInstance();
    for (bfs::recursive_directory_iterator end, dir(CommandState::getInstance().path); dir != end; ++dir) {
        dir.disable_recursion_pending(!CommandState::getInstance().recursion);
        if (!bfs::is_directory(*dir) && isMd5File(*dir) && isOrphanedChecksum(*dir)) {
            ++CommandState::getInstance().orphanedChecksums;
        }
    }
}
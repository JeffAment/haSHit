/**
 * @author https://github.com/jeffament
 */

#ifndef HASHIT_COMMANDS_H
#define HASHIT_COMMANDS_H

#include <iomanip>
#include <openssl/md5.h>
#include "PrintingAndStatus.h"
#include "CommandState.h"

namespace bfs = boost::filesystem;

class Commands {
public:
    // command functions
    static void generateCommand();

    static void checkCommand();

    static void deleteCommand();

    static void deleteOrphansCommand();

    // helper functions
    static std::string generateMD5(const bfs::directory_entry &de);

    static void createMD5File(const bfs::directory_entry &de, const std::string &hashAsString);

    static bool shouldIgnoreFile(const bfs::directory_entry &de);

    static bool md5FileExists(const bfs::directory_entry &de);

    static bool isOrphanedChecksum(const bfs::directory_entry &de);

    static void countOrphanedChecksums();

private:
    static bool isMd5File(const bfs::directory_entry &de);
};


#endif //HASHIT_COMMANDS_H

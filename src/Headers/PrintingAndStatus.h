/**
 * @author https://github.com/jeffament
 */

#ifndef HASHIT_PRINTINGANDSTATUS_H
#define HASHIT_PRINTINGANDSTATUS_H

#include <iostream>
#include "CommandState.h"

namespace bfs = boost::filesystem;

class PrintingAndStatus {
public:
    static void printUsage();

    static void printCheckStatusReport();

    static void printGenerateStatusReport();

    static void printDeleteStatusReport();

    static void printDeleteOrphansStatusReport();

    static void printString(const std::string &string, const bool &overrideQuiet = false);

    static void showProgress(const bfs::directory_entry &de, const int &blocksProcessed, const size_t &fileSize,
                      const size_t &bufferSize);
};

#endif //HASHIT_PRINTINGANDSTATUS_H
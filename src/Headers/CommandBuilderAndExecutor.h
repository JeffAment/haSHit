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
#include "CommandState.h"
#include "Commands.h"
#include "PrintingAndStatus.h"

namespace bfs = boost::filesystem;

class CommandBuilderAndExecutor {
public:
    CommandBuilderAndExecutor() = default;

    void execute();
};

#endif //HASHIT_COMMANDBUILDERANDEXECUTOR_H
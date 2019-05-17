/**
 * @author https://github.com/jeffament
 */

#include "Headers/CommandState.h"

bool CommandState::setPath(const std::string &path) {
    if (bfs::is_directory(path)) {
        this->path = path;
        return true;
    }
    return false;
}

bool CommandState::setCommand(const std::string &command) {
    if (isValidCommand(command)) {
        this->command = command;
        return true;
    }
    return false;
}

void CommandState::setQuietTrue() {
    quiet = true;
}

void CommandState::setRecursionTrue() {
    recursion = true;
}

void CommandState::setGenerateOverwriteTrue() {
    generateOverwrite = true;
}

const bool CommandState::isValidCommand(const std::string &command) {
    return std::find(VALID_COMMANDS_VECTOR.begin(), VALID_COMMANDS_VECTOR.end(), command) != VALID_COMMANDS_VECTOR.end();
}

CommandState &CommandState::getInstance() {
    //todo
    static CommandState instance;
    return instance;
}

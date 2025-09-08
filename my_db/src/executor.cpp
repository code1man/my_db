#include "my_db/executor.h"
#include <iostream>

using namespace std;

namespace my_db {

    Executor::Executor(StorageEngine& storage) : storage_(storage) {}

    bool Executor::execute(const ParsedCommand& cmd) {
        switch (cmd.type) {
        case CommandType::CREATE_DB:
            return execute_create_database(cmd);
        case CommandType::DROP_DB:
            return execute_drop_database(cmd);
        case CommandType::OPEN_DB:
            return execute_open_database(cmd);
        case CommandType::CLOSE_DB:
            return execute_close_database(cmd);
        case CommandType::CREATE_TABLE:
            return execute_create_table(cmd);
        case CommandType::DROP_TABLE:
            return execute_drop_table(cmd);
        case CommandType::EXIT:
            return false;
        default:
            cout << "Unknown command or not implemented." << endl;
            return true;
        }
    }

    bool Executor::execute_create_database(const ParsedCommand& cmd) {
        if (cmd.target.empty()) {
            cout << "Database name missing." << endl;
            return true;
        }
        storage_.myCreateDataBase(cmd.target);
        return true;
    }

    bool Executor::execute_drop_database(const ParsedCommand& cmd) {
        if (cmd.target.empty()) {
            cout << "Database name missing." << endl;
            return true;
        }
        storage_.myDropDataBase(cmd.target);
        return true;
    }

    bool Executor::execute_open_database(const ParsedCommand& cmd) {
        if (cmd.target.empty()) {
            cout << "Database name missing." << endl;
            return true;
        }
        storage_.myOpenDataBase(cmd.target);
        return true;
    }

    bool Executor::execute_close_database(const ParsedCommand& cmd) {
        storage_.myCloseDataBase();
        return true;
    }

    bool Executor::execute_create_table(const ParsedCommand& cmd) {
        if (cmd.target.empty()) {
            cout << "Table name missing." << endl;
            return true;
        }
        storage_.myCreateTable(cmd.target);
        return true;
    }

    bool Executor::execute_drop_table(const ParsedCommand& cmd) {
        if (cmd.target.empty()) {
            cout << "Table name missing." << endl;
            return true;
        }
        storage_.myDropTable(cmd.target);
        return true;
    }

} // namespace my_db
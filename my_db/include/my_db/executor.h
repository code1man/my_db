#ifndef MY_DB_EXECUTOR_H
#define MY_DB_EXECUTOR_H

#include <string>
#include "my_db/parser.h"
#include "my_db/storage/storage_engine.h"

namespace my_db {

    class Executor {
    public:
        Executor(StorageEngine& storage);
        bool execute(const ParsedCommand& cmd);

    private:
        StorageEngine& storage_; // 持有存储引擎的引用

        bool execute_create_database(const ParsedCommand& cmd);
        bool execute_drop_database(const ParsedCommand& cmd);
        bool execute_open_database(const ParsedCommand& cmd);
        bool execute_close_database(const ParsedCommand& cmd);
        bool execute_create_table(const ParsedCommand& cmd);
        bool execute_drop_table(const ParsedCommand& cmd);

    };

} // namespace my_db

#endif
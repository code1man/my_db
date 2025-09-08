#ifndef MY_DB_CLI_H
#define MY_DB_CLI_H

#include <string>

namespace my_db {

    class Parser;
    class Executor;
    /// <summary>
    /// 负责展示交互式命令行界面
    /// </summary>
    class CLI {
    public:
        CLI(Parser& parser, Executor& executor);
        void start();

    private:
        void print_welcome() const;
        void print_prompt() const;
        std::string read_input() const;

        Parser& parser_;
        Executor& executor_;
    };

} // namespace my_db

#endif
#ifndef MY_DB_PARSER_H
#define MY_DB_PARSER_H

#include <string>
#include <vector>

namespace my_db {

    // 一个非常简化的抽象语法树（AST）节点类型
    enum class CommandType {
        CREATE_DB,
        DROP_DB,
        OPEN_DB,
        CLOSE_DB,
        CREATE_TABLE,
        DROP_TABLE,
        INSERT,
        DELETE,
        UPDATE,
        SELECT,
        EXIT,
        UNKNOWN
    };

    // 一个简化的解析结果
    struct ParsedCommand {
        CommandType type{ CommandType::UNKNOWN };
        std::string target; // 如数据库名、表名
        std::vector<std::string> arguments; // 其他参数，如列名、值等
        // 对于更复杂的解析，可以用更结构化的字段
    };

    /// <summary>
    /// 编译器（负责编译原理部分）
    /// </summary>
    class Parser {
    public:
        Parser() = default;
        ParsedCommand parse(const std::string& input);

    private:
        // 辅助函数：转换为小写、分割字符串等
        void to_lower(std::string& s);
        std::vector<std::string> split_string(const std::string& s, char delimiter);
    };

} // namespace my_db

#endif
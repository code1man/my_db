#ifndef MY_DB_PARSER_H
#define MY_DB_PARSER_H

#include <string>
#include <vector>

namespace my_db {

    // һ���ǳ��򻯵ĳ����﷨����AST���ڵ�����
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

    // һ���򻯵Ľ������
    struct ParsedCommand {
        CommandType type{ CommandType::UNKNOWN };
        std::string target; // �����ݿ���������
        std::vector<std::string> arguments; // ������������������ֵ��
        // ���ڸ����ӵĽ����������ø��ṹ�����ֶ�
    };

    /// <summary>
    /// ���������������ԭ���֣�
    /// </summary>
    class Parser {
    public:
        Parser() = default;
        ParsedCommand parse(const std::string& input);

    private:
        // ����������ת��ΪСд���ָ��ַ�����
        void to_lower(std::string& s);
        std::vector<std::string> split_string(const std::string& s, char delimiter);
    };

} // namespace my_db

#endif
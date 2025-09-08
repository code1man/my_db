#include "my_db/parser.h"
#include <algorithm>
#include <sstream>

namespace my_db {
    /// <summary>
    /// 变为小写
    /// </summary>
    /// <param name="s">输入命令</param>
    void Parser::to_lower(std::string& s) {
        for (char& c : s) {
            c = std::tolower(c);
        }
    }

    /// <summary>
    /// 分割字符串
    /// </summary>
    /// <param name="s">输入的命令</param>
    /// <param name="delimiter"></param>
    /// <returns></returns>
    std::vector<std::string> Parser::split_string(const std::string& s, char delimiter) {
        std::vector<std::string> tokens;
        std::string token;
        std::istringstream tokenStream(s);
        while (std::getline(tokenStream, token, delimiter)) {
            if (!token.empty()) {
                tokens.push_back(token);
            }
        }
        return tokens;
    }

    ParsedCommand Parser::parse(const std::string& input) {
        ParsedCommand cmd;
        std::string lower_input = input;
        to_lower(lower_input);

        auto tokens = split_string(lower_input, ' ');
        if (tokens.empty()) {
            return cmd;
        }

        const std::string& first_token = tokens[0];
        if (first_token == "create") {
            if (tokens.size() > 1 && tokens[1] == "database") {
                cmd.type = CommandType::CREATE_DB;
                if (tokens.size() > 2) cmd.target = tokens[2];
            }
            else if (tokens.size() > 1 && tokens[1] == "table") {
                cmd.type = CommandType::CREATE_TABLE;
                if (tokens.size() > 2) cmd.target = tokens[2];
            }
        }
        else if (first_token == "drop") {
            if (tokens.size() > 1 && tokens[1] == "database") {
                cmd.type = CommandType::DROP_DB;
                if (tokens.size() > 2) cmd.target = tokens[2];
            }
            else if (tokens.size() > 1 && tokens[1] == "table") {
                cmd.type = CommandType::DROP_TABLE;
                if (tokens.size() > 2) cmd.target = tokens[2];
            }
        }
        else if (first_token == "open") {
            if (tokens.size() > 1 && tokens[1] == "database") {
                cmd.type = CommandType::OPEN_DB;
                if (tokens.size() > 2) cmd.target = tokens[2];
            }
        }
        else if (first_token == "close") {
            if (tokens.size() > 1 && tokens[1] == "database") {
                cmd.type = CommandType::CLOSE_DB;
                if (tokens.size() > 2) cmd.target = tokens[2];
            }
        }
        else if (first_token == "insert") {
            cmd.type = CommandType::INSERT;
            // 需要更复杂的解析
        }
        else if (first_token == "delete") {
            cmd.type = CommandType::DELETE;
        }
        else if (first_token == "update") {
            cmd.type = CommandType::UPDATE;
        }
        else if (first_token == "query") {
            cmd.type = CommandType::SELECT;
        }
        else if (first_token == "exit") {
            cmd.type = CommandType::EXIT;
        }

        return cmd;
    }

} // namespace my_db
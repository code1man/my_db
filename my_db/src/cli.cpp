#include "my_db/cli.h"
#include "my_db/parser.h"
#include "my_db/executor.h"
#include <iostream>

namespace my_db {

    CLI::CLI(Parser& parser, Executor& executor)
        : parser_(parser), executor_(executor) {}

    void CLI::start() {
        print_welcome();
        bool should_continue = true;
        while (should_continue) {
            print_prompt();
            std::string input = read_input();
            if (input.empty()) {
                continue;
            }
            ParsedCommand command = parser_.parse(input);
            should_continue = executor_.execute(command);
        }
    }

    void CLI::print_welcome() const {
        std::cout << "请按以下规则输入命令语句(命令不分大小写)" << std::endl;
        std::cout << "新建数据库: create database 数据库名字" << std::endl;
        std::cout << "删除数据库: drop database 数据库名字" << std::endl;
        std::cout << "极打开数据库: open database 数据库名字" << std::endl;
        std::cout << "关闭数据库: close database 数据库名字" << std::endl;
        std::cout << "添加新表  : create table  表名" << std::endl;
        std::cout << "            (" << std::endl;
        std::cout << "            列1(字段)名 数据类型1(大小:若为1可省略)," << std::endl;
        std::cout << "            列2(字段)名 数据类型2(大小:若为1可省略)," << std::endl;
        std::cout << "            …          …" << std::endl;
        std::cout << "            )" << std::endl;
        std::cout << "删除旧表  : drop table 表名" << std::endl;
        std::cout << "查看全表  : query all from 表名" << std::endl;
        std::cout << "查询单值  : query 列名 from 表名 where 列名 = 值(值可为all,表全部范围)" << std::endl;
        std::cout << "插入      : insert into 表名(field1,field2,…) values(value1,value2,…)" << std::endl;
        std::cout << "修改      : update 列名 = 新值 from 表名 where 列名 = 值(值可为all,表全部范围)" << std::endl;
        std::cout << "删除      : delete from 表名 where 列名 = 值(值可为all,表全部范围)" << std::endl;
        std::cout << "退出程序  : exit" << std::endl;
    }

    void CLI::print_prompt() const {
        std::cout << "my_db> ";
    }

    std::string CLI::read_input() const {
        std::string input;
        std::getline(std::cin, input);
        return input;
    }

} // namespace my_db
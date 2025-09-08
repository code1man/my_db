#ifndef MY_DB_TABLE_H
#define MY_DB_TABLE_H

#include <string>
#include <vector>
#include <cstdio>

namespace my_db {

    struct Table {
        std::string name;
        std::string path_name; // 表的文件路径
        std::vector<std::string> col_name; // 列名
        std::vector<std::string> type;     // 数据类型
        std::vector<int> size;             // 数据大小
        FILE* fp{ nullptr };                 // 文件指针

        Table() = default;
        explicit Table(const std::string& name) : name(name) {}
        ~Table(); // 析构函数负责关闭文件

        // 禁止拷贝，允许移动
        Table(const Table&) = delete;
        Table& operator=(const Table&) = delete;
        Table(Table&&) noexcept;
        Table& operator=(Table&&) noexcept;
    };

} // namespace my_db

#endif
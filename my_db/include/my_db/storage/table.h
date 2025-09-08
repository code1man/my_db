#ifndef MY_DB_TABLE_H
#define MY_DB_TABLE_H

#include <string>
#include <vector>
#include <cstdio>

namespace my_db {

    struct Table {
        std::string name;
        std::string path_name; // ����ļ�·��
        std::vector<std::string> col_name; // ����
        std::vector<std::string> type;     // ��������
        std::vector<int> size;             // ���ݴ�С
        FILE* fp{ nullptr };                 // �ļ�ָ��

        Table() = default;
        explicit Table(const std::string& name) : name(name) {}
        ~Table(); // ������������ر��ļ�

        // ��ֹ�����������ƶ�
        Table(const Table&) = delete;
        Table& operator=(const Table&) = delete;
        Table(Table&&) noexcept;
        Table& operator=(Table&&) noexcept;
    };

} // namespace my_db

#endif
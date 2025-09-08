#include "my_db/storage/table.h"
#include <utility>

namespace my_db {

    Table::~Table() {
        if (fp != nullptr) {
            fclose(fp);
            fp = nullptr;
        }
    }

    Table::Table(Table&& other) noexcept
        : name(std::move(other.name)),
        path_name(std::move(other.path_name)),
        col_name(std::move(other.col_name)),
        type(std::move(other.type)),
        size(std::move(other.size)),
        fp(other.fp) {
        other.fp = nullptr;
    }

    Table& Table::operator=(Table&& other) noexcept {
        if (this != &other) {
            // �ͷŵ�ǰ��Դ
            if (fp != nullptr) {
                fclose(fp);
            }
            // �ƶ���Դ
            name = std::move(other.name);
            path_name = std::move(other.path_name);
            col_name = std::move(other.col_name);
            type = std::move(other.type);
            size = std::move(other.size);
            fp = other.fp;
            other.fp = nullptr;
        }
        return *this;
    }

}
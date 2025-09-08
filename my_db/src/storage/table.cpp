#include "my_db/storage/table.h"
#include <utility>

using namespace std;

namespace my_db {

    Table::~Table() {
        if (fp != nullptr) {
            fclose(fp);
            fp = nullptr;
        }
    }

    Table::Table(Table&& other) noexcept
        : name(move(other.name)),
        path_name(move(other.path_name)),
        col_name(move(other.col_name)),
        type(move(other.type)),
        size(move(other.size)),
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
            name = move(other.name);
            path_name = move(other.path_name);
            col_name = move(other.col_name);
            type = move(other.type);
            size = move(other.size);
            fp = other.fp;
            other.fp = nullptr;
        }
        return *this;
    }

}
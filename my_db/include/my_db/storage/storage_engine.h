#ifndef STORAGE_ENGINE_H
#define STORAGE_ENGINE_H

#include <iostream>
#include <string>
#include <vector>
#include <cstdio>
#include <sstream>
#include <iomanip>
#include <direct.h>
#include <io.h>
#include <fstream>

namespace my_db {

    const int inf = 0x3f3f3f3f;

    struct Tables {
        std::string name;
        std::string pathName;
        std::vector<std::string> colName;
        std::vector<std::string> type;
        std::vector<int> size;
        FILE* fp;
    };

    class StorageEngine {
    public:
        StorageEngine();
        ~StorageEngine();

        std::string prePath;
        bool open;
        std::vector<Tables*> tab;

        void myOpenDataBase(std::string dataBaseName);
        void myCloseDataBase();
        void myCreateDataBase(std::string dataBaseName);
        void myDropDataBase(std::string dataBaseName);
        void myCreateTable(std::string tableName);
        void myDropTable(std::string tableName);
        void myInsert(std::string tableName, std::string value);
        void myDelete(std::string tableName, std::string isWhere);
        void myUpdate(std::string tableName, std::string toColName, std::string newValue, std::string isWhere);
        void myQuery(std::string toColName, std::string tableName, std::string isWhere = "");

    private:
        int posIsNos(std::string tableName);
    };

} // namespace my_db

#endif
#include "my_db/storage/storage_engine.h"
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

    StorageEngine::StorageEngine() {
        prePath = "";
        open = false;
    }

    StorageEngine::~StorageEngine() {
        for (int i = 0; i < tab.size(); i++) {
            if (tab[i]->fp != NULL)
                fclose(tab[i]->fp);
            delete tab[i];
        }
        tab.resize(0);
        prePath = "";
        open = false;
    }

    void StorageEngine::myOpenDataBase(std::string dataBaseName) {
        if (open) {
            std::cout << "请先关闭当前打开的数据库" << std::endl;
            return;
        }
        std::string pathName = "D:\\mini_db\\repos\\" + dataBaseName;
        if (0 != _access(pathName.c_str(), 0)) {
            std::cout << "该数据库不存在" << std::endl;
        }
        else {
            prePath = pathName + "\\";
            std::cout << "打开数据库成功" << std::endl;
            open = true;
        }
    }

    void StorageEngine::myCloseDataBase() {
        for (int i = 0; i < tab.size(); i++) {
            if (tab[i]->fp != NULL)
                fclose(tab[i]->fp);
            delete tab[i];
        }
        tab.resize(0);
        prePath = "";
        open = false;
    }

    void StorageEngine::myCreateDataBase(std::string dataBaseName) {
        std::string pathName = "D:\\mini_db\\repos\\" + dataBaseName;
        if (0 != _access(pathName.c_str(), 0)) {
            if (0 == _mkdir(pathName.c_str()))
                std::cout << "创建成功" << std::endl;
            else
                std::cout << "创建失败" << std::endl;
            return;
        }
        std::cout << "该数据库已存在" << std::endl;
    }

    void StorageEngine::myDropDataBase(std::string dataBaseName) {
        std::string pathName = "D:\\mini_db\\repos\\" + dataBaseName;
        if (0 == _access(pathName.c_str(), 0)) {
            pathName = "rd " + pathName;
            if (0 == system(pathName.c_str()))
                std::cout << "删除数据库" << dataBaseName << "成功" << std::endl;
            else
                std::cout << "删除数据库" << dataBaseName << "失败" << std::endl;
            return;
        }
        std::cout << "数据库" << dataBaseName << "不存在" << std::endl;
    }

    void StorageEngine::myCreateTable(std::string tableName) {
        std::vector<std::string> colName;
        std::vector<std::string> type;
        std::vector<int> size;
        std::string tmp;
        getchar();
        getline(std::cin, tmp);
        getline(std::cin, tmp);
        while (tmp != ")") {
            std::stringstream ss(tmp);
            std::string x;
            ss >> x; colName.push_back(x);
            ss >> x;
            int pos = x.find('(');
            if (pos == std::string::npos) {
                type.push_back(x);
                size.push_back(1);
            }
            else {
                type.push_back(x.substr(0, pos));
                int num = 0;
                for (int i = pos + 1; i < x.length() - 1; i++)
                    num = num * 10 + x[i] - '0';
                size.push_back(num);
            }
            getline(std::cin, tmp);
        }
        tableName += ".txt";
        std::string pathName = prePath + tableName;
        if (0 != _access(pathName.c_str(), 0)) {
            Tables* ptr = new Tables;
            ptr->name = tableName;
            ptr->pathName = pathName;
            ptr->colName = colName;
            ptr->type = type;
            ptr->size = size;
            ptr->fp = fopen(pathName.c_str(), "w");
            std::string wrin;
            wrin.clear();
            for (int i = 0; i < colName.size(); i++)
                wrin += colName[i] + ";" + type[i] + ";" + std::to_string(size[i]) + ";";
            wrin += "\n";
            fprintf(ptr->fp, wrin.c_str());
            fclose(ptr->fp);
            tab.push_back(ptr);
            std::cout << "创建新表成功!" << std::endl;
            return;
        }
        std::cout << "该表已经存在!" << std::endl;
    }

    void StorageEngine::myDropTable(std::string tableName) {
        tableName += ".txt";
        std::string pathName = prePath + tableName;
        if (0 != _access(pathName.c_str(), 0)) {
            std::cout << "该表不存在!" << std::endl;
        }
        else {
            for (int i = 0; i < tab.size(); i++) {
                if (tab[i]->name == tableName) {
                    if (tab[i]->fp != NULL)
                        fclose(tab[i]->fp);
                    delete tab[i];
                    tab.erase(tab.begin() + i);
                }
            }
            remove(pathName.c_str());
            std::cout << "删除成功!" << std::endl;
        }
    }

    int StorageEngine::posIsNos(std::string tableName) {
        std::string pathName = prePath + tableName + ".txt";
        if (0 != _access(pathName.c_str(), 0)) {
            std::cout << "该表不存在!" << std::endl;
            return -1;
        }
        FILE* tempfptr = fopen(pathName.c_str(), "r");
        char contant[100];
        fscanf(tempfptr, "%s", contant);
        std::string tmp = contant;
        for (int i = 0; i < tmp.size(); i++) {
            if (tmp[i] == ';')
                tmp[i] = ' ';
        }
        std::stringstream check(tmp);
        std::string x, y, z;
        Tables* nxt = new Tables;
        nxt->name = tableName;
        while (check >> x) {
            check >> y >> z;
            nxt->colName.push_back(x);
            nxt->type.push_back(y);
            nxt->size.push_back(atoi(z.c_str()));
        }
        nxt->pathName = pathName;
        tab.push_back(nxt);
        fclose(tempfptr);
        return tab.size() - 1;
    }

    void StorageEngine::myInsert(std::string tableName, std::string value) {
        if (!open) {
            std::cout << "无选中数据库!" << std::endl;
            return;
        }
        int pos = inf;
        for (int i = 0; i < tab.size(); i++) {
            if (tab[i]->name == tableName) {
                pos = i;
                break;
            }
        }
        if (pos == inf)
            pos = posIsNos(tableName);
        if (pos == -1) return;

        std::stringstream ss(value);
        std::string tmp2; ss >> tmp2;
        tab[pos]->fp = fopen(tab[pos]->pathName.c_str(), "a");
        for (int i = 0; i < tab[pos]->type.size(); i++) {
            std::string tmp = tab[pos]->type[i];
            if (tmp == "int") {
                int x; ss >> x;
                fprintf(tab[pos]->fp, "%d", x);
            }
            else if (tmp == "float") {
                float x; ss >> x;
                fprintf(tab[pos]->fp, "%f", x);
            }
            else if (tmp == "double") {
                double x; ss >> x;
                fprintf(tab[pos]->fp, "%f", x);
            }
            else if (tmp == "char") {
                if (tab[pos]->size[i] == 1) {
                    char x; ss >> x;
                    fprintf(tab[pos]->fp, "%c", x);
                }
                else {
                    int cnt = tab[pos]->size[i];
                    char* x = new char[cnt];
                    ss >> x;
                    fprintf(tab[pos]->fp, "%s", x);
                    delete[] x;
                }
            }
            if (i != tab[pos]->type.size() - 1)
                fprintf(tab[pos]->fp, "%c", ' ');
        }
        fprintf(tab[pos]->fp, "%c", '\n');
        fclose(tab[pos]->fp);
        std::cout << "插入成功!" << std::endl;
    }

    void StorageEngine::myDelete(std::string tableName, std::string isWhere) {
        if (!open) {
            std::cout << "无选中数据库!" << std::endl;
            return;
        }
        int pos = inf;
        for (int i = 0; i < tab.size(); i++) {
            if (tab[i]->name == tableName) {
                pos = i;
                break;
            }
        }
        if (pos == inf)
            pos = posIsNos(tableName);
        if (pos == -1) return;

        std::stringstream ss(isWhere);
        int wherePos = inf;
        std::string typeName, toValue, whr, deng;
        ss >> whr >> typeName >> deng >> toValue;
        for (int i = 0; i < tab[pos]->colName.size(); i++) {
            if (tab[pos]->colName[i] == typeName) {
                wherePos = i;
                break;
            }
        }

        std::string pathName2 = prePath + "tmp.txt";
        FILE* tmpfptr = fopen(pathName2.c_str(), "w");
        char sentence[1024];
        tab[pos]->fp = fopen(tab[pos]->pathName.c_str(), "r");
        fgets(sentence, 1024, tab[pos]->fp);
        fputs(sentence, tmpfptr);
        bool flag = true;
        if (toValue.length() == 3 && tolower(toValue[0]) == 'a' && tolower(toValue[1]) == 'l' && tolower(toValue[2]) == 'l')
            flag = false;

        if (flag) {
            while (!feof(tab[pos]->fp)) {
                memset(sentence, 0, sizeof(sentence));
                fgets(sentence, 1024, tab[pos]->fp);
                std::stringstream myTmp(sentence);
                std::string x;
                for (int i = 0; i <= wherePos; i++)
                    myTmp >> x;
                if (x == toValue) continue;
                fputs(sentence, tmpfptr);
            }
        }
        fclose(tmpfptr);
        fclose(tab[pos]->fp);
        remove(tab[pos]->pathName.c_str());
        if (0 == rename(pathName2.c_str(), tab[pos]->pathName.c_str()))
            std::cout << "删除成功!" << std::endl;
        else
            std::cout << "删除失败!" << std::endl;
    }

    void StorageEngine::myUpdate(std::string tableName, std::string toColName, std::string newValue, std::string isWhere) {
        if (!open) {
            std::cout << "无选中数据库!" << std::endl;
            return;
        }
        int pos = inf;
        for (int i = 0; i < tab.size(); i++) {
            if (tab[i]->name == tableName) {
                pos = i;
                break;
            }
        }
        if (pos == inf)
            pos = posIsNos(tableName);
        if (pos == -1) return;

        std::stringstream ss(isWhere);
        int wherePos = inf, updataPos = inf;
        std::string typeName, toValue, whr, deng;
        ss >> whr >> typeName >> deng >> toValue;
        for (int i = 0; i < tab[pos]->colName.size(); i++) {
            if (tab[pos]->colName[i] == typeName) {
                wherePos = i;
                break;
            }
        }
        for (int i = 0; i < tab[pos]->colName.size(); i++) {
            if (tab[pos]->colName[i] == toColName) {
                updataPos = i;
                break;
            }
        }

        std::string pathName2 = prePath + "tmp.txt";
        FILE* tmpfptr = fopen(pathName2.c_str(), "w");
        char sentence[1024];
        tab[pos]->fp = fopen(tab[pos]->pathName.c_str(), "r");
        fgets(sentence, 1024, tab[pos]->fp);
        fputs(sentence, tmpfptr);
        bool flag = true;
        if (toValue.length() == 3 && tolower(toValue[0]) == 'a' && tolower(toValue[1]) == 'l' && tolower(toValue[2]) == 'l')
            flag = false;

        if (!flag) {
            while (!feof(tab[pos]->fp)) {
                for (int i = 0; i < tab[pos]->type.size(); i++) {
                    std::string tmp = tab[pos]->type[i];
                    if (tmp == "int") {
                        int x; fscanf(tab[pos]->fp, "%d", &x);
                        if (i == updataPos)
                            x = atoi(newValue.c_str());
                        fprintf(tmpfptr, "%d", x);
                    }
                    else if (tmp == "float") {
                        float x; fscanf(tab[pos]->fp, "%f", &x);
                        if (i == updataPos)
                            x = atof(newValue.c_str());
                        fprintf(tmpfptr, "%f", x);
                    }
                    else if (tmp == "double") {
                        double x; fscanf(tab[pos]->fp, "%lf", &x);
                        if (i == updataPos)
                            x = atof(newValue.c_str());
                        fprintf(tmpfptr, "%f ", x);
                    }
                    else if (tmp == "char") {
                        if (tab[pos]->size[i] == 1) {
                            char x; fscanf(tab[pos]->fp, "%c", &x);
                            if (i == updataPos)
                                x = newValue[0];
                            fprintf(tmpfptr, "%c", x);
                        }
                        else {
                            int cnt = tab[pos]->size[i];
                            char* x = new char[cnt];
                            fscanf(tab[pos]->fp, "%s", x);
                            if (i == updataPos) {
                                strcpy(x, newValue.c_str());
                                x[newValue.length()] = '\0';
                            }
                            fprintf(tmpfptr, "%s", x);
                            delete[] x;
                        }
                    }
                    if (i != tab[pos]->type.size() - 1)
                        fprintf(tmpfptr, "%c", ' ');
                }
                fprintf(tmpfptr, "%c", '\n');
            }
        }
        else {
            while (!feof(tab[pos]->fp)) {
                memset(sentence, 0, sizeof(sentence));
                fgets(sentence, 1024, tab[pos]->fp);
                std::stringstream myTmp(sentence);
                std::string x;
                for (int i = 0; i <= wherePos; i++)
                    myTmp >> x;
                if (x == toValue) {
                    std::stringstream myTmp2(sentence);
                    std::string input;
                    input.clear();
                    for (int i = 0; i < tab[pos]->colName.size(); i++) {
                        myTmp2 >> x;
                        if (i == updataPos)
                            input += newValue;
                        else
                            input += x;
                        if (i != tab[pos]->colName.size() - 1)
                            input += " ";
                        else
                            input += "\n";
                    }
                    fputs(input.c_str(), tmpfptr);
                }
                else {
                    fputs(sentence, tmpfptr);
                }
            }
        }
        fclose(tmpfptr);
        fclose(tab[pos]->fp);
        remove(tab[pos]->pathName.c_str());
        if (0 == rename(pathName2.c_str(), tab[pos]->pathName.c_str()))
            std::cout << "更新成功!" << std::endl;
        else
            std::cout << "更新失败!" << std::endl;
    }

    void StorageEngine::myQuery(std::string toColName, std::string tableName, std::string isWhere) {
        if (!open) {
            std::cout << "无选中数据库!" << std::endl;
            return;
        }
        int pos = inf;
        for (int i = 0; i < tab.size(); i++) {
            if (tab[i]->name == tableName) {
                pos = i;
                break;
            }
        }
        if (pos == inf)
            pos = posIsNos(tableName);
        if (pos == -1) return;

        tab[pos]->fp = fopen(tab[pos]->pathName.c_str(), "r");
        char contant[1024];
        fgets(contant, sizeof(contant), tab[pos]->fp);

        if (isWhere.empty()) {
            int len = strlen(contant);
            for (int i = 0; i < len; i++) {
                if (contant[i] == ';')
                    contant[i] = ' ';
            }
            std::stringstream ss(contant);
            std::string x;
            for (int i = 0; i < tab[pos]->size.size(); i++) {
                int width = 15;
                if (tab[pos]->size[i] != 1)
                    width = tab[pos]->size[i];
                ss >> x;
                std::cout << std::left << std::setw(width) << x;
                ss >> x;
                ss >> x;
            }
            std::cout << std::endl;

            while (!feof(tab[pos]->fp)) {
                memset(contant, 0, sizeof(contant));
                fgets(contant, sizeof(contant), tab[pos]->fp);
                if (strlen(contant) == 0) break;
                std::stringstream out(contant);
                std::string x;
                for (int i = 0; i < tab[pos]->type.size(); i++) {
                    out >> x;
                    int width = 15;
                    if (tab[pos]->size[i] != 1)
                        width = tab[pos]->size[i];
                    std::string tmp = tab[pos]->type[i];
                    if (tmp == "int")
                        std::cout << std::left << std::setw(width) << atoi(x.c_str());
                    else if (tmp == "float" || tmp == "double")
                        std::cout << std::left << std::setw(width) << atof(x.c_str());
                    else if (tmp == "char")
                        std::cout << std::left << std::setw(width) << x;
                }
                std::cout << std::endl;
            }
        }
        else {
            std::stringstream ss(isWhere);
            int wherePos = inf, aimPos = inf;
            std::string typeName, toValue, whr, deng;
            ss >> whr >> typeName >> deng >> toValue;
            for (int i = 0; i < tab[pos]->colName.size(); i++) {
                if (tab[pos]->colName[i] == typeName) {
                    wherePos = i;
                    break;
                }
            }
            for (int i = 0; i < tab[pos]->colName.size(); i++) {
                if (tab[pos]->colName[i] == toColName) {
                    aimPos = i;
                    break;
                }
            }

            while (!feof(tab[pos]->fp)) {
                memset(contant, 0, sizeof(contant));
                fgets(contant, sizeof(contant), tab[pos]->fp);
                std::stringstream myTmp(contant);
                std::string x, check, out;
                for (int i = 0; i < tab[pos]->colName.size(); i++) {
                    myTmp >> x;
                    if (i == wherePos) check = x;
                    if (i == aimPos) out = x;
                }
                if (check == toValue) {
                    int width = 15;
                    if (tab[pos]->size[aimPos] != 1)
                        width = tab[pos]->size[aimPos];
                    std::string tmp = tab[pos]->type[aimPos];
                    if (tmp == "int")
                        std::cout << std::left << std::setw(width) << atoi(out.c_str()) << std::endl;
                    else if (tmp == "float")
                        std::cout << std::left << std::setw(width) << atof(out.c_str()) << std::endl;
                    else if (tmp == "double")
                        std::cout << std::left << std::setw(width) << atof(out.c_str()) << std::endl;
                    else if (tmp == "char")
                        std::cout << std::left << std::setw(width) << out << std::endl;
                }
            }
        }
        fclose(tab[pos]->fp);
    }

} // namespace my_db
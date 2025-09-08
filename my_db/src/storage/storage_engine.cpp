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

using namespace std;

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

    void StorageEngine::myOpenDataBase(string dataBaseName) {
        if (open) {
            cout << "请先关闭当前打开的数据库" << endl;
            return;
        }
        string pathName = "D:\\mini_db\\repos\\" + dataBaseName;
        if (0 != _access(pathName.c_str(), 0)) {
            cout << "该数据库不存在" << endl;
        }
        else {
            prePath = pathName + "\\";
            cout << "打开数据库成功" << endl;
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

    void StorageEngine::myCreateDataBase(string dataBaseName) {
        string pathName = "D:\\mini_db\\repos\\" + dataBaseName;
        if (0 != _access(pathName.c_str(), 0)) {
            if (0 == _mkdir(pathName.c_str()))
                cout << "创建成功" << endl;
            else
                cout << "创建失败" << endl;
            return;
        }
        cout << "该数据库已存在" << endl;
    }

    void StorageEngine::myDropDataBase(string dataBaseName) {
        string pathName = "D:\\mini_db\\repos\\" + dataBaseName;
        if (0 == _access(pathName.c_str(), 0)) {
            pathName = "rd " + pathName;
            if (0 == system(pathName.c_str()))
                cout << "删除数据库" << dataBaseName << "成功" << endl;
            else
                cout << "删除数据库" << dataBaseName << "失败" << endl;
            return;
        }
        cout << "数据库" << dataBaseName << "不存在" << endl;
    }

    void StorageEngine::myCreateTable(string tableName) {
        vector<string> colName;
        vector<string> type;
        vector<int> size;
        string tmp;
        getchar();
        getline(cin, tmp);
        getline(cin, tmp);
        while (tmp != ")") {
            stringstream ss(tmp);
            string x;
            ss >> x; colName.push_back(x);
            ss >> x;
            int pos = x.find('(');
            if (pos == string::npos) {
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
            getline(cin, tmp);
        }
        tableName += ".txt";
        string pathName = prePath + tableName;
        if (0 != _access(pathName.c_str(), 0)) {
            Tables* ptr = new Tables;
            ptr->name = tableName;
            ptr->pathName = pathName;
            ptr->colName = colName;
            ptr->type = type;
            ptr->size = size;
            ptr->fp = fopen(pathName.c_str(), "w");
            string wrin;
            wrin.clear();
            for (int i = 0; i < colName.size(); i++)
                wrin += colName[i] + ";" + type[i] + ";" + to_string(size[i]) + ";";
            wrin += "\n";
            fprintf(ptr->fp, wrin.c_str());
            fclose(ptr->fp);
            tab.push_back(ptr);
            cout << "创建新表成功!" << endl;
            return;
        }
        cout << "该表已经存在!" << endl;
    }

    void StorageEngine::myDropTable(string tableName) {
        tableName += ".txt";
        string pathName = prePath + tableName;
        if (0 != _access(pathName.c_str(), 0)) {
            cout << "该表不存在!" << endl;
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
            cout << "删除成功!" << endl;
        }
    }

    int StorageEngine::posIsNos(string tableName) {
        string pathName = prePath + tableName + ".txt";
        if (0 != _access(pathName.c_str(), 0)) {
            cout << "该表不存在!" << endl;
            return -1;
        }
        FILE* tempfptr = fopen(pathName.c_str(), "r");
        char contant[100];
        fscanf(tempfptr, "%s", contant);
        string tmp = contant;
        for (int i = 0; i < tmp.size(); i++) {
            if (tmp[i] == ';')
                tmp[i] = ' ';
        }
        stringstream check(tmp);
        string x, y, z;
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

    void StorageEngine::myInsert(string tableName, string value) {
        if (!open) {
            cout << "无选中数据库!" << endl;
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

        stringstream ss(value);
        string tmp2; ss >> tmp2;
        tab[pos]->fp = fopen(tab[pos]->pathName.c_str(), "a");
        for (int i = 0; i < tab[pos]->type.size(); i++) {
            string tmp = tab[pos]->type[i];
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
        cout << "插入成功!" << endl;
    }

    void StorageEngine::myDelete(string tableName, string isWhere) {
        if (!open) {
            cout << "无选中数据库!" << endl;
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

        stringstream ss(isWhere);
        int wherePos = inf;
        string typeName, toValue, whr, deng;
        ss >> whr >> typeName >> deng >> toValue;
        for (int i = 0; i < tab[pos]->colName.size(); i++) {
            if (tab[pos]->colName[i] == typeName) {
                wherePos = i;
                break;
            }
        }

        string pathName2 = prePath + "tmp.txt";
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
                stringstream myTmp(sentence);
                string x;
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
            cout << "删除成功!" << endl;
        else
            cout << "删除失败!" << endl;
    }

    void StorageEngine::myUpdate(string tableName, string toColName, string newValue, string isWhere) {
        if (!open) {
            cout << "无选中数据库!" << endl;
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

        stringstream ss(isWhere);
        int wherePos = inf, updataPos = inf;
        string typeName, toValue, whr, deng;
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

        string pathName2 = prePath + "tmp.txt";
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
                    string tmp = tab[pos]->type[i];
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
                stringstream myTmp(sentence);
                string x;
                for (int i = 0; i <= wherePos; i++)
                    myTmp >> x;
                if (x == toValue) {
                    stringstream myTmp2(sentence);
                    string input;
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
            cout << "更新成功!" << endl;
        else
            cout << "更新失败!" << endl;
    }

    void StorageEngine::myQuery(string toColName, string tableName, string isWhere) {
        if (!open) {
            cout << "无选中数据库!" << endl;
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
            stringstream ss(contant);
            string x;
            for (int i = 0; i < tab[pos]->size.size(); i++) {
                int width = 15;
                if (tab[pos]->size[i] != 1)
                    width = tab[pos]->size[i];
                ss >> x;
                cout << left << setw(width) << x;
                ss >> x;
                ss >> x;
            }
            cout << endl;

            while (!feof(tab[pos]->fp)) {
                memset(contant, 0, sizeof(contant));
                fgets(contant, sizeof(contant), tab[pos]->fp);
                if (strlen(contant) == 0) break;
                stringstream out(contant);
                string x;
                for (int i = 0; i < tab[pos]->type.size(); i++) {
                    out >> x;
                    int width = 15;
                    if (tab[pos]->size[i] != 1)
                        width = tab[pos]->size[i];
                    string tmp = tab[pos]->type[i];
                    if (tmp == "int")
                        cout << left << setw(width) << atoi(x.c_str());
                    else if (tmp == "float" || tmp == "double")
                        cout << left << setw(width) << atof(x.c_str());
                    else if (tmp == "char")
                        cout << left << setw(width) << x;
                }
                cout << endl;
            }
        }
        else {
            stringstream ss(isWhere);
            int wherePos = inf, aimPos = inf;
            string typeName, toValue, whr, deng;
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
                stringstream myTmp(contant);
                string x, check, out;
                for (int i = 0; i < tab[pos]->colName.size(); i++) {
                    myTmp >> x;
                    if (i == wherePos) check = x;
                    if (i == aimPos) out = x;
                }
                if (check == toValue) {
                    int width = 15;
                    if (tab[pos]->size[aimPos] != 1)
                        width = tab[pos]->size[aimPos];
                    string tmp = tab[pos]->type[aimPos];
                    if (tmp == "int")
                        cout << left << setw(width) << atoi(out.c_str()) << endl;
                    else if (tmp == "float")
                        cout << left << setw(width) << atof(out.c_str()) << endl;
                    else if (tmp == "double")
                        cout << left << setw(width) << atof(out.c_str()) << endl;
                    else if (tmp == "char")
                        cout << left << setw(width) << out << endl;
                }
            }
        }
        fclose(tab[pos]->fp);
    }

} // namespace my_db
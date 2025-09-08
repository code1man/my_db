#include "my_db/cli.h"
#include "my_db/parser.h"
#include "my_db/executor.h"
#include <iostream>

using namespace std;

namespace my_db {

    CLI::CLI(Parser& parser, Executor& executor)
        : parser_(parser), executor_(executor) {}

    void CLI::start() {
        print_welcome();
        bool should_continue = true;
        while (should_continue) {
            print_prompt();
            string input = read_input();
            if (input.empty()) {
                continue;
            }
            ParsedCommand command = parser_.parse(input);
            should_continue = executor_.execute(command);
        }
    }

    void CLI::print_welcome() const {
        cout << "�밴���¹��������������(����ִ�Сд)" << endl;
        cout << "�½����ݿ�: create database ���ݿ�����" << endl;
        cout << "ɾ�����ݿ�: drop database ���ݿ�����" << endl;
        cout << "�������ݿ�: open database ���ݿ�����" << endl;
        cout << "�ر����ݿ�: close database ���ݿ�����" << endl;
        cout << "����±�  : create table  ����" << endl;
        cout << "            (" << endl;
        cout << "            ��1(�ֶ�)�� ��������1(��С:��Ϊ1��ʡ��)," << endl;
        cout << "            ��2(�ֶ�)�� ��������2(��С:��Ϊ1��ʡ��)," << endl;
        cout << "            ��          ��" << endl;
        cout << "            )" << endl;
        cout << "ɾ���ɱ�  : drop table ����" << endl;
        cout << "�鿴ȫ��  : query all from ����" << endl;
        cout << "��ѯ��ֵ  : query ���� from ���� where ���� = ֵ(ֵ��Ϊall,��ȫ����Χ)" << endl;
        cout << "����      : insert into ����(field1,field2,��) values(value1,value2,��)" << endl;
        cout << "�޸�      : update ���� = ��ֵ from ���� where ���� = ֵ(ֵ��Ϊall,��ȫ����Χ)" << endl;
        cout << "ɾ��      : delete from ���� where ���� = ֵ(ֵ��Ϊall,��ȫ����Χ)" << endl;
        cout << "�˳�����  : exit" << endl;
    }

    void CLI::print_prompt() const {
        cout << "my_db> ";
    }

    string CLI::read_input() const {
        string input;
        getline(cin, input);
        return input;
    }

} // namespace my_db
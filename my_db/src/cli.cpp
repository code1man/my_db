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
        std::cout << "�밴���¹��������������(����ִ�Сд)" << std::endl;
        std::cout << "�½����ݿ�: create database ���ݿ�����" << std::endl;
        std::cout << "ɾ�����ݿ�: drop database ���ݿ�����" << std::endl;
        std::cout << "�������ݿ�: open database ���ݿ�����" << std::endl;
        std::cout << "�ر����ݿ�: close database ���ݿ�����" << std::endl;
        std::cout << "����±�  : create table  ����" << std::endl;
        std::cout << "            (" << std::endl;
        std::cout << "            ��1(�ֶ�)�� ��������1(��С:��Ϊ1��ʡ��)," << std::endl;
        std::cout << "            ��2(�ֶ�)�� ��������2(��С:��Ϊ1��ʡ��)," << std::endl;
        std::cout << "            ��          ��" << std::endl;
        std::cout << "            )" << std::endl;
        std::cout << "ɾ���ɱ�  : drop table ����" << std::endl;
        std::cout << "�鿴ȫ��  : query all from ����" << std::endl;
        std::cout << "��ѯ��ֵ  : query ���� from ���� where ���� = ֵ(ֵ��Ϊall,��ȫ����Χ)" << std::endl;
        std::cout << "����      : insert into ����(field1,field2,��) values(value1,value2,��)" << std::endl;
        std::cout << "�޸�      : update ���� = ��ֵ from ���� where ���� = ֵ(ֵ��Ϊall,��ȫ����Χ)" << std::endl;
        std::cout << "ɾ��      : delete from ���� where ���� = ֵ(ֵ��Ϊall,��ȫ����Χ)" << std::endl;
        std::cout << "�˳�����  : exit" << std::endl;
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
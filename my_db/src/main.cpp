#include "my_db/cli.h"
#include "my_db/parser.h"
#include "my_db/executor.h"
#include "my_db/storage/storage_engine.h"

int main() {
    // ��ʼ����ģ��
    my_db::StorageEngine storage;
    my_db::Parser parser;
    my_db::Executor executor(storage);
    my_db::CLI cli(parser, executor);

    // ��������ѭ��
    cli.start();

    return 0;
}
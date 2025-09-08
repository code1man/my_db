#include "my_db/cli.h"
#include "my_db/parser.h"
#include "my_db/executor.h"
#include "my_db/storage/storage_engine.h"

using namespace my_db;

int main() {
    // ��ʼ����ģ��
    StorageEngine storage;
    Parser parser;
    Executor executor(storage);
    CLI cli(parser, executor);

    // ��������ѭ��
    cli.start();

    return 0;
}
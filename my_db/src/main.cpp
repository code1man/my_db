#include "my_db/cli.h"
#include "my_db/parser.h"
#include "my_db/executor.h"
#include "my_db/storage/storage_engine.h"

using namespace my_db;

int main() {
    // 初始化各模块
    StorageEngine storage;
    Parser parser;
    Executor executor(storage);
    CLI cli(parser, executor);

    // 启动交互循环
    cli.start();

    return 0;
}
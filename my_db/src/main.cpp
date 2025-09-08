#include "my_db/cli.h"
#include "my_db/parser.h"
#include "my_db/executor.h"
#include "my_db/storage/storage_engine.h"

int main() {
    // 初始化各模块
    my_db::StorageEngine storage;
    my_db::Parser parser;
    my_db::Executor executor(storage);
    my_db::CLI cli(parser, executor);

    // 启动交互循环
    cli.start();

    return 0;
}
cmake_minimum_required(VERSION 3.10) # 指定 CMake 最低版本要求[1,3](@ref)
project(MyDatabase VERSION 0.1.0 LANGUAGES CXX) # 定义项目名、版本和语言[1,3](@ref)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 11) # 例如 C++11[1,3](@ref)
set(CMAKE_CXX_STANDARD_REQUIRED ON) # 强制要求支持该标准[3](@ref)

# 全局编译选项（根据需求调整）
add_compile_options(-Wall -Wextra) # 开启额外警告[1,3](@ref)

# 如果你的项目包含头文件，添加头文件搜索路径[3](@ref)
include_directories(include) # 假设头文件在 `include` 目录下

# 查找源文件（推荐明确列出源文件，GLOB 需谨慎使用）[3](@ref)
# 方式1 (明确列出): 
# set(SRC_LIST src/main.cpp src/cli.cpp src/parser.cpp) 
# 方式2 (自动查找，注意⚠️新增文件需重新运行 CMake):
file(GLOB SRC_LIST "src/*.cpp") # 获取 src 目录下所有 .cpp 文件[3](@ref)
file(GLOB SRC_LIST_STORAGE "src/storage/*.cpp") # 获取 storage 子目录下的 .cpp 文件
list(APPEND SRC_LIST ${SRC_LIST_STORAGE}) # 合并列表

# 生成可执行文件[3](@ref)
add_executable(mydb ${SRC_LIST})

# 如果需要链接库（例如数学库）
# target_link_libraries(mydb PUBLIC m) # 链接 libm[3](@ref)
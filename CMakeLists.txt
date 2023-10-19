# 设置cmake 最小版本
cmake_minimum_required(VERSION 3.10)

# 设置项目名称和版本
project(Boost_Projects VERSION 0.0.0.1)

# 寻找Boost包
find_package(Boost 1.83.0 REQUIRED)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})

    # 添加子目录
    add_subdirectory(client_sync)

    add_subdirectory(server_sync)

endif()
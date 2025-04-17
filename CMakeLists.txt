cmake_minimum_required(VERSION 3.10)
project(BisonFlexExample)
option(DEBUG "Enable debug mode" ON)
# 指定 c++ 20 标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(DEBUG)
    message(STATUS "Debug mode is enabled")
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
    set(CMAKE_BUILD_TYPE Debug)
else()
    message(STATUS "Release mode is enabled")
    set(CMAKE_BUILD_TYPE Release)
endif()

# 查找 Flex 和 Bison
find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)
# include_directories(${FLEX_INCLUDE_DIRS})

# 定义 Flex 和 Bison 的输出
FLEX_TARGET(Scanner
    src/scanner.l
    ${CMAKE_CURRENT_BINARY_DIR}/scanner.cpp)
BISON_TARGET(Parser
    src/parser.y
    ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp
    COMPILE_FLAGS "-t -v --debug"
    )
ADD_FLEX_BISON_DEPENDENCY(Scanner Parser)

# 包含构建目录（放置生成的文件）
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# 添加可执行文件
add_executable(a.out
    src/main.cpp
    src/driver.cpp
    ${FLEX_Scanner_OUTPUTS}
    ${BISON_Parser_OUTPUTS}
)

include(cmake/main.cmake)

# 是否开启测试（默认开启）
option(ENABLE_TESTS "Enable tests" ON)
if(ENABLE_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

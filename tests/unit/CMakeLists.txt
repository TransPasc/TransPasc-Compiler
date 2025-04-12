# 递归查找当前下所有 cpp 文件
file(GLOB_RECURSE TEST_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

MESSAGE(STATUS "Test sources: ${TEST_SOURCES}")

# 添加测试可执行文件
add_executable(MyProjectTests
    ${TEST_SOURCES}
)

target_include_directories(MyProjectTests
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# 链接主代码库和 GTest
target_link_libraries(MyProjectTests
    PRIVATE
    # MyProject::my_lib  # 假设主代码库通过 add_library 导出
    GTest::gtest_main
)

gtest_discover_tests(MyProjectTests)
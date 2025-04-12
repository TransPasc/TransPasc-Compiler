# 聚合 include/ast/*.hpp
file(GLOB_RECURSE AST_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include/ast/*.hpp")

# 定义生成的 ast.hpp 文件路径
set(AGGREGATED_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/include/ast/ast.hpp")

# 检查是否找到头文件
if(AST_HEADERS)
    # message(STATUS "Found AST headers: ${AST_HEADERS}")

    # 清空或创建 ast.hpp 文件
    file(WRITE "${AGGREGATED_HEADER}" "// Auto-generated header file aggregating all AST headers\n\n")
    # 添加头文件保护宏 pragma once
    file(APPEND "${AGGREGATED_HEADER}" "#pragma once\n\n")


    # 遍历头文件并追加到 ast.hpp
    foreach(header ${AST_HEADERS})
        file(RELATIVE_PATH REL_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/include" "${header}")
        file(APPEND "${AGGREGATED_HEADER}" "#include \"${REL_HEADER}\"\n")
    endforeach()
else()
    message(WARNING "No AST headers found in include/ast/")
    message(STATUS "Please ensure that the headers are located in the correct directory.")
endif()

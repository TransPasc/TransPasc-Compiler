# TODO: Add clang-format support
find_program(CLANG_FORMAT "clang-format")
if(CLANG_FORMAT)
    add_custom_target(format
        COMMAND ${CLANG_FORMAT} --style=file -i ${PROJECT_SOURCE_DIR}/src/*.cpp ${PROJECT_SOURCE_DIR}/include/*.hpp
        COMMENT "Formatting source code..."
    )
endif()

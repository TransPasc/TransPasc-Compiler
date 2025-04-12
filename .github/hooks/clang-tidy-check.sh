#!/bin/bash

# 生成编译数据库（需在 CMake 中启用）
if [ ! -f compile_commands.json ]; then
    echo "Generating compile_commands.json..."
    cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -B build .
    ln -s build/compile_commands.json .
fi

# 运行 clang-tidy 检查修改的文件
file=$(git diff --name-only --cached --diff-filter=AM | grep -E '\.(cpp|cxx|cc|hpp|h)$')
echo "${file}" | xargs -r clang-tidy -p build --checks=* --header-filter=.* --config-file=.clang-tidy
exit $?

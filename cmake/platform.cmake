
MESSAGE("Begin to load platform.cmake")
# 设置编译选项

# 所有平台通用规则
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)  # 不在目标系统查找可执行文件
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)    # 只在目标系统查找库文件
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)    # 只在目标系统查找头文件
# 自动检测架构
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(ARCH_TYPE x64)
else()
    set(ARCH_TYPE x86)
endif()

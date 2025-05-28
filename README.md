# KPC - King of Pascal Compiler

The King of Pascal Compiler - 世界上最好的 Pascal 编译器

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)

## 📖 项目简介

KPC (King of Pascal Compiler) 是一个现代化的 Pascal-S 语言编译器，实现了完整的编译流水线：词法分析 → 语法分析 → 语义分析 → 代码生成。项目采用 C++20 开发，基于 Flex + Bison 构建，支持多种目标代码格式输出。

### 🌟 核心特性

- **完整的编译器前端**：词法分析、语法分析、语义分析
- **多目标代码生成**：支持 C、LLVM IR、RISC-V 汇编代码生成
- **调试分析工具**：Token 序列和 AST 结构输出
- **错误处理机制**：精确的错误定位和友好的错误提示
- **现代化 CLI**：GNU 风格命令行界面，支持详细输出模式
- **跨平台支持**：Linux、macOS、Windows 多平台构建

### 🚀 支持的 Pascal-S 语言特性

#### 数据类型

- **基本类型**：`integer`、`real`、`boolean`、`char`、`string`
- **构造类型**：多维数组、函数和过程类型
- **常量定义**：整数、实数、字符、字符串、布尔常量

#### 控制结构

- **条件语句**：`if-then-else`
- **循环语句**：`while-do`、`for-to-do`
- **流程控制**：`break`、`continue`
- **程序结构**：`begin-end` 复合语句

#### 函数与过程

- **函数定义**：支持参数传递和返回值
- **过程定义**：支持引用参数和值参数
- **嵌套作用域**：多层函数嵌套和变量作用域管理

#### 输入输出

- **格式化输入**：`read()` 语句
- **格式化输出**：`write()` 语句

## 🛠️ 环境要求

### 构建依赖

- **编译器**：支持 C++20 的编译器 (GCC 10+, Clang 12+, MSVC 2019+)
- **构建系统**：CMake 3.10+
- **词法语法分析**：Flex 2.6+ 和 GNU Bison 3.0+

### 运行时依赖

- **C 编译器**：用于编译生成的 C 代码 (可选)
- **LLVM**：用于处理 LLVM IR 输出 (可选)

## 📦 安装指南

### 从源码构建

```bash
# 克隆项目
git clone https://github.com/TransPasc/TransPasc-Compiler
cd TransPasc-Compiler

# 创建构建目录
mkdir build && cd build

# 配置和构建
cmake ..
make -j$(nproc)

# 运行测试 (可选)
make test
```

### 预编译版本

前往 [GitHub Releases](https://github.com/TransPasc/TransPasc-Compiler/releases) 下载最新版本：

```bash
# 移动到系统路径
sudo mv kpc /usr/local/bin/
```

## 🎯 使用说明

### 基本语法

```bash
kpc [选项] [输入文件]
```

### 命令行参数

| 参数                  | 描述                   | 示例             |
| --------------------- | ---------------------- | ---------------- |
| `-i, --input <file>`  | 指定 Pascal 源文件路径 | `-i program.pas` |
| `-o, --output <file>` | 指定输出文件路径       | `-o program.c`   |
| `-f, --format <fmt>`  | 输出格式选择           | `-f c`           |
| `-v, --verbose`       | 启用详细输出模式       | `-v`             |
| `--version`           | 显示版本信息           | `--version`      |
| `-h, --help`          | 显示帮助信息           | `-h`             |

### 输出格式支持

| 格式      | 扩展名    | 描述                |
| --------- | --------- | ------------------- |
| `c`       | `.c`      | C 语言代码 (默认)   |
| `llvm-ir` | `.ll`     | LLVM 中间表示       |
| `risc-v`  | `.s`      | RISC-V 汇编代码     |
| `token`   | `.tokens` | 词法分析 Token 序列 |
| `ast`     | `.ast`    | 抽象语法树结构      |

## 💡 使用示例

### 基础编译流程

```bash
# 编译 Pascal 程序为 C 代码
kpc -i hello.pas

# 指定输出文件和格式
kpc -i math.pas -o math.c -f c

# 启用详细输出查看编译过程
kpc -i program.pas -f c -v
```

### 调试和分析

```bash
# 查看词法分析结果
kpc -i program.pas -f token -o program.tokens

# 查看语法树结构
kpc -i program.pas -f ast -o program.ast

# 生成 LLVM IR 用于优化
kpc -i program.pas -f llvm-ir -o program.ll
```

### 完整编译链

```bash
# Pascal → C → 可执行文件
kpc -i fibonacci.pas -f c -o fibonacci.c
gcc fibonacci.c -o fibonacci
./fibonacci
```

## 📝 Pascal 程序示例

### Hello World

```pascal
program hello;
begin
    write('Hello, World!');
end.
```

### 函数定义

```pascal
program factorial;
var
    n, result: integer;

function fact(x: integer): integer;
begin
    if x <= 1 then
        fact := 1
    else
        fact := x * fact(x - 1);
end;

begin
    read(n);
    result := fact(n);
    write(result);
end.
```

### 数组处理

```pascal
program array_demo;
var
    arr: array[1..10] of integer;
    i, sum: integer;
begin
    sum := 0;
    for i := 1 to 10 do
    begin
        read(arr[i]);
        sum := sum + arr[i];
    end;
    write(sum);
end.
```

## 🧪 运行测试

项目包含完整的测试套件，涵盖词法分析、语法分析、语义分析和代码生成的各个方面：

```bash
# 运行单元测试
cd build
make test

# 运行集成测试
python3 tests/black-box/run-gencode.py tests/black-box/generate/ build/debug_test
```

## 🔧 开发指南

### 项目结构

```
TransPasc-Compiler/
├── include/                 # 头文件目录
│   ├── ast/                # 抽象语法树定义
│   ├── cli/                # 命令行接口
│   ├── codeGenerate/       # 代码生成器
│   ├── semanticAnalysis/   # 语义分析器
│   └── symbolTable/        # 符号表管理
├── src/                    # 源代码目录
│   ├── scanner.l           # Flex 词法规则
│   ├── parser.y            # Bison 语法规则
│   └── ...
├── tests/                  # 测试用例
│   ├── unit/               # 单元测试
│   └── black-box/          # 集成测试
└── docs/                   # 项目文档
```

### 贡献代码

1. Fork 项目并创建功能分支
2. 编写代码并添加测试用例
3. 确保所有测试通过
4. 提交 Pull Request

详细开发指南请参考 [开发者文档](./docs/dev.md)。

## 📚 相关文档

- [设计报告](./docs/验收材料/设计报告.typ) - 详细的系统设计文档
- [开发者指南](./docs/dev.md) - 开发环境搭建和贡献指南

## 🤝 致谢

感谢所有为这个项目做出贡献的开发者：

- **蔡逸文** - 项目组长，词法分析、语法分析、文档撰写、架构设计
- **宋健** - 代码生成、测试、CI/CD 集成、多平台编译
- **李宇星** - 语义分析、错误处理、技术选型调研、代码提交规范

## 📄 许可证

本项目采用 MIT 许可证 - 详见 [LICENSE](LICENSE) 文件。

**KPC - 让 Pascal 编译变得简单而强大！** 🚀

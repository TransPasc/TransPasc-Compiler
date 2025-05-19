# LLVM IR 核心参考文档

## 1. 结构划分

### 1.1 模块 (Module)
- 顶层容器，包含全局变量、函数声明/定义等
- 示例结构：
  ```llvm
  ; 全局变量声明
  @global_var = global i32 0

  ; 函数声明
  declare i32 @printf(i8*, ...)

  ; 函数定义
  define i32 @main() {
    ; ...
  }
  ```

### 1.2 函数 (Function)
- 由基本块(Basic Block)组成
- 显式声明返回类型和参数类型
- 示例：
  ```llvm
  define i32 @add(i32 %a, i32 %b) {
    %sum = add i32 %a, %b
    ret i32 %sum
  }
  ```

### 1.3 基本块 (Basic Block)
- 以标签(label)开头
- 包含连续的非终止指令
- 必须以终止指令结尾
- 示例：
  ```llvm
  entry:
    %x = add i32 5, 3
    br label %next_block

  next_block:
    %y = mul i32 %x, 2
    ret i32 %y
  ```

## 2. 标识符命名规则

### 2.1 局部标识符
- 以 `%` 开头
- 两种形式：
  ```llvm
  ; 数字形式
  %1 = add i32 5, 3

  ; 命名形式（需用双引号包裹特殊字符）
  %"var@123" = alloca i32
  ```

### 2.2 全局标识符
- 以 `@` 开头
- 示例：
  ```llvm
  @global_counter = global i32 0
  @main = function void () { ... }
  ```

### 2.3 保留关键字
- 不能使用的名称：`void`, `define`, `global` 等
- 转义规则：使用双引号包裹特殊名称
  ```llvm
  @"illegal name" = global i32 0
  ```

## 3. 核心指令集

### 3.1 算术运算
| 指令 | 示例 | 说明 |
|------|------|------|
| `add` | `%sum = add i32 %a, %b` | 整数加法 |
| `fadd` | `%fsum = fadd double 3.14, %x` | 浮点加法 |
| `sub` | `%diff = sub i32 10, %b` | 整数减法 |
| `mul` | `%prod = mul i32 %a, 5` | 整数乘法 |

### 3.2 内存操作
| 指令 | 示例 | 说明 |
|------|------|------|
| `alloca` | `%ptr = alloca i32` | 栈空间分配 |
| `load` | `%val = load i32, i32* %ptr` | 从内存加载 |
| `store` | `store i32 42, i32* %ptr` | 存储到内存 |
| `getelementptr` | `%elem = getelementptr [10 x i32], [10 x i32]* %arr, i32 0, i32 2` | 计算地址偏移 |

### 3.3 控制流
| 指令 | 示例 | 说明 |
|------|------|------|
| `br` | `br i1 %cond, label %true, label %false` | 条件跳转 |
| `ret` | `ret i32 0` | 函数返回 |
| `switch` | `switch i32 %val, label %default [i32 0, label %case0 ...]` | 多分支选择 |

### 3.4 函数调用
```llvm
; 声明
declare i32 @puts(i8*)

; 调用
%str = getelementptr [13 x i8], [13 x i8]* @.str, i32 0, i32 0
call i32 @puts(i8* %str)
```

### 3.5 比较指令
| 指令 | 示例 | 说明 |
|------|------|------|
| `icmp` | `%cmp = icmp eq i32 %a, %b` | 整数比较 |
| `fcmp` | `%fcmp = fcmp olt double %x, 3.14` | 浮点比较 |

## 4. 其他关键信息

### 4.1 类型系统
| 类型 | 示例 | 说明 |
|------|------|------|
| 整数 | `i32`, `i8` | 位宽可指定 |
| 浮点 | `float`, `double` | IEEE 标准 |
| 指针 | `i32*`, `i8**` | 多级指针支持 |
| 数组 | `[10 x i32]` | 固定长度数组 |
| 结构体 | `%MyStruct = type { i32, double }` | 自定义聚合类型 |

### 4.2 SSA 形式
- 每个变量只能赋值一次
- 使用 Phi 节点处理控制流合并：
  ```llvm
  %result = phi i32 [ %a, %bb1 ], [ %b, %bb2 ]
  ```

### 4.3 元数据
```llvm
!0 = !{!"Debug Info Version", i32 3}
!1 = !{!"Dwarf Version", i32 4}
```

### 4.4 注释
```llvm
; 单行注释
/* 多行注释
   支持跨行 */
```
## 参考文档
- [Getting Started with LLVM Core Libraries（中文版）](https://getting-started-with-llvm-core-libraries-zh-cn.readthedocs.io/zh-cn/latest/)
- [LLVM IR入门指南](https://evian-zhang.github.io/llvm-ir-tutorial/)
- [LLVM IR入门指南](https://zhuanlan.zhihu.com/p/161780623)
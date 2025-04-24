# KPC

The King of Pascal Compiler

世界上最好的 Pascal 编译器

- [开发者文档](./docs/dev.md)

## How to start

去到 github release 中下载 latest, 移动到合适的目录即可

<!-- TODO: Complete it -->`

## How to use
```
# 基础编译
$ ./kpc -i demo.pas -f llvm-ir -v
[verbose] Parsing demo.pas...
[verbose] Generating LLVM IR...
Output written to: /path/to/demo.ll

# 自动路径生成
$ ./kpc -i src/test.pas -o build/
# 生成 build/test.c

# 混合使用参数
$ ./kpc -i input.pas -o output/result.ll -f llvm-ir
```
## Lexical rules

词法规则定义如下:

*注:pascal 不区分大小写*

```

```

## Grammar rules

<!-- TODO -->

## 测试

### 黑盒测试

- 编译结果正确性检验（TOKE 流、AST、中间代码、...）
- 运行结果正确性检验 (如和转换后的 c 代码共同对多分输入文件的输出进行比对)

输出规范

*注:{$var}表示变量组合，*

```
{$err-type}[{$row}][{$col}]:{$details}
```

其中, $err-type 可取 `词法错误`、`语法错误`、`语义错误`

例如

```
词法错误[10][5]: abc is a unknown word
```

#### 测试样例文件格式

测试样例文件用于做黑盒测试，`$program` 表示被测试的 Pascal 脚本

当 `$program`有错误时，`$result-list`为编译器应该输出的错误信息列表，按换行符分隔。

否则，`$result-list`应为AST 按照

```pascal
# Program
{$program}
# Result
{$result-list}
```

##### 示例

```pascal
# Program

program Test;
var
  x, y : integer;
  flag : boolean;
begin
  x := 10;
  y := x * 2;

  if y > 15 then
  begin
    flag := true;
    while x > 0 do
      x := x - 1;
  end
  else
    flag := false;
end.

# Result
// TODO: 完成其 ast 构建
{$result-list}

```

## Notice

c 中的函数和 Pascal 中的过程/函数不同，Pascal 中的可以嵌套，可以访问上一层的变量，相当于 cpp 中的闭包，转化为 c代码时需要注意

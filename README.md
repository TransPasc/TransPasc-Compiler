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

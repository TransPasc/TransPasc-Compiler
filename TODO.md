# TODO LIST

- 引入 docker, 消除环境差异
- 加入 pre-commit
- 加入 github actions CI/CD, 实现自动测试、发布
- 加入预处理器，处理 Pascal 不区分大小写的逻辑
  - **已通过在 scanner.l 中设置 %option caseless 实现**
- 补充 README 中的词法规则(需和 scanner.l 对应)
- 补充 README 中的文法规则(需和 parser.y 对应)
- 添加各种各样的，正确/错误测试样例
- 添加自测程序/脚本
- 编译器支持单独输出 TOKEN 流、AST、语义信息、符号表 等
- 增加 arg parse, 来控制编译器的行为(如 -h --version -o)
- 对 TOKEN 流、AST、语义信息、符号表 等支持黑盒测试
- 引入日志库

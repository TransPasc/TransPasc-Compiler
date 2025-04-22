# TODO LIST

- 引入 docker, 消除环境差异
- 加入 github actions CI/CD, 实现发布
- 补充 README 中的词法规则(需和 scanner.l 对应)
- 补充 README 中的文法规则(需和 parser.y 对应)
- 添加各种各样的，正确/错误测试样例
- 添加自测程序/脚本
- 编译器支持单独输出 TOKEN 流、AST、语义信息、符号表 等
- 增加 arg parse, 来控制编译器的行为(如 -h --version -o)
- 对 TOKEN 流、AST、语义信息、符号表 等支持黑盒测试
- 引入日志库
- fix: 输出的ast 的 location 信息错误
- fix: 部分语法未能正确识别
- 优化 AST 的输出
- 完善 ast:exception类
- 添加 ERROR 类：词法错误、语法错误、语义错误（需要细分）
- 分离声明和实现
- 添加 TRUE 、 FALSE 的 TOKEN

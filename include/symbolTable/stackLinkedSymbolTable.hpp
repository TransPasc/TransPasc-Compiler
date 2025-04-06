#pragma once
#include "symbolTable/table.hpp"

namespace XYZ {
// 栈 链 式符号表
class StackLinkedSymbolTable : public SymbolTable {
 public:
  StackLinkedSymbolTable() : SymbolTable() {};
  ~StackLinkedSymbolTable() override = default;

  bool insert(const SymbolName &symbol_name,
              std::unique_ptr<SymbolRecord> new_record) override;
  bool remove(const SymbolName &name) override;
  bool update(const SymbolName &name, SymbolRecord *record) override;
  SymbolRecord *lookup(const SymbolName &name) override;

  size_t size() const override;
  void clear() override;

  //   进入新块
  void enterBlock() override;  // 添加 override 关键字
  //   退出当前块
  void exitBlock() override;  // 添加 override 关键字

 private:
  //   块索引表，记录每个块的起始位置
  std::vector<int32_t> blockIndex;
  //   栈式符号表，存储符号记录
  std::vector<std::shared_ptr<SymbolRecord>> symbolTable;
  //   栈顶指针，指向当前栈顶位置
  int32_t top = -1;
  //   当前块索引，指示当前作用域块
  int32_t currentBlockIndex = -1;
};
}  // namespace XYZ
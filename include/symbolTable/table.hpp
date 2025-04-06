#pragma once
#include <memory>
#include <string>
#include <vector>

namespace XYZ {
class SymbolRecord;  // 前向声明 符号记录类，存储例如： lineno, type, value,
                     // etc.
// SymbolTable 基类
class SymbolTable {
 public:
  using SymbolType = std::string;

 public:
  SymbolTable() = default;
  virtual ~SymbolTable() = default;
  // 插入符号
  virtual bool insert(const SymbolType &name, SymbolRecord *record) = 0;
  // 删除符号
  virtual bool remove(const SymbolType &name) = 0;
  //   update符号
  virtual bool update(const SymbolType &name, SymbolRecord *record) = 0;
  // 查找符号
  virtual SymbolRecord *lookup(const SymbolType &name) = 0;

  virtual size_t size() const = 0;
  // 清空符号表
  virtual void clear() = 0;
};
// 栈 链 式符号表
class StackLinkedSymbolTable : public SymbolTable {
 public:
  StackLinkedSymbolTable() : SymbolTable() {};
  ~StackLinkedSymbolTable() override = default;

  bool insert(const SymbolType &name, SymbolRecord *record) override;
  bool remove(const SymbolType &name) override;
  bool update(const SymbolType &name, SymbolRecord *record) override;
  SymbolRecord *lookup(const SymbolType &name) override;

  size_t size() const override;
  void clear() override;

  //   进入新块
  void enterBlock();
  //   退出当前块
  void exitBlock();

 private:
  //   块索引表
  std::vector<int32_t> blockIndex;
  //   栈式符号表
  std::vector<std::shared_ptr<SymbolRecord>> symbolTable;
  //   栈顶指针
  int32_t top = -1;
  //   当前块索引
  int32_t currentBlockIndex = -1;
  //   当前块级别
  int32_t currentBlockLevel = -1;
};
}  // namespace XYZ
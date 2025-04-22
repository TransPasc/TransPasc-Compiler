#pragma once
#include "symbolTable/exception.hpp"
#include "symbolTable/record.hpp" // 添加此行以包含 SymbolRecord 的定义
#include "symbolTable/table.hpp"
#include "symbolTable/type.hpp"
#include <memory>
#include <stack>
#include <tuple>
#include <vector>

namespace XYZ {
// 栈 链 式符号表
class StackLinkedSymbolTable : public SymbolTable {
  using ErrType = XYZ::SymbolTableException::ErrorType;
  const static size_t HASH_TABLE_SIZE = (1 << 20); // hash表大小

public:
  StackLinkedSymbolTable() : SymbolTable() {
    // 初始化hash表
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
      hashTable[i] = -1;
    }
  };
  ~StackLinkedSymbolTable() override = default;
  /**
   * @brief 插入符号记录
   * @attention 你需要确保在插入之前，有 block。 若没有，请先调用 enterBlock
   * @param symbol_name
   * @param new_record
   */
  void insert(std::unique_ptr<SymbolRecord> new_record) override {
    if (new_record == nullptr) {
      throw XYZ::SymbolTableException(ErrType::NullPointer);
    }

    if (blockIndex.empty()) {
      throw XYZ::SymbolTableException(ErrType::NoDefaultBlock);
    }

    auto symbol_name = new_record->getName();
    auto hash_val = hash(symbol_name);
    //   如果hash表中已经存在
    auto [record, idx] = _lookup(symbol_name);
    if (record != nullptr) {
      //   如果在当前块中已经存在
      if (idx >= blockIndex.top()) {
        throw XYZ::SymbolTableException(ErrType::SymbolAlreadyExists,
                                        symbol_name);
      }
    }

    new_record->setNext(hashTable[hash_val]);
    symbolTable.push_back(std::move(new_record));

    hashTable[hash_val] = static_cast<int32_t>(symbolTable.size() - 1);
  }
  // 暂不支持手动删除
  bool remove(const SymbolName &name) override {
    throw XYZ::SymbolTableException(ErrType::UnsupportedOperation);
  }
  // 暂不支持更新
  bool update(std::shared_ptr<SymbolRecord> record) override {
    throw XYZ::SymbolTableException(ErrType::UnsupportedOperation);
  }
  //   查找
  std::shared_ptr<SymbolRecord> lookup(const SymbolName &name) override {
    auto [record, idx] = _lookup(name);
    return record;
  }

  size_t size() const override { return symbolTable.size(); }
  void clear() override {
    symbolTable.clear();
    while (!blockIndex.empty())
      blockIndex.pop();

    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
      hashTable[i] = -1;
    }
  }

  //   进入新块
  void enterBlock() override {
    blockIndex.push(static_cast<int32_t>(symbolTable.size()));
  }
  //   退出当前块
  void exitBlock() override {
    if (blockIndex.empty()) {
      throw XYZ::SymbolTableException(ErrType::NoDefaultBlock);
    }
    auto block_start = blockIndex.top();
    blockIndex.pop();
    //   删除当前块的所有符号
    while (symbolTable.size() > block_start) {
      auto record = symbolTable.back();
      if (record != nullptr) {
        //   删除符号
        symbolTable.pop_back();
        //   更新hash表
        hashTable[hash(record->getName())] = record->getNext();
      }
    }
  }

private:
  /**
   * @brief 查找符号
   * @param name
   * @return (符号表中找到的符号记录，索引)
   */
  std::tuple<std::shared_ptr<SymbolRecord>, int32_t>
  _lookup(const SymbolName &name) const {
    auto index = hashTable[hash(name)];
    //   如果hash表中没有找到
    if (index < 0 || index >= symbolTable.size()) {
      return {nullptr, -1};
    }
    //   如果hash表中有值，遍历链表
    while (index != -1) {
      auto record = symbolTable[index];
      //   如果找到，返回
      if (record->getName() == name) {
        return {record, index};
      }
      index = record->getNext();
    }
    return {nullptr, -1};
  }
  size_t hash(const SymbolName &name) const { return hash_pjw(name); }
  inline size_t hash_pjw(const SymbolName &name) const {
    unsigned int val = 0, i = 0;
    for (const auto ch : name) {
      val = (val << 2) + ch;
      if ((i = val & HASH_TABLE_SIZE)) {
        val = (val ^= (i >> 12)) & HASH_TABLE_SIZE;
      }
    }
    return val % HASH_TABLE_SIZE;
  }

private:
  //   块索引表，记录每个块的起始位置
  std::stack<int32_t> blockIndex;
  //   栈式符号表，存储符号记录
  std::vector<std::shared_ptr<SymbolRecord>> symbolTable;
  //   hash表
  int32_t hashTable[HASH_TABLE_SIZE];
};
} // namespace XYZ

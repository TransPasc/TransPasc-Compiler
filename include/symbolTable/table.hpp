#pragma once
#include <memory>
#include <string>
#include <vector>

#include "symbolTable/exception.hpp"

namespace XYZ {
class SymbolRecord; // 前向声明 符号记录类，存储例如： lineno, type, value,
                    // etc.
/**
 * @class SymbolTable
 * @brief 符号表抽象接口，可支持多级作用域管理（默认不支持）
 */
class SymbolTable {
public:
  using SymbolName = std::string;

public:
  SymbolTable() = default;
  virtual ~SymbolTable() = default;
  /**
   * @brief 在当前作用域插入符号
   * @param symbol_name 符号名称
   * @param new_record 符号记录对象（移交所有权）
   */
  virtual bool insert(std::unique_ptr<SymbolRecord> new_record) = 0;
  // 删除符号
  virtual bool remove(const SymbolName &name) = 0;
  //   update符号
  virtual bool update(std::shared_ptr<SymbolRecord> record) = 0;
  // 查找符号
  virtual std::shared_ptr<SymbolRecord> lookup(const SymbolName &name) = 0;

  virtual auto size() const -> size_t = 0;
  // 清空符号表
  virtual void clear() = 0;
  //   进入新块, 默认不实现
  virtual void enterBlock() {
    throw SymbolTableException(
        SymbolTableException::ErrorType::UnsupportedOperation,
        "enterBlock() not implemented");
  }
  //   退出当前块, 默认不实现
  virtual void exitBlock() {
    throw SymbolTableException(
        SymbolTableException::ErrorType::UnsupportedOperation,
        "exitBlock() not implemented");
  }
};
} // namespace XYZ

#pragma once
#include <memory>
#include <string>
#include <vector>

#include "symbolTable/exception.hpp"
#include "symbolTable/type.hpp"

namespace XYZ {
/**
 * @brief 符号记录基类，存储符号元数据
 */

class SymbolRecord {
  // 存储符号类型
  SymbolType *m_type = nullptr;
  // 地址
  int32_t m_address = -1;
  // 声明行号
  int32_t m_lineno = -1;
  // 引用行号
  std::vector<int32_t> m_ref_lineno;
  // 链域
  SymbolRecord *m_next = nullptr;

 public:
  virtual ~SymbolRecord() = default;
};
}  // namespace XYZ
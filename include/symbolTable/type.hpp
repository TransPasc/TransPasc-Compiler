#pragma once
#include <memory>
#include <string>
#include <vector>

#include "symbolTable/exception.hpp"

namespace XYZ {
/**
 * @class SymbolType
 * @brief 符号类型基类，存储符号类型信息
 */
class SymbolType {
 public:
  SymbolType() = default;

  ~SymbolType() = default;
};
}  // namespace XYZ
#pragma once
#include <exception>
#include <string>

namespace XYZ {
/**
 * @brief 自定义符号表异常类
 * 1. 不支持该功能 error
 * 2. 符号表已满 error
 * 3. 符号表为空 error
 * 4. 符号表中不存在该符号 error
 * 5. 符号表中已存在该符号 error
 * 6. 没有默认 block
 */
class SymbolTableException : public std::exception {
public:
  // 错误类型枚举
  enum class ErrorType {
    UnsupportedOperation,
    TableOverflow,
    TableUnderflow,
    SymbolNotFound,
    SymbolAlreadyExists,
    NoDefaultBlock,
    NotInCurrentBlock,
    NullPointer
  };

  // 构造函数，支持错误类型和自定义消息
  SymbolTableException(ErrorType type, const std::string &message = "")
      : m_type(type), m_message(message) {}

  // 获取错误消息
  const char *what() const noexcept override {
    m_cache = type2str(m_type);
    if (!m_message.empty()) {
      m_cache += m_message;
    }
    return m_cache.c_str();
  }

  // 获取错误类型
  ErrorType type() const noexcept { return m_type; }

private:
  std::string type2str(const ErrorType &type) const {
    switch (m_type) {
    case ErrorType::UnsupportedOperation:
      return "Unsupported operation: ";
    case ErrorType::TableOverflow:
      return "Table overflow: ";
    case ErrorType::TableUnderflow:
      return "Table underflow: ";
    case ErrorType::SymbolNotFound:
      return "Symbol not found: ";
    case ErrorType::SymbolAlreadyExists:
      return "Symbol already exists: ";
    case ErrorType::NoDefaultBlock:
      return "No default block: ";
    case ErrorType::NotInCurrentBlock:
      return "Not in current block: ";
    case ErrorType::NullPointer:
      return "Null pointer: ";
    default:
      return "Unknown error: ";
    }
  }

private:
  ErrorType m_type;      // 错误类型
  std::string m_message; // 错误消息
                         //   cache
  mutable std::string m_cache;
};
} // namespace XYZ

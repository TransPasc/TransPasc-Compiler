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
 */
class SymbolTableException : public std::exception {
 public:
  // 错误类型枚举
  enum class ErrorType {
    UnsupportedOperation,
    TableOverflow,
    TableUnderflow,
    SymbolNotFound,
    SymbolAlreadyExists
  };

  // 构造函数，支持错误类型和自定义消息
  SymbolTableException(ErrorType type, const std::string &message)
      : m_type(type), m_message(message) {}

  // 获取错误消息
  const char *what() const noexcept override { return m_message.c_str(); }

  // 获取错误类型
  ErrorType type() const noexcept { return m_type; }

 private:
  ErrorType m_type;       // 错误类型
  std::string m_message;  // 错误消息
};
}  // namespace XYZ
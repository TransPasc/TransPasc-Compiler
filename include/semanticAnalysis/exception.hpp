#pragma once
#include <exception>
#include <string>

namespace XYZ {
/**
 * @brief 自定义异常类，用于语义分析中的错误处理
 * 1. 不支持的语法规则
 */
class SemanticException : public std::exception {
public:
  // 错误类型枚举
  enum class ErrorType {
    UNDEFINED,    // 不支持的语法
    UNSUPPORTED,  // 不支持的特性
    REDEFINITION, // 重定义
  };

  // 构造函数，支持错误类型和自定义消息
  SemanticException(ErrorType type, const std::string &message = "")
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
    switch (type) {
    case ErrorType::UNDEFINED:
      return "Undefined error: ";
    case ErrorType::UNSUPPORTED:
      return "Unsupported error: ";
    case ErrorType::REDEFINITION:
      return "Redefinition error: ";
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

#pragma once
#include <exception>
#include <string>

namespace XYZ {
/**
 * @brief 自定义异常类，用于代码生成中的错误处理
 */
class CodeGenerateException : public std::exception {
public:
  // 错误类型枚举
  enum class ErrorCode {
    INVALID_INPUT,    // 无效输入
    FILE_NOT_FOUND,   // 文件未找到
    PARSE_ERROR,      // 解析错误
    GENERATION_ERROR, // 生成错误
    UNKNOWN_ERROR     // 未知错误
  };

  // 构造函数，支持错误代码和自定义消息
  CodeGenerateException(ErrorCode code, const std::string &message = "")
      : m_code(code), m_message(message) {}

  // 获取错误消息
  const char *what() const noexcept override {
    m_cache = code2str(m_code);
    if (!m_message.empty()) {
      m_cache += m_message;
    }
    return m_cache.c_str();
  }

  // 获取错误代码
  ErrorCode code() const noexcept { return m_code; }

private:
  std::string code2str(const ErrorCode &code) const {
    switch (code) {
    case ErrorCode::INVALID_INPUT:
      return "Invalid input error: ";
    case ErrorCode::FILE_NOT_FOUND:
      return "File not found error: ";
    case ErrorCode::PARSE_ERROR:
      return "Parse error: ";
    case ErrorCode::GENERATION_ERROR:
      return "Generation error: ";
    case ErrorCode::UNKNOWN_ERROR:
      return "Unknown error: ";
    default:
      return "Unrecognized error: ";
    }
  }

private:
  ErrorCode m_code;            // 错误代码
  std::string m_message;       // 错误消息
  mutable std::string m_cache; // 缓存
};
} // namespace XYZ

#pragma once
#include <exception>
#include <string>
namespace XYZ {
// 自定义 exception 类
class CompileError : public std::exception {
  // 词法错误、语法错误、语义错误
  enum class Type { LexicalError, SyntaxError, SemanticError };

public:
  CompileError(Type type, unsigned int line, const std::string &message = "")
      : m_type(type), m_message(message), m_line(line) {}
  const char *what() const noexcept override {
    msg_cache = "(" + std::to_string(m_line) + ")[" + getTypeString() + "] " +
                m_message;
    return msg_cache.c_str();
  }

private:
  // 获取错误类型的字符串表示
  std::string getTypeString() const {
    switch (m_type) {
    case Type::SyntaxError:
      return "Syntax Error";
    case Type::SemanticError:
      return "Semantic Error";
    case Type::LexicalError:
      return "Lexical Error";
    default:
      return "Unknown Error";
    }
  }

private:
  Type m_type;
  std::string m_message;
  mutable std::string msg_cache;
  unsigned int m_line;
};
} // namespace XYZ

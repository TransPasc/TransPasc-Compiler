#pragma once
#include "ast/ast.h"
#include "ast/visitor.h"

namespace XYZ {
enum class State {
  NORMAL,       // 普通状态
  FunctionCall, // 处于函数调用 <function_name>(...)
  FunctionDef,  // 处于函数定义 <function_name>(<params>) {...}
  IdVarPart,    // 处于数组下标 <array_name>[...]
  Scanf,        // 处于 scanf(<format_string>, ...)
  Printf,       // 处于 printf(<format_string>, ...)
};
/**
 * @brief code generator virtual base class
 * from the AST
 */
class Generator : public ASTVisitor {

public:
  Generator() = default;
  virtual ~Generator() = default;

  virtual void generateCode(ASTNode::ASTNodePtr m_root) = 0;
  virtual void setOutputFile(const std::string &filename) = 0;
};
} // namespace XYZ

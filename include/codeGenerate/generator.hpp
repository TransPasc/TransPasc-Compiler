#pragma once
#include "ast/ast.h"
#include "ast/visitor.h"

namespace XYZ {
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

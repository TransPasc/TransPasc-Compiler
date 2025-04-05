#pragma once
#include <memory>

#include "ast/ast.h"
namespace XYZ {
class ASTNode;
class ASTVisitor {
 public:
  ASTVisitor() = default;
  virtual ~ASTVisitor() = default;
  virtual void visit(std::shared_ptr<ASTNode> node) {
    // Default implementation
  };
};
}  // namespace XYZ
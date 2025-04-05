#pragma once
#include "visitor.h"

namespace XYZ {
class ASTNode {
 public:
  ASTNode() = default;
  virtual ~ASTNode() = default;
  virtual void accept(ASTVisitor &visitor) {
    visitor.visit(std::shared_ptr<ASTNode>(this));
  };
};
// 终结符节点
class TerminalNode : public ASTNode {
 public:
  TerminalNode() = default;
  ~TerminalNode() override = default;
  void accept(ASTVisitor &visitor) override {
    visitor.visit(std::shared_ptr<TerminalNode>(this));
  };
};
// 非终结符节点
class NonTerminalNode : public ASTNode {
 public:
  NonTerminalNode() = default;
  ~NonTerminalNode() override = default;
  void accept(ASTVisitor &visitor) override {
    visitor.visit(std::shared_ptr<NonTerminalNode>(this));
  };
};
}  // namespace XYZ
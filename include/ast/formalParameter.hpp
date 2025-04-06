#pragma once
#include "ast/ast.h"

namespace XYZ {

// formal_parameter 的基类
// formal_parameter := EMPTY
class FormalParameterNode : public ASTNode {
 public:
  FormalParameterNode(size_t line) : ASTNode("FormalParameter", line) {}
  ~FormalParameterNode() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }
};

// formal_parameter := LPAREN parameter_list RPAREN
class FormalParameterNode_Lparen_ParameterList_Rparen
    : public FormalParameterNode {
 public:
  FormalParameterNode_Lparen_ParameterList_Rparen(ASTNodePtr lparen,
                                                  ASTNodePtr parameterList,
                                                  ASTNodePtr rparen,
                                                  size_t line)
      : FormalParameterNode(line) {
    addChild(lparen);
    addChild(parameterList);
    addChild(rparen);
  }
  ~FormalParameterNode_Lparen_ParameterList_Rparen() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }

  ASTNodePtr getLparen() const { return m_children[0]; }
  ASTNodePtr getParameterList() const { return m_children[1]; }
  ASTNodePtr getRparen() const { return m_children[2]; }
};

}  // namespace XYZ

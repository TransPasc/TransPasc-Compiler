#pragma once
#include "ast/ast.h"

namespace XYZ {

// parameter_list 的基类
class ParameterListNode : public ASTNode {
 public:
  ParameterListNode(size_t line) : ASTNode("ParameterList", line) {}
  ~ParameterListNode() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }
};

// parameter_list := parameter
class ParameterListNode_Parameter : public ParameterListNode {
 public:
  ParameterListNode_Parameter(ASTNodePtr parameter, size_t line)
      : ParameterListNode(line) {
    addChild(parameter);
  }
  ~ParameterListNode_Parameter() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }

  ASTNodePtr getParameter() const { return m_children[0]; }
};

// parameter_list := parameter_list SEMICOLON parameter
class ParameterListNode_ParameterList_Semicolon_Parameter
    : public ParameterListNode {
 public:
  ParameterListNode_ParameterList_Semicolon_Parameter(ASTNodePtr parameterList,
                                                      ASTNodePtr semicolon,
                                                      ASTNodePtr parameter,
                                                      size_t line)
      : ParameterListNode(line) {
    addChild(parameterList);
    addChild(semicolon);
    addChild(parameter);
  }
  ~ParameterListNode_ParameterList_Semicolon_Parameter() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }

  ASTNodePtr getParameterList() const { return m_children[0]; }
  ASTNodePtr getSemicolon() const { return m_children[1]; }
  ASTNodePtr getParameter() const { return m_children[2]; }
};

}  // namespace XYZ

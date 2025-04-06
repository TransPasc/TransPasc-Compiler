#pragma once
#include "ast/ast.h"

namespace XYZ {

// variable_list 的基类
class VariableListNode : public ASTNode {
 public:
  VariableListNode(size_t line) : ASTNode("VariableList", line) {}
  ~VariableListNode() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }
};

// variable_list := variable
class VariableListNode_Variable : public VariableListNode {
 public:
  VariableListNode_Variable(ASTNodePtr variable, size_t line)
      : VariableListNode(line) {
    addChild(variable);
  }
  ~VariableListNode_Variable() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }

  ASTNodePtr getVariable() const { return m_children[0]; }
};

// variable_list := variable_list COMMA variable
class VariableListNode_VariableList_Comma_Variable : public VariableListNode {
 public:
  VariableListNode_VariableList_Comma_Variable(ASTNodePtr variableList,
                                               ASTNodePtr comma,
                                               ASTNodePtr variable, size_t line)
      : VariableListNode(line) {
    addChild(variableList);
    addChild(comma);
    addChild(variable);
  }
  ~VariableListNode_VariableList_Comma_Variable() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }

  ASTNodePtr getVariableList() const { return m_children[0]; }
  ASTNodePtr getComma() const { return m_children[1]; }
  ASTNodePtr getVariable() const { return m_children[2]; }
};

}  // namespace XYZ

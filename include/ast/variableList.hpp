#pragma once
#include "ast/ast.h"

namespace XYZ {

// variable_list 的基类
class VariableListNode : public ASTNode {
public:
  VariableListNode(size_t line) : ASTNode("VariableList", line) {}
  ~VariableListNode() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }

  // 获取变量列表的类型
  virtual std::vector<std::shared_ptr<SymbolType>> getTypeList() const = 0;
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

  std::shared_ptr<VariableNode> getVariable() const {
    return std::dynamic_pointer_cast<VariableNode>(m_children[0]);
  }
  virtual std::vector<std::shared_ptr<SymbolType>>
  getTypeList() const override {
    auto variable = getVariable();
    auto type = variable->getType();
    return {type};
  }
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

  std::shared_ptr<VariableListNode> getVariableList() const {
    return std::dynamic_pointer_cast<VariableListNode>(m_children[0]);
  }
  std::shared_ptr<TerminalNode> getComma() const {
    return std::dynamic_pointer_cast<TerminalNode>(m_children[1]);
  }
  std::shared_ptr<VariableNode> getVariable() const {
    return std::dynamic_pointer_cast<VariableNode>(m_children[2]);
  }

  /**
   * @brief 获取变量列表的类型
   * @return std::vector<std::shared_ptr<SymbolType>>
   */
  virtual std::vector<std::shared_ptr<SymbolType>>
  getTypeList() const override {
    auto variableList = getVariableList();
    auto variable = getVariable();
    auto type = variable->getType();
    auto typeList = variableList->getTypeList();
    typeList.push_back(type);
    return typeList;
  }
};

} // namespace XYZ

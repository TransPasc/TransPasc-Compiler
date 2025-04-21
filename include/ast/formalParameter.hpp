#pragma once
#include "ast/ast.h"
#include "ast/parameterList.hpp"
#include "ast/terminal.hpp"
#include "symbolTable/type.hpp"
namespace XYZ {

// formal_parameter 的基类
// formal_parameter := EMPTY
class FormalParameterNode : public ASTNode {
public:
  FormalParameterNode(size_t line) : ASTNode("FormalParameter", line) {}
  ~FormalParameterNode() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }
  virtual SymbolType::ParamsType getParams() const {
    // 默认实现返回空参数列表
    return SymbolType::ParamsType{};
  }
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

  shared_ptr<TerminalNode> getLparen() const {
    return dynamic_pointer_cast<TerminalNode>(m_children[0]);
  }
  shared_ptr<ParameterListNode> getParameterList() const {
    return dynamic_pointer_cast<ParameterListNode>(m_children[1]);
  }
  shared_ptr<TerminalNode> getRparen() const {
    return dynamic_pointer_cast<TerminalNode>(m_children[2]);
  }

  SymbolType::ParamsType getParams() const override {
    return getParameterList()->getParams();
  }
};

} // namespace XYZ

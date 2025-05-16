#pragma once
#include "ast/ast.h"
#include "ast/parameter.hpp"
namespace XYZ {

// parameter_list 的基类
class ParameterListNode : public ASTNode {
public:
  ParameterListNode(size_t line) : ASTNode("ParameterList", line) {}
  ~ParameterListNode() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }
  virtual SymbolType::ParamsType getParams() const {
    return {};
  }
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

  std::shared_ptr<ParameterNode> getParameter() const {
    return dynamic_pointer_cast<ParameterNode>(m_children[0]);
  }
  SymbolType::ParamsType getParams() const override {
    return getParameter()->getParams();
  }
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

  std::shared_ptr<ParameterListNode> getParameterList() const {
    return dynamic_pointer_cast<ParameterListNode>(m_children[0]);
  }
  std::shared_ptr<TerminalNode> getSemicolon() const {
    return dynamic_pointer_cast<TerminalNode>(m_children[1]);
  }
  std::shared_ptr<ParameterNode> getParameter() const {
    return dynamic_pointer_cast<ParameterNode>(m_children[2]);
  }
  SymbolType::ParamsType getParams() const override {
    auto params = getParameterList()->getParams();
    auto another = getParameter()->getParams();
    params.insert(params.end(), another.begin(), another.end());
    return params;
  }
};

} // namespace XYZ

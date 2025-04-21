#pragma once
#include "ast/ast.h"
#include "ast/valueParameter.hpp"
#include "ast/varParameter.hpp"
#include "symbolTable/type.hpp"
namespace XYZ {

// parameter 的基类
class ParameterNode : public ASTNode {
public:
  ParameterNode(size_t line) : ASTNode("Parameter", line) {}
  ~ParameterNode() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }
  virtual SymbolType::ParamsType getParams() const = 0;
};

// parameter := var_parameter
class ParameterNode_VarParameter : public ParameterNode {
public:
  ParameterNode_VarParameter(ASTNodePtr varParameter, size_t line)
      : ParameterNode(line) {
    addChild(varParameter);
  }
  ~ParameterNode_VarParameter() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }

  shared_ptr<VarParameterNode> getVarParameter() const {
    return dynamic_pointer_cast<VarParameterNode>(m_children[0]);
  }
  SymbolType::ParamsType getParams() const override {
    return getVarParameter()->getParams();
  }
};

// parameter := value_parameter
class ParameterNode_ValueParameter : public ParameterNode {
public:
  ParameterNode_ValueParameter(ASTNodePtr valueParameter, size_t line)
      : ParameterNode(line) {
    addChild(valueParameter);
  }
  ~ParameterNode_ValueParameter() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }

  shared_ptr<ValueParameterNode> getValueParameter() const {
    return dynamic_pointer_cast<ValueParameterNode>(m_children[0]);
  }

  SymbolType::ParamsType getParams() const override {
    return getValueParameter()->getParams();
  }
};

} // namespace XYZ

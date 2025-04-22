#pragma once
#include "ast/ast.h"
#include "ast/valueParameter.hpp"
#include "symbolTable/type.hpp"

namespace XYZ {

// var_parameter 的基类
class VarParameterNode : public ASTNode {
public:
  VarParameterNode(size_t line) : ASTNode("VarParameter", line) {}
  ~VarParameterNode() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }
  virtual SymbolType::ParamsType getParams() const = 0;
};

// var_parameter := VAR value_parameter
class VarParameterNode_Var_ValueParameter : public VarParameterNode {
public:
  VarParameterNode_Var_ValueParameter(ASTNodePtr varToken,
                                      ASTNodePtr valueParameter, size_t line)
      : VarParameterNode(line) {
    addChild(varToken);
    addChild(valueParameter);
  }
  ~VarParameterNode_Var_ValueParameter() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }

  std::shared_ptr<TerminalNode> getVarToken() const {
    return dynamic_pointer_cast<TerminalNode>(m_children[0]);
  }
  std::shared_ptr<ValueParameterNode> getValueParameter() const {
    return dynamic_pointer_cast<ValueParameterNode>(m_children[1]);
  }
  SymbolType::ParamsType getParams() const override {
    auto params = getValueParameter()->getParams();
    for (auto &param : params) {
      // 将值参数转换为引用参数
      param->first.set_ref(true);
    }
    return params;
  }
};

} // namespace XYZ

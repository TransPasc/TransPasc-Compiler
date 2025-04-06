#pragma once
#include "ast/ast.h"

namespace XYZ {

// var_parameter 的基类
class VarParameterNode : public ASTNode {
 public:
  VarParameterNode(size_t line) : ASTNode("VarParameter", line) {}
  ~VarParameterNode() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }
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

  ASTNodePtr getVarToken() const { return m_children[0]; }
  ASTNodePtr getValueParameter() const { return m_children[1]; }
};

}  // namespace XYZ

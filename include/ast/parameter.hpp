#pragma once
#include "ast/ast.h"

namespace XYZ {

// parameter 的基类
class ParameterNode : public ASTNode {
 public:
  ParameterNode(size_t line) : ASTNode("Parameter", line) {}
  ~ParameterNode() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }
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

  ASTNodePtr getVarParameter() const { return m_children[0]; }
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

  ASTNodePtr getValueParameter() const { return m_children[0]; }
};

}  // namespace XYZ

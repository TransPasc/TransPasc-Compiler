#pragma once
#include "ast/ast.h"
#include "ast/idVarpart.hpp"
#include "symbolTable/type.hpp"

namespace XYZ {

// variable 的基类
class VariableNode : public ASTNode {
public:
  VariableNode(size_t line) : ASTNode("Variable", line) {}
  ~VariableNode() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }
  virtual std::shared_ptr<SymbolType> getType() const = 0;
};

// variable := ID id_varpart
class VariableNode_Id_IdVarpart : public VariableNode {
  SymbolType valType;
  std::vector<std::pair<int32_t, int32_t>> ranges;

public:
  VariableNode_Id_IdVarpart(ASTNodePtr id, ASTNodePtr idVarpart, size_t line)
      : VariableNode(line) {
    addChild(id);
    addChild(idVarpart);
  }
  ~VariableNode_Id_IdVarpart() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }

  std::shared_ptr<TerminalNode> getId() const {
    return dynamic_pointer_cast<TerminalNode>(m_children[0]);
  }
  std::shared_ptr<IdVarPartNode> getIdVarpart() const {
    return dynamic_pointer_cast<IdVarPartNode>(m_children[1]);
  }
  void setValType(const SymbolType &type) { valType = type; }
  virtual std::shared_ptr<SymbolType> getType() const override {
    return std::make_shared<SymbolType>(valType);
  }
  virtual std::vector<std::pair<int32_t, int32_t>> getRanges() const {
    return ranges;
  }
  virtual void setRanges(const std::vector<std::pair<int32_t, int32_t>> &r) {
    ranges = r;
  }
};

} // namespace XYZ

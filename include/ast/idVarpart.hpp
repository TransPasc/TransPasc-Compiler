#pragma once
#include "ast/ast.h"
#include "ast/expressionList.hpp"
#include "symbolTable/type.hpp"
#include <memory>
#include <vector>

namespace XYZ {

// id_varpart 的基类
class IdVarPartNode : public ASTNode {
public:
  IdVarPartNode(size_t line);
  ~IdVarPartNode() override;

  void accept(ASTVisitor &visitor) override;
  virtual std::vector<std::shared_ptr<SymbolType>> getTypeList() const;
};

// id_varpart := LBRACKET expression_list RBRACKET
class IdVarPartNode_Lbracket_ExpressionList_Rbracket : public IdVarPartNode {
public:
  IdVarPartNode_Lbracket_ExpressionList_Rbracket(ASTNodePtr lbracket,
                                                 ASTNodePtr expression_list,
                                                 ASTNodePtr rbracket,
                                                 size_t line);
  ~IdVarPartNode_Lbracket_ExpressionList_Rbracket() override;

  void accept(ASTVisitor &visitor) override;

  std::shared_ptr<TerminalNode> getLbracket() const;
  std::shared_ptr<ExpressionListNode> getExpressionList() const;
  std::shared_ptr<TerminalNode> getRbracket() const;
  virtual std::vector<std::shared_ptr<SymbolType>> getTypeList() const override;
};

} // namespace XYZ

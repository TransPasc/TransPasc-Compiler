#pragma once

#include "ast/ast.h"
#include "ast/factor.hpp"
#include "ast/terminal.hpp"
#include "symbolTable/type.hpp"
#include <cassert>
#include <memory>

namespace XYZ {

// term 的基类
class TermNode : public ASTNode {
public:
  TermNode(size_t line);
  ~TermNode() override;

  void accept(ASTVisitor &visitor) override;
  virtual std::shared_ptr<SymbolType> getType() const = 0;
};

// term := factor
class TermNode_Factor : public TermNode {
public:
  TermNode_Factor(ASTNodePtr factor, size_t line);
  ~TermNode_Factor() override;

  void accept(ASTVisitor &visitor) override;

  std::shared_ptr<FactorNode> getFactor() const;
  virtual std::shared_ptr<SymbolType> getType() const override;
};

// term := term MULOP factor
class TermNode_Term_Mulop_Factor : public TermNode {
public:
  TermNode_Term_Mulop_Factor(ASTNodePtr term, ASTNodePtr mulop,
                             ASTNodePtr factor, size_t line);
  ~TermNode_Term_Mulop_Factor() override;

  void accept(ASTVisitor &visitor) override;

  std::shared_ptr<TermNode> getTerm() const;
  std::shared_ptr<TerminalNode> getMulop() const;
  std::shared_ptr<FactorNode> getFactor() const;
  virtual std::shared_ptr<SymbolType> getType() const override;
};

} // namespace XYZ

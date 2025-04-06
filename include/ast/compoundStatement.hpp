#pragma once
#include "ast/ast.h"

namespace XYZ {

// compound_statement 的基类
class CompoundStatementNode : public ASTNode {
 public:
  CompoundStatementNode(size_t line) : ASTNode("CompoundStatement", line) {}
  ~CompoundStatementNode() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }
};

// compound_statement := BEGIN statement_list END
class CompoundStatementNode_Begin_StatementList_End
    : public CompoundStatementNode {
 public:
  CompoundStatementNode_Begin_StatementList_End(ASTNodePtr beginToken,
                                                ASTNodePtr statementList,
                                                ASTNodePtr endToken,
                                                size_t line)
      : CompoundStatementNode(line) {
    addChild(beginToken);
    addChild(statementList);
    addChild(endToken);
  }
  ~CompoundStatementNode_Begin_StatementList_End() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }

  ASTNodePtr getBeginToken() const { return m_children[0]; }
  ASTNodePtr getStatementList() const { return m_children[1]; }
  ASTNodePtr getEndToken() const { return m_children[2]; }
};

}  // namespace XYZ

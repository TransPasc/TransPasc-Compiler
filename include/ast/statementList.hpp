#pragma once
#include "ast/ast.h"

namespace XYZ {

// statement_list 的基类
class StatementListNode : public ASTNode {
 public:
  StatementListNode(size_t line) : ASTNode("StatementList", line) {}
  ~StatementListNode() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }
};

// statement_list := statement
class StatementListNode_Statement : public StatementListNode {
 public:
  StatementListNode_Statement(ASTNodePtr statement, size_t line)
      : StatementListNode(line) {
    addChild(statement);
  }
  ~StatementListNode_Statement() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }

  ASTNodePtr getStatement() const { return m_children[0]; }
};

// statement_list := statement_list SEMICOLON statement
class StatementListNode_StatementList_Semicolon_Statement
    : public StatementListNode {
 public:
  StatementListNode_StatementList_Semicolon_Statement(ASTNodePtr statementList,
                                                      ASTNodePtr semicolon,
                                                      ASTNodePtr statement,
                                                      size_t line)
      : StatementListNode(line) {
    addChild(statementList);
    addChild(semicolon);
    addChild(statement);
  }
  ~StatementListNode_StatementList_Semicolon_Statement() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }

  ASTNodePtr getStatementList() const { return m_children[0]; }
  ASTNodePtr getSemicolon() const { return m_children[1]; }
  ASTNodePtr getStatement() const { return m_children[2]; }
};

}  // namespace XYZ

#pragma once
#include "ast/ast.h"

namespace XYZ {

// procedure_call 的基类
class ProcedureCallNode : public ASTNode {
public:
  ProcedureCallNode(size_t line) : ASTNode("ProcedureCall", line) {}
  ~ProcedureCallNode() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }
};

// procedure_call := ID
class ProcedureCallNode_Id : public ProcedureCallNode {
public:
  ProcedureCallNode_Id(ASTNodePtr id, size_t line) : ProcedureCallNode(line) {
    addChild(id);
  }
  ~ProcedureCallNode_Id() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }

  std::shared_ptr<TerminalNode> getId() const {
    return dynamic_pointer_cast<TerminalNode>(m_children[0]);
  }
};
// procedure_call := ID LPAREN RPAREN
class ProcedureCallNode_Id_Lparen_Rparen : public ProcedureCallNode {
public:
  ProcedureCallNode_Id_Lparen_Rparen(ASTNodePtr id, ASTNodePtr lparen,
                                     ASTNodePtr rparen, size_t line)
      : ProcedureCallNode(line) {
    addChild(id, lparen, rparen);
  }
  ~ProcedureCallNode_Id_Lparen_Rparen() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }

  std::shared_ptr<TerminalNode> getId() const {
    return dynamic_pointer_cast<TerminalNode>(m_children[0]);
  }
  std::shared_ptr<TerminalNode> getLparen() const {
    return dynamic_pointer_cast<TerminalNode>(m_children[1]);
  }
  std::shared_ptr<TerminalNode> getRparen() const {
    return dynamic_pointer_cast<TerminalNode>(m_children[2]);
  }
};

// procedure_call := ID LPAREN expression_list RPAREN
class ProcedureCallNode_Id_Lparen_ExpressionList_Rparen
    : public ProcedureCallNode {
public:
  ProcedureCallNode_Id_Lparen_ExpressionList_Rparen(ASTNodePtr id,
                                                    ASTNodePtr lparen,
                                                    ASTNodePtr expressionList,
                                                    ASTNodePtr rparen,
                                                    size_t line)
      : ProcedureCallNode(line) {
    addChild(id, lparen, expressionList, rparen);
  }
  ~ProcedureCallNode_Id_Lparen_ExpressionList_Rparen() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }

  std::shared_ptr<TerminalNode> getId() const {
    return dynamic_pointer_cast<TerminalNode>(m_children[0]);
  }
  std::shared_ptr<TerminalNode> getLparen() const {
    return dynamic_pointer_cast<TerminalNode>(m_children[1]);
  }
  std::shared_ptr<ExpressionListNode> getExpressionList() const {
    return dynamic_pointer_cast<ExpressionListNode>(m_children[2]);
  }
  std::shared_ptr<TerminalNode> getRparen() const {
    return dynamic_pointer_cast<TerminalNode>(m_children[3]);
  }
};

} // namespace XYZ

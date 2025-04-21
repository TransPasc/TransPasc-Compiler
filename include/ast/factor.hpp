#pragma once
#include "ast/ast.h"

namespace XYZ {

// factor 的基类
class FactorNode : public ASTNode {
public:
  FactorNode(size_t line) : ASTNode("Factor", line) {}
  ~FactorNode() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }
};

// factor := NUMBER
class FactorNode_Number : public FactorNode {
public:
  FactorNode_Number(ASTNodePtr number, size_t line) : FactorNode(line) {
    addChild(number);
  }
  ~FactorNode_Number() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }

  shared_ptr<TerminalNode> getNumber() const {
    return dynamic_pointer_cast<TerminalNode>(m_children[0]);
  }
};

// factor := CHAR_LITERAL
class FactorNode_CharLiteral : public FactorNode {
public:
  FactorNode_CharLiteral(ASTNodePtr charLiteral, size_t line)
      : FactorNode(line) {
    addChild(charLiteral);
  }
  ~FactorNode_CharLiteral() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }

  shared_ptr<TerminalNode> getCharLiteral() const {
    return dynamic_pointer_cast<TerminalNode>(m_children[0]);
  }
};

// factor := variable
class FactorNode_Variable : public FactorNode {
public:
  FactorNode_Variable(ASTNodePtr variable, size_t line) : FactorNode(line) {
    addChild(variable);
  }
  ~FactorNode_Variable() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }

  shared_ptr<VariableNode> getVariable() const {
    return dynamic_pointer_cast<VariableNode>(m_children[0]);
  }
};

// factor := LPAREN expression RPAREN
class FactorNode_Lparen_Expression_Rparen : public FactorNode {
public:
  FactorNode_Lparen_Expression_Rparen(ASTNodePtr lparen, ASTNodePtr expression,
                                      ASTNodePtr rparen, size_t line)
      : FactorNode(line) {
    addChild(lparen);
    addChild(expression);
    addChild(rparen);
  }
  ~FactorNode_Lparen_Expression_Rparen() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }

  shared_ptr<TerminalNode> getLparen() const {
    return dynamic_pointer_cast<TerminalNode>(m_children[0]);
  }
  shared_ptr<ExpressionNode> getExpression() const {
    return dynamic_pointer_cast<ExpressionNode>(m_children[1]);
  }
  shared_ptr<TerminalNode> getRparen() const {
    return dynamic_pointer_cast<TerminalNode>(m_children[2]);
  }
};

// factor := NOT factor
class FactorNode_Not_Factor : public FactorNode {
public:
  FactorNode_Not_Factor(ASTNodePtr notToken, ASTNodePtr factor, size_t line)
      : FactorNode(line) {
    addChild(notToken);
    addChild(factor);
  }
  ~FactorNode_Not_Factor() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }

  shared_ptr<TerminalNode> getNotToken() const {
    return dynamic_pointer_cast<TerminalNode>(m_children[0]);
  }
  shared_ptr<FactorNode> getFactor() const {
    return dynamic_pointer_cast<FactorNode>(m_children[1]);
  }
};

// factor := MINUS factor
class FactorNode_Minus_Factor : public FactorNode {
public:
  FactorNode_Minus_Factor(ASTNodePtr minus, ASTNodePtr factor, size_t line)
      : FactorNode(line) {
    addChild(minus);
    addChild(factor);
  }
  ~FactorNode_Minus_Factor() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }

  shared_ptr<TerminalNode> getMinus() const {
    return dynamic_pointer_cast<TerminalNode>(m_children[0]);
  }
  shared_ptr<FactorNode> getFactor() const {
    return dynamic_pointer_cast<FactorNode>(m_children[1]);
  }
};
// factor := ID LPAREN expression_list RPAREN
class FactorNode_ID_Lparen_ExpressionList_Rparen : public FactorNode {
public:
  FactorNode_ID_Lparen_ExpressionList_Rparen(ASTNodePtr id, ASTNodePtr lparen,
                                             ASTNodePtr expressionList,
                                             ASTNodePtr rparen, size_t line)

      : FactorNode(line) {
    addChild(id);
    addChild(lparen);
    addChild(expressionList);
    addChild(rparen);
  }
  ~FactorNode_ID_Lparen_ExpressionList_Rparen() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }

  shared_ptr<TerminalNode> getID() const {
    return dynamic_pointer_cast<TerminalNode>(m_children[0]);
  }
  shared_ptr<ExpressionListNode> getExpressionList() const {
    return dynamic_pointer_cast<ExpressionListNode>(m_children[2]);
  }
};
} // namespace XYZ

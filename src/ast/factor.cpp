#include "ast/factor.hpp"
#include <cassert>

namespace XYZ {

// FactorNode
FactorNode::FactorNode(size_t line) : ASTNode("Factor", line) {}
FactorNode::~FactorNode() = default;
void FactorNode::accept(ASTVisitor &visitor) { visitor.visit(*this); }

// FactorNode_Number
FactorNode_Number::FactorNode_Number(ASTNodePtr number, size_t line)
    : FactorNode(line) {
  addChild(number);
}
FactorNode_Number::~FactorNode_Number() = default;
void FactorNode_Number::accept(ASTVisitor &visitor) { visitor.visit(*this); }
std::shared_ptr<TerminalNode> FactorNode_Number::getNumber() const {
  return dynamic_pointer_cast<TerminalNode>(m_children[0]);
}
std::shared_ptr<SymbolType> FactorNode_Number::getType() const {
  return std::make_shared<SymbolType>(
      SymbolType::MakeBasic(BasicType::INTEGER));
}

// FactorNode_CharLiteral
FactorNode_CharLiteral::FactorNode_CharLiteral(ASTNodePtr charLiteral,
                                               size_t line)
    : FactorNode(line) {
  addChild(charLiteral);
}
FactorNode_CharLiteral::~FactorNode_CharLiteral() = default;
void FactorNode_CharLiteral::accept(ASTVisitor &visitor) {
  visitor.visit(*this);
}
std::shared_ptr<TerminalNode> FactorNode_CharLiteral::getCharLiteral() const {
  return dynamic_pointer_cast<TerminalNode>(m_children[0]);
}
std::shared_ptr<SymbolType> FactorNode_CharLiteral::getType() const {
  return std::make_shared<SymbolType>(SymbolType::MakeBasic(BasicType::CHAR));
}

// FactorNode_Variable
FactorNode_Variable::FactorNode_Variable(ASTNodePtr variable, size_t line)
    : FactorNode(line) {
  addChild(variable);
}
FactorNode_Variable::~FactorNode_Variable() = default;
void FactorNode_Variable::accept(ASTVisitor &visitor) { visitor.visit(*this); }
std::shared_ptr<VariableNode> FactorNode_Variable::getVariable() const {
  return dynamic_pointer_cast<VariableNode>(m_children[0]);
}
std::shared_ptr<SymbolType> FactorNode_Variable::getType() const {
  return getVariable()->getType();
}

// FactorNode_Lparen_Expression_Rparen
FactorNode_Lparen_Expression_Rparen::FactorNode_Lparen_Expression_Rparen(
    ASTNodePtr lparen, ASTNodePtr expression, ASTNodePtr rparen, size_t line)
    : FactorNode(line) {
  addChild(lparen);
  addChild(expression);
  addChild(rparen);
}
FactorNode_Lparen_Expression_Rparen::~FactorNode_Lparen_Expression_Rparen() =
    default;
void FactorNode_Lparen_Expression_Rparen::accept(ASTVisitor &visitor) {
  visitor.visit(*this);
}
std::shared_ptr<TerminalNode>
FactorNode_Lparen_Expression_Rparen::getLparen() const {
  return dynamic_pointer_cast<TerminalNode>(m_children[0]);
}
std::shared_ptr<ExpressionNode>
FactorNode_Lparen_Expression_Rparen::getExpression() const {
  return dynamic_pointer_cast<ExpressionNode>(m_children[1]);
}
std::shared_ptr<TerminalNode>
FactorNode_Lparen_Expression_Rparen::getRparen() const {
  return dynamic_pointer_cast<TerminalNode>(m_children[2]);
}
std::shared_ptr<SymbolType>
FactorNode_Lparen_Expression_Rparen::getType() const {
  return getExpression()->getType();
}

// FactorNode_Not_Factor
FactorNode_Not_Factor::FactorNode_Not_Factor(ASTNodePtr notToken,
                                             ASTNodePtr factor, size_t line)
    : FactorNode(line) {
  addChild(notToken);
  addChild(factor);
}
FactorNode_Not_Factor::~FactorNode_Not_Factor() = default;
void FactorNode_Not_Factor::accept(ASTVisitor &visitor) {
  visitor.visit(*this);
}
std::shared_ptr<TerminalNode> FactorNode_Not_Factor::getNotToken() const {
  return dynamic_pointer_cast<TerminalNode>(m_children[0]);
}
std::shared_ptr<FactorNode> FactorNode_Not_Factor::getFactor() const {
  return dynamic_pointer_cast<FactorNode>(m_children[1]);
}
std::shared_ptr<SymbolType> FactorNode_Not_Factor::getType() const {
  auto type = getFactor()->getType();
  assert(type->strictEq(BasicType::BOOLEAN));
  return type;
}

// FactorNode_Minus_Factor
FactorNode_Minus_Factor::FactorNode_Minus_Factor(ASTNodePtr minus,
                                                 ASTNodePtr factor, size_t line)
    : FactorNode(line) {
  addChild(minus);
  addChild(factor);
}
FactorNode_Minus_Factor::~FactorNode_Minus_Factor() = default;
void FactorNode_Minus_Factor::accept(ASTVisitor &visitor) {
  visitor.visit(*this);
}
std::shared_ptr<TerminalNode> FactorNode_Minus_Factor::getMinus() const {
  return dynamic_pointer_cast<TerminalNode>(m_children[0]);
}
std::shared_ptr<FactorNode> FactorNode_Minus_Factor::getFactor() const {
  return dynamic_pointer_cast<FactorNode>(m_children[1]);
}
std::shared_ptr<SymbolType> FactorNode_Minus_Factor::getType() const {
  auto type = getFactor()->getType();
  assert(type->strictEq(BasicType::INTEGER) || type->strictEq(BasicType::REAL));
  return type;
}

// FactorNode_ID_Lparen_ExpressionList_Rparen
FactorNode_ID_Lparen_ExpressionList_Rparen::
    FactorNode_ID_Lparen_ExpressionList_Rparen(ASTNodePtr id, ASTNodePtr lparen,
                                               ASTNodePtr expressionList,
                                               ASTNodePtr rparen, size_t line)
    : FactorNode(line) {
  addChild(id);
  addChild(lparen);
  addChild(expressionList);
  addChild(rparen);
}
FactorNode_ID_Lparen_ExpressionList_Rparen::
    ~FactorNode_ID_Lparen_ExpressionList_Rparen() = default;
void FactorNode_ID_Lparen_ExpressionList_Rparen::accept(ASTVisitor &visitor) {
  visitor.visit(*this);
}
std::shared_ptr<TerminalNode>
FactorNode_ID_Lparen_ExpressionList_Rparen::getID() const {
  return dynamic_pointer_cast<TerminalNode>(m_children[0]);
}
std::shared_ptr<ExpressionListNode>
FactorNode_ID_Lparen_ExpressionList_Rparen::getExpressionList() const {
  return dynamic_pointer_cast<ExpressionListNode>(m_children[2]);
}
void FactorNode_ID_Lparen_ExpressionList_Rparen::setRetType(
    const SymbolType &type) {
  retType = type;
}
std::shared_ptr<SymbolType>
FactorNode_ID_Lparen_ExpressionList_Rparen::getType() const {
  return std::make_shared<SymbolType>(retType);
}

} // namespace XYZ

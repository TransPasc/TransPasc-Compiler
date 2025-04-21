#include "ast/simpleExpression.hpp"
#include <cassert>

namespace XYZ {

// SimpleExpressionNode
SimpleExpressionNode::SimpleExpressionNode(size_t line)
    : ASTNode("SimpleExpression", line) {}
SimpleExpressionNode::~SimpleExpressionNode() = default;
void SimpleExpressionNode::accept(ASTVisitor &visitor) { visitor.visit(*this); }

// SimpleExpressionNode_Term
SimpleExpressionNode_Term::SimpleExpressionNode_Term(ASTNodePtr term,
                                                     size_t line)
    : SimpleExpressionNode(line) {
  addChild(term);
}
SimpleExpressionNode_Term::~SimpleExpressionNode_Term() = default;
void SimpleExpressionNode_Term::accept(ASTVisitor &visitor) {
  visitor.visit(*this);
}
std::shared_ptr<TermNode> SimpleExpressionNode_Term::getTerm() const {
  return dynamic_pointer_cast<TermNode>(m_children[0]);
}
std::shared_ptr<SymbolType> SimpleExpressionNode_Term::getType() const {
  return getTerm()->getType();
}

// SimpleExpressionNode_SimpleExpression_Plus_Term
SimpleExpressionNode_SimpleExpression_Plus_Term::
    SimpleExpressionNode_SimpleExpression_Plus_Term(ASTNodePtr simpleExpression,
                                                    ASTNodePtr plus,
                                                    ASTNodePtr term,
                                                    size_t line)
    : SimpleExpressionNode(line) {
  addChild(simpleExpression);
  addChild(plus);
  addChild(term);
}
SimpleExpressionNode_SimpleExpression_Plus_Term::
    ~SimpleExpressionNode_SimpleExpression_Plus_Term() = default;
void SimpleExpressionNode_SimpleExpression_Plus_Term::accept(
    ASTVisitor &visitor) {
  visitor.visit(*this);
}
std::shared_ptr<SimpleExpressionNode>
SimpleExpressionNode_SimpleExpression_Plus_Term::getSimpleExpression() const {
  return dynamic_pointer_cast<SimpleExpressionNode>(m_children[0]);
}
std::shared_ptr<TerminalNode>
SimpleExpressionNode_SimpleExpression_Plus_Term::getPlus() const {
  return dynamic_pointer_cast<TerminalNode>(m_children[1]);
}
std::shared_ptr<TermNode>
SimpleExpressionNode_SimpleExpression_Plus_Term::getTerm() const {
  return dynamic_pointer_cast<TermNode>(m_children[2]);
}
std::shared_ptr<SymbolType>
SimpleExpressionNode_SimpleExpression_Plus_Term::getType() const {
  auto type1 = getSimpleExpression()->getType();
  auto type2 = getTerm()->getType();
  assert(type1->strictEq(*type2));
  return type1;
}

// SimpleExpressionNode_SimpleExpression_Minus_Term
SimpleExpressionNode_SimpleExpression_Minus_Term::
    SimpleExpressionNode_SimpleExpression_Minus_Term(
        ASTNodePtr simpleExpression, ASTNodePtr minus, ASTNodePtr term,
        size_t line)
    : SimpleExpressionNode(line) {
  addChild(simpleExpression);
  addChild(minus);
  addChild(term);
}
SimpleExpressionNode_SimpleExpression_Minus_Term::
    ~SimpleExpressionNode_SimpleExpression_Minus_Term() = default;
void SimpleExpressionNode_SimpleExpression_Minus_Term::accept(
    ASTVisitor &visitor) {
  visitor.visit(*this);
}
std::shared_ptr<SimpleExpressionNode>
SimpleExpressionNode_SimpleExpression_Minus_Term::getSimpleExpression() const {
  return dynamic_pointer_cast<SimpleExpressionNode>(m_children[0]);
}
std::shared_ptr<TerminalNode>
SimpleExpressionNode_SimpleExpression_Minus_Term::getMinus() const {
  return dynamic_pointer_cast<TerminalNode>(m_children[1]);
}
std::shared_ptr<TermNode>
SimpleExpressionNode_SimpleExpression_Minus_Term::getTerm() const {
  return dynamic_pointer_cast<TermNode>(m_children[2]);
}
std::shared_ptr<SymbolType>
SimpleExpressionNode_SimpleExpression_Minus_Term::getType() const {
  auto type1 = getSimpleExpression()->getType();
  auto type2 = getTerm()->getType();
  assert(type1->strictEq(*type2));
  return type1;
}

// SimpleExpressionNode_SimpleExpression_Or_Term
SimpleExpressionNode_SimpleExpression_Or_Term::
    SimpleExpressionNode_SimpleExpression_Or_Term(ASTNodePtr simpleExpression,
                                                  ASTNodePtr orToken,
                                                  ASTNodePtr term, size_t line)
    : SimpleExpressionNode(line) {
  addChild(simpleExpression);
  addChild(orToken);
  addChild(term);
}
SimpleExpressionNode_SimpleExpression_Or_Term::
    ~SimpleExpressionNode_SimpleExpression_Or_Term() = default;
void SimpleExpressionNode_SimpleExpression_Or_Term::accept(
    ASTVisitor &visitor) {
  visitor.visit(*this);
}
std::shared_ptr<SimpleExpressionNode>
SimpleExpressionNode_SimpleExpression_Or_Term::getSimpleExpression() const {
  return dynamic_pointer_cast<SimpleExpressionNode>(m_children[0]);
}
std::shared_ptr<TerminalNode>
SimpleExpressionNode_SimpleExpression_Or_Term::getOrToken() const {
  return dynamic_pointer_cast<TerminalNode>(m_children[1]);
}
std::shared_ptr<TermNode>
SimpleExpressionNode_SimpleExpression_Or_Term::getTerm() const {
  return dynamic_pointer_cast<TermNode>(m_children[2]);
}
std::shared_ptr<SymbolType>
SimpleExpressionNode_SimpleExpression_Or_Term::getType() const {
  auto type1 = getSimpleExpression()->getType();
  auto type2 = getTerm()->getType();
  assert(type1->strictEq(*type2));
  return type1;
}

} // namespace XYZ

#include "ast/expression.hpp"

namespace XYZ {

// ExpressionNode 实现
ExpressionNode::ExpressionNode(size_t line) : ASTNode("Expression", line) {}
ExpressionNode::~ExpressionNode() = default;

void ExpressionNode::accept(ASTVisitor &visitor) { visitor.visit(*this); }

// ExpressionNode_SimpleExpression 实现
ExpressionNode_SimpleExpression::ExpressionNode_SimpleExpression(
    ASTNodePtr simpleExpression, size_t line)
    : ExpressionNode(line) {
  addChild(simpleExpression);
}

ExpressionNode_SimpleExpression::~ExpressionNode_SimpleExpression() = default;

void ExpressionNode_SimpleExpression::accept(ASTVisitor &visitor) {
  visitor.visit(*this);
}

std::shared_ptr<SimpleExpressionNode>
ExpressionNode_SimpleExpression::getSimpleExpression() const {
  return dynamic_pointer_cast<SimpleExpressionNode>(m_children[0]);
}

std::shared_ptr<SymbolType> ExpressionNode_SimpleExpression::getType() const {
  return getSimpleExpression()->getType();
}

// ExpressionNode_SimpleExpression_Relop_SimpleExpression 实现
ExpressionNode_SimpleExpression_Relop_SimpleExpression::
    ExpressionNode_SimpleExpression_Relop_SimpleExpression(
        ASTNodePtr simpleExpression1, ASTNodePtr relop,
        ASTNodePtr simpleExpression2, size_t line)
    : ExpressionNode(line) {
  addChild(simpleExpression1);
  addChild(relop);
  addChild(simpleExpression2);
}

ExpressionNode_SimpleExpression_Relop_SimpleExpression::
    ~ExpressionNode_SimpleExpression_Relop_SimpleExpression() = default;

void ExpressionNode_SimpleExpression_Relop_SimpleExpression::accept(
    ASTVisitor &visitor) {
  visitor.visit(*this);
}

std::shared_ptr<SimpleExpressionNode>
ExpressionNode_SimpleExpression_Relop_SimpleExpression::getSimpleExpression1()
    const {
  return dynamic_pointer_cast<SimpleExpressionNode>(m_children[0]);
}

std::shared_ptr<TerminalNode>
ExpressionNode_SimpleExpression_Relop_SimpleExpression::getRelop() const {
  return dynamic_pointer_cast<TerminalNode>(m_children[1]);
}

std::shared_ptr<SimpleExpressionNode>
ExpressionNode_SimpleExpression_Relop_SimpleExpression::getSimpleExpression2()
    const {
  return dynamic_pointer_cast<SimpleExpressionNode>(m_children[2]);
}

std::shared_ptr<SymbolType>
ExpressionNode_SimpleExpression_Relop_SimpleExpression::getType() const {
  auto type = SymbolType::MakeBasic(BasicType::BOOLEAN);
  return std::make_shared<SymbolType>(type);
}

} // namespace XYZ

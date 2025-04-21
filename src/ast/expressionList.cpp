#include "ast/expressionList.hpp"

namespace XYZ {

// ExpressionListNode
ExpressionListNode::ExpressionListNode(size_t line)
    : ASTNode("ExpressionList", line) {}

void ExpressionListNode::accept(ASTVisitor &visitor) { visitor.visit(*this); }

// ExpressionListNode_Expression
ExpressionListNode_Expression::ExpressionListNode_Expression(
    ASTNodePtr expression, size_t line)
    : ExpressionListNode(line) {
  addChild(expression);
}

void ExpressionListNode_Expression::accept(ASTVisitor &visitor) {
  visitor.visit(*this);
}

std::shared_ptr<ExpressionNode>
ExpressionListNode_Expression::getExpression() const {
  return std::dynamic_pointer_cast<ExpressionNode>(m_children[0]);
}

std::vector<std::shared_ptr<SymbolType>>
ExpressionListNode_Expression::getTypeList() const {
  return {getExpression()->getType()};
}

// ExpressionListNode_ExpressionList_Comma_Expression
ExpressionListNode_ExpressionList_Comma_Expression::
    ExpressionListNode_ExpressionList_Comma_Expression(
        ASTNodePtr expressionList, ASTNodePtr comma, ASTNodePtr expression,
        size_t line)
    : ExpressionListNode(line) {
  addChild(expressionList);
  addChild(comma);
  addChild(expression);
}

void ExpressionListNode_ExpressionList_Comma_Expression::accept(
    ASTVisitor &visitor) {
  visitor.visit(*this);
}

std::shared_ptr<ExpressionListNode>
ExpressionListNode_ExpressionList_Comma_Expression::getExpressionList() const {
  return std::dynamic_pointer_cast<ExpressionListNode>(m_children[0]);
}

std::shared_ptr<TerminalNode>
ExpressionListNode_ExpressionList_Comma_Expression::getComma() const {
  return std::dynamic_pointer_cast<TerminalNode>(m_children[1]);
}

std::shared_ptr<ExpressionNode>
ExpressionListNode_ExpressionList_Comma_Expression::getExpression() const {
  return std::dynamic_pointer_cast<ExpressionNode>(m_children[2]);
}

std::vector<std::shared_ptr<SymbolType>>
ExpressionListNode_ExpressionList_Comma_Expression::getTypeList() const {
  auto typeList = getExpressionList()->getTypeList();
  auto expressionType = getExpression()->getType();
  typeList.push_back(expressionType);
  return typeList;
}

} // namespace XYZ

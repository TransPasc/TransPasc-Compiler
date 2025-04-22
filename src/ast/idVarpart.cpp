#include "ast/idVarpart.hpp"
#include "ast/ast.h"

namespace XYZ {

// IdVarPartNode 实现
IdVarPartNode::IdVarPartNode(size_t line) : ASTNode("IdVarPart", line) {}
IdVarPartNode::~IdVarPartNode() = default;

void IdVarPartNode::accept(ASTVisitor &visitor) { visitor.visit(*this); }

std::vector<std::shared_ptr<SymbolType>> IdVarPartNode::getTypeList() const {
  return {};
}

// IdVarPartNode_Lbracket_ExpressionList_Rbracket 实现
IdVarPartNode_Lbracket_ExpressionList_Rbracket::
    IdVarPartNode_Lbracket_ExpressionList_Rbracket(ASTNodePtr lbracket,
                                                   ASTNodePtr expression_list,
                                                   ASTNodePtr rbracket,
                                                   size_t line)
    : IdVarPartNode(line) {
  addChild(lbracket);
  addChild(expression_list);
  addChild(rbracket);
}

IdVarPartNode_Lbracket_ExpressionList_Rbracket::
    ~IdVarPartNode_Lbracket_ExpressionList_Rbracket() = default;

void IdVarPartNode_Lbracket_ExpressionList_Rbracket::accept(
    ASTVisitor &visitor) {
  visitor.visit(*this);
}

std::shared_ptr<TerminalNode>
IdVarPartNode_Lbracket_ExpressionList_Rbracket::getLbracket() const {
  return dynamic_pointer_cast<TerminalNode>(m_children[0]);
}

std::shared_ptr<ExpressionListNode>
IdVarPartNode_Lbracket_ExpressionList_Rbracket::getExpressionList() const {
  return dynamic_pointer_cast<ExpressionListNode>(m_children[1]);
}

std::shared_ptr<TerminalNode>
IdVarPartNode_Lbracket_ExpressionList_Rbracket::getRbracket() const {
  return dynamic_pointer_cast<TerminalNode>(m_children[2]);
}

std::vector<std::shared_ptr<SymbolType>>
IdVarPartNode_Lbracket_ExpressionList_Rbracket::getTypeList() const {
  return getExpressionList()->getTypeList();
}

} // namespace XYZ

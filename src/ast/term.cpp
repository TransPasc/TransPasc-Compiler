#include "ast/term.hpp"

namespace XYZ {

// TermNode
TermNode::TermNode(size_t line) : ASTNode("Term", line) {}
TermNode::~TermNode() = default;

void TermNode::accept(ASTVisitor &visitor) { visitor.visit(*this); }

// TermNode_Factor
TermNode_Factor::TermNode_Factor(ASTNodePtr factor, size_t line)
    : TermNode(line) {
  addChild(factor);
}
TermNode_Factor::~TermNode_Factor() = default;

void TermNode_Factor::accept(ASTVisitor &visitor) { visitor.visit(*this); }

std::shared_ptr<FactorNode> TermNode_Factor::getFactor() const {
  return static_pointer_cast<FactorNode>(m_children[0]);
}

std::shared_ptr<SymbolType> TermNode_Factor::getType() const {
  return getFactor()->getType();
}

// TermNode_Term_Mulop_Factor
TermNode_Term_Mulop_Factor::TermNode_Term_Mulop_Factor(ASTNodePtr term,
                                                       ASTNodePtr mulop,
                                                       ASTNodePtr factor,
                                                       size_t line)
    : TermNode(line) {
  addChild(term);
  addChild(mulop);
  addChild(factor);
}
TermNode_Term_Mulop_Factor::~TermNode_Term_Mulop_Factor() = default;

void TermNode_Term_Mulop_Factor::accept(ASTVisitor &visitor) {
  visitor.visit(*this);
}

std::shared_ptr<TermNode> TermNode_Term_Mulop_Factor::getTerm() const {
  return static_pointer_cast<TermNode>(m_children[0]);
}

std::shared_ptr<TerminalNode> TermNode_Term_Mulop_Factor::getMulop() const {
  return static_pointer_cast<TerminalNode>(m_children[1]);
}

std::shared_ptr<FactorNode> TermNode_Term_Mulop_Factor::getFactor() const {
  return static_pointer_cast<FactorNode>(m_children[2]);
}

std::shared_ptr<SymbolType> TermNode_Term_Mulop_Factor::getType() const {
  auto termType = getTerm()->getType();
  auto factorType = getFactor()->getType();
  assert(termType->strictEq(*factorType));
  return termType;
}

} // namespace XYZ

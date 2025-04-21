#pragma once
#include "ast/ast.h"
#include "ast/terminal.hpp"
namespace XYZ {
using namespace std;
// const_decl 的基类
class ConstDeclNode : public ASTNode {
public:
  ConstDeclNode(size_t line) : ASTNode("ConstDecl", line) {}
  ~ConstDeclNode() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }
};

// const_decl := ID Relop const_val SEMICOLON
class ConstDeclNode_Id_Relop_ConstVal_Semicolon : public ConstDeclNode {
public:
  ConstDeclNode_Id_Relop_ConstVal_Semicolon(ASTNodePtr id, ASTNodePtr relop,
                                            ASTNodePtr constVal,
                                            ASTNodePtr semicolon, size_t line)
      : ConstDeclNode(line) {
    dynamic_pointer_cast<TerminalNode>(relop)->expect_str("=");

    addChild(id);
    addChild(relop);
    addChild(constVal);
    addChild(semicolon);
  }
  ~ConstDeclNode_Id_Relop_ConstVal_Semicolon() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }

  std::shared_ptr<TerminalNode> getId() const {
    return dynamic_pointer_cast<TerminalNode>(m_children[0]);
  }
  std::shared_ptr<TerminalNode> getAssignop() const {
    return dynamic_pointer_cast<TerminalNode>(m_children[1]);
  }
  std::shared_ptr<ConstValNode> getConstVal() const {
    return dynamic_pointer_cast<ConstValNode>(m_children[2]);
  }
};

// const_decl := const_decl ID Relop const_val SEMICOLON
class ConstDeclNode_ConstDecl_Id_Relop_ConstVal_Semicolon
    : public ConstDeclNode {
public:
  ConstDeclNode_ConstDecl_Id_Relop_ConstVal_Semicolon(
      ASTNodePtr constDecl, ASTNodePtr id, ASTNodePtr relop,
      ASTNodePtr constVal, ASTNodePtr semicolon, size_t line)
      : ConstDeclNode(line) {

    dynamic_pointer_cast<TerminalNode>(relop)->expect_str("=");

    addChild(constDecl);
    addChild(id);
    addChild(relop);
    addChild(constVal);
    addChild(semicolon);
  }
  ~ConstDeclNode_ConstDecl_Id_Relop_ConstVal_Semicolon() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }

  std::shared_ptr<ConstDeclNode> getConstDecl() const {
    return dynamic_pointer_cast<ConstDeclNode>(m_children[0]);
  }

  std::shared_ptr<TerminalNode> getId() const {
    return dynamic_pointer_cast<TerminalNode>(m_children[1]);
  }
  std::shared_ptr<TerminalNode> getAssignop() const {
    return dynamic_pointer_cast<TerminalNode>(m_children[2]);
  }
  std::shared_ptr<ConstValNode> getConstVal() const {
    return dynamic_pointer_cast<ConstValNode>(m_children[3]);
  }
  std::shared_ptr<TerminalNode> getSemicolon() const {
    return dynamic_pointer_cast<TerminalNode>(m_children[4]);
  }
};

} // namespace XYZ

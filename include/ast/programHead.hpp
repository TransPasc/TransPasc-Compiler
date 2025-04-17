#pragma once
#include "ast/ast.h"

namespace XYZ {

// program_head 的基类
class ProgramHeadNode : public ASTNode {
public:
  ProgramHeadNode(size_t line) : ASTNode("ProgramHead", line) {}
  ~ProgramHeadNode() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }
};

// program_head := PROGRAM ID LPAREN idlist RPAREN
class ProgramHeadNode_Program_Id_Lparen_Idlist_Rparen : public ProgramHeadNode {
public:
  ProgramHeadNode_Program_Id_Lparen_Idlist_Rparen(
      ASTNodePtr program, ASTNodePtr id, ASTNodePtr lparen, ASTNodePtr idlist,
      ASTNodePtr rparen, size_t line)
      : ProgramHeadNode(line) {
    addChild(program);
    addChild(id);
    addChild(lparen);
    addChild(idlist);
    addChild(rparen);
  }
  ~ProgramHeadNode_Program_Id_Lparen_Idlist_Rparen() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }

  ASTNodePtr getProgram() const { return m_children[0]; }
  ASTNodePtr getId() const { return m_children[1]; }
  ASTNodePtr getLparen() const { return m_children[2]; }
  ASTNodePtr getIdlist() const { return m_children[3]; }
  ASTNodePtr getRparen() const { return m_children[4]; }
};

// program_head := PROGRAM ID
class ProgramHeadNode_Program_Id : public ProgramHeadNode {
public:
  ProgramHeadNode_Program_Id(ASTNodePtr program, ASTNodePtr id, size_t line)
      : ProgramHeadNode(line) {
    addChild(program);
    addChild(id);
  }
  ~ProgramHeadNode_Program_Id() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }

  shared_ptr<TerminalNode> getProgram() const {
    return static_pointer_cast<TerminalNode>(m_children[0]);
  }
  shared_ptr<TerminalNode> getId() const {
    return static_pointer_cast<TerminalNode>(m_children[1]);
  }
};

} // namespace XYZ

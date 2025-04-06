#pragma once
#include "ast/ast.h"

namespace XYZ {

// 既是program_struct的基类，
// 也是program_struct := $NULL 的默认实现
class ProgramStructNode : public ASTNode {
 public:
  ProgramStructNode(size_t line) : ASTNode("ProgramStruct", line) {};
  ~ProgramStructNode() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }
};
// program_struct := program_head SEMICOLON program_body DOT
// 命名方式为 XXNode_YY_ZZ... XX 为类名，YY,ZZ,... 为语法规则
class ProgramStructNode_ProgramHead_Semicolon_ProgramBody_Dot
    : public ProgramStructNode {
 public:
  ProgramStructNode_ProgramHead_Semicolon_ProgramBody_Dot(
      ASTNodePtr programHead, ASTNodePtr semicolon, ASTNodePtr programBody,
      ASTNodePtr dot, size_t line)
      : ProgramStructNode(line) {
    addChild(programHead);
    addChild(semicolon);
    addChild(programBody);
    addChild(dot);
  }
  ~ProgramStructNode_ProgramHead_Semicolon_ProgramBody_Dot() override = default;
  virtual void accept(ASTVisitor &visitor) override { visitor.visit(*this); }

  ASTNodePtr getProgramHead() const { return m_children[0]; }
  ASTNodePtr getSemicolon() const { return m_children[1]; }
  ASTNodePtr getProgramBody() const { return m_children[2]; }
  ASTNodePtr getDot() const { return m_children[3]; }
};

}  // namespace XYZ
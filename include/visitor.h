#pragma once
#include <memory>

#include "ast/ast.h"
namespace XYZ {
class ASTNode;
class TerminalNode;
// ProgramStructNode
class ProgramStructNode;
class ProgramStructNode_ProgramHead_Semicolon_ProgramBody_Dot;
// ProgramHeadNode
class ProgramHeadNode;
class ProgramHeadNode_Program_Id_Lparen_Idlist_Rparen;
class ProgramHeadNode_Program_Id;
// ProgramBodyNode
class ProgramBodyNode;
class ProgramBodyNode_ConstDecls_VarDecls_SubprogramDecls_CompoundStatement;
// IdListNode
class IdListNode;
class IdListNode_Id;
class IdListNode_IdList_Comma_Id;
// ConstDeclsNode
class ConstDeclsNode;
class ConstDeclsNode_Const_ConstDecl;

class ASTVisitor {
 public:
  ASTVisitor() = default;
  virtual ~ASTVisitor() = default;
  virtual void visit(TerminalNode &node) = 0;
  virtual void visit(ProgramStructNode &node) = 0;
  virtual void visit(
      ProgramStructNode_ProgramHead_Semicolon_ProgramBody_Dot &node) = 0;
  virtual void visit(ProgramHeadNode &node) = 0;
  virtual void visit(ProgramHeadNode_Program_Id_Lparen_Idlist_Rparen &node) = 0;
  virtual void visit(ProgramHeadNode_Program_Id &node) = 0;
  virtual void visit(ProgramBodyNode &node) = 0;
  virtual void visit(
      ProgramBodyNode_ConstDecls_VarDecls_SubprogramDecls_CompoundStatement
          &node) = 0;
  virtual void visit(IdListNode &node) = 0;
  virtual void visit(IdListNode_Id &node) = 0;
  virtual void visit(IdListNode_IdList_Comma_Id &node) = 0;
  virtual void visit(ConstDeclsNode &node) = 0;
  virtual void visit(ConstDeclsNode_Const_ConstDecl &node) = 0;
};
}  // namespace XYZ
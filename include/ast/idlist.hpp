#pragma once
#include "ast/ast.h"

namespace XYZ {

// idlist 的基类
class IdListNode : public ASTNode {
 public:
  IdListNode(size_t line) : ASTNode("IdListNode", line) {}
  ~IdListNode() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }
};

// idlist := ID
class IdListNode_Id : public IdListNode {
 public:
  IdListNode_Id(ASTNodePtr id, size_t line) : IdListNode(line) { addChild(id); }
  ~IdListNode_Id() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }

  ASTNodePtr getId() const { return m_children[0]; }
};

// idlist := idlist COMMA ID
class IdListNode_IdList_Comma_Id : public IdListNode {
 public:
  IdListNode_IdList_Comma_Id(ASTNodePtr idlist, ASTNodePtr comma, ASTNodePtr id,
                             size_t line)
      : IdListNode(line) {
    addChild(idlist);
    addChild(comma);
    addChild(id);
  }
  ~IdListNode_IdList_Comma_Id() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }

  ASTNodePtr getIdList() const { return m_children[0]; }
  ASTNodePtr getComma() const { return m_children[1]; }
  ASTNodePtr getId() const { return m_children[2]; }
};

}  // namespace XYZ

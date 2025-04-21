#pragma once
#include "ast/ast.hpp"
#include "ast/terminal.hpp"
#include "exception.hpp"
#include <memory>

namespace XYZ {
using namespace std;
// idlist 的基类
class IdListNode : public ASTNode {
public:
  using IDListType = std::vector<std::shared_ptr<TerminalNode>>;
  IdListNode(size_t line) : ASTNode("IdList", line) {}
  ~IdListNode() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }
  virtual std::shared_ptr<IdListNode> getIdList() const { return nullptr; }
  virtual std::shared_ptr<TerminalNode> getId() const { return nullptr; }
  IDListType getAllIds() const {
    IDListType ids;
    auto id = getId();
    if (id) {
      ids.push_back(id);
    }
    auto idlist = getIdList();
    if (idlist) {
      auto idlist_ids = idlist->getAllIds();
      ids.insert(ids.end(), idlist_ids.begin(), idlist_ids.end());
    }
    return ids;
  }
};

// idlist := ID
class IdListNode_Id : public IdListNode {
public:
  IdListNode_Id(ASTNodePtr id, size_t line) : IdListNode(line) { addChild(id); }
  ~IdListNode_Id() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }
  std::shared_ptr<TerminalNode> getId() const override {
    return dynamic_pointer_cast<TerminalNode>(m_children[0]);
  }
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

  std::shared_ptr<IdListNode> getIdList() const override {
    return dynamic_pointer_cast<IdListNode>(m_children[0]);
  }
  std::shared_ptr<TerminalNode> getComma() const {
    return dynamic_pointer_cast<TerminalNode>(m_children[1]);
  }
  std::shared_ptr<TerminalNode> getId() const override {
    return dynamic_pointer_cast<TerminalNode>(m_children[2]);
  }
};

} // namespace XYZ

#pragma once
#include "ast/ast.h"
#include "ast/idlist.hpp"
#include "exception.hpp"

namespace XYZ {

// value_parameter 的基类
class ValueParameterNode : public ASTNode {
public:
  ValueParameterNode(size_t line) : ASTNode("ValueParameter", line) {}
  ~ValueParameterNode() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }
  virtual SymbolType::ParamsType getParams() const = 0;
};

// value_parameter := idlist COLON basic_type
class ValueParameterNode_IdList_Colon_BasicType : public ValueParameterNode {
public:
  ValueParameterNode_IdList_Colon_BasicType(ASTNodePtr idlist, ASTNodePtr colon,
                                            ASTNodePtr basicType, size_t line)
      : ValueParameterNode(line) {
    addChild(idlist);
    addChild(colon);
    addChild(basicType);
  }
  ~ValueParameterNode_IdList_Colon_BasicType() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }

  std::shared_ptr<IdListNode> getIdList() const {
    return dynamic_pointer_cast<IdListNode>(m_children[0]);
  }
  std::shared_ptr<TerminalNode> getColon() const {
    return dynamic_pointer_cast<TerminalNode>(m_children[1]);
  }
  std::shared_ptr<BasicTypeNode> getBasicType() const {
    return dynamic_pointer_cast<BasicTypeNode>(m_children[2]);
  }
  SymbolType::ParamsType getParams() const override {
    auto ids = getIdList()->getAllIds();
    auto type = *(getBasicType()->getType());
    // 将类型设置为值参数
    type.set_ref(false);
    return SymbolType::MakeParams(ids, type);
  }
};

} // namespace XYZ

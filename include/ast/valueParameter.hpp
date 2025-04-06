#pragma once
#include "ast/ast.h"

namespace XYZ {

// value_parameter 的基类
class ValueParameterNode : public ASTNode {
 public:
  ValueParameterNode(size_t line) : ASTNode("ValueParameter", line) {}
  ~ValueParameterNode() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }
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

  ASTNodePtr getIdList() const { return m_children[0]; }
  ASTNodePtr getColon() const { return m_children[1]; }
  ASTNodePtr getBasicType() const { return m_children[2]; }
};

}  // namespace XYZ

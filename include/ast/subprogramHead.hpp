#pragma once
#include "ast/ast.h"

namespace XYZ {

// subprogram_head 的基类
class SubprogramHeadNode : public ASTNode {
public:
  SubprogramHeadNode(size_t line) : ASTNode("SubprogramHead", line) {}
  ~SubprogramHeadNode() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }
};

// subprogram_head := PROCEDURE ID formal_parameter
class SubprogramHeadNode_Procedure_Id_FormalParameter
    : public SubprogramHeadNode {
public:
  SubprogramHeadNode_Procedure_Id_FormalParameter(ASTNodePtr procedureToken,
                                                  ASTNodePtr id,
                                                  ASTNodePtr formalParameter,
                                                  size_t line)
      : SubprogramHeadNode(line) {
    addChild(procedureToken);
    addChild(id);
    addChild(formalParameter);
  }
  ~SubprogramHeadNode_Procedure_Id_FormalParameter() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }

  shared_ptr<TerminalNode> getProcedureToken() const {
    return dynamic_pointer_cast<TerminalNode>(m_children[0]);
  }
  shared_ptr<TerminalNode> getId() const {
    return dynamic_pointer_cast<TerminalNode>(m_children[1]);
  }
  shared_ptr<FormalParameterNode> getFormalParameter() const {
    return dynamic_pointer_cast<FormalParameterNode>(m_children[2]);
  }
};

// subprogram_head := FUNCTION ID formal_parameter COLON basic_type
class SubprogramHeadNode_Function_Id_FormalParameter_Colon_BasicType
    : public SubprogramHeadNode {
public:
  SubprogramHeadNode_Function_Id_FormalParameter_Colon_BasicType(
      ASTNodePtr functionToken, ASTNodePtr id, ASTNodePtr formalParameter,
      ASTNodePtr colon, ASTNodePtr basicType, size_t line)
      : SubprogramHeadNode(line) {
    addChild(functionToken);
    addChild(id);
    addChild(formalParameter);
    addChild(colon);
    addChild(basicType);
  }
  ~SubprogramHeadNode_Function_Id_FormalParameter_Colon_BasicType() override =
      default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }

  shared_ptr<TerminalNode> getFunctionToken() const {
    return dynamic_pointer_cast<TerminalNode>(m_children[0]);
  }
  shared_ptr<TerminalNode> getId() const {
    return dynamic_pointer_cast<TerminalNode>(m_children[1]);
  }
  shared_ptr<FormalParameterNode> getFormalParameter() const {
    return dynamic_pointer_cast<FormalParameterNode>(m_children[2]);
  }
  shared_ptr<TerminalNode> getColon() const {
    return dynamic_pointer_cast<TerminalNode>(m_children[3]);
  }
  shared_ptr<BasicTypeNode> getBasicType() const {
    return dynamic_pointer_cast<BasicTypeNode>(m_children[4]);
  }
};

} // namespace XYZ

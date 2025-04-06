#pragma once
#include "ast/ast.h"

namespace XYZ {

// subprogram_decls 的基类
// subprogram_decls := EMPTY
class SubprogramDeclsNode : public ASTNode {
 public:
  SubprogramDeclsNode(size_t line) : ASTNode("SubprogramDecls", line) {}
  ~SubprogramDeclsNode() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }
};

// subprogram_decls := subprogram_decls subprogram
class SubprogramDeclsNode_SubprogramDecls_Subprogram
    : public SubprogramDeclsNode {
 public:
  SubprogramDeclsNode_SubprogramDecls_Subprogram(ASTNodePtr subprogramDecls,
                                                 ASTNodePtr subprogram,
                                                 size_t line)
      : SubprogramDeclsNode(line) {
    addChild(subprogramDecls);
    addChild(subprogram);
  }
  ~SubprogramDeclsNode_SubprogramDecls_Subprogram() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }

  ASTNodePtr getSubprogramDecls() const { return m_children[0]; }
  ASTNodePtr getSubprogram() const { return m_children[1]; }
};

}  // namespace XYZ

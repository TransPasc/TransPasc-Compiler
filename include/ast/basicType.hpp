#pragma once
#include "ast/ast.h"

namespace XYZ {

// basic_type 的基类
class BasicTypeNode : public ASTNode {
 public:
  BasicTypeNode(size_t line) : ASTNode("BasicType", line) {}
  ~BasicTypeNode() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }
};

// basic_type := INTEGER
class BasicTypeNode_Integer : public BasicTypeNode {
 public:
  BasicTypeNode_Integer(ASTNodePtr integerToken, size_t line)
      : BasicTypeNode(line) {
    addChild(integerToken);
  }
  ~BasicTypeNode_Integer() override = default;

  ASTNodePtr getIntegerToken() const { return m_children[0]; }
};

// basic_type := REAL
class BasicTypeNode_Real : public BasicTypeNode {
 public:
  BasicTypeNode_Real(ASTNodePtr realToken, size_t line) : BasicTypeNode(line) {
    addChild(realToken);
  }
  ~BasicTypeNode_Real() override = default;

  ASTNodePtr getRealToken() const { return m_children[0]; }
};

// basic_type := BOOLEAN
class BasicTypeNode_Boolean : public BasicTypeNode {
 public:
  BasicTypeNode_Boolean(ASTNodePtr booleanToken, size_t line)
      : BasicTypeNode(line) {
    addChild(booleanToken);
  }
  ~BasicTypeNode_Boolean() override = default;

  ASTNodePtr getBooleanToken() const { return m_children[0]; }
};

// basic_type := CHAR
class BasicTypeNode_Char : public BasicTypeNode {
 public:
  BasicTypeNode_Char(ASTNodePtr charToken, size_t line) : BasicTypeNode(line) {
    addChild(charToken);
  }
  ~BasicTypeNode_Char() override = default;

  ASTNodePtr getCharToken() const { return m_children[0]; }
};

}  // namespace XYZ

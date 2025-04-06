#pragma once
#include "ast/ast.h"

namespace XYZ {

// period 的基类
class PeriodNode : public ASTNode {
 public:
  PeriodNode(size_t line) : ASTNode("Period", line) {}
  ~PeriodNode() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }
};

// period := NUMBER DOT DOT NUMBER
class PeriodNode_Number_Dot_Dot_Number : public PeriodNode {
 public:
  PeriodNode_Number_Dot_Dot_Number(ASTNodePtr number1, ASTNodePtr dot1,
                                   ASTNodePtr dot2, ASTNodePtr number2,
                                   size_t line)
      : PeriodNode(line) {
    addChild(number1);
    addChild(dot1);
    addChild(dot2);
    addChild(number2);
  }
  ~PeriodNode_Number_Dot_Dot_Number() override = default;

  ASTNodePtr getNumber1() const { return m_children[0]; }
  ASTNodePtr getDot1() const { return m_children[1]; }
  ASTNodePtr getDot2() const { return m_children[2]; }
  ASTNodePtr getNumber2() const { return m_children[3]; }
};

// period := period COMMA NUMBER DOT DOT NUMBER
class PeriodNode_Period_Comma_Number_Dot_Dot_Number : public PeriodNode {
 public:
  PeriodNode_Period_Comma_Number_Dot_Dot_Number(
      ASTNodePtr period, ASTNodePtr comma, ASTNodePtr number1, ASTNodePtr dot1,
      ASTNodePtr dot2, ASTNodePtr number2, size_t line)
      : PeriodNode(line) {
    addChild(period);
    addChild(comma);
    addChild(number1);
    addChild(dot1);
    addChild(dot2);
    addChild(number2);
  }
  ~PeriodNode_Period_Comma_Number_Dot_Dot_Number() override = default;

  ASTNodePtr getPeriod() const { return m_children[0]; }
  ASTNodePtr getComma() const { return m_children[1]; }
  ASTNodePtr getNumber1() const { return m_children[2]; }
  ASTNodePtr getDot1() const { return m_children[3]; }
  ASTNodePtr getDot2() const { return m_children[4]; }
  ASTNodePtr getNumber2() const { return m_children[5]; }
};

}  // namespace XYZ

#pragma once
#include "ast/ast.h"

namespace XYZ {

// period 的基类
class PeriodNode : public ASTNode {
public:
  using PeriodsType = std::vector<std::shared_ptr<std::pair<int32_t, int32_t>>>;
  using PeriodsTypePtr = std::shared_ptr<PeriodsType>;

public:
  PeriodNode(size_t line) : ASTNode("Period", line) {}
  ~PeriodNode() override = default;

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); }
  virtual PeriodsTypePtr getPeriods() const = 0;
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

  shared_ptr<TerminalNode> getNumber1() const {
    return dynamic_pointer_cast<TerminalNode>(m_children[0]);
  }
  shared_ptr<TerminalNode> getDot1() const {
    return dynamic_pointer_cast<TerminalNode>(m_children[1]);
  }
  shared_ptr<TerminalNode> getDot2() const {
    return dynamic_pointer_cast<TerminalNode>(m_children[2]);
  }
  shared_ptr<TerminalNode> getNumber2() const {
    return dynamic_pointer_cast<TerminalNode>(m_children[3]);
  }
  PeriodsTypePtr getPeriods() const override {
    PeriodsTypePtr periods = make_shared<PeriodsType>();
    periods->push_back(make_shared<pair<int, int>>(getNumber1()->get<int>(),
                                                   getNumber2()->get<int>()));
    return periods;
  }
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

  shared_ptr<PeriodNode> getPeriod() const {
    return dynamic_pointer_cast<PeriodNode>(m_children[0]);
  }
  shared_ptr<TerminalNode> getComma() const {
    return dynamic_pointer_cast<TerminalNode>(m_children[1]);
  }
  shared_ptr<TerminalNode> getNumber1() const {
    return dynamic_pointer_cast<TerminalNode>(m_children[2]);
  }
  shared_ptr<TerminalNode> getDot1() const {
    return dynamic_pointer_cast<TerminalNode>(m_children[3]);
  }
  shared_ptr<TerminalNode> getDot2() const {
    return dynamic_pointer_cast<TerminalNode>(m_children[4]);
  }
  shared_ptr<TerminalNode> getNumber2() const {
    return dynamic_pointer_cast<TerminalNode>(m_children[5]);
  }
  PeriodsTypePtr getPeriods() const override {

    auto periods = getPeriod()->getPeriods();
    periods->push_back(make_shared<pair<int32_t, int32_t>>(
        getNumber1()->get<int>(), getNumber2()->get<int>()));
    return periods;
  }
};

} // namespace XYZ

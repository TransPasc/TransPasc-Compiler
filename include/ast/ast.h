#pragma once
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "visitor.h"

namespace XYZ {
class ASTNode {
public:
  using ASTNodePtr = std::shared_ptr<ASTNode>;
  ASTNode(const std::string &name, size_t line) : m_name(name), m_line(line) {};
  // 拷贝构造函数
  ASTNode(const ASTNode &other)
      : m_name(other.m_name), m_line(other.m_line),
        m_children(other.m_children) {}
  ASTNode &operator=(const ASTNode &) = default;
  virtual ~ASTNode() = default;
  virtual void accept(ASTVisitor &visitor) = 0;

  virtual size_t getLine() const { return m_line; }
  virtual std::string getName() const { return m_name; }

  /**
   * @brief 以缩进的方式打印节点
   * 用 "|-" 树形连接符号
   * 先序遍历
   * @param indent
   */
  virtual void print(std::string prefix) const {
    std::cout << prefix;
    std::cout << m_name << " [ " << m_line << "]" << std::endl;
    for (const auto &child : m_children) {
      child->print("|  " + prefix);
    }
  }
  // 只能访问子节点，不能修改
  auto begin() const { return m_children.begin(); }
  auto end() const { return m_children.end(); }

protected:
  //  只对子类开放 修改 child 节点
  void addChild(ASTNodePtr child) { m_children.push_back(child); }

protected:
  std::string m_name;
  size_t m_line = 0;
  //   size_t m_column = 0; // 暂不使用
  std::vector<ASTNodePtr> m_children;
};
} // namespace XYZ

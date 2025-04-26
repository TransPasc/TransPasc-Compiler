#pragma once
#include <cassert>
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
  // use template to make it support multiple ASTNode
  // 基础版本：处理单个节点
  template <typename T> void addChild(std::shared_ptr<T> child) {
    static_assert(std::is_base_of_v<ASTNode, T>,
                  "Child must inherit from ASTNode");

    if (!child) {
      std::cerr << "Error: null child\n";
      return;
    }
    m_children.push_back(child);
  }

  // 重载版本：支持多个参数
  template <typename First, typename... Rest>
  void addChild(First &&first, Rest &&...rest) {
    addChild(std::forward<First>(first));  // 处理第一个参数
    addChild(std::forward<Rest>(rest)...); // 递归处理剩余参数
  }

  // void addChild(ASTNodePtr child) { m_children.push_back(child); }

protected:
  std::string m_name;
  size_t m_line = 0;
  //   size_t m_column = 0; // 暂不使用
  std::vector<ASTNodePtr> m_children;
};
} // namespace XYZ

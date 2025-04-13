#pragma once
#include <memory>
#include <string>
#include <vector>

#include "symbolTable/exception.hpp"
#include "symbolTable/type.hpp"

namespace XYZ {
/**
 * @brief 符号记录基类，存储符号元数据
 */

class SymbolRecord {
  // 符号名称
  std::string m_name;
  // 符号表中的下一个符号记录
  int32_t m_next = -1;
  // 存储符号类型
  SymbolType *m_type = nullptr;
  // 地址
  int32_t m_address = -1;
  // 声明行号
  int32_t m_lineno = -1;
  // 引用行号
  std::vector<int32_t> m_ref_lineno;

public:
  SymbolRecord(std::string name, int32_t lineno, int32_t nxt = -1,
               SymbolType *type = nullptr)
      : m_name(name), m_lineno(lineno), m_next(nxt), m_type(type) {}
  virtual ~SymbolRecord() = default;

  const std::string &getName() const { return m_name; }

  int32_t getNext() const { return m_next; }

  virtual void setNext(int32_t nxt) { m_next = nxt; }
  SymbolType *getType() const { return m_type; }
  void setType(SymbolType *type) { m_type = type; }
  int32_t getAddress() const { return m_address; }
  void setAddress(int32_t address) { m_address = address; }
  int32_t getLineno() const { return m_lineno; }
  void setLineno(int32_t lineno) { m_lineno = lineno; }
  const std::vector<int32_t> &getRefLineno() const { return m_ref_lineno; }
  void addRefLineno(int32_t lineno) { m_ref_lineno.push_back(lineno); }
};
} // namespace XYZ

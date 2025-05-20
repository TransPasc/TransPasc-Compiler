#pragma once
#include <memory>
#include <string>
#include <vector>

#include "symbolTable/exception.hpp"
#include "symbolTable/recordInterface.hpp"
#include "symbolTable/type.hpp"
namespace XYZ {
/**
 * @brief 符号记录基类，存储符号元数据
 */

class LLVMIRSymbolRecord : public RecordInterface {
  // 符号名称
  std::string m_name;
  // 符号表中的下一个符号记录
  int32_t m_next = -1;
  // 存储符号类型
  std::shared_ptr<SymbolType> m_type = nullptr;
  // 当前 symbol name
  std::string m_currentName;

  bool m_isGlobal = false;

public:
  LLVMIRSymbolRecord(std::string name,
                     std::shared_ptr<SymbolType> type = nullptr,
                     int32_t nxt = -1)
      : m_name(name), m_type(type), m_next(nxt) {
    m_currentName = name;
  }
  virtual ~LLVMIRSymbolRecord() = default;

  const std::string &getName() const { return m_name; }
  int32_t getNext() const { return m_next; }
  virtual void setNext(int32_t nxt) { m_next = nxt; }
  std::shared_ptr<SymbolType> getType() const { return m_type; }
  void setType(std::shared_ptr<SymbolType> type) { m_type = type; }

  const std::string &getCurrentName() const { return m_currentName; }
  void setCurrentName(const std::string &name) { m_currentName = name; }
  bool isGlobal() const { return m_isGlobal; }
  void setGlobal(bool isGlobal) { m_isGlobal = isGlobal; }
};
} // namespace XYZ

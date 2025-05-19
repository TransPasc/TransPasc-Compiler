#pragma once
#include "codeGenerate/exception.hpp"
#include "generator.hpp"
#include "llvmIrOutputBuffer.hpp"
#include "symbolTable/stackLinkedSymbolTable.hpp"
#include "symbolTable/type.hpp"
#include "utils/utils.hpp"
#include <set>
#include <stack>
namespace XYZ {
class LLVMIrGenerator : public Generator {
  using ErrType = CodeGenerateException::ErrorCode;
  std::string m_outputFile;
  OutputBuffer m_outputBuffer;

  std::stack<State> m_stateStack; // 状态栈

  std::shared_ptr<SymbolTable> symbolTable; // 符号表

  std::size_t unname_id_num = 0; // 匿名变量数量

  std::stack<std::pair<SymbolType::ParamsType, size_t>>
      m_paramsStack; // 函数参数栈

  std::stack<std::shared_ptr<SymbolType>> m_returnTypeStack; // 函数返回值类型

  std::set<std::string> m_FmtStrSet; // 已经定义的格式化字符串

public:
  LLVMIrGenerator();
  ~LLVMIrGenerator() override;

  /**
   * @brief generate code from AST
   * if set output file, the code will be write to the file
   * if not, the code will be
   * print to the console
   * @param root the root of the AST
   */
  void generateCode(ASTNode::ASTNodePtr root) override;
  virtual void setOutputFile(const std::string &filename) override;

  dispatch_all_nodes(dispatch_override);
  // 处理节点公共代码
  void g_IdList_Type(std::shared_ptr<IdListNode> idList,
                     std::shared_ptr<TypeNode> type);

private:
  std::string symbolType2LLVMStr(const SymbolType &type);
  std::string relop2LLVMStr(const std::string &relop);
  std::string mulop2LLVMStr(const std::string &mulop);
  std::string getDefaultValue(const SymbolType &type);
  std::string getFmtStrName(const std::string &fmt);
  std::string getUnNameIdStr();
  std::string getLLVMStyleIOFormatStr(
      const std::vector<std::shared_ptr<SymbolType>> &types);
};

} // namespace XYZ

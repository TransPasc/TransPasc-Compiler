#pragma once
#include "codeGenerate/exception.hpp"
#include "generator.hpp"
#include "llvmIrOutputBuffer.hpp"
#include "symbolTable/stackLinkedSymbolTable.hpp"
#include "symbolTable/type.hpp"
#include "utils/utils.hpp"
#include <stack>
namespace XYZ {
class LLVMIrGenerator : public Generator {
  using ErrType = CodeGenerateException::ErrorCode;
  std::string m_outputFile;
  OutputBuffer m_outputBuffer;
  std::stack<State> m_stateStack;
  std::shared_ptr<SymbolTable> symbolTable;
  std::stack<std::pair<SymbolType::ParamsType, size_t>> m_paramsStack;
  std::stack<std::shared_ptr<SymbolType>> m_returnTypeStack;

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
};

} // namespace XYZ

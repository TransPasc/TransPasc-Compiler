#pragma once
#include "codeGenerate/exception.hpp"
#include "generator.hpp"
#include "symbolTable/type.hpp"
#include "utils/macro.h"
namespace XYZ {
class LLVMIrGenerator : public Generator {
  using ErrType = CodeGenerateException::ErrorCode;
  std::string m_outputFile;
  std::string m_outputBuffer;

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
};

} // namespace XYZ

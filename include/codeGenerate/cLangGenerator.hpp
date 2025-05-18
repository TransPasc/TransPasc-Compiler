#pragma once
#include "codeGenerate/exception.hpp"
#include "generator.hpp"
#include "symbolTable/stackLinkedSymbolTable.hpp"
#include "symbolTable/type.hpp"
#include "utils/macro.h"
namespace XYZ {
/**
 * @brief c language code generator
 * from the AST
 */
class CLangGenerator : public Generator {
  inline const static std::string FUNC_RES = "__result__";
  enum class State {
    NORMAL,       // 普通状态
    FunctionCall, // 处于函数调用 <function_name>(...)
    FunctionDef,  // 处于函数定义 <function_name>(<params>) {...}
    IdVarPart,    // 处于数组下标 <array_name>[...]
    Scanf,        // 处于 scanf(<format_string>, ...)
    Printf,       // 处于 printf(<format_string>, ...)
  };
  std::stack<State> m_stateStack;
  std::string m_outputFile;
  std::string m_outputBuffer;
  bool m_isRefParam = false;
  using ErrType = CodeGenerateException::ErrorCode;
  std::shared_ptr<SymbolTable> symbolTable;

  std::stack<std::pair<SymbolType::ParamsType, size_t>> m_paramsStack;
  std::stack<std::shared_ptr<SymbolType>> m_returnTypeStack;

public:
  CLangGenerator();
  ~CLangGenerator() override;
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

private:
  // 公有产生式处理
  void g_IdList_Type(std::shared_ptr<IdListNode> idList,
                     std::shared_ptr<TypeNode> type);

private:
  // 生成代码的辅助函数, 写入str到缓冲区
  void writeln(const std::string &str);
  template <class... Ts> struct overloaded : Ts... {
    using Ts::operator()...;
  };
  template <class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

  std::string symbolType2Str(const SymbolType &type);
  // 将 Pascal relop 转换为 C 语言的 relop
  std::string relop2cStyle(const std::string &relop);
  std::string mulop2cStyle(const std::string &mulop);

  /**
   * @brief 通过符号类型转换为 C 语言的输入输出的 format 字符串
   * @param types 符号类型列表
   * @return std::string
   */
  std::string
  getCStyleIOFormatStr(const std::vector<std::shared_ptr<SymbolType>> &types);
};
} // namespace XYZ

#pragma once
#include <exception>
#include <stack>
#include <string>
namespace XYZ {
struct OutputBuffer {
  enum class Section {
    TARGET_SPEC,
    TYPE_DECLS,
    GLOBAL_CONST_DECLS,
    GLOBAL_VAR_DECLS,
    FUNC_DECLS,
    FUNC_DEFS,
    ATTRIBUTE_GROUPS,
    NAMED_METADATA,
    METADATA_NODES
  };

private:
  // LLVM IR的各个部分
  std::string target_spec; // target triple/datalayout
  std::string type_decls;  // 类型系统
  std::string global_const_decls;
  std::string global_var_decls;
  std::string func_decls;
  std::string func_defs; // 包含函数体实现
  std::string attribute_groups;
  std::string named_metadata; // 具名元数据
  std::string metadata_nodes; // 元数据节点

  // 用于跟踪当前的输出部分
  std::stack<Section> sectionStack;
  std::string source_file_name;

public:
  std::string toString() {
    std::string result;
    result += "source_filename = \"" + source_file_name + ".pas\"\n\n";
    result += "; TARGET SPEC\n";
    result += target_spec + "\n";
    result += "; TYPE DECLS\n";
    result += type_decls + "\n";
    result += "; GLOBAL CONST DECLS\n";
    result += global_const_decls + "\n";
    result += "; GLOBAL VAR DECLS\n";
    result += global_var_decls + "\n";
    result += "; FUNC DECLS\n";
    result += func_decls + "\n";
    result += "; FUNC DEFS\n";
    result += func_defs + "\n";
    result += "; ATTRIBUTE GROUPS\n";
    result += attribute_groups + "\n";
    result += "; NAMED METADATA\n";
    result += named_metadata + "\n";
    result += "; METADATA NODES\n";
    result += metadata_nodes + "\n";
    result += "; END\n";
    return result;
  }
  void enterSection(Section section) { sectionStack.push(section); }
  void exitSection() {
    if (!sectionStack.empty()) {
      sectionStack.pop();
    }
  }
  void writeln(const std::string &data = "") {
    if (sectionStack.empty()) {
      throw std::runtime_error("No section is active");
    }
    switch (sectionStack.top()) {
    case Section::TARGET_SPEC:
      target_spec += data + "\n";
      break;
    case Section::TYPE_DECLS:
      type_decls += data + "\n";
      break;
    case Section::GLOBAL_CONST_DECLS:
      global_const_decls += data + "\n";
      break;
    case Section::GLOBAL_VAR_DECLS:
      global_var_decls += data + "\n";
      break;
    case Section::FUNC_DECLS:
      func_decls += data + "\n";
      break;
    case Section::FUNC_DEFS:
      func_defs += data + "\n";
      break;
    case Section::ATTRIBUTE_GROUPS:
      attribute_groups += data + "\n";
      break;
    case Section::NAMED_METADATA:
      named_metadata += data + "\n";
      break;
    case Section::METADATA_NODES:
      metadata_nodes += data + "\n";
      break;
    default:
      throw std::runtime_error("Unknown section");
    }
  }
  void setSourceFileName(const std::string &name) { source_file_name = name; }
};
}; // namespace XYZ
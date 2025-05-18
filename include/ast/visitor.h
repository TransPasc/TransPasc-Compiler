#pragma once
#include "utils/macro.h"
namespace XYZ {

class ASTVisitor {
public:
  virtual ~ASTVisitor() = default;
  // 分发到所有节点
  dispatch_all_nodes(dispatch_virtual);
};

} // namespace XYZ

#undef dispatch

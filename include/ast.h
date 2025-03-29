#pragma once

namespace XYZ {
class ASTNode {
 public:
  ASTNode() = default;
  virtual ~ASTNode() = default;
  //   virtual void accept(class ASTVisitor &visitor) = 0;
};
}  // namespace XYZ
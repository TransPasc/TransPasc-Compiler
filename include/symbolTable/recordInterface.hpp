#pragma once
#include <string>
namespace XYZ {
class RecordInterface {
public:
  virtual ~RecordInterface() = default;
  virtual const std::string &getName() const = 0;
  virtual int32_t getNext() const = 0;
  virtual void setNext(int32_t nxt) = 0;
};
}; // namespace XYZ
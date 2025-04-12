#pragma once
template <typename T> class Iterator {
public:
  virtual ~Iterator() = default;
  virtual T &operator*() const = 0;                         // 解引用
  virtual Iterator &operator++() = 0;                       // 递增
  virtual bool operator!=(const Iterator &other) const = 0; // 比较
};

#pragma once
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <variant>
#include <vector>

#include "symbolTable/exception.hpp"

namespace XYZ {
/**
 * @class SymbolType
 * @brief 符号类型基类，存储符号类型信息
 */
class SymbolType {
public:
  enum class BasicType {
    INTEGER,
    REAL,
    BOOLEAN,
    CHAR,
    STRING,
    FLOAT,
  };
  struct Array {
    std::shared_ptr<SymbolType> element_type;
    std::pair<int32_t, int32_t> range;

    Array(std::shared_ptr<SymbolType> elem, int32_t low, int32_t high)
        : element_type(std::move(elem)), range(low, high) {}
  };

  struct Record {
    struct Field {
      std::string name;
      std::shared_ptr<SymbolType> type;

      Field(std::string n, std::shared_ptr<SymbolType> t)
          : name(std::move(n)), type(std::move(t)) {}
    };

    std::vector<Field> fields;
  };

  struct Function {
    std::shared_ptr<SymbolType> return_type;
    std::vector<std::shared_ptr<SymbolType>> param_types;
  };

  struct Procedure {
    std::vector<std::shared_ptr<SymbolType>> param_types;
  };
  using Type = std::variant<std::monostate, // UNDEFINED
                            BasicType,      // BASIC
                            Array,          // ARRAY
                            Record,         // RECORD
                            Function,       // FUNCTION
                            Procedure       // PROCEDURE
                            >;

public:
  SymbolType() = default;

  ~SymbolType() = default;
  // 构造具体类型的工厂函数
  static SymbolType MakeBasic(BasicType type) {
    SymbolType st;
    st.m_var = type;
    return st;
  }

  static SymbolType MakeArray(std::shared_ptr<SymbolType> elem_type,
                              int32_t low, int32_t high) {
    SymbolType st;
    st.m_var = Array(std::move(elem_type), low, high);
    return st;
  }

  static SymbolType MakeRecord(std::vector<Record::Field> fields) {
    SymbolType st;
    st.m_var = Record{std::move(fields)};
    return st;
  }

  static SymbolType
  MakeFunction(std::shared_ptr<SymbolType> ret,
               std::vector<std::shared_ptr<SymbolType>> params) {
    SymbolType st;
    st.m_var = Function{std::move(ret), std::move(params)};
    return st;
  }

  static SymbolType
  MakeProcedure(std::vector<std::shared_ptr<SymbolType>> params) {
    SymbolType st;
    st.m_var = Procedure{std::move(params)};
    return st;
  }

  // 类型检查方法
  bool is_undefined() const noexcept {
    return std::holds_alternative<std::monostate>(m_var);
  }
  bool is_basic() const noexcept {
    return std::holds_alternative<BasicType>(m_var);
  }
  bool is_array() const noexcept {
    return std::holds_alternative<Array>(m_var);
  }
  bool is_record() const noexcept {
    return std::holds_alternative<Record>(m_var);
  }
  bool is_function() const noexcept {
    return std::holds_alternative<Function>(m_var);
  }
  bool is_procedure() const noexcept {
    return std::holds_alternative<Procedure>(m_var);
  }
  // 安全访问方法（指针版）
  template <typename T> const T *get_if() const noexcept {
    return std::get_if<T>(&m_var);
  }

  // 安全访问方法（引用版，可能抛出）
  template <typename T> const T &get() const { return std::get<T>(m_var); }

  // 访问者模式
  template <typename Visitor> decltype(auto) visit(Visitor &&vis) const {
    return std::visit(std::forward<Visitor>(vis), m_var);
  }
  // 设置常量标志
  void set_const() { is_const = true; }
  // 获取常量标志
  bool is_const_type() const { return is_const; }

  // 辅助函数：类型比较
  friend bool operator==(const SymbolType &lhs, const SymbolType &rhs) {
    if (lhs.m_var.index() != rhs.m_var.index())
      return false;

    return lhs.visit([&](const auto &l_val) -> bool {
      using T = std::decay_t<decltype(l_val)>;
      if constexpr (std::is_same_v<T, std::monostate>) {
        return true; // 两个未定义类型相等
      } else if constexpr (std::is_same_v<T, SymbolType::BasicType>) {
        return l_val == rhs.get<T>();
      } else {
        const auto *r_val = rhs.get_if<T>();
        if (!r_val)
          return false;

        if constexpr (std::is_same_v<T, SymbolType::Array>) {
          return *l_val.element_type == *r_val->element_type &&
                 l_val.range == r_val->range;
        }
        // 其他类型比较需补充实现
        else {
          throw std::runtime_error("Comparison not implemented for this type");
        }
      }
    });
  }

  // 示例：使用访问者处理不同类
  template <class... Ts> struct overloaded : Ts... {
    using Ts::operator()...;
  };
  template <class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

  void print_symbol(const SymbolType &sym) {
    sym.visit(
        overloaded{[](std::monostate) { std::cout << "Undefined type\n"; },
                   [](SymbolType::BasicType basic) {
                     std::cout << "Basic type: ";
                     switch (basic) {
                     case SymbolType::BasicType::INTEGER:
                       std::cout << "INTEGER";
                       break;
                     case SymbolType::BasicType::REAL:
                       std::cout << "REAL";
                       break;
                     case SymbolType::BasicType::BOOLEAN:
                       std::cout << "BOOLEAN";
                       break;
                     case SymbolType::BasicType::CHAR:
                       std::cout << "CHAR";
                       break;
                     case SymbolType::BasicType::STRING:
                       std::cout << "STRING";
                       break;
                     case SymbolType::BasicType::FLOAT:
                       std::cout << "FLOAT";
                       break;
                     default:
                       std::cout << "Unknown basic type";
                       break;
                     }
                     std::cout << "\n";
                   },
                   [this](const SymbolType::Array &arr) {
                     std::cout << "Array[" << arr.range.first << ".."
                               << arr.range.second << "] of ";
                     this->print_symbol(*arr.element_type);
                   },
                   [this](const SymbolType::Record &rec) {
                     std::cout << "Record with fields:\n";
                     for (const auto &field : rec.fields) {
                       std::cout << "  " << field.name << ": ";
                       this->print_symbol(*field.type);
                     }
                   },
                   [this](const SymbolType::Function &func) {
                     std::cout << "Function returning ";
                     this->print_symbol(*func.return_type);
                     std::cout << " with parameters:\n";
                     for (const auto &param : func.param_types) {
                       this->print_symbol(*param);
                     }
                   },
                   [this](const SymbolType::Procedure &proc) {
                     std::cout << "Procedure with parameters:\n";
                     for (const auto &param : proc.param_types) {
                       this->print_symbol(*param);
                     }
                   }

        });
  }

private:
  Type m_var = std::monostate{}; // 使用 std::monostate 作为默认值
  bool is_const = false;         // 是否为常量
};

} // namespace XYZ

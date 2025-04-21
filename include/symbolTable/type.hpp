#pragma once

#include "symbolTable/exception.hpp"
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <variant>
#include <vector>

#include "ast/terminal.hpp"
enum class BasicType {
  INTEGER,
  REAL,
  BOOLEAN,
  CHAR,
  STRING,
  FLOAT,
};
namespace XYZ {
/**
 * @class SymbolType
 * @brief 符号类型基类，存储符号类型信息
 */
class SymbolType {
public:
  using ParamsType =
      std::vector<std::shared_ptr<std::pair<SymbolType, std::string>>>;

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
    ParamsType param_types;
  };

  struct Procedure {
    ParamsType param_types;
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
  /**
   * @brief 生成函数参数列表
   * @param idList
   * @param type
   * @return * ParamsTypes
   */
  static ParamsType
  MakeParams(const std::vector<std::shared_ptr<TerminalNode>> &idList,
             SymbolType type) {
    ParamsType params;
    for (const auto &id : idList) {
      params.push_back(std::make_shared<std::pair<SymbolType, std::string>>(
          type, id->get<std::string>()));
    }
    return params;
  }

  static SymbolType MakeFunction(std::shared_ptr<SymbolType> ret,
                                 ParamsType params) {
    SymbolType st;
    st.m_var = Function{std::move(ret), std::move(params)};
    return st;
  }
  static SymbolType MakeProcedure(ParamsType params) {
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
  /**
   * @brief 设置为是否可变
   * @param flag true 可变，false 不可变
   */
  void set_const(bool flag = true) { is_const = flag; }
  // 获取常量标志
  bool is_const_type() const { return is_const; }

  // 设置为是否引用其他变量
  void set_ref(bool flag = true) { is_ref = flag; }
  // 获取引用标志
  bool is_ref_type() const { return is_ref; }

  // 辅助函数：类型比较
  friend bool operator==(const SymbolType &lhs, const SymbolType &rhs) {
    if (lhs.m_var.index() != rhs.m_var.index())
      return false;

    return lhs.visit([&](const auto &l_val) -> bool {
      using T = std::decay_t<decltype(l_val)>;
      if constexpr (std::is_same_v<T, std::monostate>) {
        return true; // 两个未定义类型相等
      } else if constexpr (std::is_same_v<T, BasicType>) {
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
                   [](BasicType basic) {
                     std::cout << "Basic type: ";
                     switch (basic) {
                     case BasicType::INTEGER:
                       std::cout << "INTEGER";
                       break;
                     case BasicType::REAL:
                       std::cout << "REAL";
                       break;
                     case BasicType::BOOLEAN:
                       std::cout << "BOOLEAN";
                       break;
                     case BasicType::CHAR:
                       std::cout << "CHAR";
                       break;
                     case BasicType::STRING:
                       std::cout << "STRING";
                       break;
                     case BasicType::FLOAT:
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
                       this->print_symbol(param->first);
                     }
                   },
                   [this](const SymbolType::Procedure &proc) {
                     std::cout << "Procedure with parameters:\n";
                     for (const auto &param : proc.param_types) {
                       this->print_symbol(param->first);
                     }
                   }

        });
  }

private:
  Type m_var = std::monostate{}; // 使用 std::monostate 作为默认值
  bool is_const = false;         // 是否可变
  //   是否引用的其他变量
  bool is_ref = false; // 是否引用的其他变量
};

} // namespace XYZ

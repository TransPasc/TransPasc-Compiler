#pragma once
//
/**
 * 用于 variant 的访问
 * 例如:
 *   using Type = std::variant<std::monostate, // UNDEFINED
 *                          BasicType,      // BASIC
 *                          Array,          // ARRAY
 *                          Record,         // RECORD
 *                          Function,       // FUNCTION
 *                          Procedure       // PROCEDURE
 *                          >;
 *   Type type = 1;
 * static const auto printer =
 *    overloaded{[](std::monostate) -> std::string { return "void"; },
 *               [](BasicType basic) -> std::string {
 *                 switch (basic) {
 *                 case BasicType::INTEGER:
 *                   return "i32";
 *                 case BasicType::REAL:
 *                   return "double";
 *                 case BasicType::BOOLEAN:
 *                   return "i1";
 *                 case BasicType::CHAR:
 *                   return "i8";
 *                 default:
 *                   return "i32";
 *                 }
 *               },
 *               [](auto &&) -> std::string { return "i32"; }};
 *   return type.visit(printer);
 */
template <class... Ts> struct overloaded : Ts... {
  using Ts::operator()...;
};
template <class... Ts> overloaded(Ts...) -> overloaded<Ts...>;
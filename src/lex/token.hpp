#pragma once

#include <lex/scanner.hpp>

#include <cassert>
#include <optional>
#include <variant>

namespace lex {

//////////////////////////////////////////////////////////////////////

struct Token {
  Token() = default;
  Token(TokenType type) : type(type) {
  }
  template <class T>
  Token(TokenType type, T&& value)
      : type(type),
        value(
            std::variant<std::string, uint64_t, char>(std::forward<T>(value))) {
  }
  template <class T>
  Token(TokenType type, T&& value, const Location& loc)
      : type(type),
        value(
            std::variant<std::string, uint64_t, char>(std::forward<T>(value))),
        location(loc) {
  }

  TokenType type = TokenType::UNDEFINED;
  std::optional<std::variant<std::string, uint64_t, char>> value;
  Location location;

  template <class T>
  const T& Value() const {
    assert(value.has_value());
    assert(std::holds_alternative<T>(value.value()));
    return std::get<T>(value.value());
  }
};

//////////////////////////////////////////////////////////////////////

}  // namespace lex

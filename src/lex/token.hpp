#pragma once

#include <lex/error.hpp>
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
  Token(TokenType type, T&& value, Location loc = {0, 0})
      : type(type),
        value(std::variant<std::string, uint64_t, char, LexError>(
            std::forward<T>(value))),
        location(loc) {
  }

  template <class T>
  const T& Value() const {
    assert(value.has_value());
    assert(std::holds_alternative<T>(value.value()));
    return std::get<T>(value.value());
  }

  TokenType type = TokenType::UNDEFINED;
  std::optional<std::variant<std::string, uint64_t, char, LexError>> value;
  Location location = {0, 0};
};

//////////////////////////////////////////////////////////////////////

}  // namespace lex

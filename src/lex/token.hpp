#pragma once

#include <lex/scanner.hpp>

#include <cstddef>
#include <variant>

namespace lex {

//////////////////////////////////////////////////////////////////////

struct Token {
  TokenType type;
  // TODO consider different types for semantic info
  std::string value;
  Location location;
};

//////////////////////////////////////////////////////////////////////

}  // namespace lex

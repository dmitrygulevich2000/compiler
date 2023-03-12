#pragma once

#include <error.hpp>
#include <lex/location.hpp>

#include <fmt/core.h>

struct LexError : public SyntaxError {
  LexError(const std::string& message, lex::Location loc)
      : SyntaxError(fmt::format("lex error: {}", message), loc) {
  }
};

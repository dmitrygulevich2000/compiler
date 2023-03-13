#pragma once

#include <error.hpp>
#include <lex/location.hpp>

struct RuntimeError : public LocatedError {
  RuntimeError(const std::string& msg, lex::Location loc)
      : LocatedError(fmt::format("runtime error: {}", msg), loc) {
  }
};

struct TypeError : public RuntimeError {
  TypeError(const std::string& msg, lex::Location loc)
      : RuntimeError(fmt::format("type error: {}", msg), loc) {
  }
};

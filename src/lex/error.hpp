#pragma once

#include <lex/location.hpp>

#include <fmt/core.h>

namespace lex {

struct Error {
  Error(const Location& loc, const std::string& message)
      : loc(loc), message(message) {
  }

  std::string Format() const {
    return fmt::format("{} at {}", message, loc);
  }
  friend std::ostream& operator<<(std::ostream& out, const Error& err) {
    out << err.Format();
    return out;
  }

  Location loc;
  std::string message;
};
}  // namespace lex

#pragma once

#include <fmt/core.h>
#include <fmt/ostream.h>

#include <cstddef>
#include <ostream>
#include <string>

namespace lex {

struct Location {
  size_t lineno = 0;
  size_t columnno = 0;

  void Move(size_t count = 1) {
    columnno += count;
  }

  void NewLine() {
    ++lineno;
    columnno = 0;
  }

  std::string Format() const {
    return fmt::format("line = {}, column = {}",  //
                       lineno + 1, columnno + 1);
  }
  friend std::ostream& operator<<(std::ostream& out, const Location& loc) {
    out << loc.lineno + 1 << ":" << loc.columnno + 1;
    return out;
  }
};

}  // namespace lex

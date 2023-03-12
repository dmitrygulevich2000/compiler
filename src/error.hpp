#pragma once

#include <exception>
#include <string>

#include <lex/location.hpp>

struct Error : public std::exception {
  template <typename T>
  T* As() {
    return dynamic_cast<T*>(this);
  }
};

// produced by lexer and parser
struct SyntaxError : public Error {
  SyntaxError(const std::string& msg, lex::Location loc)
      : message(fmt::format("{} at {}", msg, loc)), loc(loc) {
  }

  const char* what() const noexcept override {
    return message.c_str();
  }
  friend std::ostream& operator<<(std::ostream& out, const SyntaxError& err) {
    out << err.message;
    return out;
  }

  std::string message;
  lex::Location loc;
};

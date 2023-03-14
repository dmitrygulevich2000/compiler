#pragma once

#include <exception>
#include <string>

#include <lex/location.hpp>

struct Error : public std::exception {
  explicit Error(std::string msg) : message(std::move(msg)) {
  }

  template <typename T>
  T* As() {
    return dynamic_cast<T*>(this);
  }

  const char* what() const noexcept override {
    return message.c_str();
  }

  std::string message;
};

struct LocatedError : public Error {
  LocatedError(const std::string& msg, lex::Location loc)
      : Error(fmt::format("{} at {}", msg, loc)), loc(loc) {
  }
  lex::Location loc;
};

// produced by lexer and parser
struct SyntaxError : public LocatedError {
  SyntaxError(const std::string& msg, lex::Location loc)
      : LocatedError(msg, loc) {
  }
};

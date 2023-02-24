#pragma once

#include <lex/location.hpp>
#include <lex/token_type.hpp>

#include <fmt/core.h>

#include <filesystem>
#include <iostream>
#include <istream>
#include <span>
#include <string_view>
#include <vector>

namespace lex {

//////////////////////////////////////////////////////////////////////

class Scanner {
  // Your code goes here
 public:
  Scanner(std::istream& source) : source_(source) {
  }

  char GetSymbol() {
    char result = static_cast<char>(source_.get());
    if (result == '\n') {
      location_.NewLine();
    } else {
      location_.Move(1);
    }
    return result;
  }

  bool Match(char ch) {
    if (Eof() || Peek() != ch) {
      return false;
    }
    GetSymbol();
    return true;
  }

  std::string GetLine() {
    std::string result;
    while (!Eof() && Peek() != '\n') {
      result += GetSymbol();
    }

    if (!Eof()) {
      result += GetSymbol();
    }
    return result;
  }

  Location GetLocation() {
    return location_;
  }

  char Peek() const {
    return static_cast<char>(source_.peek());
  }

  bool Eof() const {
    return source_.eof();
  }

 private:
  std::istream& source_;
  Location location_;
};

//////////////////////////////////////////////////////////////////////

}  // namespace lex

#pragma once

#include <lex/token_type.hpp>

#include <map>
#include <string>

#include <fmt/core.h>

namespace lex {

class IdentTable {
 public:
  IdentTable() {
    Populate();
  }

  // Use-of-string-view-for-map-lookup
  // https://stackoverflow.com/questions/35525777

  TokenType LookupWord(const std::string_view word) {
    auto tok = map_.find(word);
    if (tok == map_.end()) {
      return TokenType::IDENT;
    }
    return tok->second;
  }

 private:
  void Populate() {
    map_["true"] = TokenType::TRUE;
    map_["false"] = TokenType::FALSE;
    map_["fun"] = TokenType::FUN;
    map_["var"] = TokenType::VAR;
    map_["if"] = TokenType::IF;
    map_["else"] = TokenType::ELSE;
    map_["for"] = TokenType::FOR;
    map_["return"] = TokenType::RETURN;
    map_["yield"] = TokenType::YIELD;
    map_["struct"] = TokenType::STRUCT;
    map_["match"] = TokenType::MATCH;
    map_["exports"] = TokenType::EXPORTS;

    map_["Int"] = TokenType::TY_INT;
    map_["String"] = TokenType::TY_STRING;
    map_["Bool"] = TokenType::TY_BOOL;
    map_["Unit"] = TokenType::TY_UNIT;
  }

 private:
  // What-are-transparent-comparators
  // https://stackoverflow.com/questions/20317413

  std::map<std::string, TokenType, std::less<>> map_;
};

}  // namespace lex

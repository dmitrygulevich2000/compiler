#pragma once

#include <lex/error.hpp>
#include <lex/ident_table.hpp>
#include <lex/token.hpp>

#include <fmt/format.h>

#include <optional>
#include <string>

namespace lex {

class Lexer {
 public:
  Lexer(std::istream& source);

  Token GetNextToken();
  void Advance();
  bool Matches(lex::TokenType type);

  Token Peek();
  Token GetPreviousToken();

 private:
  void SkipWhitespace();
  void SkipComments();

  Token InvalidHere(Location token_pos, const std::string& error_msg);

  ////////////////////////////////////////////////////////////////////

  Token MatchOperators();

  TokenType MustMatchOperator();

  TokenType MatchEQ(TokenType matched, TokenType single);

  ////////////////////////////////////////////////////////////////////

  std::optional<Token> MatchLiterals();

  Token MatchNumericLiteral();

  Token MatchStringLiteral();

  Token MatchCharLiteral();

  ////////////////////////////////////////////////////////////////////

  std::optional<Token> MatchWords();

  ////////////////////////////////////////////////////////////////////

 private:
  Token prev_{};
  Token peek_{};

  Scanner scanner_;
  IdentTable table_;
};

}  // namespace lex

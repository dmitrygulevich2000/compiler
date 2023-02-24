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

  Token Peek();

  Token GetPreviousToken();

  // Check current token type and maybe consume it.
  bool Matches(lex::TokenType type);

  const std::vector<Error>& Errors() const;
  size_t RecentErrorsCount() const;

 private:
  void SkipWhitespace();

  void SkipComments();

  ////////////////////////////////////////////////////////////////////

  void AddError(const Location& loc, const std::string& message);

  Token InvalidHere() {
    return Token{TokenType::INVALID, "", scanner_.GetLocation()};
  }

  ////////////////////////////////////////////////////////////////////

  Token MustMatchOperators();

  TokenType MustMatchOperator();

  TokenType MatchEQ(TokenType matched, TokenType single);

  ////////////////////////////////////////////////////////////////////

  std::optional<Token> MatchLiterals();

  Token MustMatchNumericLiteral();

  Token MustMatchStringLiteral();

  Token MustMatchCharLiteral();

  ////////////////////////////////////////////////////////////////////

  std::optional<Token> MatchWords();

  ////////////////////////////////////////////////////////////////////

 private:
  // For easy access to locations
  Token prev_{};

  // Current token
  Token peek_{};

  std::vector<Error> errors_;
  size_t recent_errors_count_;

  Scanner scanner_;
  IdentTable table_;
};

}  // namespace lex

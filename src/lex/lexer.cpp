#include <lex/lexer.hpp>

namespace lex {

Lexer::Lexer(std::istream& source) : scanner_{source} {
  GetNextToken();
}

////////////////////////////////////////////////////////////////////

Token Lexer::GetNextToken() {
  prev_ = peek_;

  SkipWhitespace();
  SkipComments();

  if (scanner_.Eof()) {
    peek_ = {TokenType::TOKEN_EOF, "", scanner_.Pos()};
    return prev_;
  }

  if (auto lit = MatchLiterals()) {
    peek_ = *lit;
    return prev_;
  }
  if (auto word = MatchWords()) {
    peek_ = *word;
    return prev_;
  }

  peek_ = MatchOperators();
  return prev_;
}

void Lexer::Advance() {
  GetNextToken();
}

bool Lexer::Matches(lex::TokenType target) {
  Token tok = Peek();
  if (tok.type == target) {
    GetNextToken();
    return true;
  }
  return false;
}

Token Lexer::Peek() {
  return peek_;
}

Token Lexer::GetPreviousToken() {
  return prev_;
}

////////////////////////////////////////////////////////////////////

bool IsWhitespace(char ch) {
  return ch == ' ' || ch == '\n' || ch == '\t';
}
bool IsNumeric(char ch) {
  return ch >= '0' && ch <= '9';
}
bool IsAlpha(char ch) {
  return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '_';
}
bool IsAlphaNumeric(char ch) {
  return IsAlpha(ch) || IsNumeric(ch);
}

////////////////////////////////////////////////////////////////////

void Lexer::SkipWhitespace() {
  while (IsWhitespace(scanner_.Peek())) {
    scanner_.GetSymbol();
  }
}

void Lexer::SkipComments() {
  while (scanner_.Peek() == '#') {
    scanner_.GetLine();
    SkipWhitespace();
  }
}

Token Lexer::InvalidHere(Location token_pos, const std::string& error_msg) {
  return Token{INVALID, LexError(error_msg, scanner_.Pos()), token_pos};
}

////////////////////////////////////////////////////////////////////

Token Lexer::MatchOperators() {
  // Your code goes here
  auto loc = scanner_.Pos();
  try {
    TokenType type = MustMatchOperator();
    return {type, "", loc};
  } catch (LexError& err) {
    return {INVALID, err, loc};
  }
}

TokenType Lexer::MustMatchOperator() {
  auto loc = scanner_.Pos();
  char ch = scanner_.GetSymbol();
  switch (ch) {
    case '+':
      return TokenType::PLUS;
    case '-':
      return TokenType::MINUS;
    case '*':
      return TokenType::STAR;
    case '/':
      return TokenType::DIV;
    case '!':
      return MatchEQ(TokenType::NOT_EQ, TokenType::NOT);
    case '=':
      return MatchEQ(TokenType::EQ, TokenType::ASSIGN);
    case '<':
      return TokenType::LT;
    case '>':
      return TokenType::GT;

    case '(':
      return TokenType::LEFT_PAREN;
    case ')':
      return TokenType::RIGHT_PAREN;
    case '{':
      return TokenType::LEFT_BRACE;
    case '}':
      return TokenType::RIGHT_BRACE;
    case '[':
      return TokenType::LEFT_BRACK;
    case ']':
      return TokenType::RIGHT_BRACK;
    case '.':
      return TokenType::DOT;
    case ',':
      return TokenType::COMMA;
    case ';':
      return TokenType::SEMICOLON;
    case ':':
      return TokenType::COLON;
    case '|':
      return TokenType::PIPE;
    default:
      throw LexError(fmt::format("unexpected symbol {}", ch), loc);
  }
}

TokenType Lexer::MatchEQ(TokenType matched, TokenType single) {
  if (scanner_.Peek() == '=') {
    scanner_.GetSymbol();
    return matched;
  } else {
    return single;
  }
}

////////////////////////////////////////////////////////////////////

std::optional<Token> Lexer::MatchLiterals() {
  char ch = scanner_.Peek();

  if (ch == '\'') {
    return MatchCharLiteral();

  } else if (ch == '"') {
    return MatchStringLiteral();

  } else if (IsNumeric(ch)) {
    return MatchNumericLiteral();
  }

  return {};
}

////////////////////////////////////////////////////////////////////

Token Lexer::MatchNumericLiteral() {
  // Your code goes here
  std::string number;
  auto loc = scanner_.Pos();

  char ch = scanner_.Peek();
  while (!scanner_.Eof() && IsNumeric(ch)) {
    number += scanner_.GetSymbol();
    ch = scanner_.Peek();
  }

  if (number.empty()) {
    return {INVALID, LexError{"not a number", scanner_.Pos()}};
  }
  uint64_t value = std::stoul(number);
  return {TokenType::NUMBER, value, loc};
}

////////////////////////////////////////////////////////////////////

Token Lexer::MatchCharLiteral() {
  auto loc = scanner_.Pos();

  if (!scanner_.Match('\'')) {  // skip left '
    return InvalidHere(loc, "not a char literal");
  }
  if (scanner_.Eof()) {
    return InvalidHere(loc, "unexpected EOF: missing closing quote(')");
  }

  char result = scanner_.GetSymbol();
  if (result == '\'') {
    return {INVALID, LexError("empty char literal", loc), loc};
  }

  if (!scanner_.Match('\'')) {  // skip right '
    return InvalidHere(loc, "missing closing quote(')");
  }
  return {TokenType::CHAR, result, loc};
}

////////////////////////////////////////////////////////////////////

Token Lexer::MatchStringLiteral() {
  std::string str;
  auto loc = scanner_.Pos();

  if (!scanner_.Match('"')) {  // skip left "
    return InvalidHere(loc, "not a string literal");
  }
  if (scanner_.Eof()) {
    return InvalidHere(loc, "unexpected EOF: missing closing quotes(\")");
  }

  char ch = scanner_.Peek();
  while (ch != '"') {
    str += scanner_.GetSymbol();
    if (scanner_.Eof()) {
      return InvalidHere(loc, "unexpected EOF: missing closing quotes(\")");
    }

    ch = scanner_.Peek();
  }

  scanner_.GetSymbol();  // skip right "
  return {TokenType::STRING, str, loc};
}

////////////////////////////////////////////////////////////////////

std::optional<Token> Lexer::MatchWords() {
  std::string name;
  auto loc = scanner_.Pos();

  char ch = scanner_.Peek();
  if (!IsAlpha(ch)) {
    return {};
  }
  name += scanner_.GetSymbol();

  ch = scanner_.Peek();
  while (!scanner_.Eof() && IsAlphaNumeric(ch)) {
    name += scanner_.GetSymbol();
    ch = scanner_.Peek();
  }

  TokenType type = table_.LookupWord(name);
  return Token{type, name, loc};
}

}  // namespace lex

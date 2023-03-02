#include <lex/lexer.hpp>

namespace lex {

Lexer::Lexer(std::istream& source) : scanner_{source} {
  GetNextToken();
}

////////////////////////////////////////////////////////////////////

Token Lexer::GetNextToken() {
  prev_ = peek_;
  recent_errors_count_ = 0;

  SkipWhitespace();
  SkipComments();

  if (scanner_.Eof()) {
    peek_ = {TokenType::TOKEN_EOF, "", scanner_.GetLocation()};
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

  peek_ = MustMatchOperators();
  return prev_;
}

////////////////////////////////////////////////////////////////////

Token Lexer::GetPreviousToken() {
  return prev_;
}

////////////////////////////////////////////////////////////////////

void Lexer::Advance() {
  GetNextToken();
}

////////////////////////////////////////////////////////////////////

bool Lexer::Matches(lex::TokenType target) {
  Token tok = Peek();
  if (tok.type == target) {
    GetNextToken();
    return true;
  }
  return false;
}

////////////////////////////////////////////////////////////////////

Token Lexer::Peek() {
  return peek_;
}

////////////////////////////////////////////////////////////////////

const std::vector<Error>& Lexer::Errors() const {
  return errors_;
}

size_t Lexer::RecentErrorsCount() const {
  return recent_errors_count_;
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

void Lexer::SkipWhitespace() {
  while (IsWhitespace(scanner_.Peek())) {
    scanner_.GetSymbol();
  }
}

////////////////////////////////////////////////////////////////////

void Lexer::SkipComments() {
  while (scanner_.Peek() == '#') {
    scanner_.GetLine();
    SkipWhitespace();
  }
}

////////////////////////////////////////////////////////////////////

void Lexer::AddError(const Location& loc, const std::string& message) {
  errors_.emplace_back(loc, message);
  ++recent_errors_count_;
}

////////////////////////////////////////////////////////////////////

Token Lexer::MustMatchOperators() {
  // Your code goes here
  auto loc = scanner_.GetLocation();
  return {MustMatchOperator(), "", loc};
}

TokenType Lexer::MustMatchOperator() {
  // Your code goes here
  auto loc = scanner_.GetLocation();
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
      return MatchEQ(TokenType::EQUALS, TokenType::ASSIGN);
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
      AddError(loc, fmt::format("unexpected symbol {}", ch));
      return TokenType::INVALID;
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
  // Your code goes here
  char ch = scanner_.Peek();

  if (ch == '\'') {
    return MustMatchCharLiteral();

  } else if (ch == '"') {
    return MustMatchStringLiteral();

  } else if (IsNumeric(ch)) {
    return MustMatchNumericLiteral();
  }

  return {};
}

////////////////////////////////////////////////////////////////////

Token Lexer::MustMatchNumericLiteral() {
  // Your code goes here
  std::string number;
  auto loc = scanner_.GetLocation();

  char ch = scanner_.Peek();
  while (!scanner_.Eof() && IsNumeric(ch)) {
    number += scanner_.GetSymbol();
    ch = scanner_.Peek();
  }

  if (number.empty()) {
    AddError(loc, "not a number");
    return InvalidHere();
  }
  return {TokenType::NUMBER, number, loc};
}

////////////////////////////////////////////////////////////////////

Token Lexer::MustMatchCharLiteral() {
  auto loc = scanner_.GetLocation();

  if (!scanner_.Match('\'')) {  // skip left '
    AddError(loc, "not a char literal");
    return InvalidHere();
  }
  if (scanner_.Eof()) {
    AddError(loc, "unexpected EOF: trailing opening quote(')");
    return InvalidHere();
  }

  char result = scanner_.GetSymbol();
  if (result == '\'') {
    AddError(loc, "empty char literal");
    return InvalidHere();
  }

  if (!scanner_.Match('\'')) {  // skip right '
    AddError(loc, "missing closing quote(')");
    return InvalidHere();
  }
  return {TokenType::CHAR, result, loc};
}

////////////////////////////////////////////////////////////////////

Token Lexer::MustMatchStringLiteral() {
  // Your code goes here
  std::string str;
  auto loc = scanner_.GetLocation();

  if (!scanner_.Match('"')) {  // skip left "
    AddError(loc, "not a string literal");
    return InvalidHere();
  }
  if (scanner_.Eof()) {
    AddError(loc, "unexpected EOF: missing closing quotes(\")");
    return InvalidHere();
  }

  char ch = scanner_.Peek();
  while (ch != '"') {
    str += scanner_.GetSymbol();
    if (scanner_.Eof()) {
      AddError(loc, "unexpected EOF: missing closing quotes(\")");
      return InvalidHere();
    }

    ch = scanner_.Peek();
  }

  scanner_.GetSymbol();  // skip right "
  return {TokenType::STRING, str, loc};
}

////////////////////////////////////////////////////////////////////

std::optional<Token> Lexer::MatchWords() {
  // Your code goes here
  std::string name;
  auto loc = scanner_.GetLocation();

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

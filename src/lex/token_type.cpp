#include <lex/token_type.hpp>

namespace lex {

std::string FormatTokenType(TokenType tt) {
  switch (tt) {
    case TokenType::UNDEFINED:
      return "UNDEFINED";
    case TokenType::INVALID:
      return "INVALID";
    case TokenType::TOKEN_EOF:
      return "EOF";
    case TokenType::NUMBER:
      return "NUMBER";
    case TokenType::STRING:
      return "STRING";
    case TokenType::CHAR:
      return "CHAR";
    case TokenType::PLUS:
      return "+";
    case TokenType::MINUS:
      return "-";
    case TokenType::STAR:
      return "*";
    case TokenType::DIV:
      return "/";
    case TokenType::ASSIGN:
      return "=";
    case TokenType::EQUALS:
      return "==";
    case TokenType::NOT_EQ:
      return "!=";
    case TokenType::NOT:
      return "!";
    case TokenType::LT:
      return "<";
    case TokenType::GT:
      return ">";
    case TokenType::LEFT_PAREN:
      return "(";
    case TokenType::RIGHT_PAREN:
      return ")";
    case TokenType::LEFT_BRACE:
      return "{";
    case TokenType::RIGHT_BRACE:
      return "}";
    case TokenType::LEFT_BRACK:
      return "[";
    case TokenType::RIGHT_BRACK:
      return "]";
    case TokenType::DOT:
      return ".";
    case TokenType::COMMA:
      return ",";
    case TokenType::SEMICOLON:
      return ";";
    case TokenType::COLON:
      return ":";
    case TokenType::PIPE:
      return "|";
    case TokenType::IDENTIFIER:
      return "IDENT";
    case TokenType::TRUE:
      return "true";
    case TokenType::FALSE:
      return "false";
    case TokenType::FUN:
      return "fun";
    case TokenType::VAR:
      return "var";
    case TokenType::IF:
      return "if";
    case TokenType::ELSE:
      return "else";
    case TokenType::FOR:
      return "for";
    case TokenType::RETURN:
      return "return";
    case TokenType::YIELD:
      return "yield";
    case TokenType::STRUCT:
      return "struct";
    case TokenType::MATCH:
      return "match";
    case TokenType::EXPORTS:
      return "exports";
    case TokenType::TY_INT:
      return "TY_Int";
    case TokenType::TY_STRING:
      return "TY_String";
    case TokenType::TY_BOOL:
      return "TY_Bool";
    case TokenType::TY_UNIT:
      return "TY_Unit";
  }
}

std::ostream& operator<<(std::ostream& out, TokenType tt) {
  out << FormatTokenType(tt);
  return out;
}

}  // namespace lex

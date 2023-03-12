#include <parse/error.hpp>
#include <parse/parser.hpp>

Parser::Parser(lex::Lexer& l) : lexer_{l} {
}

std::vector<Decl*> Parser::ParseAll() {
  std::vector<Decl*> result;
  while (Stmt* stmt = ParseStmt()) {
    if (Decl* decl = stmt->As<Decl>()) {
      result.push_back(decl);
    } else {
      // TODO error
    }
    if (lexer_.Matches(lex::TOKEN_EOF)) {
      break;
    }
  }
  return result;
}

// LEFT_PAREN is consumed
std::vector<Expr*> Parser::ParseCSV() {
  std::vector<Expr*> result;
  while (!lexer_.Matches(lex::RIGHT_PAREN)) {
    Expr* elem = ParseExpr();
    // TODO error?
    if (!lexer_.Matches(lex::COMMA)) {
      // TODO error
    }
    result.push_back(elem);
  }

  return result;
}

std::vector<lex::Token> Parser::ParseParams() {
  std::vector<lex::Token> result;
  while (lexer_.Matches(lex::IDENT)) {
    result.push_back(lexer_.GetPreviousToken());
  }
  return result;
}

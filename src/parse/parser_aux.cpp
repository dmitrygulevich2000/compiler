#include <parse/error.hpp>
#include <parse/parser.hpp>

Parser::Parser(lex::Lexer& l) : lexer_{l} {
}

std::vector<Decl*> Parser::ParseAll() {
  std::vector<Decl*> result;
  auto loc = lexer_.Peek().location;
  do {
    Stmt* stmt = ParseStmt();
    if (!stmt) {
      // error happened
      continue;
    }
    if (Decl* decl = stmt->As<Decl>()) {
      result.push_back(decl);
    } else {
      AddError(ParseError("ParseAll meet non-decl", loc));
    }
    loc = lexer_.Peek().location;

  } while (!lexer_.Matches(lex::TOKEN_EOF));
  return result;
}

/////////////////////////////////////////////////////////////////////////

void Parser::AddError(const ParseError& err) {
  errors_.push_back(static_cast<SyntaxError>(err));
}

void Parser::SynchronizeBlock() {
  while (!lexer_.Matches(lex::SEMICOLON) && !lexer_.Matches(lex::TOKEN_EOF)) {
    lex::Token tok = lexer_.Peek();
    lex::TokenType type = tok.type;
    if (type == lex::VAR || type == lex::FUN || type == lex::RIGHT_BRACE) {
      break;
    }
    if (type == lex::INVALID) {
      errors_.push_back(tok.Value<LexError>());
    }

    lexer_.Advance();
  }
}

void Parser::Synchronize() {
  while (!lexer_.Matches(lex::SEMICOLON) && !lexer_.Matches(lex::TOKEN_EOF)) {
    lex::Token tok = lexer_.Peek();
    lex::TokenType type = tok.type;
    if (type == lex::VAR || type == lex::FUN) {
      break;
    }
    if (type == lex::INVALID) {
      errors_.push_back(tok.Value<LexError>());
    }

    lexer_.Advance();
  }
}

// LEFT_PAREN is consumed
std::vector<Expr*> Parser::ParseCSV() {
  std::vector<Expr*> result;
  auto loc = lexer_.Peek().location;
  while (!lexer_.Matches(lex::RIGHT_PAREN)) {
    Expr* elem = ParseExpr();
    if (!elem) {
      throw ParseError("missing expression in (list,)", loc);
    }
    if (!lexer_.Matches(lex::COMMA) && lexer_.Peek().type != lex::RIGHT_PAREN) {
      throw ParseError("expected , between elements", lexer_.Peek().location);
    }
    loc = lexer_.Peek().location;
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

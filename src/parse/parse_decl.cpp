#include <parse/error.hpp>
#include <parse/parser.hpp>

Decl* Parser::ParseDecl() {
  if (VarDecl* var = ParseVarDecl()) {
    return var;
  }
  if (FunDecl* fun = ParseFunDecl()) {
    return fun;
  }
  return nullptr;
}

VarDecl* Parser::ParseVarDecl() {
  if (!lexer_.Matches(lex::VAR)) {
    return nullptr;
  }
  auto var_pos = lexer_.GetPreviousToken().location;
  if (!lexer_.Matches(lex::IDENT)) {
    // TODO error
  }
  lex::Token name = lexer_.GetPreviousToken();

  if (!lexer_.Matches(lex::ASSIGN)) {
    // TODO error
  }

  Expr* definition = ParseExpr();
  // TODO error?
  lexer_.Matches(lex::SEMICOLON);
  return new VarDecl(name, definition, var_pos);
}

FunDecl* Parser::ParseFunDecl() {
  if (!lexer_.Matches(lex::FUN)) {
    return nullptr;
  }
  auto fun_pos = lexer_.GetPreviousToken().location;
  if (!lexer_.Matches(lex::IDENT)) {
    // TODO error
  }
  lex::Token name = lexer_.GetPreviousToken();
  auto params = ParseParams();

  if (!lexer_.Matches(lex::ASSIGN)) {
    // TODO error
  }

  Expr* body = ParseExpr();
  // TODO error?
  lexer_.Matches(lex::SEMICOLON);
  return new FunDecl(name, params, body, fun_pos);
}

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

// TODO need know End() location for better error positioning

VarDecl* Parser::ParseVarDecl() {
  if (!lexer_.Matches(lex::VAR)) {
    return nullptr;
  }
  auto var_pos = lexer_.Peek().location;
  if (!lexer_.Matches(lex::IDENT)) {
    throw ParseError("expected identifier after var", var_pos);
  }
  lex::Token name = lexer_.GetPreviousToken();

  if (!lexer_.Matches(lex::ASSIGN)) {
    throw ParseError("expected assignment", lexer_.Peek().location);
  }

  Expr* definition = ParseExpr();
  if (!definition) {
    throw ParseError("missing definition of var",
                     lexer_.Peek().location);
  }
  if (!lexer_.Matches(lex::SEMICOLON)) {
    throw ParseError("missing ; after var decl",
                     lexer_.Peek().location);
  }
  return new VarDecl(name, definition, var_pos);
}

FunDecl* Parser::ParseFunDecl() {
  if (!lexer_.Matches(lex::FUN)) {
    return nullptr;
  }
  auto fun_pos = lexer_.Peek().location;
  if (!lexer_.Matches(lex::IDENT)) {
    throw ParseError("expected identifier after fun", fun_pos);
  }
  lex::Token name = lexer_.GetPreviousToken();
  auto params = ParseParams();

  if (!lexer_.Matches(lex::ASSIGN)) {
    throw ParseError("expected assignment", lexer_.Peek().location);
  }

  Expr* body = ParseExpr();
  if (!body) {
    throw ParseError("missing definition of fun",
                     lexer_.Peek().location);
  }
  if (!lexer_.Matches(lex::SEMICOLON)) {
    throw ParseError("missing ; after fun decl",
                     lexer_.Peek().location);
  }
  return new FunDecl(name, params, body, fun_pos);
}

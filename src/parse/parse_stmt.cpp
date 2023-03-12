#include <parse/error.hpp>
#include <parse/parser.hpp>

Stmt* Parser::ParseStmt() {
  if (Decl* decl = ParseDecl()) {
    return decl;
  }

  Expr* expr = ParseExpr();
  // TODO error?
  if (lexer_.Matches(lex::SEMICOLON)) {
    return new ExprStmt(expr);
  }
  if (lexer_.Matches(lex::ASSIGN)) {
    auto op = lexer_.GetPreviousToken();
    Expr* rhs = ParseExpr();
    // TODO error?
    lexer_.Matches(lex::SEMICOLON);
    return new AssignmentStmt(op, expr->As<LvalueExpr>(), rhs);
  }

  return expr;
}
Stmt* Parser::ParseForStmt() {
  // TODO impl ParseForStmt
  FMT_ASSERT(false, "unimplemented");
}

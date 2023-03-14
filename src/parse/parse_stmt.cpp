#include <parse/error.hpp>
#include <parse/parser.hpp>

Stmt* Parser::ParseStmtOrThrow() {
  if (Decl* decl = ParseDecl()) {
    return decl;
  }

  Expr* expr = ParseExpr();
  if (!expr) {
    lexer_.Matches(lex::SEMICOLON);
    return nullptr;
  }

  if (lexer_.Matches(lex::SEMICOLON)) {
    return new ExprStmt(expr);
  }

  if (lexer_.Matches(lex::ASSIGN)) {
    auto op = lexer_.GetPreviousToken();
    Expr* rhs = ParseExpr();
    if (!rhs) {
      throw ParseError("missing expr to assign", op.location);
    }
    if (!lexer_.Matches(lex::SEMICOLON)) {
      throw ParseError("missing ; after assignment", op.location);
    }
    if (!expr->As<LvalueExpr>()) {
      throw ParseError("cannot assign to non-lvalue expr", op.location);
    }
    return new AssignmentStmt(op, expr->As<LvalueExpr>(), rhs);
  }

  return expr;
}

Stmt* Parser::ParseStmt() {
  try {
    Stmt* parsed = ParseStmtOrThrow();
    return parsed;
  } catch (ParseError& err) {
    Synchronize();
    AddError(err);
    return nullptr;
  }
}

Stmt* Parser::ParseForStmt() {
  // TODO impl ParseForStmt
  FMT_ASSERT(false, "unimplemented");
}

#include <parse/parser.hpp>
#include <parse/parse_error.hpp>

////////////////////////////////////////////////////////////////////

Expr* Parser::ParseExpr() {
  return ParseComparison();
}

///////////////////////////////////////////////////////////////////

Expr* Parser::ParseKeywordExpresssion() {
  if (auto return_statement = ParseReturnStmt()) {
    return return_statement;
  }

  if (auto yield_statement = ParseYieldStmt()) {
    return yield_statement;
  }

  if (auto if_expr = ParseIfExpr()) {
    return if_expr;
  }

  if (auto match_expr = ParseMatchExpr()) {
    return match_expr;
  }

  if (auto new_expr = ParseNewExpr()) {
    return new_expr;
  }

  return nullptr;
}

///////////////////////////////////////////////////////////////////

Expr* Parser::ParseDeref() {
  std::abort();
}

///////////////////////////////////////////////////////////////////

Expr* Parser::ParseAddressof() {
  std::abort();
}

///////////////////////////////////////////////////////////////////

Expr* Parser::ParseIfExpr() {
  std::abort();  // Your code goes here
}

////////////////////////////////////////////////////////////////////

Expr* Parser::ParseMatchExpr() {
  std::abort();
}

////////////////////////////////////////////////////////////////////

Expr* Parser::ParseNewExpr() {
  std::abort();
}

////////////////////////////////////////////////////////////////////

Expr* Parser::ParseBlockExpr() {
  std::abort();  // Your code goes here
}

////////////////////////////////////////////////////////////////////

Expr* Parser::ParseComparison() {
  std::abort();  // Your code goes here
}

////////////////////////////////////////////////////////////////////

Expr* Parser::ParseBinary() {
  std::abort();  // Your code goes here
}

////////////////////////////////////////////////////////////////////

Expr* Parser::ParseUnary() {
  std::abort();  // Your code goes here
}

///////////////////////////////////////////////////////////////////

// Assume lex::TokenType::ARROW has already been parsed
Expr* Parser::ParseIndirectFieldAccess(Expr* expr) {
}

///////////////////////////////////////////////////////////////////

Expr* Parser::ParseFieldAccess(Expr* expr) {
}

////////////////////////////////////////////////////////////////////

Expr* Parser::ParseIndexingExpr(Expr* expr) {
  std::abort();
}

////////////////////////////////////////////////////////////////////

Expr* Parser::ParseFnCallExpr(Expr* expr, lex::Token id) {
  std::abort();  // Your code goes here
}

////////////////////////////////////////////////////////////////////

Expr* Parser::ParseFnCallUnnamed(Expr* expr) {
  std::abort();
}

////////////////////////////////////////////////////////////////////

Expr* Parser::ParsePostfixExprs() {
  std::abort();  // Your code goes here
}

////////////////////////////////////////////////////////////////////

Expr* Parser::ParsePrimary() {
  // Try parsing grouping first

  // > Your code goes here

  // Then keyword expressions

  // > Your code goes here

  // Then all the base cases: IDENT, INT, TRUE, FALSE, ETC...

  // > Your code goes here

  FMT_ASSERT(false, "Unreachable!");
}

////////////////////////////////////////////////////////////////////

// var t = {.field = 3, .bar = true,};
Expr* Parser::ParseCompoundInitializer(lex::Token curly) {
  std::abort();
}

// Short-hand notation: .<Tag> <Expr>
// e.g: `.some 5` which is the same as `{ .some = 5 }`
Expr* Parser::ParseSignleFieldCompound() {
  std::abort();
}

////////////////////////////////////////////////////////////////////

Expr* Parser::ParseReturnStmt() {
  std::abort();
}

///////////////////////////////////////////////////////////////////

Expr* Parser::ParseYieldStmt() {
  std::abort();
}

///////////////////////////////////////////////////////////////////

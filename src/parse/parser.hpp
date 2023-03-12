#pragma once

#include <ast/ast.hpp>
#include <parse/error.hpp>

#include <lex/lexer.hpp>

class Parser {
 public:
  Parser(lex::Lexer& l);

  std::vector<Decl*> ParseAll();

  const std::vector<SyntaxError>& Errors() {
    return errors_;
  }

  Stmt* ParseStmt();

 private:
  // expressions
  Expr* ParseExpr();
  Expr* ParseEqualityExpr();
  Expr* ParseComparisonExpr();
  Expr* ParseAdditiveExpr();
  Expr* ParseMultiplicativeExpr();
  Expr* ParseUnaryExpr();

  Expr* ParsePostfixExpr();
  // embedded in ParsePostfixExpr
  Expr* ParseFnCallExpr();
  Expr* ParseFieldAccessExpr();

  Expr* ParsePrimaryExpr();
  Expr* ParseLiteralExpr();
  Expr* ParseVarAccessExpr();

  Expr* ParseKeywordExpr();
  Expr* ParseReturnExpr();
  Expr* ParseBlockExpr();
  Expr* ParseIfExpr();

  // statements
  // embedded in ParseStmt
  ExprStmt* ParseExprStmt();
  AssignmentStmt* ParseAssignmentStmt();
  Stmt* ParseForStmt();

  // declarations
  Decl* ParseDecl();
  VarDecl* ParseVarDecl();
  FunDecl* ParseFunDecl();

  // helpers
  std::vector<Expr*> ParseCSV();
  std::vector<lex::Token> ParseParams();

 private:
  lex::Lexer& lexer_;
  std::vector<SyntaxError> errors_;
};

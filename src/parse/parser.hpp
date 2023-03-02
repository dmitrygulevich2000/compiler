#pragma once

#include <ast/declarations.hpp>

#include <lex/lexer.hpp>

class Parser {
 public:
  Parser(lex::Lexer& l);

  ///////////////////////////////////////////////////////////////////


  ///////////////////////////////////////////////////////////////////

  Stmt* ParseStmt();

  Stmt* ParseExprStmt();
  AssignmentStmt* ParseAssignment(LvalueExpr* target);

  ////////////////////////////////////////////////////////////////////

  Decl* ParseDecl();

  Decl* ParsePrototype();
  FunDecl* ParseFunPrototype();
  FunDecl* ParseFunDecl();
  VarDecl* ParseVarDecl();
  FunDecl* ParseFunDeclStandalone();

  ////////////////////////////////////////////////////////////////////

  Expr* ParseExpr();

  Expr* ParseKeywordExpresssion();

  Expr* ParseReturnStmt();
  Expr* ParseYieldStmt();
  Expr* ParseIfExpr();
  Expr* ParseMatchExpr();
  Expr* ParseNewExpr();

  Expr* ParseBlockExpr();

  Expr* ParseComparison();
  Expr* ParseBinary();

  Expr* ParseUnary();
  Expr* ParseDeref();
  Expr* ParseAddressof();

  // Precedence 1
  Expr* ParsePostfixExprs();
  Expr* ParseFieldAccess(Expr* expr);
  Expr* ParseIndirectFieldAccess(Expr* expr);
  Expr* ParseIndexingExpr(Expr* expr);
  Expr* ParseFnCallUnnamed(Expr* expr);
  Expr* ParseFnCallExpr(Expr* expr, lex::Token id);

  Expr* ParseCompoundInitializer(lex::Token id);
  Expr* ParseSignleFieldCompound();
  Expr* ParsePrimary();

  ////////////////////////////////////////////////////////////////////

  ////////////////////////////////////////////////////////////////////

 private:
  std::string FormatLocation();

  auto ParseCSV() -> std::vector<Expr*>;
  auto ParseFormals() -> std::vector<lex::Token>;

  bool Matches(lex::TokenType type);
  void Consume(lex::TokenType type);
  bool MatchesComparisonSign(lex::TokenType type);

 private:
  lex::Lexer& lexer_;
};

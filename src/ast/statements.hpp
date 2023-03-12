#pragma once

#include <ast/tree_node.hpp>

#include <lex/token.hpp>

#include <vector>

//////////////////////////////////////////////////////////////////////

class Stmt : public TreeNode {};

//////////////////////////////////////////////////////////////////////

class ExprStmt : public Stmt {
 public:
  explicit ExprStmt(Expr* expr) : expr(expr) {
  }

  void Accept(Visitor* v) override {
    return v->VisitExprStmt(this);
  }

  lex::Location GetLocation();

  Expr* expr;
};

//////////////////////////////////////////////////////////////////////

class AssignmentStmt : public Stmt {
 public:
  AssignmentStmt(lex::Token op, LvalueExpr* lhs, Expr* rhs)
      : op(std::move(op)), lhs(lhs), rhs(rhs) {
  }

  void Accept(Visitor* v) override {
    return v->VisitAssignmentStmt(this);
  }

  lex::Location GetLocation() override {
    return op.location;
  }

  lex::Token op;  // now only "="
  LvalueExpr* lhs;
  Expr* rhs;
};

//////////////////////////////////////////////////////////////////////

#pragma once

#include <ast/statements.hpp>

#include <lex/token.hpp>

#include <variant>
#include <vector>
#include "statements.hpp"

//////////////////////////////////////////////////////////////////////

class Expr : public Stmt {
 public:
  // Later

  // virtual types::Type* GetType() = 0;
};

//// operations priority (from high to low):
// https://en.cppreference.com/w/c/language/operator_precedence
// . -> fn(call)
// - !
// * / %
// + -
// < <= > >=
// == !=

//// order of parsing expressions (operands of lower are higher (or exprs)):
// ident constant (expr)
// . -> fn(expr,)
// ^
// |                        return yield
// |----------------------->if match
// |                        {}-block
// |
// - !
// * / %
// + -
// < <= > >=
// == !=

//////////////////////////////////////////////////////////////////////

// Assignable entity
class LvalueExpr : public Expr {};

//////////////////////////////////////////////////////////////////////

// == !=
class EqualityExpr : public Expr {
 public:
  EqualityExpr(lex::Token op, Expr* lhs, Expr* rhs)
      : op(std::move(op)), lhs(lhs), rhs(rhs) {
  }

  virtual void Accept(Visitor* v) override {
    return v->VisitEqualityExpr(this);
  }

  virtual lex::Location GetLocation() override {
    return op.location;
  }

  lex::Token op;
  Expr* lhs;
  Expr* rhs;
};

//////////////////////////////////////////////////////////////////////

// < <= > >=
class ComparisonExpr : public Expr {
 public:
  ComparisonExpr(lex::Token op, Expr* lhs, Expr* rhs)
      : op(std::move(op)), lhs(lhs), rhs(rhs) {
  }

  virtual void Accept(Visitor* v) override {
    return v->VisitComparisonExpr(this);
  }

  virtual lex::Location GetLocation() override {
    return op.location;
  }

  lex::Token op;
  Expr* lhs;
  Expr* rhs;
};

//////////////////////////////////////////////////////////////////////

// + -
class AdditiveExpr : public Expr {
 public:
  AdditiveExpr(lex::Token op, Expr* lhs, Expr* rhs)
      : op(std::move(op)), lhs(lhs), rhs(rhs) {
  }

  virtual void Accept(Visitor* v) override {
    return v->VisitAdditiveExpr(this);
  }

  virtual lex::Location GetLocation() override {
    return op.location;
  }

  lex::Token op;
  Expr* lhs;
  Expr* rhs;
};

//////////////////////////////////////////////////////////////////////

// * / %
class MultiplicativeExpr : public Expr {
 public:
  MultiplicativeExpr(lex::Token op, Expr* lhs, Expr* rhs)
      : op(std::move(op)), lhs(lhs), rhs(rhs) {
  }

  virtual void Accept(Visitor* v) override {
    return v->VisitMultiplicativeExpr(this);
  }

  virtual lex::Location GetLocation() override {
    return op.location;
  }

  lex::Token op;
  Expr* lhs;
  Expr* rhs;
};

//////////////////////////////////////////////////////////////////////

// - !
class UnaryExpr : public Expr {
 public:
  UnaryExpr(lex::Token op, Expr* operand)
      : op(std::move(op)), operand(operand) {
  }

  virtual void Accept(Visitor* v) override {
    return v->VisitUnaryExpr(this);
  }

  virtual lex::Location GetLocation() override {
    return op.location;
  }

  lex::Token op;
  Expr* operand;
};

//////////////////////////////////////////////////////////////////////

// fn(call)
class FnCallExpr : public LvalueExpr {
 public:
  FnCallExpr(Expr* callable, const std::vector<Expr*>& args)
      : callable(callable), args(args) {
  }

  virtual void Accept(Visitor* v) override {
    return v->VisitFnCallExpr(this);
  }

  virtual lex::Location GetLocation() override {
    return callable->GetLocation();
  }

  Expr* callable;
  std::vector<Expr*> args;
};

//////////////////////////////////////////////////////////////////////

// id.fid
class FieldAccessExpr : public LvalueExpr {
 public:
  FieldAccessExpr(Expr* obj, lex::Token field,
                  lex::Location dot_pos = lex::Location{})
      : object(obj), field(std::move(field)), dot_pos(dot_pos) {
  }

  void Accept(Visitor* v) override {
    return v->VisitFieldAccessExpr(this);
  }

  lex::Location GetLocation() override {
    return dot_pos;
  }

  Expr* object;
  lex::Token field;
  lex::Location dot_pos;
};

//////////////////////////////////////////////////////////////////////

// "abc" 'a' 123 true false
class LiteralExpr : public Expr {
 public:
  explicit LiteralExpr(lex::Token lit) : lit(std::move(lit)) {
  }

  virtual void Accept(Visitor* v) override {
    return v->VisitLiteralExpr(this);
  }

  virtual lex::Location GetLocation() override {
    return lit.location;
  }

  lex::Token lit;
};

//////////////////////////////////////////////////////////////////////

// i
class VarAccessExpr : public LvalueExpr {
 public:
  explicit VarAccessExpr(lex::Token ident) : ident(std::move(ident)) {
  }

  virtual void Accept(Visitor* v) override {
    return v->VisitVarAccessExpr(this);
  }

  virtual lex::Location GetLocation() override {
    return ident.location;
  }

  lex::Token ident;
};

//////////////////////////////////////////////////////////////////////

class IfExpr : public Expr {
 public:
  IfExpr(VarDecl* decl, Expr* condition, Expr* then_expr, Expr* else_expr,
         lex::Location if_pos = {0, 0})
      : decl(decl),
        condition(condition),
        then_expr(then_expr),
        else_expr(else_expr),
        if_pos(if_pos) {
  }
  IfExpr(Expr* condition, Expr* then_expr, Expr* else_expr,
         lex::Location if_pos = {0, 0})
      : condition(condition),
        then_expr(then_expr),
        else_expr(else_expr),
        if_pos(if_pos) {
  }

  virtual void Accept(Visitor* v) override {
    return v->VisitIfExpr(this);
  }

  virtual lex::Location GetLocation() override {
    return if_pos;
  }

  VarDecl* decl = nullptr;
  Expr* condition;
  Expr* then_expr;
  Expr* else_expr = nullptr;
  lex::Location if_pos;
};

//////////////////////////////////////////////////////////////////////

class BlockExpr : public Expr {
 public:
  BlockExpr(std::vector<Stmt*> flow, Expr* end_expr = nullptr,
            lex::Location lbrace_pos = {0, 0})
      : flow(std::move(flow)), end_expr(end_expr), lbrace_pos(lbrace_pos) {
  }

  virtual void Accept(Visitor* v) override {
    return v->VisitBlockExpr(this);
  }

  virtual lex::Location GetLocation() override {
    return lbrace_pos;
  }

  std::vector<Stmt*> flow;
  Expr* end_expr = nullptr;
  lex::Location lbrace_pos;
};

//////////////////////////////////////////////////////////////////////

class ReturnExpr : public Expr {
 public:
  explicit ReturnExpr(Expr* returned, lex::Location ret_pos = {0, 0})
      : returned(returned), ret_pos(ret_pos) {
  }

  virtual void Accept(Visitor* v) override {
    return v->VisitReturnExpr(this);
  }

  virtual lex::Location GetLocation() override {
    return ret_pos;
  }

  Expr* returned;
  lex::Location ret_pos;
};

//////////////////////////////////////////////////////////////////////

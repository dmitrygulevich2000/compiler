#pragma once

#include <string>

//////////////////////////////////////////////////////////////////////

class Expr;
class EqualityExpr;
class ComparisonExpr;
class AdditiveExpr;
class MultiplicativeExpr;
class UnaryExpr;
// postfix expr
class FnCallExpr;
class FieldAccessExpr;
// primary expr
class LiteralExpr;
class VarAccessExpr;
// keyword expr
class IfExpr;
class BlockExpr;
class ReturnExpr;

//////////////////////////////////////////////////////////////////////

class Stmt;
class ExprStmt;
class AssignmentStmt;
class ForStmt;

//////////////////////////////////////////////////////////////////////

class Decl;  // is Stmt
class VarDecl;
class FunDecl;

//////////////////////////////////////////////////////////////////////

class Visitor {
 public:
  virtual ~Visitor() = default;

  // expressions
  virtual void VisitEqualityExpr(EqualityExpr* expr) = 0;
  virtual void VisitComparisonExpr(ComparisonExpr* expr) = 0;
  virtual void VisitAdditiveExpr(AdditiveExpr* expr) = 0;
  virtual void VisitMultiplicativeExpr(MultiplicativeExpr* expr) = 0;
  virtual void VisitUnaryExpr(UnaryExpr* expr) = 0;

  virtual void VisitFnCallExpr(FnCallExpr* expr) = 0;
  virtual void VisitFieldAccessExpr(FieldAccessExpr* expr) = 0;
  virtual void VisitLiteralExpr(LiteralExpr* expr) = 0;
  virtual void VisitVarAccessExpr(VarAccessExpr* expr) = 0;

  virtual void VisitIfExpr(IfExpr* expr) = 0;
  virtual void VisitBlockExpr(BlockExpr* expr) = 0;
  virtual void VisitReturnExpr(ReturnExpr* expr) = 0;

  // statements
  virtual void VisitExprStmt(ExprStmt* stmt) = 0;
  virtual void VisitAssignmentStmt(AssignmentStmt* stmt) = 0;
  virtual void VisitForStmt(ForStmt* stmt) = 0;

  // declarations
  virtual void VisitVarDecl(VarDecl* decl) = 0;
  virtual void VisitFunDecl(FunDecl* decl) = 0;
};

//////////////////////////////////////////////////////////////////////

#pragma once

#include <ast/expressions.hpp>
#include <ast/visitors/visitor.hpp>

#include <ostream>
#include <string>

class ConsolePrinter : public Visitor {
 public:
  ConsolePrinter(std::ostream& out, size_t depth = 0)
      : out(out), depth_(depth) {
  }

  // expressions
  virtual void VisitEqualityExpr(EqualityExpr* expr) {
    out << Prefix() << expr->op.type << std::endl;
    auto into = Into();  // RAII for ++depth_ :)
    expr->lhs->Accept(this);
    expr->rhs->Accept(this);
  };
  virtual void VisitComparisonExpr(ComparisonExpr* expr) {
    out << Prefix() << expr->op.type << std::endl;
    auto into = Into();
    expr->lhs->Accept(this);
    expr->rhs->Accept(this);
  };
  virtual void VisitAdditiveExpr(AdditiveExpr* expr) {
    out << Prefix() << expr->op.type << std::endl;
    auto into = Into();
    expr->lhs->Accept(this);
    expr->rhs->Accept(this);
  };
  virtual void VisitMultiplicativeExpr(MultiplicativeExpr* expr) {
    out << Prefix() << expr->op.type << std::endl;
    auto into = Into();
    expr->lhs->Accept(this);
    expr->rhs->Accept(this);
  };
  virtual void VisitUnaryExpr(UnaryExpr* expr) {
    out << Prefix() << expr->op.type << std::endl;
    auto into = Into();
    expr->operand->Accept(this);
  };

  virtual void VisitFnCallExpr(FnCallExpr* expr) {
    out << Prefix() << "CALL" << std::endl;

    auto into1 = Into();
    expr->callable->Accept(this);
    out << Prefix() << "()" << std::endl;

    auto into2 = Into();
    for (Expr* arg : expr->args) {
      arg->Accept(this);
    }
  }
  virtual void VisitFieldAccessExpr(FieldAccessExpr* expr) {
    out << Prefix() << "ACCESS" << std::endl;
    auto into = Into();

    expr->object->Accept(this);
    out << Prefix() << "." << std::endl;
    out << Prefix() << expr->field.Value<std::string>() << std::endl;
  }
  virtual void VisitLiteralExpr(LiteralExpr* expr) {
    out << Prefix();
    switch (expr->lit.type) {
      case lex::STRING:
        out << "\"" << expr->lit.Value<std::string>() << "\"";
        break;
      case lex::NUMBER:
        out << expr->lit.Value<uint64_t>();
        break;
      case lex::CHAR:
        out << expr->lit.Value<char>();
        break;
      case lex::TRUE:
        out << "true";
        break;
      case lex::FALSE:
        out << "false";
        break;
      default:
        FMT_ASSERT(false, "unexpected token type in LiteralExpr");
    }
    out << std::endl;
  }
  virtual void VisitVarAccessExpr(VarAccessExpr* expr) {
    out << Prefix() << expr->ident.Value<std::string>() << std::endl;
  }

  virtual void VisitIfExpr(IfExpr* expr) {
    out << Prefix() << "if" << std::endl;
    auto into = Into();
    if (expr->decl) {
      expr->decl->Accept(this);
    }
    expr->condition->Accept(this);
    expr->then_expr->Accept(this);
    if (expr->else_expr) {
      expr->else_expr->Accept(this);
    }
  }
  virtual void VisitBlockExpr(BlockExpr* expr) {
    out << Prefix() << "{}" << std::endl;
    auto into = Into();
    for (Stmt* elem : expr->flow) {
      elem->Accept(this);
    }
    if (expr->end_expr) {
      expr->end_expr->Accept(this);
    }
  }
  virtual void VisitReturnExpr(ReturnExpr* expr) {
    out << Prefix() << "return" << std::endl;
    auto into = Into();
    expr->returned->Accept(this);
  }

  // statements
  virtual void VisitExprStmt(ExprStmt* stmt) {
    out << Prefix() << ";" << std::endl;
    stmt->expr->Accept(this);
  }
  virtual void VisitAssignmentStmt(AssignmentStmt* stmt) {
    out << Prefix() << stmt->op.type << std::endl;
    auto into = Into();
    stmt->lhs->Accept(this);
    stmt->rhs->Accept(this);
  }
  virtual void VisitForStmt(ForStmt* stmt) {
    throw std::runtime_error("VisitForStmt not implemented in ConsolePrinter");
  }

  // declarations
  virtual void VisitVarDecl(VarDecl* decl) {
    out << Prefix() << "var" << std::endl;
    auto into = Into();
    out << Prefix() << decl->ident.Value<std::string>() << std::endl;
    if (decl->type_name) {
      out << Prefix() << decl->type_name.value().Value<std::string>()
          << std::endl;
    }
    decl->definition->Accept(this);
  }
  virtual void VisitFunDecl(FunDecl* decl) {
    out << Prefix() << "func" << std::endl;
    auto into = Into();
    out << Prefix() << decl->ident.Value<std::string>() << std::endl;

    {
      out << Prefix() << "()" << std::endl;
      auto into = Into();
      for (const lex::Token& arg : decl->args) {
        out << Prefix() << arg.Value<std::string>() << std::endl;
      }
    }
    decl->definition->Accept(this);
  }

 private:
  std::string Prefix() const {
    std::string result(2 * depth_, ' ');
    for (size_t i = 0; i < result.size(); i += 2) {
      result[i] = '|';
    }
    if (!result.empty()) {
      result.back() = '_';
    }
    return result;
  }

  struct HelperInto {
    HelperInto(ConsolePrinter* p) : p(p) {
      ++p->depth_;
    }
    ~HelperInto() {
      --p->depth_;
    }

    ConsolePrinter* p;
  };
  HelperInto Into() {
    return {this};
  }

  std::ostream& out;
  size_t depth_;
};

#pragma once

#include <cassert>
#include <optional>
#include <variant>

#include <ast/declarations.hpp>
#include <ast/expressions.hpp>
#include <ast/visitor/visitor.hpp>
#include <type/type.hpp>

#include "./error.hpp"

struct TypedValue {
  TypedValue() = default;
  explicit TypedValue(Type type) : type(type) {
  }
  template <class T>
  TypedValue(Type type, T&& value)
      : type(type),
        value(std::variant<int64_t, bool, std::string, char>(
            std::forward<T>(value))) {
  }

  template <class T>
  const T& As() const {
    assert(value.has_value());
    assert(std::holds_alternative<T>(value.value()));
    return std::get<T>(value.value());
  }

  Type type;
  // TODO maybe also char[] and void*
  std::optional<std::variant<int64_t, bool, std::string, char>> value;
};

std::ostream& operator<<(std::ostream& out, const TypedValue& val) {
  if (val.type.tag == TY_INT) {
    out << val.As<int64_t>();
  }
  if (val.type.tag == TY_BOOL) {
    out << std::boolalpha << val.As<bool>();
  }
  if (val.type.tag == TY_STRING) {
    out << "\"" << val.As<std::string>() << "\"";
  }
  if (val.type.tag == TY_CHAR) {
    out << "'" << val.As<char>() << "'";
  }
  if (val.type.tag == TY_NEVER) {
    out << "!";
  }
  if (val.type.tag == TY_UNIT) {
    out << "()";
  }
  return out;
}

/////////////////////////////////////////////////////////////////////////////

class Interpreter : public Visitor {
 public:
  const TypedValue& GetResult() {
    return result_;
  }

  void VisitEqualityExpr(EqualityExpr* expr) override {
    auto lhs = Visit(expr->lhs);
    auto rhs = Visit(expr->rhs);
    if (!Same(lhs.type, rhs.type)) {
      throw TypeError("equality operands of different type",
                      expr->GetLocation());
    }

    if (lhs.type.tag == TY_INT) {
      result_ = {builtin_bool, SwitchEqComp(expr->op.type, lhs.As<int64_t>(),
                                            rhs.As<int64_t>())};
      return;
    }
    if (lhs.type.tag == TY_BOOL) {
      result_ = {builtin_bool, lhs.As<bool>() == rhs.As<bool>()};
      return;
    }
    if (lhs.type.tag == TY_STRING) {
      result_ = {builtin_bool,
                 SwitchEqComp(expr->op.type, lhs.As<std::string>(),
                              rhs.As<std::string>())};
      return;
    }
    if (lhs.type.tag == TY_CHAR) {
      result_ = {builtin_bool,
                 SwitchEqComp(expr->op.type, lhs.As<char>(), rhs.As<char>())};
      return;
    }
    throw TypeError("equality operands of unsupported type",
                    expr->GetLocation());
  }
  void VisitComparisonExpr(ComparisonExpr* expr) override {
    auto lhs = Visit(expr->lhs);
    auto rhs = Visit(expr->rhs);
    if (!Same(lhs.type, rhs.type)) {
      throw TypeError("comparison operands of different type",
                      expr->GetLocation());
    }

    if (lhs.type.tag == TY_INT) {
      result_ = {builtin_bool, SwitchEqComp(expr->op.type, lhs.As<int64_t>(),
                                            rhs.As<int64_t>())};
      return;
    }
    if (lhs.type.tag == TY_BOOL) {
      result_ = {builtin_bool, lhs.As<bool>() == rhs.As<bool>()};
      return;
    }
    if (lhs.type.tag == TY_STRING) {
      result_ = {builtin_bool,
                 SwitchEqComp(expr->op.type, lhs.As<std::string>(),
                              rhs.As<std::string>())};
      return;
    }
    if (lhs.type.tag == TY_CHAR) {
      result_ = {builtin_bool,
                 SwitchEqComp(expr->op.type, lhs.As<char>(), rhs.As<char>())};
      return;
    }
    throw TypeError("comparison operands of unsupported type",
                    expr->GetLocation());
  }
  void VisitAdditiveExpr(AdditiveExpr* expr) override {
    auto lhs = Visit(expr->lhs);
    auto rhs = Visit(expr->rhs);
    if (!Same(lhs.type, rhs.type)) {
      throw TypeError("additive operands of different type",
                      expr->GetLocation());
    }

    if (lhs.type.tag == TY_INT) {
      result_ = {builtin_int, SwitchMulAdd(expr->op.type, lhs.As<int64_t>(),
                                           rhs.As<int64_t>())};
      return;
    }
    if (lhs.type.tag == TY_STRING && expr->op.type == lex::PLUS) {
      result_ = {builtin_string, lhs.As<std::string>() + rhs.As<std::string>()};
      return;
    }
    throw TypeError("additive operands of unsupported type",
                    expr->GetLocation());
  }
  void VisitMultiplicativeExpr(MultiplicativeExpr* expr) override {
    auto lhs = Visit(expr->lhs);
    auto rhs = Visit(expr->rhs);
    if (!Same(lhs.type, rhs.type)) {
      throw TypeError("multiplicative operands of different type",
                      expr->GetLocation());
    }

    if (lhs.type.tag == TY_INT) {
      result_ = {builtin_int, SwitchMulAdd(expr->op.type, lhs.As<int64_t>(),
                                           rhs.As<int64_t>())};
      return;
    }
    if (lhs.type.tag == TY_STRING && expr->op.type == lex::PLUS) {
      result_ = {builtin_string, lhs.As<std::string>() + rhs.As<std::string>()};
      return;
    }
    throw TypeError("multiplicative operands of unsupported type",
                    expr->GetLocation());
  }
  void VisitUnaryExpr(UnaryExpr* expr) override {
    auto operand = Visit(expr->operand);

    if (operand.type.tag == TY_INT && expr->op.type == lex::MINUS) {
      result_ = {builtin_int, -operand.As<int64_t>()};
      return;
    }
    if (operand.type.tag == TY_BOOL && expr->op.type == lex::NOT) {
      result_ = {builtin_bool, !operand.As<bool>()};
      return;
    }
    throw TypeError("unary operand of unsupported type", expr->GetLocation());
  }

  void VisitFnCallExpr(FnCallExpr* expr) override {
    throw std::runtime_error("unimplemented");
  }
  void VisitFieldAccessExpr(FieldAccessExpr* expr) override {
    throw std::runtime_error("unimplemented");
  }
  void VisitLiteralExpr(LiteralExpr* expr) override {
    switch (expr->lit.type) {
      case lex::STRING:
        result_ = {builtin_string, expr->lit.Value<std::string>()};
        break;
      case lex::NUMBER:
        result_ = {builtin_int,
                   static_cast<int64_t>(expr->lit.Value<uint64_t>())};
        break;
      case lex::CHAR:
        result_ = {builtin_char, expr->lit.Value<char>()};
        break;
      case lex::TRUE:
        result_ = {builtin_bool, true};
        break;
      case lex::FALSE:
        result_ = {builtin_bool, false};
        break;
      default:
        FMT_ASSERT(false, "unexpected token type in LiteralExpr");
    }
  };
  void VisitVarAccessExpr(VarAccessExpr* expr) override {
    throw std::runtime_error("unimplemented");
  }

  void VisitIfExpr(IfExpr* expr) override {
    throw std::runtime_error("unimplemented");
  }
  void VisitBlockExpr(BlockExpr* expr) override {
    for (Stmt* stmt : expr->flow) {
      Visit(stmt);
    }
    if (expr->end_expr) {
      Visit(expr->end_expr);
    } else {
      result_ = TypedValue{Type{TY_UNIT}};
    }
  }
  void VisitReturnExpr(ReturnExpr* expr) override {
    throw std::runtime_error("unimplemented");
  }

  // statements
  void VisitExprStmt(ExprStmt* stmt) override {
    Visit(stmt->expr);
    result_ = TypedValue{Type{TY_UNIT}};
  }
  void VisitAssignmentStmt(AssignmentStmt* stmt) override {
    throw std::runtime_error("unimplemented");
  }
  void VisitForStmt(ForStmt* stmt) override {
    throw std::runtime_error("unimplemented");
  }

  // declarations
  void VisitVarDecl(VarDecl* decl) override {
    throw std::runtime_error("unimplemented");
  }
  void VisitFunDecl(FunDecl* decl) override {
    throw std::runtime_error("unimplemented");
  }

 private:
  TypedValue& Visit(Stmt* any) {
    any->Accept(this);
    return result_;
  }
  template <class T>
  bool SwitchEqComp(lex::TokenType op, T lhs, T rhs) {
    switch (op) {
      case lex::EQ:
        return lhs == rhs;
      case lex::NOT_EQ:
        return lhs != rhs;
      case lex::LT:
        return lhs < rhs;
      case lex::LEQ:
        return lhs <= rhs;
      case lex::GT:
        return lhs > rhs;
      case lex::GEQ:
        return lhs >= rhs;
      default:
        FMT_ASSERT(false, "unreachable");
    }
  }
  int64_t SwitchMulAdd(lex::TokenType op, int64_t lhs, int64_t rhs) {
    switch (op) {
      case lex::PLUS:
        return lhs + rhs;
      case lex::MINUS:
        return lhs - rhs;
      case lex::STAR:
        return lhs * rhs;
      case lex::DIV:
        return lhs / rhs;
      case lex::REM:
        return lhs % rhs;
      default:
        FMT_ASSERT(false, "unreachable");
    }
  }

  static bool Same(const Type& ty_one, const Type& ty_two) {
    return ty_one.tag == ty_two.tag;
  }

  TypedValue result_;
};

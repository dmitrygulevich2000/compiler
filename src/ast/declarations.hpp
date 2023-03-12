#pragma once

#include <ast/statements.hpp>

#include <lex/token.hpp>

#include <vector>

//////////////////////////////////////////////////////////////////////

class Decl : public Stmt {
 public:
  virtual std::string_view GetName() = 0;
};

//////////////////////////////////////////////////////////////////////

class VarDecl : public Decl {
 public:
  VarDecl(lex::Token ident, Expr* definition, lex::Location var_pos = {0, 0})
      : ident(std::move(ident)), definition(definition), var_pos(var_pos) {
  }

  void Accept(Visitor* v) override {
    return v->VisitVarDecl(this);
  }

  lex::Location GetLocation() override {
    return var_pos;
  }

  std::string_view GetName() override {
    return {ident.Value<std::string>()};
  }

  lex::Token ident;
  // TODO support type?
  Expr* definition = nullptr;
  lex::Location var_pos;
};

//////////////////////////////////////////////////////////////////////

class FunDecl : public Decl {
 public:
  FunDecl(lex::Token ident, std::vector<lex::Token> args, Expr* definition,
          lex::Location fun_pos = {0, 0})
      : ident(ident),
        args(std::move(args)),
        definition(definition),
        fun_pos(fun_pos) {
  }

  void Accept(Visitor* v) override {
    return v->VisitFunDecl(this);
  }

  lex::Location GetLocation() override {
    return fun_pos;
  }

  std::string_view GetName() override {
    return {ident.Value<std::string>()};
  }

  lex::Token ident;
  // TODO support types?
  std::vector<lex::Token> args;
  Expr* definition;
  lex::Location fun_pos;
};

//////////////////////////////////////////////////////////////////////

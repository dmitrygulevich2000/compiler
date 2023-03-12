#include <lex/token_type.hpp>
#include <parse/error.hpp>
#include <parse/parser.hpp>

// TODO handle INVALID tokens & EOF errors

Expr* Parser::ParseExpr() {
  return ParseEqualityExpr();
}

Expr* Parser::ParseEqualityExpr() {
  Expr* lhs = ParseComparisonExpr();

  while (lexer_.Matches(lex::EQ) || lexer_.Matches(lex::NOT_EQ)) {
    lex::Token op = lexer_.GetPreviousToken();
    Expr* rhs = ParseComparisonExpr();
    if (!rhs) {
      // TODO: error
    }
    lhs = new EqualityExpr(op, lhs, rhs);
  }

  return lhs;
}
Expr* Parser::ParseComparisonExpr() {
  Expr* lhs = ParseAdditiveExpr();

  while (lexer_.Matches(lex::LT) || lexer_.Matches(lex::LEQ) ||
         lexer_.Matches(lex::GT) || lexer_.Matches(lex::GEQ)) {
    lex::Token op = lexer_.GetPreviousToken();
    Expr* rhs = ParseAdditiveExpr();
    if (!rhs) {
      // TODO: error
    }
    lhs = new ComparisonExpr(op, lhs, rhs);
  }

  return lhs;
}
Expr* Parser::ParseAdditiveExpr() {
  Expr* lhs = ParseMultiplicativeExpr();

  while (lexer_.Matches(lex::PLUS) || lexer_.Matches(lex::MINUS)) {
    lex::Token op = lexer_.GetPreviousToken();
    Expr* rhs = ParseMultiplicativeExpr();
    if (!rhs) {
      // TODO: error
    }
    lhs = new AdditiveExpr(op, lhs, rhs);
  }

  return lhs;
}
Expr* Parser::ParseMultiplicativeExpr() {
  Expr* lhs = ParseUnaryExpr();

  while (lexer_.Matches(lex::STAR) || lexer_.Matches(lex::DIV) ||
         lexer_.Matches(lex::REM)) {
    lex::Token op = lexer_.GetPreviousToken();
    Expr* rhs = ParseUnaryExpr();
    if (!rhs) {
      // TODO: error
    }
    lhs = new MultiplicativeExpr(op, lhs, rhs);
  }

  return lhs;
}
Expr* Parser::ParseUnaryExpr() {
  if (lexer_.Matches(lex::MINUS) || lexer_.Matches(lex::NOT)) {
    lex::Token op = lexer_.GetPreviousToken();
    Expr* operand = ParseUnaryExpr();
    if (!operand) {
      // TODO: error
    }
    return new UnaryExpr(op, operand);
  }

  if (Expr* postfix = ParsePostfixExpr()) {
    return postfix;
  }

  if (Expr* keyword = ParseKeywordExpr()) {
    return keyword;
  }

  return nullptr;
}

Expr* Parser::ParsePostfixExpr() {
  Expr* prefix = ParsePrimaryExpr();

  while (lexer_.Matches(lex::LEFT_PAREN) || lexer_.Matches(lex::DOT)) {
    // ParseFnCallExpr
    if (lexer_.GetPreviousToken().type == lex::LEFT_PAREN) {
      auto args = ParseCSV();
      // TODO error?
      prefix = new FnCallExpr(prefix, args);
    }
    // ParseFieldAccessExpr
    if (lexer_.GetPreviousToken().type == lex::DOT) {
      auto field = lexer_.GetNextToken();
      // TODO ensure field is ident
      prefix = new FieldAccessExpr(prefix, field);
    }
  }
  return prefix;
}

Expr* Parser::ParsePrimaryExpr() {
  if (Expr* lit = ParseLiteralExpr()) {
    return lit;
  }
  if (Expr* var = ParseVarAccessExpr()) {
    return var;
  }
  if (lexer_.Matches(lex::LEFT_PAREN)) {
    Expr* expr = ParseExpr();
    // TODO error?
    lexer_.Advance();  // TODO ensure here is RIGHT_PAREN
    return expr;
  }

  return nullptr;
}
Expr* Parser::ParseLiteralExpr() {
  if (lexer_.Matches(lex::NUMBER) || lexer_.Matches(lex::STRING) ||
      lexer_.Matches(lex::CHAR) || lexer_.Matches(lex::TRUE) ||
      lexer_.Matches(lex::FALSE)) {
    return new LiteralExpr(lexer_.GetPreviousToken());
  }

  // TODO handle EOF and INVALID
  return nullptr;
}
Expr* Parser::ParseVarAccessExpr() {
  if (lexer_.Matches(lex::IDENT)) {
    return new VarAccessExpr(lexer_.GetPreviousToken());
  }

  // TODO handle EOF and INVALID
  return nullptr;
}

Expr* Parser::ParseKeywordExpr() {
  if (Expr* ret = ParseReturnExpr()) {
    return ret;
  }
  if (Expr* block = ParseBlockExpr()) {
    return block;
  }
  if (Expr* if_expr = ParseIfExpr()) {
    return if_expr;
  }
  return nullptr;
}

Expr* Parser::ParseReturnExpr() {
  if (!lexer_.Matches(lex::RETURN)) {
    return nullptr;
  }
  auto ret_pos = lexer_.GetPreviousToken().location;
  Expr* returned = ParseExpr();
  // TODO error?
  lexer_.Matches(lex::SEMICOLON);  // skip ; if any
  return new ReturnExpr(returned, ret_pos);
}

Expr* Parser::ParseBlockExpr() {
  if (!lexer_.Matches(lex::LEFT_BRACE)) {
    return nullptr;
  }
  auto lbrace_pos = lexer_.GetPreviousToken().location;

  std::vector<Stmt*> flow;
  while (!lexer_.Matches(lex::RIGHT_BRACE)) {
    Stmt* curr = ParseStmt();
    if (Expr* end_expr = curr->As<Expr>()) {
      lexer_.Advance();  // TODO ensure here is RIGHT_BRACE
      return new BlockExpr(flow, end_expr, lbrace_pos);
    }
    flow.push_back(curr);
  }

  return new BlockExpr(flow, nullptr, lbrace_pos);
}

Expr* Parser::ParseIfExpr() {
  if (!lexer_.Matches(lex::IF)) {
    return nullptr;
  }
  auto if_pos = lexer_.GetPreviousToken().location;

  Stmt* var_decl = ParseStmt();
  // TODO error?
  Expr* cond = nullptr;
  if (Expr* cond_parsed = var_decl->As<Expr>()) {
    cond = cond_parsed;
    var_decl = nullptr;
  } else {
    cond = ParseExpr();
    // TODO error?
  }

  lexer_.Matches(lex::THEN);  // skip then if any
  Expr* then_expr = ParseExpr();
  // TODO error?

  Expr* else_expr = nullptr;
  if (lexer_.Matches(lex::ELSE)) {
    else_expr = ParseExpr();
    // TODO error?
  }

  return new IfExpr(var_decl ? var_decl->As<VarDecl>() : nullptr, cond,
                    then_expr, else_expr, if_pos);
}

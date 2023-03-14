#include <lex/token_type.hpp>
#include <parse/error.hpp>
#include <parse/parser.hpp>

// TODO handle INVALID tokens & EOF errors?
// maybe null result trigger error in all cases

Expr* Parser::ParseExpr() {
  return ParseEqualityExpr();
}

Expr* Parser::ParseEqualityExpr() {
  Expr* lhs = ParseComparisonExpr();
  if (!lhs) {
    return nullptr;
  }

  while (lexer_.Matches(lex::EQ) || lexer_.Matches(lex::NOT_EQ)) {
    lex::Token op = lexer_.GetPreviousToken();
    Expr* rhs = ParseComparisonExpr();
    if (!rhs) {
      throw ParseError(fmt::format("missing rhs of {}", op.type), op.location);
    }
    lhs = new EqualityExpr(op, lhs, rhs);
  }

  return lhs;
}
Expr* Parser::ParseComparisonExpr() {
  Expr* lhs = ParseAdditiveExpr();
  if (!lhs) {
    return nullptr;
  }

  while (lexer_.Matches(lex::LT) || lexer_.Matches(lex::LEQ) ||
         lexer_.Matches(lex::GT) || lexer_.Matches(lex::GEQ)) {
    lex::Token op = lexer_.GetPreviousToken();
    Expr* rhs = ParseAdditiveExpr();
    if (!rhs) {
      throw ParseError(fmt::format("missing rhs of {}", op.type), op.location);
    }
    lhs = new ComparisonExpr(op, lhs, rhs);
  }

  return lhs;
}
Expr* Parser::ParseAdditiveExpr() {
  Expr* lhs = ParseMultiplicativeExpr();
  if (!lhs) {
    return nullptr;
  }

  while (lexer_.Matches(lex::PLUS) || lexer_.Matches(lex::MINUS)) {
    lex::Token op = lexer_.GetPreviousToken();
    Expr* rhs = ParseMultiplicativeExpr();
    if (!rhs) {
      throw ParseError(fmt::format("missing rhs of {}", op.type), op.location);
    }
    lhs = new AdditiveExpr(op, lhs, rhs);
  }

  return lhs;
}
Expr* Parser::ParseMultiplicativeExpr() {
  Expr* lhs = ParseUnaryExpr();
  if (!lhs) {
    return nullptr;
  }

  while (lexer_.Matches(lex::STAR) || lexer_.Matches(lex::DIV) ||
         lexer_.Matches(lex::REM)) {
    lex::Token op = lexer_.GetPreviousToken();
    Expr* rhs = ParseUnaryExpr();
    if (!rhs) {
      throw ParseError(fmt::format("missing rhs of {}", op.type), op.location);
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
      throw ParseError(fmt::format("missing operand of {}", op.type),
                       op.location);
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
  if (!prefix) {
    return nullptr;
  }

  while (lexer_.Matches(lex::LEFT_PAREN) || lexer_.Matches(lex::DOT)) {
    // ParseFnCallExpr
    if (lexer_.GetPreviousToken().type == lex::LEFT_PAREN) {
      auto args = ParseCSV();
      prefix = new FnCallExpr(prefix, args);
    }
    // ParseFieldAccessExpr
    if (lexer_.GetPreviousToken().type == lex::DOT) {
      auto field = lexer_.GetNextToken();
      if (field.type != lex::IDENT && field.type != lex::NUMBER) {
        throw ParseError(fmt::format("couldn't access field {}", field.type),
                         prefix->GetLocation());
      }
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
    auto pos = lexer_.GetPreviousToken().location;
    Expr* expr = ParseExpr();
    if (!expr) {
      throw ParseError("missing expr inside ()", pos);
    }
    if (!lexer_.Matches(lex::RIGHT_PAREN)) {
      throw ParseError("missing closing )", lexer_.Peek().location);
    }
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

  if (lexer_.Matches(lex::TOKEN_EOF)) {
  }
  return nullptr;
}
Expr* Parser::ParseVarAccessExpr() {
  if (lexer_.Matches(lex::IDENT)) {
    return new VarAccessExpr(lexer_.GetPreviousToken());
  }

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
  if (lexer_.Matches(lex::SEMICOLON)) {
    return new ReturnExpr(nullptr, ret_pos);
  }
  Expr* returned = ParseExpr();
  if (!returned) {
    throw ParseError("missing return expression", ret_pos);
  }
  if (!lexer_.Matches(lex::SEMICOLON)) {
    throw ParseError("missing ; after return", lexer_.Peek().location);
  }
  return new ReturnExpr(returned, ret_pos);
}

Expr* Parser::ParseBlockExpr() {
  if (!lexer_.Matches(lex::LEFT_BRACE)) {
    return nullptr;
  }
  auto lbrace_pos = lexer_.GetPreviousToken().location;

  std::vector<Stmt*> flow;
  Expr* end_expr = nullptr;
  while (lexer_.Peek().type != lex::RIGHT_BRACE) {
    Stmt* curr = nullptr;
    try {
      curr = ParseStmtOrThrow();
    } catch (ParseError& err) {
      SynchronizeBlock();
      AddError(err);
    }

    if (!curr) {
      if (lexer_.Peek().type == lex::TOKEN_EOF) {
        break;
      }
      continue;
    }
    if ((end_expr = curr->As<Expr>())) {
      break;
    }
    flow.push_back(curr);
  }

  if (!lexer_.Matches(lex::RIGHT_BRACE)) {
    throw ParseError("missing closing } after block", lexer_.Peek().location);
  }
  return new BlockExpr(flow, end_expr, lbrace_pos);
}

Expr* Parser::ParseIfExpr() {
  if (!lexer_.Matches(lex::IF)) {
    return nullptr;
  }
  auto if_pos = lexer_.GetPreviousToken().location;

  Stmt* var_decl = ParseStmtOrThrow();
  if (!var_decl) {
    throw ParseError("missing statement after if", if_pos);
  }
  Expr* cond = nullptr;
  if (Expr* cond_parsed = var_decl->As<Expr>()) {
    cond = cond_parsed;
    var_decl = nullptr;
  } else {
    cond = ParseExpr();
    if (!cond) {
      throw ParseError("missing if condition", lexer_.Peek().location);
    }
    if (!var_decl->As<VarDecl>()) {
      throw ParseError("if init stmt must declare variable",
                       lexer_.Peek().location);
    }
  }

  lexer_.Matches(lex::THEN);  // skip then if any
  Expr* then_expr = ParseExpr();
  if (!then_expr) {
    throw ParseError("missing then expression", lexer_.Peek().location);
  }

  Expr* else_expr = nullptr;
  if (lexer_.Matches(lex::ELSE)) {
    else_expr = ParseExpr();
    if (!else_expr) {
      throw ParseError("missing else expression", lexer_.Peek().location);
    }
  }

  return new IfExpr(var_decl ? var_decl->As<VarDecl>() : nullptr, cond,
                    then_expr, else_expr, if_pos);
}

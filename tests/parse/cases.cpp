#include <ast/ast.hpp>
#include <lex/token_type.hpp>
#include <parse/error.hpp>

#include <catch2/catch.hpp>

TEST_CASE("TreeNode cast") {
  Stmt* s = new LiteralExpr(lex::Token{lex::TRUE});

  CHECK(s->As<Expr>());
}

TEST_CASE("Errors") {
  ParseError err("missing closing )");
  CHECK_THROWS_AS([&](){ throw err; }(), SyntaxError);
  CHECK(std::string(err.what()) == "parse error: missing closing ) at 1:1");

  LexError lerr("bad literal", {1, 10});
  err = ParseError(lerr);
  CHECK_THROWS_AS([&](){ throw err; }(), Error);
  CHECK(std::string(err.what()) == "lexer error: bad literal at 2:11");
}

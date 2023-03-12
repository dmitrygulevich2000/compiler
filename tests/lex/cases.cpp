#include <lex/lexer.hpp>

// Finally,
#include <fmt/core.h>
#include <fmt/ostream.h>
#include <catch2/catch.hpp>

TEST_CASE("ExampleErrors", "[lex]") {
  std::stringstream source("? + mama /= forward ''7 \"dsb");
  lex::Lexer l{source};

  CHECK(l.Matches(lex::TokenType::INVALID));
  CHECK(l.Matches(lex::TokenType::PLUS));
  CHECK(l.Matches(lex::TokenType::IDENTIFIER));
  CHECK(l.Matches(lex::TokenType::DIV));
  CHECK(l.Matches(lex::TokenType::ASSIGN));
  CHECK(l.Matches(lex::TokenType::IDENTIFIER));
  CHECK(l.Matches(lex::TokenType::INVALID));
  CHECK(l.Matches(lex::TokenType::INVALID));
  CHECK(l.Matches(lex::TokenType::INVALID));
  CHECK(l.Matches(lex::TokenType::TOKEN_EOF));

  CHECK(l.Errors().size() == 4);
  for (size_t i = 0; i < l.Errors().size(); ++i) {
    fmt::print("{}\n", l.Errors()[i]);
  }
}

//////////////////////////////////////////////////////////////////////

TEST_CASE("Lexer: Just works", "[lex]") {
  std::stringstream source("1 + 2");
  lex::Lexer l{source};

  CHECK(l.Matches(lex::TokenType::NUMBER));
  CHECK(l.Matches(lex::TokenType::PLUS));
  CHECK(l.Matches(lex::TokenType::NUMBER));
  CHECK(l.Matches(lex::TokenType::TOKEN_EOF));
}

//////////////////////////////////////////////////////////////////////

TEST_CASE("Braces", "[lex]") {
  std::stringstream source("1 + (1)");
  lex::Lexer l{source};

  CHECK(l.Matches(lex::TokenType::NUMBER));
  CHECK(l.Matches(lex::TokenType::PLUS));
  CHECK(l.Matches(lex::TokenType::LEFT_PAREN));
  CHECK(l.Matches(lex::TokenType::NUMBER));
  CHECK(l.Matches(lex::TokenType::RIGHT_PAREN));
}

///////////////////////////////////////////////////////////////////

TEST_CASE("Keywords", "[lex]") {
  std::stringstream source(
      "var fun for if else "
      "return yield true false");
  lex::Lexer l{source};
  CHECK(l.Matches(lex::TokenType::VAR));
  CHECK(l.Matches(lex::TokenType::FUN));
  CHECK(l.Matches(lex::TokenType::FOR));
  CHECK(l.Matches(lex::TokenType::IF));
  CHECK(l.Matches(lex::TokenType::ELSE));
  CHECK(l.Matches(lex::TokenType::RETURN));
  CHECK(l.Matches(lex::TokenType::YIELD));
  CHECK(l.Matches(lex::TokenType::TRUE));
  CHECK(l.Matches(lex::TokenType::FALSE));
}

//////////////////////////////////////////////////////////////////////

TEST_CASE("Consequent", "[lex]") {
  std::stringstream source("!true");
  lex::Lexer l{source};

  CHECK(l.Matches(lex::TokenType::NOT));
  CHECK(l.Matches(lex::TokenType::TRUE));
}

//////////////////////////////////////////////////////////////////////

TEST_CASE("Comments", "[lex]") {
  std::stringstream source(
      "# Comment if var a = 1; \n"
      "# One more comment \n"
      "1 # Token then comment \n"  // <--- Token
      "# Comment with no newline");
  lex::Lexer l{source};

  // parses to just `1`
  CHECK(l.Matches(lex::TokenType::NUMBER));
}

//////////////////////////////////////////////////////////////////////

TEST_CASE("Statement", "[lex]") {
  std::stringstream source("var abc = 0;");
  lex::Lexer l{source};

  CHECK(l.Matches(lex::TokenType::VAR));
  CHECK(l.Matches(lex::TokenType::IDENTIFIER));
  CHECK(l.Matches(lex::TokenType::ASSIGN));
  CHECK(l.Matches(lex::TokenType::NUMBER));
  CHECK(l.Matches(lex::TokenType::SEMICOLON));
}

//////////////////////////////////////////////////////////////////////

TEST_CASE("String literal", "[lex]") {
  std::stringstream source("\"Hello world\"");
  lex::Lexer l{source};

  CHECK(l.Matches(lex::TokenType::STRING));
}

//////////////////////////////////////////////////////////////////////

TEST_CASE("Funtion declaration args", "[lex]") {
  std::stringstream source("(a1, a2)");
  lex::Lexer l{source};

  CHECK(l.Matches(lex::TokenType::LEFT_PAREN));
  CHECK(l.Matches(lex::TokenType::IDENTIFIER));
  CHECK(l.Matches(lex::TokenType::COMMA));
  CHECK(l.Matches(lex::TokenType::IDENTIFIER));
  CHECK(l.Matches(lex::TokenType::RIGHT_PAREN));
}

//////////////////////////////////////////////////////////////////////

TEST_CASE("Curly", "[lex]") {
  std::stringstream source("{ }");
  lex::Lexer l{source};

  CHECK(l.Matches(lex::TokenType::LEFT_BRACE));
  CHECK(l.Matches(lex::TokenType::RIGHT_BRACE));
}

//////////////////////////////////////////////////////////////////////

TEST_CASE("Assign vs Equals", "[lex]") {
  std::stringstream source("== = ==");
  lex::Lexer l{source};

  CHECK(l.Matches(lex::TokenType::EQUALS));
  CHECK(l.Matches(lex::TokenType::ASSIGN));
  CHECK(l.Matches(lex::TokenType::EQUALS));
}

//////////////////////////////////////////////////////////////////////

TEST_CASE("Lex types", "[lex]") {
  std::stringstream source(": Int Bool String Unit");
  lex::Lexer l{source};

  CHECK(l.Matches(lex::TokenType::COLON));

  CHECK(l.Matches(lex::TokenType::TY_INT));
  CHECK(l.Matches(lex::TokenType::TY_BOOL));
  CHECK(l.Matches(lex::TokenType::TY_STRING));
  CHECK(l.Matches(lex::TokenType::TY_UNIT));
}

//////////////////////////////////////////////////////////////////////

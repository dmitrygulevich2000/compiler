#include <ast/ast.hpp>
#include <ast/visitor/printer.hpp>
#include <lex/lexer.hpp>
#include <parse/parser.hpp>

#include <sstream>

#include <catch2/catch.hpp>

///////////////////////////////////////////////// ParseStmt

void TestParseError(const std::string& str) {
  std::cout << "-----------------" << std::endl;
  std::stringstream source(str);
  lex::Lexer lexer(source);
  Parser parser(lexer);
  Stmt* tree = parser.ParseStmt();

  CHECK_FALSE(tree);
  CHECK(lexer.Matches(lex::TOKEN_EOF));
  CHECK_FALSE(parser.Errors().empty());

  std::cout << "errors report:" << std::endl;
  for (const SyntaxError& err : parser.Errors()) {
    std::cout << err.what() << std::endl;
  }
}

TEST_CASE("ParseStmt errors") {
  SECTION("no (") {
    TestParseError(R"(
var x = (2 + 2 * 2;)");
  }

  SECTION("no + rhs") {
    TestParseError(R"(
var x = 2 + )");
  }

  SECTION("empty ()") {
    TestParseError(R"(
7 - ())");
  }

  SECTION("no } after expr") {
    TestParseError(R"(
{
2 + 2;
true
)");
  }
  SECTION("no } after stmt") {
    TestParseError(R"(
{
2 + 2;
)");
  }

  SECTION("lex err, missing arg") {
    TestParseError(R"(
f(7, '',))");
  }

  SECTION("lex err, missing =") {
    TestParseError(R"(
fun main argc '' = 0)");
  }
}

void TestParseOK(const std::string& str) {
  std::cout << "-----------------" << std::endl;
  std::stringstream source(str);
  lex::Lexer lexer(source);

  Parser parser(lexer);
  Stmt* tree = parser.ParseStmt();

  CHECK(lexer.Matches(lex::TOKEN_EOF));
  CHECK(parser.Errors().empty());

  if (!tree) {
    std::cout << "null" << std::endl;
  } else {
    ConsolePrinter printer(std::cout);
    tree->Accept(&printer);
  }
}

TEST_CASE("ParseStmt OK") {
  SECTION("empty") {
    TestParseOK(R"()");
  }
  SECTION(";") {
    TestParseOK(R"(;)");
  }
  SECTION("omitted * / call") {
    TestParseOK(R"(var x = 2(pos + 1);)");
  }

  SECTION("associative -") {
    TestParseOK(R"(3 - 2 - 1)");
  }
  SECTION("var decl") {
    TestParseOK(R"(
var x = 0;
)");
  }
  SECTION("fun decl") {
    TestParseOK(R"(
fun main argc argv = 0;
)");
  }
  SECTION("assignment") {
    TestParseOK(R"(
x = true;
)");
  }
  SECTION("block") {
    TestParseOK(R"(
{
  var x = 0;
  x = "mama";
  'i'
}
)");
  }
  SECTION("field access") {
    TestParseOK(R"(
(object + 1).comp_field.tuple_field.1
)");
  }
  SECTION("fun call") {
    TestParseOK(R"(
sum(x, obj.y, 5 * 5);
)");
  }
  SECTION("op precedence") {
    TestParseOK(R"(
(1 - 5 * vec.size()) / 2 > -99
)");
  }
  SECTION("if, etc") {
    TestParseOK(R"(
fun IsCorrect answer = {
  if var true_ans = TheAnswerToLifeTheUniverseAndEverything(time.now()); answer == true_ans {
    return true;
  };
  false
};
)");
  }
}

///////////////////////////////////////////////// ParseAll

void TestParseAllError(const std::string& str) {
  std::cout << "-----------------" << std::endl;
  std::stringstream source(str);
  lex::Lexer lexer(source);

  Parser parser(lexer);
  auto decls = parser.ParseAll();

  CHECK_FALSE(parser.Errors().empty());
  CHECK(lexer.Matches(lex::TOKEN_EOF));

  ConsolePrinter printer(std::cout);

  std::cout << "parsed:" << std::endl;
  for (Decl* decl : decls) {
    decl->Accept(&printer);
  }
  std::cout << "errors report:" << std::endl;
  for (const SyntaxError& err : parser.Errors()) {
    std::cout << err.what() << std::endl;
  }
}

TEST_CASE("ParseAll errors") {
  TestParseAllError(R"(
var x = 2 + 2 * 2

fun main = {
  var msg = "hello";
  var x = 2 * (5 + 1;

  var y = -x
  (y / 2) # consumes together with above var decl
};)");

  TestParseAllError(R"(
fun foo = {
  if true else
}

var y = 5;;
)");
}

void TestParseAllOK(const std::string& str) {
  std::cout << "-----------------" << std::endl;
  std::stringstream source(str);
  lex::Lexer lexer(source);

  Parser parser(lexer);
  auto decls = parser.ParseAll();

  CHECK(parser.Errors().empty());
  CHECK(lexer.Matches(lex::TOKEN_EOF));

  ConsolePrinter printer(std::cout);

  for (Decl* decl : decls) {
    decl->Accept(&printer);
  }
}

TEST_CASE("ParseAll OK") {
  TestParseAllOK(R"(
# it's five
var x = 5;

fun main argc argv = {
  printf("Hello, %s
", argv(1));
  # return 0
};


var str = "boom";
)");
}

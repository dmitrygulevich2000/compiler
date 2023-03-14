#include <ast/visitor/printer.hpp>
#include <lex/lexer.hpp>
#include <parse/parser.hpp>

#include <sstream>

void ParseAndPrint(const std::string& str) {
  std::stringstream source(str);
  lex::Lexer lexer(source);

  Parser parser(lexer);
  Stmt* tree = parser.ParseStmt();

  ConsolePrinter printer(std::cout);
  tree->Accept(&printer);
  assert(lexer.Matches(lex::TOKEN_EOF));
}

void ParseAllAndPrint(const std::string& str) {
  std::stringstream source(str);
  lex::Lexer lexer(source);

  Parser parser(lexer);
  auto decls = parser.ParseAll();

  ConsolePrinter printer(std::cout);
  for (Decl* decl : decls) {
    decl->Accept(&printer);
  }
  assert(lexer.Matches(lex::TOKEN_EOF));
}

void ExampleArithmetic() {
  std::cout << "------ arithmetic ----" << std::endl;
  ParseAndPrint(R"(3 - 2 - 1)");
}

void ExampleVar() {
  std::cout << "------ var decl ----" << std::endl;
  ParseAndPrint(R"(
var x = 0;
)");
}

void ExampleFunc() {
  std::cout << "------ fun decl ----" << std::endl;
  ParseAndPrint(R"(
fun main argc argv = 0;
)");
}

void ExampleAssignment() {
  std::cout << "------ assignment ----" << std::endl;
  ParseAndPrint(R"(
x = true;
)");
}

void ExampleBlock() {
  std::cout << "------ block expr ----" << std::endl;
  ParseAndPrint(R"(
{
  var x = 0;
  x = "mama";
  'i'
}
)");
}

void ExampleFieldAccess() {
  std::cout << "------ field access ----" << std::endl;
  ParseAndPrint(R"(
(object + 1).comp_field.tuple_field.1
)");
}

void ExampleFnCall() {
  std::cout << "------ fun call ----" << std::endl;
  ParseAndPrint(R"(
sum(x, obj.y, 5 * 5);
)");
}

void ExampleBigArithmetic() {
  std::cout << "------ big arithmetic ----" << std::endl;
  ParseAndPrint(R"(
(1 - 5 * vec.size()) / 2 > -99
)");
}

void ExampleComplex() {
  std::cout << "------ if expr, etc ----" << std::endl;
  ParseAndPrint(R"(
fun IsCorrect answer = {
  if var true_ans = TheAnswerToLifeTheUniverseAndEverything(time.now()); answer == true_ans {
    return true;
  };
  false
};
)");
}

void ExampleFile() {
  std::cout << "------ ParseAll, comments ----" << std::endl;
  ParseAllAndPrint(R"(
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

int main() {
  ExampleArithmetic();
  ExampleVar();
  ExampleFunc();
  ExampleAssignment();
  ExampleBlock();
  ExampleFieldAccess();
  ExampleFnCall();

  ExampleBigArithmetic();
  ExampleComplex();

  ExampleFile();
}

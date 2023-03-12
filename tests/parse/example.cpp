#include <ast/visitors/printer.hpp>
#include <lex/lexer.hpp>
#include <parse/parser.hpp>

#include <sstream>

void ParseAndPrint(std::istream& in) {
  lex::Lexer lexer(in);

  Parser parser(lexer);
  Stmt* tree = parser.ParseStmt();

  ConsolePrinter printer(std::cout);
  tree->Accept(&printer);
  assert(lexer.Matches(lex::TOKEN_EOF));
}

void ParseAllAndPrint(std::istream& in) {
  lex::Lexer lexer(in);

  Parser parser(lexer);
  auto decls = parser.ParseAll();

  ConsolePrinter printer(std::cout);
  for (Decl* decl : decls) {
    decl->Accept(&printer);
  }
  assert(lexer.Matches(lex::TOKEN_EOF));
}

void ExampleArithmetic() {
  std::stringstream source(R"(3 - 2 - 1)");
  ParseAndPrint(source);
}

void ExampleBigArithmetic() {
  std::stringstream source(R"(
(1 - 5 * vec.size()) / 2 > -99
)");
  ParseAndPrint(source);
}

void ExampleVar() {
  std::stringstream source(R"(
var x = 0;
)");
  ParseAndPrint(source);
}

void ExampleFunc() {
  std::stringstream source(R"(
fun main argc argv = 0;
)");
  ParseAndPrint(source);
}

void ExampleAssignment() {
  std::stringstream source(R"(
x = true;
)");
  ParseAndPrint(source);
}

void ExampleBlock() {
  std::stringstream source(R"(
{
  var x = 0;
  x = "mama";
  'i'
}
)");
  ParseAndPrint(source);
}

void ExampleFieldAccess() {
  std::stringstream source(R"(
(object + 1).comp_field.tuple_field.1
)");
  ParseAndPrint(source);
}

void ExampleFnCall() {
  std::stringstream source(R"(
sum(x, obj.y, 5 * 5);
)");
  ParseAndPrint(source);
}

void ExampleComplex() {
  std::stringstream source(R"(
fun IsCorrect answer = {
  if var true_ans = TheAnswerToLifeTheUniverseAndEverything(time.now()); answer == true_ans {
    return true;
  };
  false
}
)");
  ParseAndPrint(source);
}

void ExampleFile() {
  std::stringstream source(R"(
# it's five
var x = 5;

fun main argc argv = {
  printf("Hello, %s
", argv(1));
  # return 0
}


var str = "boom";
)");
  ParseAllAndPrint(source);
}

int main() {
  ExampleArithmetic();
  std::cout << "----------------" << std::endl;
  ExampleBigArithmetic();

  std::cout << "----------------" << std::endl;
  ExampleVar();
  std::cout << "----------------" << std::endl;
  ExampleFunc();
  std::cout << "----------------" << std::endl;
  ExampleAssignment();
  std::cout << "----------------" << std::endl;
  ExampleBlock();
  std::cout << "----------------" << std::endl;
  ExampleFieldAccess();
  std::cout << "----------------" << std::endl;
  ExampleFnCall();

  std::cout << "----------------" << std::endl;
  ExampleComplex();

  std::cout << "----------------" << std::endl;
  ExampleFile();
}

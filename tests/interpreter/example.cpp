#include <ast/visitor/printer.hpp>
#include <interpreter/interpreter.hpp>
#include <lex/lexer.hpp>
#include <parse/parser.hpp>

#include <algorithm>
#include <sstream>

void ParseAndEval(std::istream& in) {
  lex::Lexer lexer(in);

  Parser parser(lexer);
  Stmt* tree = parser.ParseStmt();

  //  ConsolePrinter printer(std::cout);
  //  tree->Accept(&printer);

  Interpreter inter;
  tree->Accept(&inter);
  assert(lexer.Matches(lex::TOKEN_EOF));

  std::cout << inter.GetResult() << std::endl;
}

void ParseAllAndEval(std::istream& in) {
  lex::Lexer lexer(in);

  Parser parser(lexer);
  auto decls = parser.ParseAll();

  Interpreter inter;
  for (Decl* decl : decls) {
    decl->Accept(&inter);
  }
  std::cout << inter.GetResult() << std::endl;

  assert(lexer.Matches(lex::TOKEN_EOF));
}

void ExampleArithmetic() {
  std::cout << "----- arithmetic ----------" << std::endl;
  {
    std::string expr = R"(5 + (1 + 4)*8 - 2 - 1)";
    std::cout << ">>> " << expr << std::endl;
    std::stringstream source(expr);
    ParseAndEval(source);
  }
  {
    std::string expr = R"(--43)";
    std::cout << ">>> " << expr << std::endl;
    std::stringstream source(expr);
    ParseAndEval(source);
  }
  {
    std::string expr = R"(!(5 + (1 + 4)*8 - 2 - 1 < --43))";
    std::cout << ">>> " << expr << std::endl;
    std::stringstream source(expr);
    ParseAndEval(source);
  }
}

void ExampleStmtBlock() {
  std::cout << "----- stmt/block expr ----------" << std::endl;
  {
    std::string expr = R"(2 + 2;)";
    std::cout << ">>> " << expr << std::endl;
    std::stringstream source(expr);
    ParseAndEval(source);
  }
  {
    std::string expr = R"({
2 + 2;
})";
    std::cout << ">>> " << expr << std::endl;
    std::stringstream source(expr);
    ParseAndEval(source);
  }
  {
    std::string expr = R"({
2 + 2;
!true
})";
    std::cout << ">>> " << expr << std::endl;
    std::stringstream source(expr);
    ParseAndEval(source);
  }
}

int main() {
  ExampleArithmetic();
  ExampleStmtBlock();
}

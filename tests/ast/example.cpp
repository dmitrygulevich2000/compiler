#include <ast/ast.hpp>
#include <ast/visitors/printer.hpp>

#include <iostream>

int main() {
  ConsolePrinter printer(std::cout);

  auto literal = new LiteralExpr({lex::NUMBER, 42ul});
  literal->Accept(&printer);

  std::cout << "------" << std::endl;
  auto arithmetic =
      new AdditiveExpr({lex::PLUS},
                       literal,
                       new MultiplicativeExpr(
                           {lex::STAR},
                           new LiteralExpr({lex::STRING, "mama"}),
                           new VarAccessExpr({lex::IDENT, "x"})
                           )
                       );
  arithmetic->Accept(&printer);

  std::cout << "------" << std::endl;
  auto access = new FieldAccessExpr(
      new FieldAccessExpr(
          new VarAccessExpr({lex::IDENT, "object"}),
          lex::Token(lex::IDENT, "field")
          ),
      {lex::IDENT, "Method"}
      );
  access->Accept(&printer);

  std::cout << "------" << std::endl;
  auto call = new FnCallExpr(
      access,
      {literal, arithmetic}
      );
  call->Accept(&printer);

  std::cout << "------" << std::endl;
  auto condition = new EqualityExpr(
      {lex::EQ},
      new VarAccessExpr({lex::IDENT, "ans"}),
      new UnaryExpr(
          {lex::MINUS},
          literal
          )
      );
  condition->Accept(&printer);

  std::cout << "------" << std::endl;
  auto function = new FunDecl(
      {lex::IDENT, "IsCorrect"},
      {{lex::IDENT, "answer"}},
      new BlockExpr(
          {dynamic_cast<Stmt*>(new ExprStmt(new IfExpr(
              new VarDecl(
                  {lex::IDENT, "true_ans"},
                  new FnCallExpr(
                      new VarAccessExpr({lex::IDENT, "TheAnswerToLifeTheUniverseAndEverything"}),
                      {new FnCallExpr(
                          new FieldAccessExpr(
                              new VarAccessExpr({lex::IDENT, "time"}),
                              {lex::IDENT, "now"}
                          ),
                          {}
                      )}
                      )
                  ),
              new EqualityExpr(
                  {lex::EQ},
                  new VarAccessExpr({lex::IDENT, "answer"}),
                  new VarAccessExpr({lex::IDENT, "true_ans"})
                  ),
              new ReturnExpr(new LiteralExpr(lex::Token{lex::TRUE})),
              nullptr
              )
                                            ))
          },
          new LiteralExpr(lex::Token{lex::FALSE})
          )
      );
  function->Accept(&printer);
}

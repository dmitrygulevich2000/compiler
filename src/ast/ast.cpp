#include <ast/expressions.hpp>
#include <lex/location.hpp>

lex::Location ExprStmt::GetLocation() {
  return expr->GetLocation();
}

#pragma once

#include <cstdlib>
#include <ostream>

namespace lex {

//////////////////////////////////////////////////////////////////////

enum TokenType {
  // Your code goes here

  UNDEFINED,
  INVALID,
  TOKEN_EOF,

  // literals
  NUMBER,  // 4287
  STRING,  // "abc"
  CHAR,    // 'i'

  // operators
  PLUS,    // +
  MINUS,   // -
  STAR,    // *
  DIV,     // /
  REM,     // %
  ASSIGN,  // =
  EQ,      // ==
  NOT_EQ,  // !=
  NOT,     // !
  LT,      // <
  LEQ,     // <=
  GT,      // >
  GEQ,     // >=

  // delimiters
  LEFT_PAREN,   // (
  RIGHT_PAREN,  // )
  LEFT_BRACE,   // {
  RIGHT_BRACE,  // }
  LEFT_BRACK,   // [
  RIGHT_BRACK,  // ]
  DOT,          // .
  COMMA,        // ,
  SEMICOLON,    // ;
  COLON,        // :
  PIPE,         // |

  // keywords
  IDENT,    // main, x
  TRUE,     // true
  FALSE,    // false
  FUN,      // fun
  VAR,      // var
  IF,       // if
  THEN,     // then
  ELSE,     // else
  FOR,      // for
  RETURN,   // return
  YIELD,    // yield
  STRUCT,   // struct
  MATCH,    // match
  EXPORTS,  // exports

  // built-in types
  TY_INT,     // Int
  TY_STRING,  // String
  TY_BOOL,    // Bool
  TY_UNIT,    // Unit
};

////////////////////////////////////////////////////////////////

std::string FormatTokenType(TokenType tt);

std::ostream& operator<<(std::ostream& out, TokenType tt);

////////////////////////////////////////////////////////////////

}  // namespace lex

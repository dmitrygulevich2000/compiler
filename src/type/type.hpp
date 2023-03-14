#pragma once

#include <string>

enum TypeTag {
  TY_INT,
  TY_BOOL,
  TY_STRING,
  TY_CHAR,

  TY_NEVER,
  TY_UNIT,
};

std::ostream& operator<<(std::ostream& out, TypeTag tt);

struct Type {
  TypeTag tag = TY_NEVER;
};

Type builtin_int = Type{.tag = TY_INT};
Type builtin_bool = Type{.tag = TY_BOOL};
Type builtin_string = Type{.tag = TY_STRING};
Type builtin_char = Type{.tag = TY_CHAR};

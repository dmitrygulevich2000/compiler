#pragma once

#include <ast/tree_node.hpp>
#include <ast/visitors/abort_visitor.hpp>

#include <fmt/core.h>

//////////////////////////////////////////////////////////////////////

template <typename T>
class ReturnVisitor : public AbortVisitor {
 public:
  T Eval(TreeNode* expr) {
    FMT_ASSERT(expr, "LexError: evaluating null expression");
    expr->Accept(this);
    return return_value;
  }

 protected:
  T return_value;
};

//////////////////////////////////////////////////////////////////////


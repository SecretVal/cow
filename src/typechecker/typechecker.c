// TODO: Revamp this hole typechecker because the current version is quite bruce
// force Also it would be nice to show where the problem is
#include "typechecker.h"
void typecheck_ast(Grammar ast) {
  for (size_t i = 0; i < ast.count; ++i) {
    Rule decl = ast.items[i];
    switch (decl.rhs.kind) {
    case EK_SEQ:
      Sequence s = decl.rhs.seq;
      for (size_t j = 0; j < s.count; ++j) {
        Expression e = s.items[j];
        if (e.kind != EK_TYPE)
          continue;
        bool found = false;
        for (size_t k = 0; k < ast.count; ++k) {
          if (strcmp(ast.items[k].lhs, e.type) == 0)
            found = true;
        }
        if (!found) {
          println("%s is not declared anywhere", e.type);
          exit(1);
        }
      }
      break;
    case EK_CHOICE:
      Choice c = decl.rhs.choice;
      for (size_t j = 0; j < c.count; ++j) {
        Expression e = c.items[j];
        if (e.kind != EK_TYPE)
          continue;
        bool found = false;
        for (size_t k = 0; k < ast.count; ++k) {
          if (strcmp(ast.items[k].lhs, e.type) == 0)
            found = true;
        }
        if (!found) {
          println("%s is not declared anywhere", e.type);
          exit(1);
        }
      }
    case EK_RANGE:
      break;
    case EK_TYPE:
      bool found = false;
      for (size_t j = 0; j < ast.count; ++j) {
        if (strcmp(ast.items[j].lhs, decl.rhs.type) == 0)
          found = true;
      }
      if (!found) {
        println("%s is not declared anywhere", decl.rhs.type);
        exit(1);
      }
    case EK_CHAR:
      break;
    default:
      println("%s:%s: Did not handle one case", __FILE__, __LINE__);
      exit(1);
    }
  }
}

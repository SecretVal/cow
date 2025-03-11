#ifndef TYPECHECKER_H
#define TYPECHECKER_H

#include "../ast/parser.h"
#include <stdbool.h>

void typecheck_ast(Grammar ast);

#endif // TYPECHECKER_H

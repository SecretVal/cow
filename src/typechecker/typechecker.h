#ifndef TYPECHECKER_H
#define TYPECHECKER_H

#include "../grammar/parser.h"
#include <stdbool.h>

void typecheck_ast(Grammar ast);

#endif // TYPECHECKER_H

#ifndef PARSER_H
#define PARSER_H
#define STC_STRIP_PREFIX
#include <stc.h>

#include "lexer.h"

#include <ctype.h>
#include <stdio.h>

typedef struct Expression Expression;

typedef enum {
  EK_SEQ,
  EK_CHOICE,
  EK_RANGE,

  // TODO: new name?
  EK_TYPE,

  EK_CHAR,
} ExpressionKind;

typedef struct {
  Expression *items;
  size_t count;
  size_t cap;
  bool goes_on;
} Sequence;

typedef struct {
  Expression *items;
  size_t count;
  size_t cap;
} Choice;

typedef struct {
  char start;
  char end;
} Range;

struct Expression {
  ExpressionKind kind;
  union {
    char *type;
    char c;
    Sequence seq;
    Choice choice;
    Range range;
  };
};

void print_expr(Expression expr, int level);

typedef struct {
  char *lhs;
  Expression rhs;
} Rule;

void print_rule(Rule rule);

typedef struct {
  Rule *items;
  size_t count;
  size_t cap;
} Grammar;

void print_grammar(Grammar g);

typedef struct {
  Tokens tokens;
  Grammar grammar;
  Token tok;
} Parser;

Expression parse_expr(Parser *parser);

Token parser_consume(Parser *parser);

Token parser_expect(Parser *parser, TokenKind tk);

Sequence parse_seq(Parser *parser);

Choice parse_choice(Parser *parser);

Range parse_range(Parser *parser);

Expression parse_expr(Parser *parser);

Rule parse_rule(Parser *parser);

Parser parse(Lexer lexer);
#endif // PARSER_H

#ifndef LEXER_H
#define LEXER_H

#define STC_STRIP_PREFIX
#include <stc.h>

#include <ctype.h>
#include <stdio.h>

typedef enum {
  TK_ERROR,
  TK_IDENT,
  TK_NUM,

  TK_EQ, // `=`

  TK_LB,  // `[`
  TK_RB,  // `]`
  TK_LSQ, // `{`
  TK_RSQ, // `}`
  TK_DOT, // `.`

  TK_SQUOTE, // `'`

  // keywords
  TK_RANGE,
} TokenKind;

typedef struct {
  char *name;
  TokenKind kind;
} TokenMap;

char *tk_char(TokenKind tk);

typedef struct {
  size_t row;
  size_t col;
  const char *file;
} Location;

typedef struct {
  TokenKind kind;
  union {
    char *ident;
  };
  Location loc;
} Token;

typedef struct {
  Token *items;
  size_t count;
  size_t cap;
} Tokens;

typedef struct {
  Tokens tokens;
  char ch;
  StringBuilder input;
  Location loc;
} Lexer;

char lexer_consume(Lexer *lexer);

Token tokenize_ident(Lexer *lexer);

Token tokenize_punct(Lexer *lexer);

Lexer tokenize(const char *file, StringBuilder input);

#endif // LEXER_H

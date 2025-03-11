#include "lexer.h"

const TokenMap TOKEN_MAP[] = {
    {"TK_ERROR", TK_ERROR}, {"TK_IDENT", TK_IDENT}, {"TK_NUM", TK_NUM},
    {"TK_EQ", TK_EQ},       {"TK_LB", TK_LB},       {"TK_RB", TK_RB},
    {"TK_LSQ", TK_LSQ},     {"TK_RSQ", TK_RSQ},     {"TK_SQUOTE", TK_SQUOTE},
    {"TK_RANGE", TK_RANGE}, {"TK_DOT", TK_DOT}};

const size_t TOKEN_MAP_SZ = sizeof(TOKEN_MAP) / sizeof(TOKEN_MAP[0]);

const TokenMap KEYWORDS[] = {{"range", TK_RANGE}};

const size_t KEYWORD_SZ = sizeof(KEYWORDS) / sizeof(KEYWORDS[0]);

char *tk_char(TokenKind tk) {
  for (size_t i = 0; i < TOKEN_MAP_SZ; ++i) {
    if (TOKEN_MAP[i].kind == tk)
      return TOKEN_MAP[i].name;
  }
  todo("What error?");
}

char lexer_consume(Lexer *lexer) {
  shift(lexer->input.items, lexer->input.count);
  lexer->ch = lexer->input.items[0];
  return lexer->ch;
}

Token tokenize_ident(Lexer *lexer) {
  char c = lexer->ch;
  Token t = {0};
  StringBuilder ident = {0};
  while (isalnum(c)) {
    da_push(&ident, c);
    c = lexer_consume(lexer);
  }
  da_push(&ident, '\0');

  t = (Token){
      .kind = TK_IDENT,
      .ident = ident.items,
  };

  for (size_t i = 0; i < KEYWORD_SZ; ++i) {
    if (strcmp(KEYWORDS[i].name, ident.items) == 0)
      t.kind = KEYWORDS[0].kind;
  }
  return t;
}

Token tokenize_punct(Lexer *lexer) {
  char c = lexer->ch;
  StringBuilder str = {0};
  da_push(&str, c);
  da_push(&str, '\0');
  Token t = {0};

  switch (c) {
  case '=':
    t.kind = TK_EQ;
    break;
  case '[':
    t.kind = TK_LB;
    break;
  case ']':
    t.kind = TK_RB;
    break;
  case '{':
    t.kind = TK_LSQ;
    break;
  case '}':
    t.kind = TK_RSQ;
    break;
  case '\'':
    t.kind = TK_SQUOTE;
    break;
  case '.':
    t.kind = TK_DOT;
    break;
  }
  t.ident = str.items;
  lexer_consume(lexer);

  return t;
}

Lexer tokenize(const char *file, StringBuilder input) {
  Lexer lexer = {
      .input = input,
      .loc =
          {
              .file = file,
              .col = 1,
              .row = 1,
          },
  };
  while (lexer.input.count > 0) {
    lexer.ch = lexer.input.items[0];
    lexer.loc.col++;
    if (isspace(lexer.ch)) {
      switch (lexer.ch) {
      case '\n':
        lexer.loc.row++;
        lexer.loc.col = 0;
        break;
      default:
        break;
      }
      lexer_consume(&lexer);
      continue;
    }
    if (isalnum(lexer.ch)) {
      Token t = tokenize_ident(&lexer);
      t.loc = lexer.loc;
      da_push(&lexer.tokens, t);
      continue;
    }
    if (ispunct(lexer.ch)) {
      Token t = tokenize_punct(&lexer);
      t.loc = lexer.loc;
      da_push(&lexer.tokens, t);
      continue;
    }
  }
  return lexer;
}

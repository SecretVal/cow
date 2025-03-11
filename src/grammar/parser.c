#include "parser.h"

void print_expr(Expression expr, int level) {
  printf("%*s - ", level * 2, "");
  switch (expr.kind) {
  case EK_SEQ:
    println("Sequence:");
    for (size_t i = 0; i < expr.seq.count; ++i) {
      print_expr(expr.seq.items[i], level + 1);
    }
    if (expr.seq.goes_on) {
      println("%*s - [..]", (level + 1) * 2, "");
    }
    break;
  case EK_CHOICE:
    println("Choice:");
    for (size_t i = 0; i < expr.choice.count; ++i) {
      print_expr(expr.seq.items[i], level + 1);
    }
    break;
  case EK_RANGE:
    println("Range: %c -> %c", expr.range.start, expr.range.end);
    break;
  case EK_TYPE:
    println("Type: %s", expr.type);
    break;
  case EK_CHAR:
    println("Char: %c", expr.c);
    break;
  }
  return;
}

void print_decl(Rule decl) {
  println("- %s", decl.lhs);
  print_expr(decl.rhs, 0);
  return;
}

void print_grammar(Grammar g) {
  for (size_t i = 0; i < g.count; ++i) {
    print_decl(g.items[i]);
  }
}

Token parser_consume(Parser *parser) {
  shift(parser->tokens.items, parser->tokens.count);
  parser->tok = parser->tokens.items[0];
  return parser->tok;
}

Token parser_expect(Parser *parser, TokenKind tk) {
  if (parser->tok.kind != tk) {
    println("Got %s but expected %s", tk_char(parser->tok.kind), tk_char(tk));
    exit(1);
  }

  Token t = parser->tok;
  parser_consume(parser);
  return t;
}

Sequence parse_seq(Parser *parser) {
  Sequence s = {0};
  parser_expect(parser, TK_LB);
  Expression e;
  while (parser->tok.kind != TK_RB && parser->tok.kind != TK_DOT) {
    e = parse_expr(parser);
    da_push(&s, e);
  }
  if (parser->tok.kind == TK_DOT) {
    parser_expect(parser, TK_DOT);
    parser_expect(parser, TK_DOT);
    s.goes_on = true;
  }
  parser_expect(parser, TK_RB);
  return s;
}

Choice parse_choice(Parser *parser) {
  Choice c = {0};
  parser_expect(parser, TK_LSQ);
  Expression e;
  while (parser->tok.kind != TK_RSQ) {
    e = parse_expr(parser);
    da_push(&c, e);
  };
  parser_expect(parser, TK_RSQ);
  return c;
}

Range parse_range(Parser *parser) {
  parser_expect(parser, TK_RANGE);
  Range r = {0};
  Expression start = parse_expr(parser);
  Expression end = parse_expr(parser);
  r.start = start.c;
  r.end = end.c;
  return r;
}

Expression parse_expr(Parser *parser) {
  Expression e;
  switch (parser->tok.kind) {
  case TK_LB:
    e.kind = EK_SEQ;
    e.seq = parse_seq(parser);
    break;
  case TK_LSQ:
    e.kind = EK_CHOICE;
    e.choice = parse_choice(parser);
    break;
  case TK_IDENT:
    e.kind = EK_TYPE;
    e.type = parser->tok.ident;
    parser_consume(parser);
    break;
  case TK_SQUOTE:
    e.kind = EK_CHAR;
    parser_expect(parser, TK_SQUOTE);
    e.c = parser->tok.ident[0];
    parser_consume(parser);
    parser_expect(parser, TK_SQUOTE);
    break;
  case TK_RANGE:
    e.kind = EK_RANGE;
    e.range = parse_range(parser);
    break;
  default:
    println("%s:%ld:%ld: Error: Not a valid expr: %s (%d)",
            parser->tok.loc.file, parser->tok.loc.row, parser->tok.loc.col,
            tk_char(parser->tok.kind), parser->tok.kind);
    exit(1);
    break;
  }
  return e;
}

Rule parse_decl(Parser *parser) {
  Rule decl;
  decl.lhs = parser_expect(parser, TK_IDENT).ident;
  parser_expect(parser, TK_EQ);
  decl.rhs = parse_expr(parser);
  return decl;
}

Parser parse(Lexer lexer) {
  Parser parser = {
      .tokens = lexer.tokens,
      .tok = lexer.tokens.items[0],
  };
  while (parser.tokens.count > 0) {
    da_push(&parser.grammar, parse_decl(&parser));
  }
  return parser;
}

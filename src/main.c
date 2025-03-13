#define STC_IMPLEMENTATION
#define STC_STRIP_PREFIX
#include <stc.h>

#include "grammar/lexer.h"
#include "grammar/parser.h"

#include "typechecker/typechecker.h"

#include "cli/cli.h"

#include <ctype.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
  char *program = shift(argv, argc);
  if (argc < 1) {
    println("Usage: %s <opts> <file>", program);
    log(STC_ERROR, "No input file");
    return 1;
  }
  Cli cli = parse_cli(argv, argc);

  StringBuilder file_contents = {0};
  read_file(cli.input_file, &file_contents);
  if (file_contents.count == 0) {
    log(STC_INFO, "Empty file: exiting early");
    return 0;
  }
  Lexer l = tokenize(cli.input_file, file_contents);
  if (cli_arg_enabled(cli, PREFIX "tokens"))
    for (size_t i = 0; i < l.tokens.count; ++i) {
      log(STC_INFO, "%s", tk_char(l.tokens.items[i].kind));
    }
  Parser p = parse(l);
  if (cli_arg_enabled(cli, PREFIX "ast"))
    print_grammar(p.grammar);
  typecheck_grammar(p.grammar);
  return 0;
}

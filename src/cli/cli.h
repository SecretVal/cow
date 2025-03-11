#ifndef CLI_H
#define CLI_H
#define STC_STRIP_PREFIX
#include <stc.h>
#define PREFIX "+"

typedef struct {
  char *name;
  char *s_name;
} ArgDef;

typedef struct {
  ArgDef *def;
  bool enabled;
} Arg;

typedef struct {
  char *input_file;

  Arg *items;
  size_t count;
  size_t cap;
} Cli;

Cli parse_cli(char **argv, int argc);

bool cli_arg_enabled(Cli cli, char *name);

#endif // CLI_H

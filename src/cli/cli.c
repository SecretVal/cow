#include "cli.h"

#include <string.h>

ArgDef CLI_ARGS[] = {
    {
        .name = PREFIX "ast",
        .s_name = PREFIX "a",
    },
    {
        .name = PREFIX "tokens",
        .s_name = PREFIX "t",
    },
};

const size_t ARG_COUNT = sizeof(CLI_ARGS) / sizeof(CLI_ARGS[0]);

Cli parse_cli(char **argv, int argc) {
  Cli cli = {0};
  while (argc > 0) {
    char *arg = shift(argv, argc);
    for (size_t i = 0; i < ARG_COUNT; ++i) {
      ArgDef def = CLI_ARGS[i];
      if (strcmp(arg, def.name) == 0 || strcmp(arg, def.s_name) == 0) {
        da_push(&cli, ((Arg){
                          .def = &CLI_ARGS[i],
                          .enabled = true,
                      }));
      } else {
        cli.input_file = arg;
      }
    }
  }
  return cli;
}

bool cli_arg_enabled(Cli cli, char *name) {
  for (size_t i = 0; i < cli.count; ++i) {
    if (strcmp(cli.items[i].def->name, name) == 0 ||
        strcmp(cli.items[i].def->s_name, name) == 0) {
      return true;
    }
  }
  return false;
  ;
}

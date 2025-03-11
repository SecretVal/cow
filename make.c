#define STC_IMPLEMENTATION
#define STC_STRIP_PREFIX
#include <stc.h>
#include <stdbool.h>

#define project_name "./cow"
#define c_version "c23"

#define build_dir "build/"

const char *SOURCE_FILES[] = {
    "src/main.c",

    "src/cli/cli.c",

    "src/grammar/lexer.c",
    "src/grammar/parser.c",

    "src/typechecker/typechecker.c",
};

const size_t SOURCE_FILE_COUNT = sizeof(SOURCE_FILES) / sizeof(SOURCE_FILES[0]);

int build(bool debug) {
  Cmd cmd = {0};
  cmd_push(&cmd, STC_COMPILER);
  cmd_push(&cmd, "-std=" c_version);
  cmd_push(&cmd, "-o", build_dir project_name);
  cmd_push(&cmd, "-Wall", "-Wextra", "-pedantic", "-Werror",
           "-Wno-override-init");
  if (debug)
    cmd_push(&cmd, "-ggdb");
  for (size_t i = 0; i < SOURCE_FILE_COUNT; ++i) {
    cmd_push(&cmd, SOURCE_FILES[i]);
  }
  return cmd_exec(&cmd);
}

int main(int argc, char **argv) {
  rebuild_file(argv, argc);

  if (create_dir(build_dir) != 0)
    return 1;

  (void)shift(argv, argc);
  if (argc <= 0) {
    log(STC_ERROR, "Please provide commad like `build` or ´run´");
    return 1;
  }
  bool valid_cmd = false;
  char *sub_cmd = shift(argv, argc);
  if (strcmp(sub_cmd, "build") == 0) {
    bool debug = false;
    if (argc > 0) {
      char *a = shift(argv, argc);
      if (strcmp(a, "debug") == 0)
        debug = true;
    }
    int r = build(debug);
    if (r != 0)
      return r;
    valid_cmd = true;
  }
  if (strcmp(sub_cmd, "run") == 0) {
    build(true);
    Cmd cmd = {0};
    cmd_push(&cmd, project_name);
    while (argc > 0)
      cmd_push(&cmd, shift(argv, argc));
    int res = cmd_exec(&cmd);
    if (res != 0)
      return res;
    valid_cmd = true;
  }
  if (!valid_cmd) {
    log(STC_ERROR, " %s is not a known command", sub_cmd);
  }
  return 0;
}

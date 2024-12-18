#include "include/interpreter.h"

#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char ** argv) {
  if (argc < 2) {
    (void)printf("missing input file\n");
    return -1;
  }

  int i;
  for (i = 0; i < 256; ++i)
    if (argv[1][i] == '\0') break;

  char buf[5] = {0};
  memcpy(buf, argv[1] + i - 4, 5);

  if (strcmp(buf, ".alu") != 0) {
    (void)printf("incorrect file type or file name greater than 15 characters\n");
    return -1;
  }

  struct computer comp = {0};
  comp.reg_count = 6;

  prog_t prog = {0};
  read_prog(argv[1], &prog);

  run_prog(&prog, &comp);

  return 0;
}
#ifndef interpreter_h
#define interpreter_h

#include "emalu.h"

typedef enum {
  MOVE,
  PROC,
  GOTO,
  WAIT,
  SHFT,
  BIEQ,
  BNEQ
} command_e;

typedef struct {
  char code[50][13];
  int lines;
  int curr_line;
} prog_t;

void read_prog(char *, prog_t *);
void run_prog(prog_t *, struct computer *);

#endif
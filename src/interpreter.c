#include "include/interpreter.h"
#include "include/graphics.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

command_e command(const char * cmd) {
  if (strcmp(cmd, "move") == 0)
    return MOVE;
  else if (strcmp(cmd, "proc") == 0)
    return PROC;
  else if (strcmp(cmd, "goto") == 0)
    return GOTO;
  else if (strcmp(cmd, "wait") == 0)
    return WAIT;
  else if (strcmp(cmd, "shft") == 0)
    return SHFT;
  else if (strcmp(cmd, "bieq") == 0)
    return BIEQ;
  else if (strcmp(cmd, "bneq") == 0)
    return BNEQ;

  (void)printf("\033[15;1H\033[2KINVALID INSTRUCTION: %s", cmd);
  (void)fflush(stdout);
  deinit();
  exit(-1);
}

int reg_idx(const char * buf) {
  if (buf[0] != '$' && buf[1] != 'r') return -1;
  return buf[2] - '0';
}

void move(const char * cmd, struct computer * comp) {
  char dst[4] = {0};
  memcpy(dst, cmd + 5, 3);
  dst[3] = '\0';

  int dst_idx = reg_idx(dst);
  if (dst_idx == -1) {
    (void)printf("\033[15;1H\033[2KINVALID SYNTAX: %s", cmd);
    (void)fflush(stdout);
    deinit();
    exit(-1);
  }

  if (dst_idx < 0 || dst_idx > comp->reg_count - 1) {
    (void)printf("\033[15;1H\033[2KINVALID DST: %s", cmd);
    (void)fflush(stdout);
    deinit();
    exit(-1);
  }

  char src[4] = {0};
  memcpy(src, cmd + 9, 4);

  int src_idx = reg_idx(src);

  if (src_idx == -1) {
    src_idx = atoi(src);
    write_reg(comp, dst_idx, src_idx);
  }
  else if (src_idx >= 0 && src_idx < comp->reg_count) {
    cp_reg(comp, dst_idx, src_idx);
  }
  else {
    (void)printf("\033[15;1H\033[2KINVALID SRC: %s", cmd);
    (void)fflush(stdout);
    deinit();
    exit(-1);
  }
}

void go(const char * cmd, prog_t * prog) {
  char buf[4] = {0};
  memcpy(buf, cmd + 5, 3);
  buf[3] = '\0';

  int line = atoi(buf);

  if (line < 1 || line > 50) {
    (void)printf("\033[15;1H\033[2KINVALID JUMP: %s", cmd);
    (void)fflush(stdout);
    deinit();
    exit(-1);
  }

  prog->curr_line = line - 1;
}

void shft(const char * cmd, struct computer * comp) {
  char buf[4] = {0};
  memcpy(buf, cmd + 5, 3);
  buf[3] = '\0';

  int reg = reg_idx(buf);
  if (reg == -1) {
    (void)printf("\033[15;1H\033[2KINVALID SYNTAX: %s", cmd);
    (void)fflush(stdout);
    deinit();
    exit(-1);
  }
  else if (reg < 0 || reg > comp->reg_count - 1) {
    (void)printf("\033[15;1H\033[2KINVALID REG: %s", cmd);
    (void)fflush(stdout);
    deinit();
    exit(-1);
  }

  memcpy(buf, cmd + 9, 4);
  int amount = atoi(buf);

  amount < 0 ? reg_left_shift(comp, reg, abs(amount)) : reg_right_shift(comp, reg, amount);
}

int bieq(const char * cmd, struct computer * comp, prog_t * prog) {
  char buf[4] = {0};
  memcpy(buf, cmd + 5, 3);
  buf[3] = '\0';
  int val = atoi(buf);

  int reg_val = 0;
  read_reg(comp, 0, &reg_val);

  if (reg_val != val) return 0;

  memcpy(buf, cmd + 9, 4);
  val = atoi(buf);

  if (val < 0 || val > 50) {
    (void)printf("\033[15;1H\033[2KINVALID JUMP: %s", cmd);
    (void)fflush(stdout);
    deinit();
    exit(-1);
  }

  prog->curr_line = val - 1;

  return 1;
}

int bneq(const char * cmd, struct computer * comp, prog_t * prog) {
  char buf[4] = {0};
  memcpy(buf, cmd + 5, 3);
  buf[3] = '\0';
  int val = atoi(buf);

  int reg_val = 0;
  read_reg(comp, 0, &reg_val);

  if (reg_val == val) return 0;

  memcpy(buf, cmd + 9, 4);
  val = atoi(buf);

  if (val < 0 || val > 50) {
    (void)printf("\033[15;1H\033[2KINVALID JUMP: %s", cmd);
    (void)fflush(stdout);
    deinit();
    exit(-1);
  }

  prog->curr_line = val - 1;

  return 1;
}

void read_prog(char * path, prog_t * prog) {
  FILE * file = fopen(path, "r");
  if (file == NULL) {
    (void)printf("\033[15;1H\033[2KFAILED TO OPEN PROGRAM");
    (void)fflush(stdout);
    deinit();
    exit(-1);
  }

  char buf[13] = {0};
  while (fgets(buf, sizeof(buf), file) != NULL && prog->lines != 50) {
    if (strcmp(buf, "\n") == 0) continue;
    memcpy(prog->code[prog->lines++], buf, 13);
  }

  (void)fclose(file);
}

void run_prog(prog_t * prog, struct computer * comp) {
  init();

  if (comp->reg_count < 3) comp->reg_count = 3;
  else if (comp->reg_count > 6) comp->reg_count = 6;

  if (comp->clock < 500000) comp->clock = 500000;
  else if (comp->clock > 5000000) comp->clock = 5000000;

  draw(comp, prog);

  int cycles = 0;
  while (prog->curr_line != prog->lines && cycles != 256) {
    const char * code = prog->code[prog->curr_line];

    char cmd[5] = {0};
    memcpy(cmd, code, 4);
    cmd[4] = '\0';

    int wait = 0;
    switch (command(cmd)) {
      case MOVE:
        move(code, comp);
        break;
      case PROC:
        if (!proc(comp)) break;
        (void)printf("\033[22;1HOVERFLOW : ln %d", prog->curr_line + 1);
        (void)fflush(stdout);
        deinit();
        exit(-1);
      case GOTO:
        go(code, prog);
        continue;
      case WAIT:
        wait = 1;
        break;
      case SHFT:
        shft(code, comp);
        break;
      case BIEQ:
        if (!bieq(code, comp, prog)) ++prog->curr_line;
        continue;
      case BNEQ:
        if (!bneq(code, comp, prog)) ++prog->curr_line;
        continue;
    }

    draw(comp, prog);

    if (!wait) usleep(comp->clock);
    else {
      (void)getchar();
      (void)fflush(stdin);
      ++prog->curr_line;
      continue;
    }

    ++prog->curr_line;
    ++cycles;
  }

  deinit();
}
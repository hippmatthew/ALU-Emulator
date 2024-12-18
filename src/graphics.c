#include "include/graphics.h"

#include <stdio.h>
#include <termios.h>
#include <unistd.h>

void draw_ha(struct computer * comp, int x, int y) {
  (void)printf("\033[%d;%dH%d\033[%d;%dH%d",
    y, x + 1, comp->alu.half_adder.bit1,
    y, x + 5, comp->alu.half_adder.bit2
  );
  (void)printf("\033[%d;%dH-------", y + 1, x);

  for (int i = 0; i < 2; ++i) {
    (void)printf("\033[%d;%dH|\033[%d;%dH|",
      y + 2 + i, x,
      y + 2 + i, x + 6
    );
  }

  (void)printf("\033[%d;%dH-------", y + 4, x);
  (void)printf("\033[%d;%dH%d\033[%d;%dH%d",
    y + 5, x + 1, comp->alu.half_adder.carry,
    y + 5, x + 5, comp->alu.half_adder.out
  );

  (void)fflush(stdout);
}

void draw_fa(struct computer * comp, int idx, int x, int y) {
  (void)printf("\033[%d;%dH%d\033[%d;%dH%d\033[%d;%dH%d",
    y, x, comp->alu.full_adders[idx].bit1,
    y, x + 3, comp->alu.full_adders[idx].bit2,
    y, x + 6, comp->alu.full_adders[idx].carry_in
  );
  (void)printf("\033[%d;%dH-------", y + 1, x);

  for (int i = 0; i < 2; ++i) {
    (void)printf("\033[%d;%dH|\033[%d;%dH|",
      y + 2 + i, x,
      y + 2 + i, x + 6
    );
  }

  (void)printf("\033[%d;%dH-------", y + 4, x);
  (void)printf("\033[%d;%dH%d\033[%d;%dH%d",
    y + 5, x + 1, comp->alu.full_adders[idx].carry,
    y + 5, x + 5, comp->alu.full_adders[idx].out
  );

  (void)fflush(stdout);
}

void display_reg(struct computer * comp, int idx, int x, int y) {
  (void)printf("\033[%d;%dH---------------------", y, x);
  (void)printf("\033[%d;%dH|", y + 1, x);

  for (int i = 0; i < 8; ++i) {
    (void)printf("\033[%d;%dH%d",
      y + 1, x + 3 + 2 * i, comp->regs[idx].bits[i]
    );
  }

  (void)printf("\033[%d;%dH|", y + 1, x + 20);
  (void)printf("\033[%d;%dH---------------------", y + 2, x);
  (void)fflush(stdout);
}

void display_alu(struct computer * comp, int x, int y) {
  for (int i = 0; i < 7; ++i)
    draw_fa(comp, i, x + 8 * i, y);
  draw_ha(comp, x + 56, y);
}

void init() {
  struct termios terminal;

  (void)tcgetattr(STDIN_FILENO, &terminal);
  terminal.c_lflag &= ~(ICANON | ECHO);
  (void)tcsetattr(STDIN_FILENO, TCSANOW, &terminal);

  (void)printf("\033[2J\033[H\033[?25l");
  (void)fflush(stdout);
}

void deinit() {
  struct termios terminal;

  (void)tcgetattr(STDIN_FILENO, &terminal);
  terminal.c_lflag |= (ICANON | ECHO);
  (void)tcsetattr(STDIN_FILENO, TCSANOW, &terminal);

  (void)printf("\033[?25h\033[41;1H\033[2K");
  (void)fflush(stdout);
}

void draw(struct computer * comp, prog_t * prog) {
  int res[6] = {0};

  for (int i = 0; i < comp->reg_count; ++i) {
    (void)printf("\033[%d;%dH$r%d", (i < 3) + (i >= 3) * 6, 2 + 22 * (i % 3), i);
    display_reg(comp, i, 2 + 22 * (i % 3), (i < 3) * 2 + (i >= 3) * 7);

    read_reg(comp, i, &res[i]);
    (void)printf("\033[21;%dH       ", 1 + 10 * i);
    (void)fflush(stdout);
    (void)printf("\033[21;%dH$r%d=%d", 1 + 10 * i, i, res[i]);
    (void)fflush(stdout);
  }

  display_alu(comp, 2, 13);

  for (int i = 0; i < prog->lines; ++i) {
    (void)printf("\033[%d;80H\033[K", i + 1);
    if (i == prog->curr_line) (void)printf("> ");
    (void)printf("%s", prog->code[i]);
  }
  (void)fflush(stdout);
}
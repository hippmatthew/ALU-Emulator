#include "include/emalu.h"

#include <string.h>
#include <stdio.h>

void proc_ha(struct computer * comp) {
  comp->alu.half_adder.out = comp->alu.half_adder.bit1 ^ comp->alu.half_adder.bit2;
  comp->alu.half_adder.carry = comp->alu.half_adder.bit1 & comp->alu.half_adder.bit2;
}

void proc_fa(struct computer * comp, int idx) {
  comp->alu.full_adders[idx].out =  comp->alu.full_adders[idx].bit1 ^
                                    comp->alu.full_adders[idx].bit2 ^
                                    comp->alu.full_adders[idx].carry_in;
  comp->alu.full_adders[idx].carry =
    ((comp->alu.full_adders[idx].bit1 ^ comp->alu.full_adders[idx].bit2) & comp->alu.full_adders[idx].carry_in)
    | (comp->alu.full_adders[idx].bit1 & comp->alu.full_adders[idx].bit2);
}

void read_reg(struct computer * comp, int src, int * out) {
  *out = 0;
  for (int i = 0; i < 8; ++i) {
    *out <<= 1;
    *out |= comp->regs[src].bits[i];
  }
}

void write_reg(struct computer * comp, int dst, int wd) {
  char bit = 0;
  for (int i = 0; i < 8; ++i)
    comp->regs[dst].bits[7 - i] = (wd >> i) & 1;
}

void cp_reg(struct computer * comp, int dst, int src) {
  memcpy(comp->regs[dst].bits, comp->regs[src].bits, 8);
}

int proc(struct computer * comp) {
  int a = comp->alu.half_adder.bit1 = comp->regs[1].bits[7];
  int b = comp->alu.half_adder.bit2 = comp->regs[2].bits[7];

  proc_ha(comp);

  int s = comp->regs[0].bits[7] = comp->alu.half_adder.out;
  int c = comp->alu.full_adders[6].carry_in = comp->alu.half_adder.carry;

  for (int i = 6; i >= 0; --i) {
    a = comp->alu.full_adders[i].bit1 = comp->regs[1].bits[i];
    b = comp->alu.full_adders[i].bit2 = comp->regs[2].bits[i];

    proc_fa(comp, i);

    s = comp->regs[0].bits[i] = comp->alu.full_adders[i].out;
    int co = comp->alu.full_adders[i - 1].carry_in = comp->alu.full_adders[i].carry;
  }

  return comp->alu.full_adders[0].carry;
}

void reg_left_shift(struct computer * comp, int idx, int amount) {
  for (int i = 0; i < 8 - amount; ++i)
    comp->regs[idx].bits[i] = comp->regs[idx].bits[i + amount];

  for (int i = 7; i >= 8 - amount; --i)
    comp->regs[idx].bits[i] = 0;
}

void reg_right_shift(struct computer * comp, int idx, int amount) {
  for (int i = 7; i >= amount; --i)
    comp->regs[idx].bits[i] = comp->regs[idx].bits[i - amount];

  for (int i = 0; i < amount; ++i)
    comp->regs[idx].bits[i] = 0;
}
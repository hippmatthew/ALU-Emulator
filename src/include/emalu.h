#ifndef emalu_h
#define emalu_h

typedef struct {
  char bit1;
  char bit2;
  char out;
  char carry;
} half_adder_t;

typedef struct {
  char bit1;
  char bit2;
  char carry_in;
  char out;
  char carry;
} full_adder_t;

typedef struct {
  char bits[8];
} reg8_t;

struct ALU8 {
  half_adder_t half_adder;
  full_adder_t full_adders[7];
};

struct computer {
  reg8_t regs[6];
  struct ALU8 alu;
  float clock;
  int reg_count;
};

void read_reg(struct computer *, int, int *);
void write_reg(struct computer *, int, int);
void cp_reg(struct computer *, int, int);
int proc(struct computer *);
void reg_left_shift(struct computer *, int, int);
void reg_right_shift(struct computer *, int, int);

#endif
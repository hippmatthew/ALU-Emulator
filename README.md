# ALU Emulator

Just a fun little ALU emulator that you can write "assembly" for and watch it process everything

## Running a file

just use `./alu <path/to/file>`

## Instructions

> Instructions are read as follows:
> 1. 4 char identifier
> 2. 3 char input 1
> 3. 3 char input 2
>    
> there are spaces in between each section as well

#### move *dst* *src*

moves the `src` into `dst`. `dst` must be a register. `src` can be a register or an integer.

#### shft *reg* *val*

shift `reg` by the amount specified by `val`. If `val` is negative, shifts left. If `val` is positive, shifts right.
`reg` must be a register. `val` is an integer.

#### proc

triggers the ALU to calculate a result. `$r1` and `$r2` are used as inputs. `$r0` is used as the ouput.

#### wait

waits for a character input from the user before proceeding

#### goto *line*

changes the current line of the program to the value specified in `line`. `line` is an int.

#### bieq *val* *line*

changes the current line of the program to the value specified in `line` only if the value in `$r0` is equal to the value specified in `val`. `val` and `line` are ints.

#### bneq *val* *line*

changes the current line of the program to the value specified in `line` only if the value in `$r0` is not equal to the value specified in `val`. `val` and `line` are ints.

> for both `bieq` and `bneq`, spacing between inputs is important. Make sure that the number is within its correct boundary as specified at the beginning of this section

## Example Programs

- test.alu: just goes through every instruction to make sure they all work
- counter.alu: increments a counter by 1 on repeat
- fibonacci.alu: computes the fibonacci sequence until it overflows the ALU
- factorial.alu: computes 5! and stores the result in $r3

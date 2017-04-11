#ifndef STATE_H_
#define STATE_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <stack.h>

struct _State
{
    int8_t* reg;
    uint32_t IR;
    uint8_t PC;
    uint8_t SP;
    uint8_t BP;
    bool* stat;
    Stack* stack;
    int8_t tmp;
};

typedef struct _State State;

/* Initialisation */
unsigned int state_init(State* state);
unsigned int state_free(State* state);

/* Access */
unsigned int state_set_reg(unsigned int reg, int8_t value, State* state);
unsigned int state_get_reg(unsigned int reg, int8_t* data, State* state);

unsigned int state_set_instruction_register(uint32_t data, State* state);
unsigned int state_get_instruction_register(uint32_t* data, State* state);

unsigned int state_set_program_counter(uint8_t data, State* state);
unsigned int state_get_program_counter(uint8_t* data, State* state);

unsigned int state_set_status(unsigned int bit, bool data, State* state);
unsigned int state_get_status(unsigned int bit, bool* data, State* state);

/* I/O */
unsigned int state_write(FILE* file, State* state);
unsigned int state_read(FILE* file, State* state);

/* Utilities */
unsigned int state_reset_status(State* state);

#endif /* STATE_H_ */


#ifndef CONTROL_H_
#define CONTROL_H_

#include <stdint.h>

unsigned int fetch(uint8_t addr, uint32_t* code, unsigned int len,
                    State* state);
unsigned int decode(uint8_t* opcode, uint8_t* dest, uint8_t* src,
                    int8_t* value, State* state);
unsigned int execute(uint8_t opcode, uint8_t dest, uint8_t src, int8_t value,
                    State* state);

unsigned int process(uint32_t* code, unsigned int len, State* state);

#endif /* CONTROL_H_ */


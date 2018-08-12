#ifndef ISA_H_
#define ISA_H_

#include <stdint.h>

#include "state.h"

#define ISA_NUM_REGS 8
#define ISA_NUM_STATUS_BITS 4
#define ISA_WORD_SIZE 32

#define ISA_STATUS_OVERFLOW 0
#define ISA_STATUS_CARRY 1
#define ISA_STATUS_ZERO 2
#define ISA_STATUS_NEGATIVE 3

#define ISA_STACK_BASE_ADDR 0x8000
#define ISA_STACK_MAX_ADDR 0x10000

/* opcode definitions */
#define ISA_OPCODE_NOP 0x00
#define ISA_OPCODE_MOV 0x01
#define ISA_OPCODE_LDL 0x02
#define ISA_OPCODE_ADD 0x03
#define ISA_OPCODE_SUB 0x04
#define ISA_OPCODE_MUL 0x05
#define ISA_OPCODE_DIV 0x06
#define ISA_OPCODE_MOD 0x07
#define ISA_OPCODE_JMP 0x08
#define ISA_OPCODE_JPZ 0x09
#define ISA_OPCODE_JPC 0x0A
#define ISA_OPCODE_JPV 0x0B
#define ISA_OPCODE_JPE 0x0C
#define ISA_OPCODE_INC 0x0D
#define ISA_OPCODE_DEC 0x0E
#define ISA_OPCODE_CLL 0x0F
#define ISA_OPCODE_RET 0x10
#define ISA_OPCODE_PSH 0x11
#define ISA_OPCODE_POP 0x12
#define ISA_OPCODE_LOR 0x13
#define ISA_OPCODE_AND 0x14
#define ISA_OPCODE_XOR 0x15
#define ISA_OPCODE_NOT 0x16
#define ISA_OPCODE_SWP 0x17
#define ISA_OPCODE_CMP 0x18

/* Operations */
unsigned int isa_op_nop(State* state);
unsigned int isa_op_mov(uint8_t dest, uint8_t src, State* state);
unsigned int isa_op_ldl(uint8_t dest, int8_t value, State* state);
unsigned int isa_op_add(uint8_t dest, uint8_t src, State* state);
unsigned int isa_op_sub(uint8_t dest, uint8_t src, State* state);
unsigned int isa_op_mul(uint8_t dest, uint8_t src, State* state);
unsigned int isa_op_div(uint8_t dest, uint8_t src, State* state);
unsigned int isa_op_mod(uint8_t dest, uint8_t src, State* state);
unsigned int isa_op_jmp(int8_t addr, State* state);
unsigned int isa_op_jpz(int8_t addr, State* state);
unsigned int isa_op_jpc(int8_t addr, State* state);
unsigned int isa_op_jpv(int8_t addr, State* state);
unsigned int isa_op_psh(uint8_t reg, State* state);
unsigned int isa_op_pop(uint8_t reg, State* state);

#endif /* ISA_H_ */


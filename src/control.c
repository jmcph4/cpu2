#include "error.h"
#include "isa.h"
#include "state.h"
#include "control.h"

unsigned int fetch(uint8_t addr, uint32_t* code, unsigned int len,
                    State* state)
{
    if(code == NULL)
    {
        return CPU2_ERR_NULL;
    }

    if(addr >= len)
    {
        return CPU2_ERR_BOUNDS;
    }

    unsigned int res = state_set_instruction_register(code[addr], state);

    if(res != CPU2_ERR_SUCCESS)
    {
        return res;
    }

    return CPU2_ERR_SUCCESS;
}

unsigned int decode(uint8_t* opcode, uint8_t* dest, uint8_t* src,
                     int8_t* value, State* state)
{
    if(opcode == NULL || dest == NULL || src == NULL || value == NULL)
    {
        return CPU2_ERR_NULL;
    }

    uint32_t word = 0;

    unsigned int res = state_get_instruction_register(&word, state);

    if(res != CPU2_ERR_SUCCESS)
    {
        return res;
    }

    *opcode = (word >> (32-8)); /* shift away 32-8 lowest bits */
    *dest = (word >> (32-16));
    *src = (word >> (32-24));
    *value = (word & 0xFF); /* mask off lowest 8 bits */

    return CPU2_ERR_SUCCESS;
}

unsigned int execute(uint8_t opcode, uint8_t dest, uint8_t src, int8_t value,
                      State* state)
{
    switch(opcode)
    {
        case ISA_OPCODE_NOP:
            isa_op_nop(state);
            break;
        case ISA_OPCODE_MOV:
            isa_op_mov(dest, src, state);
            break;
        case ISA_OPCODE_LDL:
            isa_op_ldl(dest, value, state);
            break;
        case ISA_OPCODE_ADD:
            isa_op_add(dest, src, state);
            break;
        case ISA_OPCODE_JMP:
            isa_op_jmp(src, state);
            break;
        case ISA_OPCODE_JPV:
            isa_op_jpv(src, state);
            break;
        case ISA_OPCODE_PSH:
            isa_op_psh(src, state);
            break;
        case ISA_OPCODE_POP:
            isa_op_pop(dest, state);
            break;
    }

    return CPU2_ERR_SUCCESS;
}

unsigned int process(uint32_t* code, unsigned int len, State* state)
{
    if(code == NULL || state == NULL)
    {
        return CPU2_ERR_NULL;
    }

    unsigned int res = 0;
    
    uint8_t op = 0;
    uint8_t dst = 0;
    uint8_t src = 0;
    int8_t val = 0;

    uint8_t pc = 0;

    while(pc < len)
    {
        /* get program counter value */
        res = state_get_program_counter(&pc, state);

        if(res != CPU2_ERR_SUCCESS)
        {
            return res;
        }

        res = fetch(pc, code, len, state);

        if(res != CPU2_ERR_SUCCESS)
        {
            if(res == CPU2_ERR_BOUNDS)
            {
                /* fault osmehow? */
            }
            else
            {
                return res;
            }
        }

        res = decode(&op, &dst, &src, &val, state);

        if(res != CPU2_ERR_SUCCESS)
        {
            return res;
        }

        res = execute(op, dst, src, val, state);

        if(res != CPU2_ERR_SUCCESS)
        {
            return res;
        }

        /* get program counter value */
        res = state_get_program_counter(&pc, state);

        if(res != CPU2_ERR_SUCCESS)
        {
            return res;
        }

        /* increment program counter */
        res = state_set_program_counter(pc + 1, state);

        if(res != CPU2_ERR_SUCCESS)
        {
            return res;
        }

        /* get program counter value */
        res = state_get_program_counter(&pc, state);

        if(res != CPU2_ERR_SUCCESS)
        {
            return res;
        }

        state_write(stdout, state);
    }

    return CPU2_ERR_SUCCESS;
}


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/* JCRL */
#include <constants.h>
#include <stack.h>

#include "error.h"
#include "isa.h"
#include "state.h"

unsigned int state_init(State* state)
{
    if(state == NULL)
    {
        return CPU2_ERR_NULL;
    }

    state->reg = calloc(ISA_NUM_REGS, sizeof(int8_t));

    if(state->reg == NULL)
    {
        return CPU2_ERR_SYS_MEM;
    }

    state->IR = 0;
    state->PC = 0;
    state->SP = 0;
    state->BP = 0;

    state->tmp = 0;
    
    state->stat = calloc(ISA_NUM_STATUS_BITS, sizeof(bool));

    if(state->stat == NULL) 
    {
        free(state->reg);
        return CPU2_ERR_SYS_MEM;
    }

    state->stack = calloc(1, sizeof(Stack));
    
    if(state->stack == NULL)
    {
        free(state->reg);
        free(state->stat);
        return CPU2_ERR_SYS_MEM;
    }

    unsigned int res = stack_init(state->stack);

    if(res != JCRL_ERR_OK)
    {
        free(state->reg);
        free(state->stat);
        free(state->stack);
        return CPU2_ERR_JCRL;
    }

    return CPU2_ERR_SUCCESS;
}

unsigned int state_free(State* state)
{
    if(state == NULL)
    {
        return CPU2_ERR_NULL;
    }

    unsigned int res = stack_free(NULL, state->stack);

    if(res != JCRL_ERR_OK)
    {
        return CPU2_ERR_JCRL;
    }

    free(state->reg);
    free(state->stat);

    return CPU2_ERR_SUCCESS;
}

unsigned int state_set_reg(unsigned int reg, int8_t data, State* state)
{
    if(state == NULL)
    {
        return CPU2_ERR_NULL;
    }

    if(reg >= ISA_NUM_REGS)
    {
        return CPU2_ERR_BOUNDS;
    }

    state->reg[reg] = data;

    return CPU2_ERR_SUCCESS;
}

unsigned int state_get_reg(unsigned int reg, int8_t* data, State* state)
{
    if(state == NULL || data == NULL)
    {
        return CPU2_ERR_NULL;
    }

    if(reg >= ISA_NUM_REGS)
    {
        return CPU2_ERR_BOUNDS;
    }

    *data = state->reg[reg];

    return CPU2_ERR_SUCCESS;
}

unsigned int state_set_instruction_register(uint32_t data, State* state)
{
    if(state == NULL)
    {
        return CPU2_ERR_NULL;
    }

    state->IR = data;

    return CPU2_ERR_SUCCESS;
}

unsigned int state_get_instruction_register(uint32_t* data, State* state)
{
    if(data == NULL || state == NULL)
    {
        return CPU2_ERR_NULL;
    }

    *data = state->IR;

    return CPU2_ERR_SUCCESS;
}

unsigned int state_set_program_counter(uint8_t data, State* state)
{
    if(state == NULL)
    {
        return CPU2_ERR_NULL;
    }

    state->PC = data;

    return CPU2_ERR_SUCCESS;
}

unsigned int state_get_program_counter(uint8_t* data, State* state)
{
    if(data == NULL || state == NULL)
    {
        return CPU2_ERR_NULL;
    }

    *data = state->PC;

    return CPU2_ERR_SUCCESS;
}

unsigned int state_set_status(unsigned int bit, bool data, State* state)
{
    if(state == NULL)
    {
        return CPU2_ERR_NULL;
    }

    if(bit >= ISA_NUM_STATUS_BITS)
    {
        return CPU2_ERR_BOUNDS;
    }

    state->stat[bit] = data;

    return CPU2_ERR_SUCCESS;
}

unsigned int state_get_status(unsigned int bit, bool* data, State* state)
{
    if(data == NULL || state == NULL)
    {
        return CPU2_ERR_NULL;
    }

    if(bit >= ISA_NUM_STATUS_BITS)
    {
        return CPU2_ERR_BOUNDS;
    }

    *data = state->stat[bit];

    return CPU2_ERR_SUCCESS;
}

/* I/O */
unsigned int state_write(FILE* file, State* state)
{
    if(file == NULL || state == NULL)
    {
        return CPU2_ERR_NULL;
    }

    for(unsigned int i=0;i<ISA_NUM_REGS;i++)
    {
        fprintf(file, "r%d: %#x (%d)\n", i, state->reg[i], state->reg[i]);
    }

    fprintf(file, "IR: %#x (%d)\n", state->IR, state->IR);
    fprintf(file, "PC: %#x (%d)\n", state->PC, state->PC);
    fprintf(file, "SP: %#x (%d)\n", state->SP, state->SP);
    fprintf(file, "BP: %#x (%d)\n", state->BP, state->BP);

    fprintf(file, "STATUS: ");

    for(unsigned int i=0;i<ISA_NUM_STATUS_BITS;i++)
    {
        if(state->stat[i])
        {
            switch(i)
            {
                case ISA_STATUS_OVERFLOW:
                    fprintf(file, "V ");
                    break;
                case ISA_STATUS_CARRY:
                    fprintf(file, "C ");
                    break; 
                case ISA_STATUS_ZERO:
                    fprintf(file, "Z ");
                    break;
                case ISA_STATUS_NEGATIVE:
                    fprintf(file, "N ");
                    break;
            }
        }
    }

    fprintf(file, "\n");

    return CPU2_ERR_SUCCESS;
}

/* Utilities */
unsigned int state_reset_status(State* state)
{
    if(state == NULL)
    {
        return CPU2_ERR_NULL;
    }

    unsigned int res = 0;

    for(unsigned int i=0;i<ISA_NUM_STATUS_BITS;i++)
    {
        res = state_set_status(i, false, state);

        if(res != CPU2_ERR_SUCCESS)
        {
            return res;
        }
    }

    return CPU2_ERR_SUCCESS;
}


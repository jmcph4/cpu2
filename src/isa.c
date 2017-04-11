#include <stdlib.h>

#include "error.h"
#include "isa.h"
#include "state.h"

unsigned int isa_op_nop(State* state)
{
    if(state == NULL)
    {
        return CPU2_ERR_BOUNDS;
    }

    unsigned int a = 1+1;
    a++;

    return CPU2_ERR_SUCCESS;
}

unsigned int isa_op_mov(uint8_t dest, uint8_t src, State* state)
{
    if(state == NULL)
    {
        return CPU2_ERR_NULL;
    }

    int8_t src_val = 0;

    /* get value of source register */
    unsigned int res = state_get_reg(src, &src_val, state);

    if(res != CPU2_ERR_SUCCESS)
    {
        return res;
    }

    /* write value of source register to destination register */
    res = state_set_reg(dest, src_val, state);

    if(res != CPU2_ERR_SUCCESS)
    {
        return res;
    }

    return CPU2_ERR_SUCCESS;
}

unsigned int isa_op_ldl(uint8_t dest, int8_t value, State* state)
{
    if(state == NULL)
    {
        return CPU2_ERR_NULL;
    }

    unsigned int res = state_set_reg(dest, value, state);

    if(res != CPU2_ERR_SUCCESS)
    {
        return res;
    }

    return CPU2_ERR_SUCCESS;
}

unsigned int isa_op_add(uint8_t dest, uint8_t src, State* state)
{
    if(state == NULL)
    {
        return CPU2_ERR_NULL;
    }

    int8_t src_val = 0;
    int8_t dst_val = 0;

    unsigned int res = state_get_reg(src, &src_val, state);

    if(res != CPU2_ERR_SUCCESS)
    {
        return res;
    }

    res = state_get_reg(dest, &dst_val, state);

    if(res != CPU2_ERR_SUCCESS)
    {
        return res;
    }

    int8_t add_result = src_val + dst_val;

    res = state_set_reg(dest, add_result, state);

    if(res != CPU2_ERR_SUCCESS)
    {
        return res;
    }

    return CPU2_ERR_SUCCESS;
}


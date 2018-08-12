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

    /* clear status flags */
    unsigned int res = state_reset_status(state);

    if(res != CPU2_ERR_SUCCESS)
    {
        return res;
    }

    int8_t src_val = 0;
    int8_t dst_val = 0;

    /* get value in source register */
    res = state_get_reg(src, &src_val, state);

    if(res != CPU2_ERR_SUCCESS)
    {
        return res;
    }

    /* get value in destination register */
    res = state_get_reg(dest, &dst_val, state);

    if(res != CPU2_ERR_SUCCESS)
    {
        return res;
    }

    int8_t add_result = src_val + dst_val;

    if(add_result == 0)
    {
        res = state_set_status(ISA_STATUS_ZERO, true, state);

        if(res != CPU2_ERR_SUCCESS)
        {
            return res;
        }

        if(src_val != 0 && dst_val)
        {
            res = state_set_status(ISA_STATUS_OVERFLOW, true, state);
           
            if(res != CPU2_ERR_SUCCESS)
            {
                return res;
            }
        }
    }
    else if(add_result < 0)
    {
        res = state_set_status(ISA_STATUS_NEGATIVE, true, state);

        if(res != CPU2_ERR_SUCCESS)
        {
            return res;
        }
        
        if(src_val > 0 && dst_val > 0)
        {
            res = state_set_status(ISA_STATUS_OVERFLOW, true, state);

            if(res != CPU2_ERR_SUCCESS)
            {
                return res;
            }
        }
    }
    else if(add_result > 0)
    {
        if(src_val < 0 && dst_val < 0)
        {
            res = state_set_status(ISA_STATUS_OVERFLOW, true, state);

            if(res != CPU2_ERR_SUCCESS)
            {
                return res;
            }
        }
    }

    res = state_set_reg(dest, add_result, state);

    if(res != CPU2_ERR_SUCCESS)
    {
        return res;
    }

    return CPU2_ERR_SUCCESS;
}

unsigned int isa_op_sub(uint8_t dest, uint8_t src, State* state)
{
    if(state == NULL)
    {
        return CPU2_ERR_NULL;
    }

    /* clear status flags */
    unsigned int res = state_reset_status(state);

    if(res != CPU2_ERR_SUCCESS)
    {
        return res;
    }

    int8_t src_val = 0;
    int8_t dst_val = 0;

    res = state_get_reg(src, &src_val, state);

    if(res != CPU2_ERR_SUCCESS)
    {
        return res;
    }

    res = state_get_reg(dest, &dst_val, state);

    if(res != CPU2_ERR_SUCCESS)
    {
        return res;
    }

    int8_t sub_result = dst_val - src_val;

    if(sub_result == 0)
    {
        res = state_set_status(ISA_STATUS_ZERO, true, state);

        if(res != CPU2_ERR_SUCCESS)
        {
            return res;
        }

        if(src_val != dst_val)
        {
            res = state_set_status(ISA_STATUS_OVERFLOW, true, state);
           
            if(res != CPU2_ERR_SUCCESS)
            {
                return res;
            }
        }
    }
    else if(sub_result < 0)
    {
        res = state_set_status(ISA_STATUS_NEGATIVE, true, state);

        if(res != CPU2_ERR_SUCCESS)
        {
            return res;
        }
        
        if(dst_val > src_val)
        {
            res = state_set_status(ISA_STATUS_OVERFLOW, true, state);

            if(res != CPU2_ERR_SUCCESS)
            {
                return res;
            }
        }
    }
    else if(sub_result > 0)
    {
        if(src_val > dst_val)
        {
            res = state_set_status(ISA_STATUS_OVERFLOW, true, state);

            if(res != CPU2_ERR_SUCCESS)
            {
                return res;
            }
        }
    }

    res = state_set_reg(dest, sub_result, state);

    if(res != CPU2_ERR_SUCCESS)
    {
        return res;
    }

    return CPU2_ERR_SUCCESS;
}

unsigned int isa_op_mul(uint8_t dest, uint8_t src, State* state)
{
    if(state == NULL) /* null guard */
    {
        return CPU2_ERR_NULL;
    }

    /* clear status flags */
    unsigned int res = state_reset_status(state);

    if(res != CPU2_ERR_SUCCESS)
    {
        return res;
    }

    int8_t src_val = 0;
    int8_t dst_val = 0;

    /* get value in source register */
    res = state_get_reg(src, &src_val, state);

    if(res != CPU2_ERR_SUCCESS)
    {
        return res;
    }

    /* get value in destination register */
    res = state_get_reg(dest, &dst_val, state);

    if(res != CPU2_ERR_SUCCESS)
    {
        return res;
    }

    int8_t mul_result = src_val * dst_val;

    if(mul_result == 0)
    {
        res = state_set_status(ISA_STATUS_ZERO, true, state);

        if(res != CPU2_ERR_SUCCESS)
        {
            return res;
        }

        if(src_val != 0 && dst_val)
        {
            res = state_set_status(ISA_STATUS_OVERFLOW, true, state);
           
            if(res != CPU2_ERR_SUCCESS)
            {
                return res;
            }
        }
    }
    else if(mul_result < 0)
    {
        res = state_set_status(ISA_STATUS_NEGATIVE, true, state);

        if(res != CPU2_ERR_SUCCESS)
        {
            return res;
        }
        
        if(src_val > 0 && dst_val > 0)
        {
            res = state_set_status(ISA_STATUS_OVERFLOW, true, state);

            if(res != CPU2_ERR_SUCCESS)
            {
                return res;
            }
        }
    }
    else if(mul_result > 0)
    {
        if(src_val < 0 && dst_val < 0)
        {
            res = state_set_status(ISA_STATUS_OVERFLOW, true, state);

            if(res != CPU2_ERR_SUCCESS)
            {
                return res;
            }
        }
    }

    res = state_set_reg(dest, mul_result, state);

    if(res != CPU2_ERR_SUCCESS)
    {
        return res;
    }

    return CPU2_ERR_SUCCESS;
}

unsigned int isa_op_div(uint8_t dest, uint8_t src, State* state)
{
    if(state == NULL) /* null guard */
    {
        return CPU2_ERR_NULL;
    }

    /* clear status flags */
    unsigned int res = state_reset_status(state);

    if(res != CPU2_ERR_SUCCESS)
    {
        return res;
    }

    int8_t src_val = 0;
    int8_t dst_val = 0;

    /* get value in source register */
    res = state_get_reg(src, &src_val, state);

    if(res != CPU2_ERR_SUCCESS)
    {
        return res;
    }

    /* get value in destination register */
    res = state_get_reg(dest, &dst_val, state);

    if(res != CPU2_ERR_SUCCESS)
    {
        return res;
    }

    int8_t div_result = src_val / dst_val;

    if(div_result == 0)
    {
        res = state_set_status(ISA_STATUS_ZERO, true, state);

        if(res != CPU2_ERR_SUCCESS)
        {
            return res;
        }

        if(src_val != 0 && dst_val)
        {
            res = state_set_status(ISA_STATUS_OVERFLOW, true, state);
           
            if(res != CPU2_ERR_SUCCESS)
            {
                return res;
            }
        }
    }
    else if(div_result < 0)
    {
        res = state_set_status(ISA_STATUS_NEGATIVE, true, state);

        if(res != CPU2_ERR_SUCCESS)
        {
            return res;
        }
        
        if(src_val > 0 && dst_val > 0)
        {
            res = state_set_status(ISA_STATUS_OVERFLOW, true, state);

            if(res != CPU2_ERR_SUCCESS)
            {
                return res;
            }
        }
    }
    else if(div_result > 0)
    {
        if(src_val < 0 && dst_val < 0)
        {
            res = state_set_status(ISA_STATUS_OVERFLOW, true, state);

            if(res != CPU2_ERR_SUCCESS)
            {
                return res;
            }
        }
    }

    res = state_set_reg(dest, div_result, state);

    if(res != CPU2_ERR_SUCCESS)
    {
        return res;
    }

    return CPU2_ERR_SUCCESS;
}

unsigned int isa_op_mod(uint8_t dest, uint8_t src, State* state)
{
    if(state == NULL) /* null guard */
    {
        return CPU2_ERR_NULL;
    }

    /* clear status flags */
    unsigned int res = state_reset_status(state);

    if(res != CPU2_ERR_SUCCESS)
    {
        return res;
    }

    int8_t src_val = 0;
    int8_t dst_val = 0;

    /* get value in source register */
    res = state_get_reg(src, &src_val, state);

    if(res != CPU2_ERR_SUCCESS)
    {
        return res;
    }

    /* get value in destination register */
    res = state_get_reg(dest, &dst_val, state);

    if(res != CPU2_ERR_SUCCESS)
    {
        return res;
    }

    int8_t mod_result = src_val % dst_val;

    if(mod_result == 0) /* check for zero result */
    {
        res = state_set_status(ISA_STATUS_ZERO, true, state);

        if(res != CPU2_ERR_SUCCESS)
        {
            return res;
        }
    }

    res = state_set_reg(dest, mod_result, state);

    if(res != CPU2_ERR_SUCCESS)
    {
        return res;
    }

    return CPU2_ERR_SUCCESS;
}

unsigned int isa_op_jmp(int8_t addr, State* state)
{
    if(state == NULL)
    {
        return CPU2_ERR_NULL;
    }

    if(addr < 0)
    {
        return CPU2_ERR_BOUNDS;
    }    

    unsigned int res = state_set_program_counter(addr, state);

    if(res != CPU2_ERR_SUCCESS)
    {
        return res;
    }

    /* reset status register */
    res = state_reset_status(state);

    if(res != CPU2_ERR_SUCCESS)
    {
        return res;
    }

    return CPU2_ERR_SUCCESS;
}


unsigned int isa_op_jpz(int8_t addr, State* state)
{
    if(state == NULL)
    {
        return CPU2_ERR_NULL;
    }

    bool zero = false;
    
    unsigned int res = state_get_status(ISA_STATUS_ZERO, &zero, state);

    if(zero)
    {
        res = isa_op_jmp(addr, state);

        if(res != CPU2_ERR_SUCCESS)
        {
            return res;
        }
    }

    return CPU2_ERR_SUCCESS;
}

unsigned int isa_op_jpv(int8_t addr, State* state)
{
    if(state == NULL)
    {
        return CPU2_ERR_NULL;
    }

    bool overflow = false;
    
    unsigned int res = state_get_status(ISA_STATUS_OVERFLOW, &overflow, state);

    if(overflow)
    {
        res = isa_op_jmp(addr, state);

        if(res != CPU2_ERR_SUCCESS)
        {
            return res;
        }
    }

    return CPU2_ERR_SUCCESS;
}

unsigned int isa_op_psh(uint8_t reg, State* state)
{
    if(state == NULL)
    {
        return CPU2_ERR_NULL;
    }

    unsigned int res = 0;
    int8_t reg_val = 0;

    /* get value of register */
    res = state_get_reg(reg, &reg_val, state);

    if(res != CPU2_ERR_SUCCESS)
    {
        return res;
    }

    /* push contents of register onto stack */
    res = state_push(reg_val, state);

    if(res != CPU2_ERR_SUCCESS)
    {
        return res;
    }

    return CPU2_ERR_SUCCESS;
}

unsigned int isa_op_pop(uint8_t reg, State* state)
{
    if(state == NULL)
    {
        return CPU2_ERR_NULL;
    }

    unsigned int res = 0;

    int8_t* stk_val = calloc(1, sizeof(int8_t));

    if(stk_val == NULL)
    {
        return CPU2_ERR_SYS_MEM;
    }

    /* pop item off of stack */
    res = state_pop(stk_val, state);

    if(res != CPU2_ERR_SUCCESS)
    {
        return res;
    }

    /* set value of register accordingly */
    res = state_set_reg(reg, *stk_val, state);

    if(res != CPU2_ERR_SUCCESS)
    {
        return res;
    }

    return CPU2_ERR_SUCCESS;
}



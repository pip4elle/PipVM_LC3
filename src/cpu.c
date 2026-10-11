#include "cpu.h"
#include "memory.h"
#include "opcodes.h"

int fetch(uint16_t *instr, cpu_t *cpu, ram_t *ram)
{
    if (!instr || !cpu || !ram)
        return 0;

    *instr = ram->memory[cpu->registers[R_PC]];
    cpu->registers[R_PC]++;

    return 1;
}

int execute(uint16_t *instr, cpu_t *cpu, ram_t *ram)
{
    if (!instr || !cpu || !ram)
        return 0;

    static int (*opFunMap[OP_COUNT])(uint16_t *, cpu_t *, ram_t *) = { // [opcode -> function] mapping 
        op_br,
        op_add,
        op_ld,
        op_st,
        op_jsr,
        op_and,
        op_ldr,
        op_str,
        op_rti,
        op_not,
        op_ldi,
        op_sti,
        op_jmp,
        op_res,
        op_lea,
        op_trap
    };

    if(!(opFunMap[*instr >> 12](instr, cpu, ram))) // [*instr >> 12] last 4 bits
        return 0;

    return 1;
}

void setcc(uint16_t reg, cpu_t *cpu)
{
    uint16_t regval = cpu->registers[reg];
    
    if (regval == 0)
        cpu->registers[R_PSR] = F_ZERO;
    else if (GET_BIT(regval, 15) == 0)
        cpu->registers[R_PSR] = F_POS;
    else
        cpu->registers[R_PSR] = F_NEG;
}
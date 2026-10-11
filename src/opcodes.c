#include "opcodes.h"
#include "cpu.h"
#include "memory.h"
#include "trapcodes.h"
#include "input.h"

#include <stdio.h>

int op_br      (uint16_t *instr, cpu_t *cpu, ram_t *ram)
{
    uint16_t pcoffset = MID_BITS(*instr, 0, 8);
    uint16_t condflag = MID_BITS(*instr, 9, 11);
    
    if (cpu->registers[R_PSR] & condflag)
        cpu->registers[R_PC] += SEXT16(pcoffset, 9);

    return 1;
}

int op_add     (uint16_t *instr, cpu_t *cpu, ram_t *ram)
{
    uint16_t dr = MID_BITS(*instr, 9, 11);
    uint16_t sr1 = MID_BITS(*instr, 6, 8);

    if (GET_BIT(*instr, 5) == 0)
    {
        uint16_t sr2 = MID_BITS(*instr, 0, 2);
        cpu->registers[dr] = cpu->registers[sr1] + cpu->registers[sr2];
    } 
    else
    {
        uint16_t imm5 = SEXT16(MID_BITS(*instr, 0, 4), 5);
        cpu->registers[dr] = cpu->registers[sr1] + imm5;
    }

    setcc(dr, cpu);

    return 1;
}

int op_ld      (uint16_t *instr, cpu_t *cpu, ram_t *ram)
{
    uint16_t dr = MID_BITS(*instr, 9, 11);
    uint16_t pcoffset = SEXT16(MID_BITS(*instr, 0, 8), 9);

    cpu->registers[dr] = mem_read(ram, (uint16_t)(cpu->registers[R_PC] + pcoffset));

    setcc(dr, cpu);

    return 1;
}

int op_st      (uint16_t *instr, cpu_t *cpu, ram_t *ram)
{
    uint16_t sr = MID_BITS(*instr, 9, 11);
    uint16_t pcoffset = SEXT16(MID_BITS(*instr, 0, 8), 9);

    ram->memory[(uint16_t)(cpu->registers[R_PC] + pcoffset)] = cpu->registers[sr];

    return 1;
}

int op_jsr     (uint16_t *instr, cpu_t *cpu, ram_t *ram)
{
    uint64_t tmp = cpu->registers[R_PC];

    if (GET_BIT(*instr, 11) == 0)
    {
        uint16_t br = MID_BITS(*instr, 6, 8);
        cpu->registers[R_PC] = cpu->registers[br];
    }
    else
    {
        uint16_t pcoffset = MID_BITS(*instr, 0, 10);
        cpu->registers[R_PC] += SEXT16(pcoffset, 11);
    }

    cpu->registers[R_R7] = tmp;

    return 1;    
}

int op_and     (uint16_t *instr, cpu_t *cpu, ram_t *ram)
{
    uint16_t dr = MID_BITS(*instr, 9, 11);
    uint16_t sr1 = MID_BITS(*instr, 6, 8);

    if (GET_BIT(*instr, 5) == 0)
    {
        uint16_t sr2 = MID_BITS(*instr, 0, 2);
        cpu->registers[dr] = cpu->registers[sr1] & cpu->registers[sr2];
    } 
    else
    {
        uint16_t imm5 = SEXT16(MID_BITS(*instr, 0, 4), 5);
        cpu->registers[dr] = cpu->registers[sr1] & imm5;
    }

    setcc(dr, cpu);

    return 1;
}

int op_ldr     (uint16_t *instr, cpu_t *cpu, ram_t *ram)
{
    uint16_t dr = MID_BITS(*instr, 9, 11);
    uint16_t br = MID_BITS(*instr, 6, 8);
    uint16_t pcoffset = SEXT16(MID_BITS(*instr, 0, 5), 6);

    cpu->registers[dr] = mem_read(ram, (uint16_t)(cpu->registers[br] + pcoffset));

    setcc(dr, cpu);

    return 1;
}

int op_str     (uint16_t *instr, cpu_t *cpu, ram_t *ram)
{
    uint16_t sr = MID_BITS(*instr, 9, 11);
    uint16_t br = MID_BITS(*instr, 6, 8);
    uint16_t pcoffset = SEXT16(MID_BITS(*instr, 0, 5), 6);

    ram->memory[(uint16_t)(cpu->registers[br] + pcoffset)] = cpu->registers[sr];

    return 1;
}

int op_rti     (uint16_t *instr, cpu_t *cpu, ram_t *ram)
{
    return 0;
}

int op_not     (uint16_t *instr, cpu_t *cpu, ram_t *ram)
{
    uint16_t dr = MID_BITS(*instr, 9, 11);
    uint16_t sr = MID_BITS(*instr, 6, 8);

    cpu->registers[dr] = ~cpu->registers[sr];

    setcc(dr, cpu);

    return 1;
}

int op_ldi     (uint16_t *instr, cpu_t *cpu, ram_t *ram)
{
    uint16_t dr = MID_BITS(*instr, 9, 11);
    uint16_t pcoffset = SEXT16(MID_BITS(*instr, 0, 8), 9);

    cpu->registers[dr] = mem_read(ram, mem_read(ram, (uint16_t)(cpu->registers[R_PC] + pcoffset)));

    setcc(dr, cpu);

    return 1;
}

int op_sti     (uint16_t *instr, cpu_t *cpu, ram_t *ram)
{
    uint16_t sr = MID_BITS(*instr, 9, 11);
    uint16_t pcoffset = SEXT16(MID_BITS(*instr, 0, 8), 9);

    ram->memory[ram->memory[(uint16_t)(cpu->registers[R_PC] + pcoffset)]] = cpu->registers[sr];

    return 1;
}

int op_jmp     (uint16_t *instr, cpu_t *cpu, ram_t *ram)
{
    uint16_t br = MID_BITS(*instr, 6, 8);
    
    cpu->registers[R_PC] = cpu->registers[br];

    return  1;
}

int op_res     (uint16_t *instr, cpu_t *cpu, ram_t *ram)
{
    return 0;
}

int op_lea     (uint16_t *instr, cpu_t *cpu, ram_t *ram)
{
    uint16_t dr = MID_BITS(*instr, 9, 11);
    uint16_t pcoffset = SEXT16(MID_BITS(*instr, 0, 8), 9);

    cpu->registers[dr] = cpu->registers[R_PC] + pcoffset;

    setcc(dr, cpu);

    return 1;
}

int op_trap    (uint16_t *instr, cpu_t *cpu, ram_t *ram)
{
    cpu->registers[R_R7] = cpu->registers[R_PC];
    uint16_t trapvect = ZEXT16(MID_BITS(*instr, 0, 7), 8);

    switch (trapvect)
    {
        case T_GETC:
            t_getc(cpu);
            break;

        case T_OUT:
            t_out(cpu);
            break;

        case T_PUTS:
            t_puts(cpu, ram);
            break;

        case T_IN:
            t_in(cpu);
            break;

        case T_PUTSP:
            t_putsp(cpu, ram);
            break;

        case T_HALT:
            t_halt(cpu);
            break;
            
        default:
            fprintf(stderr, "Error: invalid trap code (HEX: %x).\n", trapvect);
            return 0;
    }

    return 1;
}
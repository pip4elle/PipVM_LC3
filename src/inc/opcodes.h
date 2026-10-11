#ifndef OPCODES_H
#define OPCODES_H

#include "cpu.h"
#include "memory.h"

#include <stdint.h>

#define GET_BIT(k, n) (((k) >> (n)) & 1) // get k's nth bit
#define LAST_BIT(k, n) ((k) & ((1 << (n)) - 1))
#define MID_BITS(k, m, n) LAST_BIT((k) >> (m), ((n) - (m) + 1)) // extract bits from k (m inclusive to n inclusive)
#define SEXT16(k, n) ((uint16_t)((((k) & ((1u << (n)) - 1)) ^ (1u << ((n) - 1))) - (1u << ((n) - 1)))) // sign extend k made of n bits to 16 bits
#define ZEXT16(k, n) ((uint16_t)((k) & ((1u << (n)) - 1))) // zero extend k made of n bits to 16 bits

typedef enum
{
    OP_BR = 0, // branch
    OP_ADD,    // add
    OP_LD,     // load
    OP_ST,     // store
    OP_JSR,    // jump register
    OP_AND,    // bitwise and
    OP_LDR,    // load register
    OP_STR,    // store register
    OP_RTI,    // unused
    OP_NOT,    // bitwise not
    OP_LDI,    // load indirect
    OP_STI,    // store indirect
    OP_JMP,    // jump
    OP_RES,    // reserved (unused)
    OP_LEA,    // load effective address
    OP_TRAP,   // execute trap
    OP_COUNT   // USED FOR ARRAY SIZE
} opcode_t;

int op_br      (uint16_t *instr, cpu_t *cpu, ram_t *ram);
int op_add     (uint16_t *instr, cpu_t *cpu, ram_t *ram);
int op_ld      (uint16_t *instr, cpu_t *cpu, ram_t *ram);
int op_st      (uint16_t *instr, cpu_t *cpu, ram_t *ram);
int op_jsr     (uint16_t *instr, cpu_t *cpu, ram_t *ram);
int op_and     (uint16_t *instr, cpu_t *cpu, ram_t *ram);
int op_ldr     (uint16_t *instr, cpu_t *cpu, ram_t *ram);
int op_str     (uint16_t *instr, cpu_t *cpu, ram_t *ram);
int op_rti     (uint16_t *instr, cpu_t *cpu, ram_t *ram);
int op_not     (uint16_t *instr, cpu_t *cpu, ram_t *ram);
int op_ldi     (uint16_t *instr, cpu_t *cpu, ram_t *ram);
int op_sti     (uint16_t *instr, cpu_t *cpu, ram_t *ram);
int op_jmp     (uint16_t *instr, cpu_t *cpu, ram_t *ram);
int op_res     (uint16_t *instr, cpu_t *cpu, ram_t *ram);
int op_lea     (uint16_t *instr, cpu_t *cpu, ram_t *ram);
int op_trap    (uint16_t *instr, cpu_t *cpu, ram_t *ram);

#endif
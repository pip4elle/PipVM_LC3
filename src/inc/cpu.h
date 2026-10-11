#ifndef CPU_H
#define CPU_H

#include "memory.h"

#include <stdint.h>

typedef enum
{
    R_R0 = 0,
    R_R1,
    R_R2,
    R_R3,
    R_R4,
    R_R5,
    R_R6,
    R_R7,
    R_PC,   // Program Counter
    R_PSR,  // Processor Status Register
    R_RUNNING,
    R_COUNT // For Array Size
} reg_t;

typedef enum
{
    F_POS = 1 << 0,  // 0001
    F_ZERO = 1 << 1, // 0010
    F_NEG = 1 << 2   // 0100
} flag_t;

typedef struct
{
    uint16_t *registers;
} cpu_t;

int fetch(uint16_t *instr, cpu_t *cpu, ram_t *ram);
int execute(uint16_t *instr, cpu_t *cpu, ram_t *ram);
void setcc(uint16_t reg, cpu_t *cpu);

#endif
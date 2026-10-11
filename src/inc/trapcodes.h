#ifndef TRAP_H
#define TRAP_H

#include "cpu.h"
#include "memory.h"

typedef enum
{
    T_GETC  = 0x20, // get character from keyboard, not echoed onto the terminal
    T_OUT   = 0x21, // output a character
    T_PUTS  = 0x22, // output a word string
    T_IN    = 0x23, // get character from keyboard, echoed onto the terminal
    T_PUTSP = 0x24, // output a byte string
    T_HALT  = 0x25  // halt the program
} trapcode_t;

void t_getc     (cpu_t *cpu);
void t_out      (cpu_t *cpu);
void t_puts     (cpu_t *cpu, ram_t *ram);
void t_in       (cpu_t *cpu);
void t_putsp    (cpu_t *cpu, ram_t *ram);
void t_halt     (cpu_t *cpu);

#endif
#include "trapcodes.h"
#include "cpu.h"
#include "memory.h"

#include <stdio.h>

void t_getc(cpu_t *cpu)
{
    cpu->registers[R_R0] = (uint16_t)getchar();
    setcc(R_R0, cpu);
}

void t_out(cpu_t *cpu)
{
    putc((char)cpu->registers[R_R0], stdout);
    fflush(stdout);
}

void t_puts(cpu_t *cpu, ram_t *ram)
{
    uint16_t *c = &(ram->memory[cpu->registers[R_R0]]);

    while (*c)
    {
        putc((char)(*c), stdout);
        ++c;
    }

    fflush(stdout);
}

void t_in(cpu_t *cpu)
{
    printf("Enter a character: ");
    
    char c = getchar();
    
    putc(c, stdout);
    fflush(stdout);
    
    cpu->registers[R_R0] = (uint16_t)c;
    
    setcc(R_R0, cpu);
}

void t_putsp(cpu_t *cpu, ram_t *ram)
{
    // one char per byte (two bytes per word) need to swap back to big endian format

    uint16_t *c = &(ram->memory[cpu->registers[R_R0]]);

    while (*c)
    {
        char char1 = (*c) & 0xFF;
        
        putc(char1, stdout);

        char char2 = (*c) >> 8;

        if (char2)
            putc(char2, stdout);
        
        c++;
    }

    fflush(stdout);
}

void t_halt(cpu_t *cpu)
{
    puts("HALT");
    fflush(stdout);
    cpu->registers[R_RUNNING] = 0u;
}
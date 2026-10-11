#include "init.h"
#include "cpu.h"
#include "memory.h"
#include "trapcodes.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

void swap16(uint16_t *val) { // from Little Endian to Big Endian
    *val = (*val << 8) | (*val >> 8);
} 

int init(cpu_t *cpu, ram_t *ram)
{
    if (!cpu || !ram)
        return 0;

    cpu->registers = (uint16_t *)calloc(R_COUNT, sizeof(uint16_t));

    ram->size = MEM_SLOTS;
    ram->memory = (uint16_t *)calloc(ram->size, sizeof(uint16_t));
    
    if (!cpu->registers || !ram->memory)
    {
        free(cpu->registers);
        free(ram->memory);

        cpu->registers = NULL;
        ram->memory = NULL;

        return 0;
    }

    cpu->registers[R_PSR] = F_ZERO;
    cpu->registers[R_RUNNING] = 1u;

    return 1;
}

int load(cpu_t* cpu, ram_t *ram, char *file)
{
    FILE *fptr = fopen(file, "rb");

    if (!fptr)
        return 0;

    uint16_t origin;
    if (fread(&origin, sizeof(uint16_t), 1, fptr) != 1)
    {
        fclose(fptr);
        return 0;
    }
    swap16(&origin);

    size_t maxwords = MEM_SLOTS - origin;
    size_t nread = fread(&ram->memory[origin], sizeof(uint16_t), maxwords, fptr);
    fclose(fptr);

    if (!nread || (origin + nread - 1) > USR_END || origin < USR_START)
        return 0;

    for (size_t i = 0; i < nread; i++)
        swap16(&ram->memory[origin + i]);
    
    cpu->registers[R_PC] = origin;
    
    return 1;
}
#ifndef INIT_H
#define INIT_H

#include "cpu.h"
#include "memory.h"

void swap16(uint16_t *val); // from Little Endian to Big Endian (for modern architectures es. x86_64)

int init(cpu_t *cpu, ram_t *ram);
int load(cpu_t *cpu, ram_t *ram, char *file);

#endif
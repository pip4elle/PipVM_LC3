#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>
#include <stddef.h>

#define MEM_SLOTS   0x10000 // 2^16

#define USR_START   0x3000
#define USR_END     0xfdff

#define TRAP_START  0x0000
#define TRAP_END    0x00ff

#define MR_KBSR 0xFE00
#define MR_KBDR 0xFE02

typedef struct
{
    uint16_t *memory;
    size_t size;
} ram_t;

#endif
#ifndef INPUT_H
#define INPUT_H

#include "memory.h"
#include <stdint.h>

void     disable_input_buffering(void);
void     restore_input_buffering(void);
uint16_t mem_read(ram_t *ram, uint16_t addr);

#endif
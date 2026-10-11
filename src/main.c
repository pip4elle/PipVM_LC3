#include "cpu.h"
#include "memory.h"
#include "opcodes.h"
#include "init.h"
#include "usage.h"
#include "input.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

static void handle_sigint(int sig)
{
    (void)sig;
    restore_input_buffering();
    printf("\n");
    exit(130);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        usage(argv[0]);
        return 1;
    }

    cpu_t *cpu = (cpu_t *)calloc(1, sizeof(cpu_t));
    ram_t *ram = (ram_t *)calloc(1, sizeof(ram_t));

    int exitCode = 0;
    uint16_t instr;

    if (!cpu || !ram)
    {
        fprintf(stderr, "Error: can't allocate memory for CPU or RAM.\n");
        exitCode = 1;
        goto cleanup;
    }

    if (!init(cpu, ram))
    {
        fprintf(stderr, "Error: init() failed.\n");
        exitCode = 1;
        goto cleanup;
    }

    if (!load(cpu, ram, argv[1])) // Loads binary into the ram
    {
        fprintf(stderr, "Error: load() failed.\n");
        exitCode = 1;
        goto cleanup;
    }

    signal(SIGINT, handle_sigint);
    disable_input_buffering();

    while (cpu->registers[R_RUNNING])
    {
        if (!fetch(&instr, cpu, ram))
        {
            fprintf(stderr, "Error: fetch() failed.\n");
            exitCode = 1;
            goto cleanup;
        }

        if (!execute(&instr, cpu, ram))
        {
            fprintf(stderr, "Error: execute() failed (Bad opcode).\n");
            exitCode = 1;
            goto cleanup;
        }
    }

cleanup:
    restore_input_buffering();
    
    if (cpu)
        free(cpu->registers);
    if (ram)
        free(ram->memory);

    free(cpu);
    free(ram);

    return exitCode;
}
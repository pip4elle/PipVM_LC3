#include "input.h"

#include <stdio.h>
#include <sys/select.h>
#include <termios.h>
#include <unistd.h>

static struct termios orig_tio;
static int tio_saved = 0;

void disable_input_buffering(void)
{
    if (tcgetattr(STDIN_FILENO, &orig_tio) != 0)
        return;

    tio_saved = 1;

    struct termios raw = orig_tio;
    raw.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &raw);
}

void restore_input_buffering(void)
{
    if (tio_saved)
        tcsetattr(STDIN_FILENO, TCSANOW, &orig_tio);
}

static int check_key(void)
{
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(STDIN_FILENO, &readfds);

    struct timeval timeout = {0, 0}; // not bloccking

    return select(1, &readfds, NULL, NULL, &timeout) > 0;
}

uint16_t mem_read(ram_t *ram, uint16_t addr)
{
    if (addr == MR_KBSR)
    {
        if (check_key())
        {
            ram->memory[MR_KBSR] = 1u << 15;
            ram->memory[MR_KBDR] = (uint16_t)getchar();
        }
        else
            ram->memory[MR_KBSR] = 0;
    }

    return ram->memory[addr];
}
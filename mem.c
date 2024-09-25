#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "mem.h"
#include "command.h"

static bool mem_read(uint8_t argc, char *argv[])
{
    if (argc != 2) {
        printf("usage: rd <addr>\n");
        return false;
    }

    uint16_t addr = strtoul(argv[1], NULL, 16);
    uint8_t val = (*(volatile uint8_t *)(addr));

    printf("0x%02X\n", val);

    return true;
}

static bool mem_write(uint8_t argc, char *argv[])
{
    if (argc != 3) {
        printf("usage: wr <addr> <byte_val>\n");
        return false;
    }

    uint16_t addr = strtoul(argv[1], NULL, 16);
    uint8_t val = strtoul(argv[2], NULL, 16);

    (*(volatile uint8_t *)(addr)) = val;

    return true;
}

void mem_init(void)
{
    cmd_register("rd", mem_read);
    cmd_register("wr", mem_write);
}

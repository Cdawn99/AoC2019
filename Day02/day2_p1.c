#include "intcode.h"

#define DAWN_IMPLEMENTATION
#include "../dawn_utils.h"

#include <stdio.h>

int main(int argc, char **argv) {
    char *program = dawn_shift_args(&argc, &argv);
    if (argc == 0) {
        printf("Usage: %s <program.txt>\n", program);
        return 1;
    }

    char *fp = dawn_shift_args(&argc, &argv);
    Program p = read_program(fp);
    if (p.capacity == 0) return 1;

    p.items[1] = 12;
    p.items[2] = 2;

    run_program(&p);
    printf("Value at position 0: %ld\n", p.items[0]);

    free_program(p);

    return 0;
}

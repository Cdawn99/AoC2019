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

    int64_t noun = 69;
    int64_t verb = 79;

    p.items[1] = noun;
    p.items[2] = verb;

    run_program(&p);
    printf("Value at position 0: %ld\n", p.items[0]);
    printf("Expected value:      %d\n", 19690720);

    printf("100 * noun + verb == %ld\n", 100 * noun + verb);

    free_program(p);

    return 0;
}

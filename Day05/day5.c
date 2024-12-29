#define DAWN_IMPLEMENTATION
#include "../dawn_utils.h"

#include "intcode.h"

int main(int argc, char **argv) {
    char *program = dawn_shift_args(&argc, &argv);
    if (argc == 0) {
        printf("Usage: %s <program.txt>\n", program);
        return 1;
    }

    char *fp = dawn_shift_args(&argc, &argv);
    Program p = read_program(fp);

    run_program(&p);

    free_program(p);

    return 0;
}

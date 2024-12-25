#define DAWN_IMPLEMENTATION
#include "../dawn_utils.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
    char *program = dawn_shift_args(&argc, &argv);
    if (argc == 0) {
        printf("Usage: %s <module_masses.txt>\n", program);
        return 1;
    }

    char *fp = dawn_shift_args(&argc, &argv);
    DawnStringBuilder masses = {0};
    if (!dawn_read_entire_file(fp, &masses)) return 1;
    DAWN_SB_APPEND_BUF(&masses, "", 1);

    uint64_t total_fuel = 0;
    char delim[] = "\n";
    char *token = strtok(masses.items, delim);
    while (token) {
        uint64_t fuel = atoi(token) / 3 - 2;
        // printf("Fuel: %zu\n", fuel);
        total_fuel += fuel;
        token = strtok(NULL, delim);
    }

    printf("Sum of fule requirements: %zu\n", total_fuel);

    DAWN_SB_FREE(masses);
    return 0;
}

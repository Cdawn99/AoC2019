#include "day12.h"

#define DAWN_IMPLEMENTATION
#include "../dawn_utils.h"

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

int main(int argc, char **argv) {
    char *program = dawn_shift_args(&argc, &argv);
    if (argc == 0) {
        printf("Usage: %s <moon_info.txt> <num_steps>\n", program);
        return 1;
    }

    char *fp = dawn_shift_args(&argc, &argv);
    if (argc == 0) {
        printf("Usage: %s <moon_info.txt> <num_steps>\n", program);
        return 1;
    }

    char *num_steps = dawn_shift_args(&argc, &argv);
    int64_t steps = parse_int(num_steps);

    Moons moons = moons_init(fp);
    int64_t E_tot = moons_total_energy(moons);
    printf("0: Total energy: %" PRId64 "\n", E_tot);

    for (int64_t i = 0; i < steps; i++) {
        moons_step(moons);
        int64_t E_tot = moons_total_energy(moons);
        printf("%" PRId64 ": Total energy: %" PRId64 "\n", i + 1, E_tot);
    }

    moons_free(moons);

    return 0;
}

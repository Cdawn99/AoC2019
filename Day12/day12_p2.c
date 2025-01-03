#include "day12.h"

#define DAWN_IMPLEMENTATION
#include "../dawn_utils.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

static size_t gcd(size_t a, size_t b) {
    if (b == 0) return a;
    return gcd(b, a % b);
}

static size_t lcm(size_t a, size_t b) {
    return a * (b / gcd(a, b));
}

int main(int argc, char **argv) {
    char *program = dawn_shift_args(&argc, &argv);
    if (argc == 0) {
        printf("Usage: %s <moon_info.txt>\n", program);
        return 1;
    }

    char *fp = dawn_shift_args(&argc, &argv);
    Moons moons = moons_init(fp);

    size_t x_cycle = find_coordinate_cycle(moons, X);
    size_t y_cycle = find_coordinate_cycle(moons, Y);
    size_t z_cycle = find_coordinate_cycle(moons, Z);

    size_t lcm_xy = x_cycle > y_cycle ? lcm(x_cycle, y_cycle) : lcm(y_cycle, x_cycle);
    size_t lcm_xyz = lcm_xy > z_cycle ? lcm(lcm_xy, z_cycle) : lcm(z_cycle, lcm_xy);

    printf("Number of steps to repeat: %zu\n", lcm_xyz);

    moons_free(moons);

    return 0;
}

#include "day14.h"

#define DAWN_IMPLEMENTATION
#include "../dawn_utils.h"

#include <inttypes.h>
#include <stdint.h>

int main(int argc, char **argv) {
    char *program = dawn_shift_args(&argc, &argv);
    if (argc == 0) {
        printf("Usage: %s <reactions.txt>\n", program);
        return 1;
    }

    char *fp = dawn_shift_args(&argc, &argv);
    Reactions rs = reactions_init(fp);

    double ore_amount = compute_exact_ore_requirement(rs);
    printf("Amount of fuel that can be produced: %" PRIu64 "\n", (uint64_t)(1000000000000.0/ore_amount));

    reactions_free(rs);

    return 0;
}

#include "day14.h"

#define DAWN_IMPLEMENTATION
#include "../dawn_utils.h"

int main(int argc, char **argv) {
    char *program = dawn_shift_args(&argc, &argv);
    if (argc == 0) {
        printf("Usage: %s <reactions.txt>\n", program);
        return 1;
    }

    char *fp = dawn_shift_args(&argc, &argv);
    Reactions rs = reactions_init(fp);

    size_t ore_amount = compute_ore_requirement(rs);
    printf("Amount of ore required: %zu\n", ore_amount);

    reactions_free(rs);

    return 0;
}

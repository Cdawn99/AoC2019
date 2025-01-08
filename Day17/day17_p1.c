#include "day17.h"

#define DAWN_IMPLEMENTATION
#include "../dawn_utils.h"

#include <stdio.h>
#include <string.h>


int main(int argc, char **argv) {
    char *program = dawn_shift_args(&argc, &argv);
    if (argc == 0) {
        printf("Usage: %s <program.txt>\n", program);
        return 1;
    }

    char *fp = dawn_shift_args(&argc, &argv);

    DawnStringBuilder map_data = get_map_data(fp);

    printf("%.*s", (int)map_data.length, map_data.items);

    Map map = map_init(map_data);

    size_t sum = get_sum_of_alignment_parameters(map);
    printf("Sum of alignment parameters: %zu\n", sum);

    map_free(map);
    DAWN_SB_FREE(map_data);

    return 0;
}

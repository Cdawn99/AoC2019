#define DAWN_IMPLEMENTATION
#include "../dawn_utils.h"
#include "day3.h"

#include <stdio.h>

int main(int argc, char **argv) {
    char *program = dawn_shift_args(&argc, &argv);
    if (argc == 0) {
        printf("Usage: %s <wire_info.txt>\n", program);
        return 1;
    }

    char *fp = dawn_shift_args(&argc, &argv);
    DawnStringBuilder info = {0};
    if (!dawn_read_entire_file(fp, &info)) return 1;

    char *wire1 = info.items;
    char *wire2 = info.items;
    while (*wire2 != '\n') wire2++;
    wire2++;

    Wire w1 = wire_init(wire1);
    Wire w2 = wire_init(wire2);

    GridInfo grid_info = get_grid_info(w1, w2);
    Grid grid = grid_init(grid_info);

    grid_write_wires(grid, w1, w2);

    // grid_print(grid);

    int min_cross_dist = get_min_crossing_dist(grid);
    printf("Minimun crossing dist: %d\n", min_cross_dist);

    grid_free(grid);
    wire_free(w2);
    wire_free(w1);
    DAWN_SB_FREE(info);

    return 0;
}

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

    Intersections i1 = intersections_init(grid, w1);
    Intersections i2 = intersections_init(grid, w2);

    int min_delay = get_min_total_delay(i1, i2);
    printf("Min delay: %d\n", min_delay);

    intersections_free(i2);
    intersections_free(i1);
    grid_free(grid);
    wire_free(w2);
    wire_free(w1);
    DAWN_SB_FREE(info);

    return 0;
}

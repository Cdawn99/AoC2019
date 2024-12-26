#ifndef DAY3_H_
#define DAY3_H_

#include <stddef.h>

typedef struct {
    size_t center_x;
    size_t center_y;
    size_t width;
    size_t height;
} GridInfo;

GridInfo get_grid_info(char *w1, char *w2);

typedef struct {
    GridInfo info;
    char **grid;
} Grid;

Grid grid_init(GridInfo info);
void grid_free(Grid grid);

void grid_write_wires(Grid g, char *w1, char *w2);
size_t get_min_crossing_dist(Grid g);

void grid_print(Grid g);

#endif // DAY3_H_

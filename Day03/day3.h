#ifndef DAY3_H_
#define DAY3_H_

#include <stddef.h>

typedef struct {
    int length;
    int step_x;
    int step_y;
} WireSegment;

typedef struct {
    size_t length;
    size_t capacity;
    WireSegment *items;
} Wire;

Wire wire_init(char *desc);
void wire_free(Wire w);

typedef struct {
    int center_x;
    int center_y;
    int width;
    int height;
} GridInfo;

GridInfo get_grid_info(Wire w1, Wire w2);

typedef struct {
    GridInfo info;
    char **grid;
} Grid;

Grid grid_init(GridInfo info);
void grid_free(Grid g);
void grid_print(Grid g);

void grid_write_wires(Grid g, Wire w1, Wire w2);
int get_min_crossing_dist(Grid g);

typedef struct {
    int x;
    int y;
    int delay;
} Intersection;

typedef struct {
    size_t length;
    size_t capacity;
    Intersection *items;
} Intersections;

Intersections intersections_init(Grid g, Wire w);
void intersections_free(Intersections is);

int get_min_total_delay(Intersections is1, Intersections is2);

#endif // DAY3_H_

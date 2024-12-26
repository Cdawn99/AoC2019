#include "day3.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

GridInfo get_grid_info(char *w1, char *w2) {
    int x = 0;
    int y = 0;

    int min_x = 0;
    int min_y = 0;

    int max_x = 0;
    int max_y = 0;

    while (*w1 != '\0') {
        switch (*w1) {
            case 'L':
            case 'R': {
                int dx = atoi(w1 + 1);
                x += *w1 == 'R' ? dx : -dx;
            } break;

            case 'U':
            case 'D': {
                int dy = atoi(w1 + 1);
                y += *w1 == 'U' ? dy : -dy;
            } break;
        }

        if (x > max_x) max_x = x;
        if (x < min_x) min_x = x;

        if (y > max_y) max_y = y;
        if (y < min_y) min_y = y;

        while (*w1 != ',' && *w1 != '\0') w1++;
        if (*w1 == ',') w1++;
    }

    x = 0;
    y = 0;

    while (*w2 != '\0') {
        switch (*w2) {
            case 'L':
            case 'R': {
                int dx = atoi(w2 + 1);
                x += *w2 == 'R' ? dx : -dx;
            } break;

            case 'U':
            case 'D': {
                int dy = atoi(w2 + 1);
                y += *w2 == 'U' ? dy : -dy;
            } break;
        }
        while (*w2 != ',' && *w2 != '\0') w2++;

        if (x > max_x) max_x = x;
        if (x < min_x) min_x = x;

        if (y > max_y) max_y = y;
        if (y < min_y) min_y = y;

        if (*w2 == ',') w2++;
    }

    return (GridInfo){
        .center_x = abs(min_x),
        .center_y = abs(min_y),
        .width = max_x - min_x + 1,
        .height = max_y - min_y + 1,
    };
}

Grid grid_init(GridInfo info) {
    char *grid_data = malloc((info.width + 1) * info.height * sizeof(*grid_data));
    assert(grid_data && "Ran out of RAM");
    memset(grid_data, '.', (info.width + 1) * info.height);
    grid_data[(info.width + 1) * info.height - 1] = '\0';

    char **grid = malloc(info.height * sizeof(*grid));
    assert(grid && "Ran out of RAM");

    grid[0] = grid_data;
    for (size_t i = 1; i < info.height; i++) {
        grid[i] = grid_data + i * (info.width + 1);
        grid[i][-1] = '\n';
    }

    grid[info.center_y][info.center_x] = 'o';

    return (Grid){
        .info = info,
        .grid = grid,
    };
}

void grid_free(Grid grid) {
    if (grid.grid) free(grid.grid[0]);
    free(grid.grid);
}

void grid_write_wires(Grid g, char *w1, char *w2) {
    size_t x = g.info.center_x;
    size_t y = g.info.center_y;

    while (*w1 != '\0') {
        int dx = 0;
        int dy = 0;

        switch (*w1) {
            case 'L':
            case 'R': {
                dx = atoi(w1 + 1);
            } break;

            case 'U':
            case 'D': {
                dy = atoi(w1 + 1);
            } break;
        }

        while (dx > 0) {
            if (*w1 == 'R') {
                x += 1;
            } else {
                x -= 1;
            }

            g.grid[y][x] = '1';

            dx--;
        }

        while (dy > 0) {
            if (*w1 == 'U') {
                y += 1;
            } else {
                y -= 1;
            }

            g.grid[y][x] = '1';

            dy--;
        }

        while (*w1 != ',' && *w1 != '\0') w1++;
        if (*w1 == ',') w1++;
    }

    x = g.info.center_x;
    y = g.info.center_y;

    while (*w2 != '\0') {
        int dx = 0;
        int dy = 0;

        switch (*w2) {
            case 'L':
            case 'R': {
                dx = atoi(w2 + 1);
            } break;

            case 'U':
            case 'D': {
                dy = atoi(w2 + 1);
            } break;
        }

        while (dx > 0) {
            if (*w2 == 'R') {
                x += 1;
            } else {
                x -= 1;
            }

            if (g.grid[y][x] == '1') {
                g.grid[y][x] = 'x';
            } else {
                g.grid[y][x] = '2';
            }

            dx--;
        }

        while (dy > 0) {
            if (*w2 == 'U') {
                y += 1;
            } else {
                y -= 1;
            }

            if (g.grid[y][x] == '1') {
                g.grid[y][x] = 'x';
            } else {
                g.grid[y][x] = '2';
            }

            dy--;
        }

        while (*w2 != ',' && *w2 != '\0') w2++;
        if (*w2 == ',') w2++;
    }
}

size_t get_min_crossing_dist(Grid g) {
    size_t dist = (size_t)-1;

    for (size_t y = 0; y < g.info.height; y++) {
        for (size_t x = 0; x < g.info.width; x++) {
            if (g.grid[y][x] != 'x') continue;

            size_t dx = abs((int)x - (int)g.info.center_x);
            size_t dy = abs((int)y - (int)g.info.center_y);

            if (dist > dx + dy) dist = dx + dy;
        }
    }

    return dist;
}

void grid_print(Grid g) {
    puts(g.grid[0]);
}

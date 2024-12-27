#include "day3.h"
#include "../dawn_utils.h"

#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

Wire wire_init(char *desc) {
    Wire wire = {0};
    while (*desc != '\n') {
        WireSegment ws = {0};
        switch (*desc) {
            case 'R':
                ws.step_x = 1;
                break;
            case 'U':
                ws.step_y = 1;
                break;
            case 'L':
                ws.step_x = -1;
                break;
            case 'D':
                ws.step_y = -1;
                break;
        }
        desc++;
        int len = 0;
        while (*desc != '\n' && *desc != ',') {
            len *= 10;
            len += *desc - '0';
            desc++;
        }
        ws.length = len;
        DAWN_DA_APPEND(&wire, ws);
        if (*desc != '\n') desc++;
    }
    return wire;
}

void wire_free(Wire w) {
    DAWN_DA_FREE(w);
}

GridInfo get_grid_info(Wire w1, Wire w2) {
    int min_x = 0;
    int min_y = 0;
    int max_x = 0;
    int max_y = 0;
    int px = 0;
    int py = 0;
    for (size_t i = 0; i < w1.length; i++) {
        WireSegment it = w1.items[i];
        px += it.step_x * it.length;
        py += it.step_y * it.length;
        if (px > max_x) max_x = px;
        if (py > max_y) max_y = py;
        if (px < min_x) min_x = px;
        if (py < min_y) min_y = py;
    }
    px = 0;
    py = 0;
    for (size_t i = 0; i < w2.length; i++) {
        WireSegment it = w2.items[i];
        px += it.step_x * it.length;
        py += it.step_y * it.length;
        if (px > max_x) max_x = px;
        if (py > max_y) max_y = py;
        if (px < min_x) min_x = px;
        if (py < min_y) min_y = py;
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
    assert(grid_data && "Buy more RAM lol");
    memset(grid_data, '.', (info.width + 1) * info.height);
    grid_data[(info.width + 1) * info.height - 1] = '\0';

    char **grid = malloc(info.height * sizeof(*grid));
    assert(grid && "Buy more RAM lol");

    grid[0] = grid_data;
    for (int i = 1; i < info.height; i++) {
        grid[i] = grid_data + i * (info.width + 1);
        grid[i][-1] = '\n';
    }

    grid[info.center_y][info.center_x] = 'o';

    return (Grid){
        .grid = grid,
        .info = info,
    };
}

void grid_free(Grid g) {
    if (g.grid) free(g.grid[0]);
    free(g.grid);
}

void grid_print(Grid g) {
    puts(g.grid[0]);
}

void grid_write_wires(Grid g, Wire w1, Wire w2) {
    int px = g.info.center_x;
    int py = g.info.center_y;
    for (size_t i = 0; i < w1.length; i++) {
        WireSegment it = w1.items[i];
        for (int j = 0; j < it.length; j++) {
            px += it.step_x;
            py += it.step_y;
            g.grid[py][px] = '1';
        }
    }
    px = g.info.center_x;
    py = g.info.center_y;
    for (size_t i = 0; i < w2.length; i++) {
        WireSegment it = w2.items[i];
        for (int j = 0; j < it.length; j++) {
            px += it.step_x;
            py += it.step_y;
            if (g.grid[py][px] == '1') {
                g.grid[py][px] = 'x';
            } else {
                g.grid[py][px] = '2';
            }
        }
    }
}

int get_min_crossing_dist(Grid g) {
    int dist = INT_MAX;
    for (int y = 0; y < g.info.height; y++) {
        for (int x = 0; x < g.info.width; x++) {
            if (g.grid[y][x] != 'x') continue;
            int dx = abs(x - g.info.center_x);
            int dy = abs(y - g.info.center_y);
            if (dist > dx + dy) dist = dx + dy;
        }
    }
    return dist;
}

static bool intersection_contains(Intersections is, Intersection i) {
    for (size_t j = 0; j < is.length; j++) {
        Intersection it = is.items[j];
        if (it.x == i.x && it.y == i.y) return true;
    }
    return false;
}

Intersections intersections_init(Grid g, Wire w) {
    Intersections is = {0};
    int px = g.info.center_x;
    int py = g.info.center_y;
    int delay = 0;
    for (size_t i = 0; i < w.length; i++) {
        WireSegment it = w.items[i];
        for (int j = 0; j < it.length; j++) {
            px += it.step_x;
            py += it.step_y;
            delay += 1;
            Intersection candidate = {.x = px, .y = py, .delay = delay};
            if (g.grid[py][px] == 'x' && !intersection_contains(is, candidate)) {
                DAWN_DA_APPEND(&is, candidate);
            }
        }
    }
    return is;
}

void intersections_free(Intersections is) {
    DAWN_DA_FREE(is);
}

int get_min_total_delay(Intersections is1, Intersections is2) {
    int min_delay = INT_MAX;
    for (size_t i = 0; i < is1.length; i++) {
        Intersection it = is1.items[i];
        for (size_t j = 0; j < is2.length; j++) {
            Intersection jt = is2.items[j];
            if (it.x == jt.x && it.y == jt.y) {
                int total_delay = it.delay + jt.delay;
                if (total_delay < min_delay) min_delay = total_delay;
                break;
            }
        }
    }
    return min_delay;
}

#include "day18.h"

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

typedef struct {
    Position p;
    size_t dist;
} LElem;

typedef struct {
    size_t length;
    size_t capacity;
    LElem *items;
} List;

Map map_init(DawnStringBuilder map_data) {
    Map map = {0};
    char delim[] = "\n";
    char *token = strtok(map_data.items, delim);
    while (token) {
        DAWN_DA_APPEND(&map, token);
        token = strtok(NULL, delim);
    }
    map.width = strlen(map.items[0]);
    return map;
}

void map_free(Map map) {
    DAWN_DA_FREE(map);
}

size_t count_keys(Map map) {
    size_t count = 0;
    for (size_t y = 1; y < map.length; y++) {
        for (size_t x = 1; x < map.width; x++) {
            if ('a' <= map.items[y][x] && map.items[y][x] <= 'z') count++;
        }
    }
    return count;
}

Position get_starting_position(Map map) {
    for (size_t y = 1; y < map.length; y++) {
        for (size_t x = 1; x < map.width; x++) {
            if (map.items[y][x] == '@') return (Position){.x = x, .y = y};
        }
    }
    assert(0 && "UNREACHABLE");
}

static bool is_key_collected(char *keys, char key) {
    while (*keys != '\0') {
        if (*keys == key) return true;
        keys++;
    }
    return false;
}

static bool list_contains(List l, LElem e, size_t offset) {
    for (size_t i = offset; i < l.length; i++) {
        if (l.items[i].p.x == e.p.x && l.items[i].p.y == e.p.y) return true;
    }
    return false;
}

static size_t do_compute_shortest_path(Map map, Position start_pos, char *keys, size_t keys_len, List *q) {
    if (keys_len == MAX_KEY_COUNT) return 0;
    size_t path_len = SIZE_MAX;
    size_t offset = q->length;
    LElem l = {.p = start_pos, .dist = 0};
    DAWN_DA_APPEND(q, l);
    for (size_t i = offset; i < q->length; i++) {
        LElem it = q->items[i];
        if ('a' <= map.items[it.p.y][it.p.x] && map.items[it.p.y][it.p.x] <= 'z' && !is_key_collected(keys, map.items[it.p.y][it.p.x])) {
            if (it.dist > path_len) continue;
            keys[keys_len] = map.items[it.p.y][it.p.x];
            size_t len = do_compute_shortest_path(map, it.p, keys, keys_len + 1, q);
            keys[keys_len] = 0;
            if (path_len > len + it.dist) path_len = len + it.dist;
            continue;
        }
        Position neighbors[] = {
            {it.p.x, it.p.y - 1},
            {it.p.x, it.p.y + 1},
            {it.p.x - 1, it.p.y},
            {it.p.x + 1, it.p.y},
        };
        for (size_t j = 0; j < 4; j++) {
            char n = map.items[neighbors[j].y][neighbors[j].x];
            if (n == '#' || ('A' <= n && n <= 'Z' && !is_key_collected(keys, n + 25 + 'a' - 'Z'))) continue;
            LElem nl = {.p = neighbors[j], .dist = it.dist + 1};
            if (list_contains(*q, nl, offset)) continue;
            DAWN_DA_APPEND(q, nl);
        }
    }
    q->length = offset;
    return path_len;
}

size_t compute_shortest_path(Map map, Position start_pos) {
    size_t path_len = SIZE_MAX;
    char keys[MAX_KEY_COUNT + 1] = {0};
    size_t keys_len = 0;
    List q = {0};
    LElem l = {.p = start_pos, .dist = 0};
    DAWN_DA_APPEND(&q, l);
    for (size_t i = 0; i < q.length; i++) {
        LElem it = q.items[i];
        if ('a' <= map.items[it.p.y][it.p.x] && map.items[it.p.y][it.p.x] <= 'z') {
            if (it.dist > path_len) continue;
            keys[keys_len] = map.items[it.p.y][it.p.x];
            size_t len = do_compute_shortest_path(map, it.p, keys, keys_len + 1, &q);
            keys[keys_len] = 0;
            if (path_len > len + it.dist) path_len = len + it.dist;
            continue;
        }
        Position neighbors[] = {
            {it.p.x, it.p.y - 1},
            {it.p.x, it.p.y + 1},
            {it.p.x - 1, it.p.y},
            {it.p.x + 1, it.p.y},
        };
        for (size_t j = 0; j < 4; j++) {
            char n = map.items[neighbors[j].y][neighbors[j].x];
            if (n == '#' || ('A' <= n && n <= 'Z')) continue;
            LElem nl = {.p = neighbors[j], .dist = it.dist + 1};
            if (list_contains(q, nl, 0)) continue;
            DAWN_DA_APPEND(&q, nl);
        }
    }
    DAWN_DA_FREE(q);
    return path_len;
}

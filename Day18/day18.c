#include "day18.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARRAY_SIZE(arr) (sizeof((arr))/sizeof((arr)[0]))

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

Position get_starting_position(Map map) {
    for (size_t y = 1; y < map.length; y++) {
        for (size_t x = 1; x < map.width; x++) {
            if (map.items[y][x] == '@') return (Position){.x = x, .y = y};
        }
    }
    assert(0 && "UNREACHABLE");
}

static bool is_processed(Adjacency q, Elem e) {
    for (size_t i = 0; i < q.length; i++) {
        if (q.items[i].p.x == e.p.x && q.items[i].p.y == e.p.y) return true;
    }
    return false;
}

static int compare_chars(const void *v1, const void *v2) {
    char c1 = *(char*)v1;
    char c2 = *(char*)v2;
    if (c1 < c2) return -1;
    if (c1 > c2) return 1;
    return 0;
}

AdjacencyList adjacency_list_init(Map map, Position starting_position) {
    AdjacencyList al = {0};
    Adjacency a = {
        .name = '@',
        .p = starting_position,
    };
    DAWN_DA_APPEND(&al, a);
    for (size_t y = 0; y < map.length; y++) {
        for (size_t x = 0; x < map.width; x++) {
            if ('a' <= map.items[y][x] && map.items[y][x] <= 'z') {
                a = (Adjacency){
                    .name = map.items[y][x],
                    .p = (Position){.x = x, .y = y},
                };
                DAWN_DA_APPEND(&al, a);
            }
        }
    }
    Adjacency q = {0};
    for (size_t i = 0; i < al.length; i++) {
        Adjacency *it = al.items + i;
        Elem e = {
            .name = it->name,
            .p = it->p,
        };
        DAWN_DA_APPEND(&q, e);
        for (size_t j = 0; j < q.length; j++) {
            e = q.items[j];
            Elem ns[] = {
                {.p = (Position){.x = e.p.x, .y = e.p.y - 1}},
                {.p = (Position){.x = e.p.x, .y = e.p.y + 1}},
                {.p = (Position){.x = e.p.x - 1, .y = e.p.y}},
                {.p = (Position){.x = e.p.x + 1, .y = e.p.y}},
            };
            size_t ns_size = ARRAY_SIZE(ns);
            for (size_t k = 0; k < ns_size; k++) {
                if (i == 0 && 'A' <= map.items[ns[k].p.y][ns[k].p.x] && map.items[ns[k].p.y][ns[k].p.x] <= 'Z') continue;
                if (map.items[ns[k].p.y][ns[k].p.x] == '#' || is_processed(q, ns[k])) continue;
                ns[k].name = map.items[ns[k].p.y][ns[k].p.x];
                ns[k].dist = e.dist + 1;
                ns[k].doors_size = e.doors_size;
                strncpy(ns[k].doors, e.doors, e.doors_size);
                if ('A' <= ns[k].name && ns[k].name <= 'Z') {
                    ns[k].doors[ns[k].doors_size++] = ns[k].name;
                    qsort(ns[k].doors, ns[k].doors_size, sizeof(*ns[k].doors), compare_chars);
                } else if ('a' <= ns[k].name && ns[k].name <= 'z') {
                    DAWN_DA_APPEND(it, ns[k]);
                }
                DAWN_DA_APPEND(&q, ns[k]);
            }
        }
        q.length = 0;
    }
    DAWN_DA_FREE(q);
    return al;
}

void adjacency_list_free(AdjacencyList al) {
    for (size_t i = 0; i < al.length; i++) {
        DAWN_DA_FREE(al.items[i]);
    }
    DAWN_DA_FREE(al);
}

static inline char to_door(char key) {
    return key - 25 - 'a' + 'Z';
}

// TODO: Figure out the doors.
size_t compute_shortest_path(AdjacencyList al) {
    Adjacency q = {0};
    Elem e = {.name = al.items[0].name};
    DAWN_DA_APPEND(&q, e);
    for (size_t i = 0; i < q.length; i++) {
        Elem it = q.items[i];
        Adjacency *ns = al.items;
        while (ns->name != it.name) ns++;
        for (size_t j = 0; j < ns->length; j++) {
            Elem jt = ns->items[j];
            bool is_processed = false;
            for (size_t k = 0; k < i; k++) {
                if (q.items[k].name == jt.name) is_processed = true;
            }
            if (is_processed) continue;
            size_t dist = it.dist + jt.dist;
            size_t idx = i + 1;
            for (; idx < q.length; idx++) if (q.items[idx].name == jt.name) break;
            if (idx == q.length) {
                Elem ne = {
                    .name = jt.name,
                    .dist = dist,
                };
                DAWN_DA_APPEND(&q, ne);
            } else if (q.items[idx].dist > dist) q.items[idx].dist = dist;
            for (size_t k = idx; k > i; k--) {
                if (q.items[k - 1].dist <= q.items[k].dist) break;
                Elem temp = q.items[k - 1];
                q.items[k - 1] = q.items[k];
                q.items[k] = temp;
            }
        }
    }
    for (size_t i = 0; i < q.length; i++) {
        printf("%c ", q.items[i].name);
    }
    putchar('\n');
    size_t path_len = q.items[q.length - 1].dist;
    DAWN_DA_FREE(q);
    return path_len;
}

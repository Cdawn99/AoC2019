#ifndef DAY18_H_
#define DAY18_H_

#include "../dawn_utils.h"

#include <stddef.h>

#define MAX_KEY_COUNT (26 + 1)

typedef struct {
    size_t width;
    size_t length;
    size_t capacity;
    char **items;
} Map;

Map map_init(DawnStringBuilder map_data);
void map_free(Map map);

typedef struct {
    size_t x;
    size_t y;
} Position;

Position get_starting_position(Map map);

typedef struct {
    char name;
    Position p;
    size_t dist;
    size_t doors_size;
    char doors[MAX_KEY_COUNT];
} Elem;

typedef struct {
    char name;
    Position p;
    size_t length;
    size_t capacity;
    Elem *items;
} Adjacency;

typedef struct {
    size_t length;
    size_t capacity;
    Adjacency *items;
} AdjacencyList;

AdjacencyList adjacency_list_init(Map map, Position starting_position);
void adjacency_list_free(AdjacencyList al);

size_t compute_shortest_path(AdjacencyList al);

#endif // DAY18_H_

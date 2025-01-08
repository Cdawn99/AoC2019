#ifndef DAY18_H_
#define DAY18_H_

#include "../dawn_utils.h"

#include <stddef.h>

#define MAX_KEY_COUNT 16

typedef struct {
    size_t width;
    size_t length;
    size_t capacity;
    char **items;
} Map;

Map map_init(DawnStringBuilder map_data);
void map_free(Map map);
size_t count_keys(Map map);

typedef struct {
    size_t x;
    size_t y;
} Position;

Position get_starting_position(Map map);

size_t compute_shortest_path(Map map, Position start_pos);

#endif // DAY18_H_

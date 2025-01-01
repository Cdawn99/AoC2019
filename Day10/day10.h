#ifndef DAY10_H_
#define DAY10_H_

#include "../dawn_utils.h"

#include <stddef.h>
#include <stdint.h>

typedef struct {
    size_t width;
    size_t length;
    size_t capacity;
    char **items;
} Map;

Map map_init(DawnStringBuilder map_data);
void map_free(Map map);

typedef struct {
    int64_t x;
    int64_t y;
    size_t detectable_asteroids;
} Asteroid;

Asteroid get_optimal_asteroid(Map map);
Asteroid get_200th_vaporized_asteroid(Asteroid laser, Map map);

#endif // DAY10_H_

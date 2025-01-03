#ifndef DAY12_H_
#define DAY12_H_

#include <stddef.h>
#include <stdint.h>

int64_t parse_int(char *str);
int64_t absolute_value(int64_t x);

typedef enum {
    X = 0,
    Y,
    Z,
} Coordinate;

typedef struct {
    int64_t pos[3];
    int64_t vel[3];
} Moon;

typedef struct {
    size_t length;
    size_t capacity;
    Moon *items;
} Moons;

Moons moons_init(char *filepath);
void moons_free(Moons moons);

void moons_step(Moons moons);
int64_t moons_total_energy(Moons moons);
void moons_print(Moons moons);

size_t find_coordinate_cycle(Moons moons, Coordinate coord);

#endif // DAY12_H_

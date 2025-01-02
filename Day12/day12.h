#ifndef DAY12_H_
#define DAY12_H_

#include <stddef.h>
#include <stdint.h>

int64_t parse_int(char *str);

typedef struct {
    int64_t x;
    int64_t y;
    int64_t z;
} Vector3;

Vector3 vector_subrtact(Vector3 v1, Vector3 v2);

typedef struct {
    Vector3 pos;
    Vector3 vel;
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

#endif // DAY12_H_

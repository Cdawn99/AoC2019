#include "day10.h"

#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define PI (atan(1) * 4)

typedef struct {
    size_t length;
    size_t capacity;
    Asteroid *items;
} Asteroids;

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

static int64_t gcd(int64_t a, int64_t b) {
    if (b == 0) return a;
    return gcd(b, a % b);
}

static Asteroids asteroids_init(Map map) {
    Asteroids asteroids = {0};
    for (size_t y = 0; y < map.length; y++) {
        for (size_t x = 0; x < map.width; x++) {
            if (map.items[y][x] == '#') {
                Asteroid asteroid = {.x = x, .y = y};
                DAWN_DA_APPEND(&asteroids, asteroid);
            }
        }
    }
    return asteroids;
}

static void asteroids_free(Asteroids asteroids) {
    DAWN_DA_FREE(asteroids);
}

static bool asteroid_in_line_of_sight(Asteroid *base, Asteroid *tip, Map map, int64_t dx, int64_t dy) {
    int64_t x = base->x + dx;
    int64_t y = base->y + dy;
    while (x != tip->x || y != tip->y) {
        if (map.items[y][x] == '#') return false;
        x += dx;
        y += dy;
    }
    return true;
}

static Asteroid asteroids_get_optimal(Asteroids asteroids) {
    Asteroid optimal = asteroids.items[0];
    for (size_t i = 1; i < asteroids.length; i++) {
        if (optimal.detectable_asteroids < asteroids.items[i].detectable_asteroids) {
            optimal = asteroids.items[i];
        }
    }
    return optimal;
}

Asteroid get_optimal_asteroid(Map map) {
    Asteroids asteroids = asteroids_init(map);

    for (size_t i = 0; i < asteroids.length - 1; i++) {
        Asteroid *it = &asteroids.items[i];
        for (size_t j = i + 1; j < asteroids.length; j++) {
            Asteroid *jt = &asteroids.items[j];

            int64_t dy = jt->y - it->y;
            int64_t dx = jt->x - it->x;

            int64_t ay = labs(dy);
            int64_t ax = labs(dx);

            int64_t f = ay > ax ? gcd(ay, ax) : gcd(ax, ay);
            dy /= f;
            dx /= f;

            if (asteroid_in_line_of_sight(it, jt, map, dx, dy)) {
                it->detectable_asteroids++;
                jt->detectable_asteroids++;
            }
        }
    }

    Asteroid optimal = asteroids_get_optimal(asteroids);

    asteroids_free(asteroids);

    return optimal;
}

static void enumerate_los_asteroids(Map map, Asteroid asteroid, Asteroids *los) {
    for (size_t y = 0; y < map.length; y++) {
        for (size_t x = 0; x < map.width; x++) {
            if (map.items[y][x] != '#') continue;
            if (x == (size_t)asteroid.x && y == (size_t)asteroid.y) continue;

            Asteroid candidate = {.x = x, .y = y};

            int64_t dy = candidate.y - asteroid.y;
            int64_t dx = candidate.x - asteroid.x;

            int64_t ay = labs(dy);
            int64_t ax = labs(dx);

            int64_t f = ay > ax ? gcd(ay, ax) : gcd(ax, ay);
            dy /= f;
            dx /= f;

            if (asteroid_in_line_of_sight(&asteroid, &candidate, map, dx, dy)) {
                DAWN_DA_APPEND(los, candidate);
            }
        }
    }
}

/**
 * Get the base asteroid.
 * If `asteroid != NULL`, then register the given asteroid as the base asteroid.
 */
static Asteroid get_base_asteroid(Asteroid *asteroid) {
    static Asteroid base = {0};
    if (asteroid) {
        base = *asteroid;
    }
    return base;
}

static int compare_asteroids(const void *asteroid1, const void *asteroid2) {
    const Asteroid *a1 = asteroid1;
    const Asteroid *a2 = asteroid2;
    Asteroid base = get_base_asteroid(NULL);

    Asteroid v0 = {
        .x = 0,
        .y = -1
    };
    double l0 = sqrt(v0.x * v0.x + v0.y * v0.y);
    Asteroid v1 = {
        .x = a1->x - base.x,
        .y = a1->y - base.y,
    };
    double l1 = sqrt(v1.x * v1.x + v1.y * v1.y);
    Asteroid v2 = {
        .x = a2->x - base.x,
        .y = a2->y - base.y,
    };
    double l2 = sqrt(v2.x * v2.x + v2.y * v2.y);

    double t1 = acos((v0.x * v1.x + v0.y * v1.y) / (l0 * l1));
    if (v1.x < 0) t1 = 2*PI - t1;
    double t2 = acos((v0.x * v2.x + v0.y * v2.y) / (l0 * l2));
    if (v2.x < 0) t2 = 2*PI - t2;

    if (t1 < t2) return -1;
    if (t1 > t2) return 1;
    return 0;
}

Asteroid get_200th_vaporized_asteroid(Asteroid laser, Map map) {
    Asteroid vaporized = {0};

    Asteroids los = {0};
    enumerate_los_asteroids(map, laser, &los);

    // Register the laser as a base.
    get_base_asteroid(&laser);

    size_t vaporized_count = 0;
    while (los.length != 0) {
        qsort(los.items, los.length, sizeof(*los.items), compare_asteroids);

        for (size_t i = 0; i < los.length; i++) {
            vaporized = los.items[i];
            map.items[vaporized.y][vaporized.x] = '.';
            vaporized_count++;
            if (vaporized_count == 200) break;
        }
        if (vaporized_count == 200) break;

        los.length = 0;
        enumerate_los_asteroids(map, laser, &los);
    }

    asteroids_free(los);

    return vaporized;
}

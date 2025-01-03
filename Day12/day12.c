#include "day12.h"
#include "../dawn_utils.h"

#include <inttypes.h>
#include <stdbool.h>
#include <string.h>

int64_t parse_int(char *str) {
    int64_t n = 0;
    int64_t sign = *str == '-' ? -1 : 1;
    if (sign == -1) str++;
    while (*str != '\0') {
        if (*str < '0' || '9' < *str) break;
        n *= 10;
        n += *str - '0';
        str++;
    }
    return sign * n;
}

int64_t absolute_value(int64_t x) {
    return x >= 0 ? x : -x;
}

Moons moons_init(char *filepath) {
    Moons moons = {0};
    DawnStringBuilder info = {0};
    if (!dawn_read_entire_file(filepath, &info)) return moons;
    DAWN_SB_APPEND_BUF(&info, "", 1);

    char delim[] = "\n";
    char *token = strtok(info.items, delim);
    while (token) {
        Moon moon = {0};

        while (*token != '-' && (*token < '0' || '9' < *token)) token++;
        moon.pos[X] = parse_int(token);
        while (*token == '-' || ('0' <= *token && *token <= '9')) token++;

        while (*token != '-' && (*token < '0' || '9' < *token)) token++;
        moon.pos[Y] = parse_int(token);
        while (*token == '-' || ('0' <= *token && *token <= '9')) token++;

        while (*token != '-' && (*token < '0' || '9' < *token)) token++;
        moon.pos[Z] = parse_int(token);
        while (*token == '-' || ('0' <= *token && *token <= '9')) token++;

        DAWN_DA_APPEND(&moons, moon);

        token = strtok(NULL, delim);
    }

    DAWN_SB_FREE(info);

    return moons;
}

void moons_free(Moons moons) {
    DAWN_DA_FREE(moons);
}

void moons_step(Moons moons) {
    for (size_t i = 0; i < moons.length - 1; i++) {
        Moon *it = moons.items + i;
        for (size_t j = i + 1; j < moons.length; j++) {
            Moon *jt = moons.items + j;

            int64_t pos_diff[3] = {
                it->pos[X] - jt->pos[X],
                it->pos[Y] - jt->pos[Y],
                it->pos[Z] - jt->pos[Z],
            };

            if (pos_diff[X] != 0) pos_diff[X] /= absolute_value(pos_diff[X]);
            if (pos_diff[Y] != 0) pos_diff[Y] /= absolute_value(pos_diff[Y]);
            if (pos_diff[Z] != 0) pos_diff[Z] /= absolute_value(pos_diff[Z]);

            jt->vel[X] += pos_diff[X];
            jt->vel[Y] += pos_diff[Y];
            jt->vel[Z] += pos_diff[Z];

            pos_diff[X] *= -1;
            pos_diff[Y] *= -1;
            pos_diff[Z] *= -1;

            it->vel[X] += pos_diff[X];
            it->vel[Y] += pos_diff[Y];
            it->vel[Z] += pos_diff[Z];
        }
    }

    for (size_t i = 0; i < moons.length; i++) {
        Moon *it = moons.items + i;
        it->pos[X] += it->vel[X];
        it->pos[Y] += it->vel[Y];
        it->pos[Z] += it->vel[Z];
    }
}

int64_t moons_total_energy(Moons moons) {
    int64_t E_tot = 0;
    for (size_t i = 0; i < moons.length; i++) {
        Moon it = moons.items[i];
        int64_t pos_l1_norm = absolute_value(it.pos[X]) + absolute_value(it.pos[Y]) + absolute_value(it.pos[Z]);
        int64_t vel_l1_norm = absolute_value(it.vel[X]) + absolute_value(it.vel[Y]) + absolute_value(it.vel[Z]);
        E_tot += pos_l1_norm * vel_l1_norm;
    }
    return E_tot;
}

void moons_print(Moons moons) {
    for (size_t i = 0; i < moons.length; i++) {
        Moon it = moons.items[i];
        printf("pos=<%" PRId64 ", %" PRId64 ", %" PRId64 ">, "
               "vel=<%" PRId64 ", %" PRId64 ", %" PRId64 ">\n",
               it.pos[X], it.pos[Y], it.pos[Z], it.vel[X], it.vel[Y], it.vel[Z]);
    }
}

typedef struct {
    int64_t p[4];
    int64_t v[4];
} PV;

typedef struct {
    size_t length;
    size_t capacity;
    PV *items;
} List;

static bool list_contains(List list, PV value) {
    for (size_t i = 0; i < list.length; i++) {
        if (list.items[i].p[0] == value.p[0] && list.items[i].v[0] == value.v[0]
            && list.items[i].p[1] == value.p[1] && list.items[i].v[1] == value.v[1]
            && list.items[i].p[2] == value.p[2] && list.items[i].v[2] == value.v[2]
            && list.items[i].p[3] == value.p[3] && list.items[i].v[3] == value.v[3]) return true;
    }
    return false;
}

size_t find_coordinate_cycle(Moons moons, Coordinate coord) {
    List visited = {0};
    PV pv = {0};
    for (size_t i = 0; i < moons.length; i++) {
        pv.p[i] = moons.items[i].pos[coord];
        pv.v[i] = moons.items[i].vel[coord];
    }
    DAWN_DA_APPEND(&visited, pv);
    while (true) {
        for (size_t i = 0; i < moons.length - 1; i++) {
            Moon *it = moons.items + i;
            for (size_t j = i + 1; j < moons.length; j++) {
                Moon *jt = moons.items + j;
                int64_t pos_diff = it->pos[coord] - jt->pos[coord];
                if (pos_diff != 0) pos_diff /= absolute_value(pos_diff);
                jt->vel[coord] += pos_diff;
                pos_diff *= -1;
                it->vel[coord] += pos_diff;
            }
        }

        for (size_t i = 0; i < moons.length; i++) {
            Moon *it = moons.items + i;
            it->pos[coord] += it->vel[coord];
        }

        for (size_t i = 0; i < moons.length; i++) {
            pv.p[i] = moons.items[i].pos[coord];
            pv.v[i] = moons.items[i].vel[coord];
        }
        if (list_contains(visited, pv)) break;
        DAWN_DA_APPEND(&visited, pv);
    }
    size_t cycle = visited.length;
    DAWN_DA_FREE(visited);
    return cycle;
}

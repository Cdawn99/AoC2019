#include "day12.h"
#include "../dawn_utils.h"

#include <inttypes.h>
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

Vector3 vector_subrtact(Vector3 v1, Vector3 v2) {
    return (Vector3){
        .x = v1.x - v2.x,
        .y = v1.y - v2.y,
        .z = v1.z - v2.z,
    };
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
        moon.pos.x = parse_int(token);
        while (*token == '-' || ('0' <= *token && *token <= '9')) token++;

        while (*token != '-' && (*token < '0' || '9' < *token)) token++;
        moon.pos.y = parse_int(token);
        while (*token == '-' || ('0' <= *token && *token <= '9')) token++;

        while (*token != '-' && (*token < '0' || '9' < *token)) token++;
        moon.pos.z = parse_int(token);
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

static void vector_add_in_place(Vector3 *v1, Vector3 v2) {
    v1->x += v2.x;
    v1->y += v2.y;
    v1->z += v2.z;
}

void moons_step(Moons moons) {
    for (size_t i = 0; i < moons.length - 1; i++) {
        Moon *it = moons.items + i;
        for (size_t j = i + 1; j < moons.length; j++) {
            Moon *jt = moons.items + j;

            Vector3 pos_diff = vector_subrtact(it->pos, jt->pos);
            if (pos_diff.x != 0) pos_diff.x /= labs(pos_diff.x);
            if (pos_diff.y != 0) pos_diff.y /= labs(pos_diff.y);
            if (pos_diff.z != 0) pos_diff.z /= labs(pos_diff.z);
            vector_add_in_place(&jt->vel, pos_diff);

            pos_diff.x *= -1;
            pos_diff.y *= -1;
            pos_diff.z *= -1;
            vector_add_in_place(&it->vel, pos_diff);
        }
    }

    for (size_t i = 0; i < moons.length; i++) {
        Moon *it = moons.items + i;
        vector_add_in_place(&it->pos, it->vel);
    }
}

static int64_t vector_l1_norm(Vector3 v) {
    return labs(v.x) + labs(v.y) + labs(v.z);
}

int64_t moons_total_energy(Moons moons) {
    int64_t E_tot = 0;
    for (size_t i = 0; i < moons.length; i++) {
        Moon it = moons.items[i];
        E_tot += vector_l1_norm(it.pos) * vector_l1_norm(it.vel);
    }
    return E_tot;
}

void moons_print(Moons moons) {
    for (size_t i = 0; i < moons.length; i++) {
        Moon it = moons.items[i];
        printf("pos=<%" PRId64 ", %" PRId64 ", %" PRId64 ">, "
               "vel=<%" PRId64 ", %" PRId64 ", %" PRId64 ">\n",
               it.pos.x, it.pos.y, it.pos.z, it.vel.x, it.vel.y, it.vel.z);
    }
}

#include "intcode.h"

#define DAWN_IMPLEMENTATION
#include "../dawn_utils.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

typedef enum {
    EMPTY = 0,
    WALL,
    BLOCK,
    PADDLE,
    BALL,
} TileId;

typedef struct {
    int x;
    int y;
    TileId id;
} Tile;

bool tile_pos_eq(Tile t1, Tile t2) {
    return t1.x == t2.x && t1.y == t2.y;
}

typedef struct {
    size_t length;
    size_t capacity;
    Tile *items;
} Tiles;

void add_tile(Tiles *ts, Tile t) {
    for (size_t i = 0; i < ts->length; i++) {
        if (tile_pos_eq(ts->items[i], t)) {
            ts->items[i].id = t.id;
            return;
        }
    }
    DAWN_DA_APPEND(ts, t);
}

static int parse_int(char *s) {
    int n = 0;
    while ('0' <= *s && *s <= '9') {
        n *= 10;
        n += *s - '0';
        s++;
    }
    return n;
}

int main(int argc, char **argv) {
    char *program = dawn_shift_args(&argc, &argv);
    if (argc == 0) {
        printf("Usage: %s <program.txt>\n", program);
        return 1;
    }

    char *fp = dawn_shift_args(&argc, &argv);
    Program p = read_program(fp);

    DawnStringBuilder output = run_program(&p);
    DAWN_SB_APPEND_BUF(&output, "", 1);

    Tiles ts = {0};
    char delim[] = "\n";
    char *token = strtok(output.items, delim);
    while (token) {
        Tile t = {0};

        t.x = parse_int(token);
        token = strtok(NULL, delim);

        t.y = parse_int(token);
        token = strtok(NULL, delim);

        t.id = parse_int(token);
        token = strtok(NULL, delim);

        add_tile(&ts, t);
    }

    size_t block_count = 0;
    for (size_t i = 0; i < ts.length; i++) {
        if (ts.items[i].id == BLOCK) block_count++;
    }

    printf("Number of block tiles: %zu\n", block_count);

    DAWN_DA_FREE(ts);
    DAWN_SB_FREE(output);
    free_program(p);

    return 0;
}

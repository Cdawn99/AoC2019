#include "day17.h"
#include "intcode.h"

#include <stdbool.h>
#include <string.h>

static char parse_char(char *s) {
    char c = 0;
    while (*s != '\0') {
        c *= 10;
        c += *s - '0';
        s++;
    }
    return c;
}

DawnStringBuilder get_map_data(char *filepath) {
    Program p = read_program(filepath);

    DawnStringBuilder output = run_program(&p);
    free_program(p);

    DAWN_SB_APPEND_BUF(&output, "", 1);

    DawnStringBuilder map_data = {0};

    char delim[] = "\n";
    char *token = strtok(output.items, delim);
    char buffer[1] = {0};
    while (token) {
        buffer[0] = parse_char(token);
        DAWN_SB_APPEND_BUF(&map_data, buffer, 1);
        token = strtok(NULL, delim);
    }
    DAWN_SB_FREE(output);

    DAWN_SB_APPEND_BUF(&map_data, "", 1);
    return map_data;
}

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

size_t get_sum_of_alignment_parameters(Map map) {
    size_t sum = 0;
    for (size_t y = 1; y < map.length - 1; y++) {
        for (size_t x = 1; x < map.width - 1; x++) {
            if (map.items[y][x] != '#') continue;
            bool up = map.items[y - 1][x] == '#';
            bool down = map.items[y + 1][x] == '#';
            bool left = map.items[y][x - 1] == '#';
            bool right = map.items[y][x + 1] == '#';
            sum += (up && down && left && right) * x * y;
        }
    }
    return sum;
}

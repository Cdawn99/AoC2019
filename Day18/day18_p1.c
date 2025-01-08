#include "day18.h"

#define DAWN_IMPLEMENTATION
#include "../dawn_utils.h"

int main(int argc, char **argv) {
    char *program = dawn_shift_args(&argc, &argv);
    if (argc == 0) {
        printf("Usage: %s <map.txt>\n", program);
        return 1;
    }

    char *fp = dawn_shift_args(&argc, &argv);
    DawnStringBuilder map_data = {0};
    if (!dawn_read_entire_file(fp, &map_data)) return 1;
    Map map = map_init(map_data);

    Position starting_pos = get_starting_position(map);

    size_t shortest_path = compute_shortest_path(map, starting_pos);
    printf("Shortest path to gather all keys: %zu\n", shortest_path);

    map_free(map);
    DAWN_SB_FREE(map_data);

    return 0;
}

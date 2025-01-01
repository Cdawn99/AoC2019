#include "day10.h"

#define DAWN_IMPLEMENTATION
#include "../dawn_utils.h"

#include <inttypes.h>

int main(int argc, char **argv) {
    char *program = dawn_shift_args(&argc, &argv);
    if (argc == 0) {
        printf("Usage: %s <map.txt>\n", program);
        return 1;
    }

    char *fp = dawn_shift_args(&argc, &argv);
    DawnStringBuilder map_data = {0};
    if (!dawn_read_entire_file(fp, &map_data)) return 1;
    DAWN_SB_APPEND_BUF(&map_data, "", 1);

    Map map = map_init(map_data);

    Asteroid optimal = get_optimal_asteroid(map);
    Asteroid bet_winner = get_200th_vaporized_asteroid(optimal, map);

    printf("Bet winner: %" PRId64 "\n", 100 * bet_winner.x + bet_winner.y);

    map_free(map);
    DAWN_SB_FREE(map_data);

    return 0;
}

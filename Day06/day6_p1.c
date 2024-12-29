#include "day6.h"

#define DAWN_IMPLEMENTATION
#include "../dawn_utils.h"

int main(int argc, char **argv) {
    char *program = dawn_shift_args(&argc, &argv);
    if (argc == 0) {
        printf("Usage: %s <orbit_info.txt>\n", program);
        return 1;
    }

    char *fp = dawn_shift_args(&argc, &argv);
    Graph g = graph_init(fp);

    size_t checksum = get_orbit_checksum(g);
    printf("Checksum: %zu\n", checksum);

    graph_free(g);

    return 0;
}

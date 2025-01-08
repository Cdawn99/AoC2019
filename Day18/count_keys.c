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

    size_t key_count = count_keys(map);
    printf("Number of keys: %zu\n", key_count);

    map_free(map);
    DAWN_SB_FREE(map_data);

    return 0;
}

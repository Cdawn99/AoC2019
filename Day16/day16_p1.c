#include "day16.h"

#define DAWN_IMPLEMENTATION
#include "../dawn_utils.h"

#include <inttypes.h>
#include <stdio.h>

#define PHASE_COUNT 100

int main(int argc, char **argv) {
    char *program = dawn_shift_args(&argc, &argv);
    if (argc == 0) {
        printf("Usage: %s <signal.txt>\n", program);
        return 1;
    }

    char *fp = dawn_shift_args(&argc, &argv);
    List signal = list_init(fp);

    flawed_frequency_transmission(&signal, PHASE_COUNT);
    for (size_t i = 0; i < 8; i++) {
        printf("%" PRId64, signal.items[i]);
    }
    putchar('\n');

    list_free(signal);

    return 0;
}

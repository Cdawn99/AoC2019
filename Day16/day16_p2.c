#include "day16.h"

#define DAWN_IMPLEMENTATION
#include "../dawn_utils.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

#define PHASE_COUNT 100
#define REPEAT_COUNT 10000

int main(int argc, char **argv) {
    char *program = dawn_shift_args(&argc, &argv);
    if (argc == 0) {
        printf("Usage: %s <signal.txt>\n", program);
        return 1;
    }

    char *fp = dawn_shift_args(&argc, &argv);

    List signal = list_init(fp);
    size_t offset = extract_offset(signal);

    size_t relevant_len = signal.length * REPEAT_COUNT - offset;
    int8_t *s = malloc(sizeof(*s) * relevant_len);
    for (size_t i = 0; i < relevant_len; i++) {
        s[i] = signal.items[(offset + i) % signal.length];
    }

    for (size_t i = 0; i < PHASE_COUNT; i++) {
        for (size_t j = relevant_len - 2; j < relevant_len; j--) {
            s[j] = (s[j] + s[j + 1]) % 10;
        }
    }

    for (size_t i = 0; i < 8; i++) {
        printf("%" PRId8, s[i]);
    }
    putchar('\n');

    free(s);
    list_free(signal);

    return 0;
}

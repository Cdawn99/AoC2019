#define DAWN_IMPLEMENTATION
#include "../dawn_utils.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

bool is_password(size_t candidate) {
    size_t digit_count[10] = {0};

    size_t next = candidate % 10;
    candidate /= 10;
    digit_count[next]++;

    while (candidate > 0) {
        size_t digit = candidate % 10;
        candidate /= 10;

        if (digit > next) return false;

        digit_count[digit]++;
        next = digit;
    }

    for (size_t i = 0; i < 10; i++) {
        if (digit_count[i] == 2) return true;
    }

    return false;
}

int main(int argc, char **argv) {
    char *program = dawn_shift_args(&argc, &argv);
    if (argc == 0) {
        printf("Usage: %s <range.txt>\n", program);
        return 1;
    }

    char *fp = dawn_shift_args(&argc, &argv);
    DawnStringBuilder content = {0};
    if (!dawn_read_entire_file(fp, &content)) return 1;
    content.items[content.length - 1] = '\0';

    char *s1 = content.items;
    char *s2 = content.items;
    while (*s2 != '-') s2++;
    *s2 = '\0';
    s2++;

    size_t n1 = atoi(s1);
    size_t n2 = atoi(s2);

    DAWN_SB_FREE(content);

    size_t password_count = 0;
    for (size_t i = n1; i <= n2; i++) {
        if (is_password(i)) password_count++;
    }

    printf("Number of different passwords: %zu\n", password_count);

    return 0;
}
